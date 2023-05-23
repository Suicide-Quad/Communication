CC = g++
CFLAGS = -Wall
EXEC_NAME = detect generate calibrate
CFLAGS = `pkg-config --cflags opencv4`
LIBS = `pkg-config --libs opencv4`
OBJ_FILES = detect.o generate.o calibrate.o
INSTALL_DIR = /usr/bin

all: $(EXEC_NAME)

clean:
	rm $(EXEC_NAME) $(OBJ_FILES)

detect: detect.o
	$(CC) -o detect detect.o $(LIBS)

generate: generate.o
	$(CC) -o generate generate.o $(LIBS)

calibrate: calibrate.o
	$(CC) -o calibrate calibrate.o $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

%.o: %.cc
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

%.o: %.c
	gcc $(CFLAGS) $(INCLUDES) -o $@ -c $<

install :
	cp $(EXEC_NAME) $(INSTALL_DIR)
