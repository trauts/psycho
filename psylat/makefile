# target object files

TARGET=psylat

OBJ=psylat.o

CFLAGS=-pedantic -Wall -I../include

all:	$(TARGET)

install:	
	echo write installer

$(TARGET): $(OBJ) ../psylib/psylib.o
	$(CC) -o $(TARGET) $(OBJ) ../psylib/psylib.o

clean:
	rm -rf $(TARGET) $(OBJ)

psylat.o:	psylat.c ../include/psy.h ../include/psyfile.h ../include/sjb.h
