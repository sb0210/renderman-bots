CC=g++

OPENGLLIB= -lGL
GLEWLIB= -lGLEW
GLFWLIB = -lglfw
LIBS=$(OPENGLLIB) $(GLEWLIB) $(GLFWLIB)
LDFLAGS=-L/usr/local/lib 
CPPFLAGS=-I/usr/local/include -I./

BIN=mesh
SRCS=main.cpp gl_framework.cpp shader_util.cpp meshing.cpp
INCLUDES=gl_framework.hpp shader_util.hpp  meshing.h

all: $(BIN)

$(BIN): $(SRCS) $(INCLUDES)
	g++ $(CPPFLAGS) $(SRCS) -o $(BIN) $(LDFLAGS) $(LIBS) -g

clean:
	rm -f *~ *.o $(BIN)
