# Source file Objects
OBJS = \
	engine/math.o \
	engine/main.o \
	game_fnaf1.o

# Application Name
MAIN = FMH

# Libraries
LIBS		= -lstdc++ -lm -lc

# Compiler (C++)
CXX			= g++

# Compiler Flags
CFLAGS = -Wall -g

# Run
install:
	# Create Object Files
	# FIXME - Make automatic
	$(CXX) $(CFLAGS) -c engine/math.cpp
	$(CXX) $(CFLAGS) -c engine/main.cpp
	mv *.o ../build/
	$(CXX) $(CFLAGS) -c game_fnaf1.cpp

	# Build into Executable 
	$(CXX) $(CFLAGS) $(OBJS) -o $(MAIN)

	# Move Object Files to Build Directory
	mv *.o ../build/
