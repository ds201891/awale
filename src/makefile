CC=gcc
CFLAGS=-O3 -lpthread -D_REENTRANT -fopenmp -Wall
LDFLAGS=-fopenmp -Wall
EXEC=awale

all: $(EXEC)

awale_lib.o: awale_lib.c awale_lib.h
	$(CC) -o awale_lib.o -c awale_lib.c $(CFLAGS)
	
benchmark.o: benchmark.c benchmark.h awale_lib.h
	$(CC) -o benchmark.o -c benchmark.c $(CFLAGS)

main.o: main.c benchmark.h awale_lib.h
	$(CC) -o main.o -c main.c $(CFLAGS)

awale: awale_lib.o benchmark.o main.o
	$(CC) -o awale main.o benchmark.o awale_lib.o $(LDFLAGS)

clean:
	rm -rf *.o $(EXEC)
