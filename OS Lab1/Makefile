CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LFLAGS = 
LIBS = -lm
SOURCES = myshell.c internals.c utility.c
OBJECTS = $(subst .c,.o,$(SOURCES))
BIN = myshell
.PHONY: clean help

myshell : $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@ 

%.o : %.c
	$(CC) $(CFLAGS) -c $< 

all : $(BIN)

clean:
	rm -f $(OBJECTS) $(BIN) *~

help:
	@echo "Valid targets:"
	@echo "  all:    generates all binary files"
	@echo "  clean:  removes .o and binary files"
