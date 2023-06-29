default: main.c
	gcc -Wall -g -o flopper main.c

clean:
	rm -rf *.exe *.exe.stackdump
