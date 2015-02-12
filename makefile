
CC = gcc
CFLAGS = -I. -c
LFLAGS = -lm
MODULES = main.o density_map.o rw.o

PROGRAMS = main.out 

all:$(PROGRAMS)

%.out:%.o $(MODULES)
	$(CC) $^ $(LFLAGS) -o $@
run: 
	./main.out
clean:
		rm -rf *.o *.out 