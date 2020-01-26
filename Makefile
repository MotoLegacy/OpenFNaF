# Source file Objects
OBJS = \
	src/engine/main.o \
	src/engine/window.o \
	src/engine/math.o \
	src/engine/ai_handler.o \
	src/game_fnaf1.o \

# Application Name
NAME = FMH

# Libraries
LIBS		= -lstdc++ -lm -lc -lglut -IGLU -IGL

# Compiler (C++)
CXX			= g++

# Compiler Flags
CFLAGS = -Wall -g

# Run
install:
	# Make build folder if not exist
	mkdir -p build

	# Create Object Files
	# FIXME - Make automatic
	$(CXX) $(CFLAGS) -c src/engine/main.cpp
	$(CXX) $(CFLAGS) -c src/engine/window.cpp
	$(CXX) $(CFLAGS) -c src/engine/math.cpp
	$(CXX) $(CFLAGS) -c src/engine/ai_handler.cpp
	$(CXX) $(CFLAGS) -c src/game_fnaf1.cpp
	mv *.o src/engine/

	# Build into Executable 
	$(CXX) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS)

	# Move Object Files to build directory
	mv src/engine/*.o build
