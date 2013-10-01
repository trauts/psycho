# target object files

TARGET=psychk

OBJ=psychk.o

CFLAGS=-pedantic -Wall -I../include

all:	$(TARGET)

install:	
	echo write installer

$(TARGET): $(OBJ) ../psylib/psylib.o
	$(CC) -o $(TARGET) $(OBJ) ../psylib/psylib.o

clean:
	rm -rf $(TARGET) $(OBJ)

psychk.o:	psychk.c ../include/sjb.h ../include/psyfile.h ../include/psy.h
