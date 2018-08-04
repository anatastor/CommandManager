
CC ?= gcc

PROG = example

LIB = cmdmanager.a

OBJ = cmdmanager.o


all: $(OBJ) $(PROG) $(LIB)


$(PROG): $(PROG).c $(OBJ)
	$(CC) -o $@ $^ 

$(LIB): $(OBJ)
	ar rcs $@ $^


.o: .c .h
	$(CC) -c $@ $^


.PHONY:
	all
	clean

clean:
	rm $(OBJ)
