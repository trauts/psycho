# target object files

TARGET=psylib.o

OBJ=sjb.o psyfile.o psycheck.o

CFLAGS=-pedantic -Wall -I../include

all:	$(TARGET)

install:	
	echo write installer

$(TARGET): $(OBJ)
	$(LD) -r -o $(TARGET) $(OBJ)

clean:
	rm -rf $(TARGET) $(OBJ)

psyfile.o:	psyfile.c ../include/sjb.h ../include/psyfile.h ../include/psy.h 
psycheck.o:	psycheck.c ../include/sjb.h ../include/psyfile.h ../include/psy.h
sjb.o:	sjb.c ../include/sjb.h
