debug:
	g++ ./Source/Main.cpp -Wextra -Wall -fsanitize=address -o ../Voxelize.elf -g -lSDL3 -lGL -lGLEW -lm
