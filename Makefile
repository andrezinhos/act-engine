TARGET = bin/app

CXX = clang++
GLFW = external/glfw
LUA = C:/lua/src

CXXFLAGS = -Wall -std=c++17 -Iexternal -I$(LUA)
CXXFLAGS += -O2 -DNDEBUG -march=x86-64-v2 -mtune=generic
CXXFLAGS += -fno-math-errno -fno-trapping-math
LDFLAGS = -L$(GLFW) -L$(LUA) -lglfw3 -llua54
LDFLAGS += -lgdi32 -lopengl32 -luser32 -lshell32

SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp, build/%.o, $(SRC))

$(TARGET): $(OBJ)
	@if not exist bin mkdir bin
	$(CXX) $(OBJ) -o $(TARGET) $(LDFLAGS)

build/%.o: src/%.cpp
	@if not exist build mkdir build
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
