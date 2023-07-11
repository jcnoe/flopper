default: main.c
	gcc -Wall -g -c -o print.o print.c
	gcc -Wall -g -c -o main.o main.c
	gcc -Wall -g -c -o game.o game.c	
	gcc -Wall -g -c -o structures.o structures.c
	gcc -Wall -g -o flopper print.o game.o structures.o main.o

clean:
	rm -rf *.exe *.exe.stackdump *.o
