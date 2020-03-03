# Source file Objects
OBJS = \
	src/engine/game_fnaf1.o \
	src/engine/options.o \
	src/engine/time.o \
	src/engine/game.o \
	src/engine/main.o \
	src/engine/save.o \
	src/engine/window.o \
	src/engine/math.o \
	src/engine/ai_handler.o \

# Application Name
NAME = FMH

# Libraries
LIBS		= -lm -lc -lglut -IGLU -IGL

# Compiler (C)
CXX			= gcc

# Compiler Flags
CFLAGS = -Wall -g

# Run
install:
	# Make build folder if not exist
	mkdir -p build

	# Create Object Files
	# FIXME - Make automatic
	$(CXX) $(CFLAGS) -c src/game_fnaf1.c
	$(CXX) $(CFLAGS) -c src/engine/options.c
	$(CXX) $(CFLAGS) -c src/engine/time.c
	$(CXX) $(CFLAGS) -c src/engine/game.c
	$(CXX) $(CFLAGS) -c src/engine/main.c
	$(CXX) $(CFLAGS) -c src/engine/save.c
	$(CXX) $(CFLAGS) -c src/engine/window.c
	$(CXX) $(CFLAGS) -c src/engine/math.c
	$(CXX) $(CFLAGS) -c src/engine/ai_handler.c
	mv *.o src/engine/

	# Build into Executable 
	$(CXX) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS)

	# Move Object Files to build directory
	mv src/engine/*.o build/

# Delete binaries
clean: 
	rm build/*.o
	rm $(NAME)