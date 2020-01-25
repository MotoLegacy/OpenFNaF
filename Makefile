# Source file Objects
OBJS = \
	src/engine/main.o \
	src/engine/window.o

# Application Name
#NAME = FMH

# Libraries
LIBS		= -lstdc++ -lm -lc -lglut -IGLU -IGL

# Compiler (C++)
CXX			= g++

# Compiler Flags
CFLAGS = -Wall -g

# Run
install:
	# Create Object Files
	# FIXME - Make automatic
	$(CXX) $(CFLAGS) -c src/engine/main.cpp
	$(CXX) $(CFLAGS) -c src/engine/window.cpp
	mv *.o src/engine/

	# Build into Executable 
	$(CXX) $(CFLAGS) -o FMH $(OBJS) $(LIBS)

	# Move Object Files to build directory
	mv src/engine/*.o build
