CPPCOMPILER = clang++
CPPFLAGS = -std=c++11 

TARGET = main
SOURCE = main.cpp


$(TARGET): $(SOURCE)
	$(CPPCOMPILER) $(CPPFLAGS) $(SOURCE) -o $@
	# clang++ -std=c++11 main.cpp -o main

run: 
	./$(TARGET)

clean:
	rm main
	
.PHONY:
	run clean
