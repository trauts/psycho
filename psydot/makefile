# target object files

TARGET=psydot

OBJ=psydot.o psyolddot.o

CFLAGS=-pedantic -Wall -I../include

all:	$(TARGET)

install:	
	echo write installer

$(TARGET): $(OBJ) ../psylib/psylib.o
	$(CC) -o $(TARGET) $(OBJ) ../psylib/psylib.o

clean:
	rm -rf $(TARGET) $(OBJ)

psydot.o:	psydot.c ../include/psy.h ../include/psyfile.h ../include/sjb.h

psyolddot.o:	psyolddot.c
