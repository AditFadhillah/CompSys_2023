# GCC=gcc -g -Wall -Wextra -pedantic -std=gnu11 
GCC=gcc -g -Wall -Wextra -pedantic -std=gnu11 -O



all: simulate main
rebuild: clean all

# main uses simulate
main: *.c 
	$(GCC) *.c -o main

simulate: *.c
	$(GCC) *.c -o simulate


zip: ../src.zip

../src.zip: clean
	cd .. && zip -r src.zip src/Makefile src/*.c src/*.h

clean:
	rm -rf *.o main vgcore*
	rm -rf *.o simulate vgcore*