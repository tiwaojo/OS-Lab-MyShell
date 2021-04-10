CC = gcc
CFLAGS = -Wall -Wextra -std=gnu99 #-pthread
LFLAGS = 
LIBS = 
SOURCES = banker.c #utility.c queue.c sigtrap.c #test.c
OBJECTS = $(subst .c,.o,$(SOURCES))
BIN = banker
# EXE = process hostd #test
.PHONY: clean help

# .PHONY: debug
# debug: CFLAGS += -O0 -g3
# debug: $(EXE)

banker : $(OBJECTS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@ -lpthread

%.o : %.c
	$(CC) $(CFLAGS) -c $< 

all : $(BIN)

clean:
	rm -f $(OBJECTS) $(BIN) *~

help:
	@echo "Valid targets:"
	@echo "  all:    generates all binary files"
	@echo "  debug:  generates debug binary files"
	@echo "  clean:  removes .o and .exe files"
