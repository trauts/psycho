# target object files

TARGET=psyran

OBJ=psyran.o

CFLAGS=-pedantic -Wall -I../include

all:	$(TARGET)

install:	
	echo write installer

$(TARGET): $(OBJ) ../psylib/psylib.o
	$(CC) -o $(TARGET) $(OBJ) ../psylib/psylib.o

clean:
	rm -rf $(TARGET) $(OBJ)

psyran.o:	psyran.c ../include/psy.h ../include/psyfile.h ../include/sjb.h
