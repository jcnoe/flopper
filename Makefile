default: main.c
	gcc -c -o print.o print.c
	gcc -c -o main.o main.c
	gcc -Wall -g -o flopper print.o main.o

clean:
	rm -rf *.exe *.exe.stackdump *.o
