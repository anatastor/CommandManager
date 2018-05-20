
CC ?= gcc

PROG = example

OBJ = cmdmanager.o


all: $(PROG)


$(PROG): $(PROG).c $(OBJ)
	$(CC) -o $@ $^ 


.o: .c .h
	$(CC) -c $@ $^


.PHONY:
	all
	clean

clean:
	rm $(OBJ)
