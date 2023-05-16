CC = g++
CFLAGS = -Wall
EXEC_NAME = test 
CFLAGS = `pkg-config --cflags opencv4 gtk+-2.0` -D WITH_GTK=ON
LIBS = `pkg-config --libs opencv4 gtk+-2.0` -D WITH_GTK=ON
OBJ_FILES = main.o 
INSTALL_DIR = /usr/bin

all : $(EXEC_NAME)

clean :
	rm $(EXEC_NAME) $(OBJ_FILES)

$(EXEC_NAME) : $(OBJ_FILES)
	$(CC) -o $(EXEC_NAME) $(OBJ_FILES) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

%.o: %.cc
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $<

%.o: %.c
	gcc $(CFLAGS) $(INCLUDES) -o $@ -c $<

install :
	cp $(EXEC_NAME) $(INSTALL_DIR)
