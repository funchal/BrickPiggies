.SUFFIXES:
.SECONDARY:

SHELL = /bin/sh

CXX = g++
CPPFLAGS = -Wall -Wextra -Wshadow -Wmissing-declarations -MMD -MP -std=c++11 -Ilodepng
CXXFLAGS = -g -fmessage-length=0

LD = g++
LDFLAGS = -rdynamic -lGL -lSDL2 -lGLEW -lGLU

TARGET = BrickPiggies
OBJS = \
	.Game.o \
	.Window.o \
	.Error.o \
	.Shader.o \
	.Program.o \
	.Texture.o \
	lodepng/.lodepng.o

.PHONY: check
check: all
	./$(TARGET)

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	find . -name '*.d' -delete
	find . -name '*.o' -delete
	$(RM) $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $^ -o $@ $(CXXFLAGS) $(LDFLAGS)

.%.o: %.cpp $(MAKEFILE_LIST)
	$(CXX) -c $< -o $@ $(CPPFLAGS) $(CXXFLAGS)

-include $(shell find . -name '*.d')