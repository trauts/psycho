# target object files

TARGET=psymfg

OBJ=psymfg.o

CFLAGS=-pedantic -Wall -I../include

all:	$(TARGET)

install:	
	echo write installer

$(TARGET): $(OBJ) ../psylib/psylib.o
	$(CC) -o $(TARGET) $(OBJ) ../psylib/psylib.o

clean:
	rm -rf $(TARGET) $(OBJ)

psymfg.o:	psymfg.c ../include/sjb.h ../include/psyfile.h ../include/psy.h
