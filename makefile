
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

# CC = gcc
# CFLAGS = -I. 
# DEPS = proto.h allvars.h 
# OBJ = main.o cells.o 

# %.o: %.c $(DEPS)
# 	  $(CC) -c -o $@ $< $(CFLAGS)

# main.out: $(OBJ)
# 	  gcc -o $@ $^ $(CFLAGS) 	  

# edit:
# 	emacs *.c *.h &

# clean:
# 	rm -r *.o *.out  