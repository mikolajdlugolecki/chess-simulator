LIBS = -lGL -lglfw -lGLEW
HEADERS = src/*.h
FILES = src/*.cpp
chess: $(FILES) $(HEADERS)
	g++ -o chess $(FILES) $(LIBS) -I.
