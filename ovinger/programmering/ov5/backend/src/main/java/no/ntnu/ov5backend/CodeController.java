package no.ntnu.ov5backend;

import java.io.IOException;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@CrossOrigin(origins="*")
@RequestMapping("/api")
public class CodeController {
  private final CodeService codeService;
  private static final Logger LOGGER = LoggerFactory.getLogger(CodeController.class);

  public CodeController(CodeService codeService){
    this.codeService = codeService;
  }

  @PostMapping("/compile")
  public String compileAndRun(@RequestBody CodeRequestDTO request) throws IOException {
    LOGGER.info("POST /api/compile: {}", request.getCode());
    return codeService.compileAndRun(request.getCode());
  }
}
