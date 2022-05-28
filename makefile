all: maze mazep

maze:
	gcc -Wall -fopenmp -g maze.c -o maze
mazep:
	gcc -Wall -fopenmp -g maze.c -o mazep
clean:
	rm maze mazep