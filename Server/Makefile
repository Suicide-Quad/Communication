CC=gcc
CFLAGS=-Wall
EXEC=main
LDFLAGS= -lm
SRC=$(shell find . -name "*.c")
OBJ=$(patsubst %.c, %.o, $(SRC))

all: $(EXEC)	
	./$(EXEC)

main: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)
	
%.o: %.c 
	$(CC) -c $^ $(CFLAGS)
	
clean:
	rm -rf *.o
	rm -rf */*.o
	rm -rf $(EXEC)
