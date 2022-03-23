CC = g++ 
CFLAGS= --std=c++14

all: characternode.h encoder.cpp
	$(CC) $(CFLAGS) encoder.cpp -o encoder
clean:
	rm -rf encoder
