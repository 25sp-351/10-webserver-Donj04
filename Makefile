MAIN = webserver

OBJS = $(MAIN).o client_handler.o request_parser.o responder.o process_request.o

CC = gcc
CFLAGS = -g -Wall -Wextra


# compile commands

rebuild: clean build
r: rebuild

build: $(MAIN)
b: build


# dependencies

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


# remove generated files

clean:
	rm -f $(MAIN) $(OBJS)
c: clean


.PHONY: rebuild build clean r b c
