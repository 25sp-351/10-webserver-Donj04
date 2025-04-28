MAIN = webserver

OBJS = $(MAIN).o client_handler.o request_parser.o

CC = gcc
CFLAGS = -g -Wall -Wextra


# compile commands

all: build

build: $(MAIN)

rebuild: clean build


# dependencies

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

$(MAIN).o: $(MAIN).c client_handler.o request_parser.o

client_handler.o: client_handler.c client_handler.h request_parser.h

request_parser.o: request_parser.c request_parser.h client_handler.h


# remove generated files

clean:
	rm -f $(MAIN) $(OBJS)
