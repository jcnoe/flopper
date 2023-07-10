default: main.c
	gcc -Wall -g -c -o print.o print.c
	gcc -Wall -g -c -o main.o main.c
	gcc -Wall -g -o flopper print.o main.o

clean:
	rm -rf *.exe *.exe.stackdump *.o
