MAIN = webserver

OBJS = $(MAIN).o client_handler.o request_parser.o responder.o

CC = gcc
CFLAGS = -g -Wall -Wextra


# compile commands

all: rebuild

build: $(MAIN)

rebuild: clean build


# dependencies

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


# remove generated files

clean:
	rm -f $(MAIN) $(OBJS)
