package no.ntnu.ov5backend;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Comparator;
import java.util.concurrent.TimeUnit;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.stereotype.Service;

@Service
public class CodeService {
  private static final Logger LOGGER = LoggerFactory.getLogger(CodeService.class);
  private void createDockerfile(Path dir){
    try{
      Path dockerfile = dir.resolve("Dockerfile");
      String dockerfileContent = """
          FROM gcc:latest
          WORKDIR /app
          COPY main.cpp .
          RUN g++ -o main main.cpp
          CMD ["./main"]
          """;
      // eller clang++ -o main main.cpp

      Files.writeString(dockerfile, dockerfileContent);
    } catch (IOException error) {
      throw new RuntimeException(error);
    }
  }

  public String compileAndRun(String code) throws IOException {
    Path sourceFile = null;
    Path tempDir = null;
    try{
      tempDir = Files.createTempDirectory("temp");

      sourceFile = tempDir.resolve("main.cpp");

      Files.writeString(sourceFile, code);

      createDockerfile(tempDir);

      Path buildOut = tempDir.resolve("build.out");
      Process buildImage = new ProcessBuilder(
          "docker", "build", "-t", "backend", tempDir.toString()
      ).redirectOutput(buildOut.toFile()).redirectError(buildOut.toFile()).start();

      boolean finishedBuilding = buildImage.waitFor(60, TimeUnit.SECONDS);
      if (!finishedBuilding){
        buildImage.destroyForcibly();
      }

      String buildOutput = Files.readString(buildOut);

      StringBuilder output = new StringBuilder();
      output.append(buildOutput);

      Path runOut = tempDir.resolve("run.out");
      Process runContainer = new ProcessBuilder(
          "docker", "run", "--rm", "backend"
      ).redirectOutput(runOut.toFile()).redirectError(runOut.toFile()).start();

      boolean finishedRunning = runContainer.waitFor(30, TimeUnit.SECONDS);
      if (!finishedRunning){
        runContainer.destroyForcibly();
      }

      output.append(Files.readString(runOut));
      LOGGER.info(String.valueOf(output));
      return output.toString().trim();


    } catch (Exception e) {
      throw new RuntimeException(e);
    // cleanup
    } finally {
      if(tempDir!=null) {
        Files.walk(tempDir)
            .sorted(Comparator.reverseOrder()).forEach(p -> {
              try {
                Files.deleteIfExists(p);
              } catch (IOException e) {
                LOGGER.info("delete files failed");
              }
            });
      }
    }
  }
}
