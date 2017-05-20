CFLAGS = -g -Wall

all: shell

shell: shell_obj

shell_obj: main.o term.o term.h debug.h 
	gcc -o shell $(CFLAGS) main.c term.c 
