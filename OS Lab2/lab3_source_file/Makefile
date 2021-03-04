CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LFLAGS = 
LIBS = -lm
SOURCES = main.c
OBJECTS = $(subst .c,.o,$(SOURCES))
BIN = main
.PHONY: clean 

sudoku : $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@ 

%.o : %.c
	$(CC) $(CFLAGS) -c $< 

all : $(BIN)

clean:
	rm -f $(OBJECTS) $(BIN) *~

help:
	@echo "Valid targets:"
	# @echo "  all:    generates all binary files"
	@echo "  clean:  removes .o and binary files"
