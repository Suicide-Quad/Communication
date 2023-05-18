CC=gcc
CFLAGS=-Wall
EXEC=main
LDFLAGS=

all: $(EXEC)

main: communication.o  main.o 
	$(CC) -o $@ $^ $(LDFLAGS)

main.o: main.c
	$(CC) -c $^ $(CFLAGS)

communication.o: communication.c 
	$(CC) -c $^ $(CFLAGS)

clean:
	rm -rf *.o
	rm -rf $(EXEC)