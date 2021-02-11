##
# Makefile for SDL Snake
##
C_COMPILER = gcc
C_FLAGS = -O3 -DNDEBUG
TARGET_OUTPUT = Snake

# Specify location of SDL Headers here:
INCLUDE_PATH = #SDL headers here

# Specify location of SDL Libraries here:
LIBRARY_PATH = #SDL libraries here

# Detecting os from:
# https://stackoverflow.com/questions/714100/os-detecting-makefile
ifeq ($(OS), Windows_NT)
# Disable console on windows
C_FLAGS_EXTRA = -Wl,-subsystem,windows
else
C_FLAGS_EXTRA =
endif

all: clock.o game.o main.o queue.o render.o
	$(C_COMPILER) clock.o game.o main.o queue.o render.o $(C_FLAGS) $(C_FLAGS_EXTRA) -L$(LIBRARY_PATH) -lSDL2-2.0 -lSDL2_ttf-2.0 -o $(TARGET_OUTPUT)
	
clean:
	rm *.o $(TARGET_OUTPUT)
    
clock.o:
	$(C_COMPILER) clock.c $(C_FLAGS) -I$(INCLUDE_PATH) -c -o clock.o
    
game.o:
	$(C_COMPILER) game.c $(C_FLAGS) -I$(INCLUDE_PATH) -c -o game.o
    
main.o:
	$(C_COMPILER) main.c $(C_FLAGS) -I$(INCLUDE_PATH) -c -o main.o
    
queue.o:
	$(C_COMPILER) queue.c $(C_FLAGS) -I$(INCLUDE_PATH) -c -o queue.o
    
render.o:
	$(C_COMPILER) render.c $(C_FLAGS) -I$(INCLUDE_PATH) -c -o render.o
