CFLAGS = -Wall -g -std=c99 -D _POSIX_C_SOURCE=200809L -Werror

all: build

build: 
	gcc $(CFLAGS) UnixLs.c -o UnixLs

run: build
	./UnixLs

demo:
	gcc infodemo.c -o demo
	./demo

valgrind: build
	valgrind --leak-check=full ./UnixLs

clean: 
	rm -f UnixLs demo