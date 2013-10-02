CC=gcc
BIN=/usr/local/bin

all:	psymfg/psymfg psylat/psylat psychk/psychk psyran/psyran psyfn/psyfn psydot/psydot


install:	all
	cp	psymfg/psymfg.exe\
		psylat/psylat.exe\
		psychk/psychk.exe\
		psyran/psyran.exe\
		psydot/psydot.exe\
		psyfn/psyfn.exe	$(BIN)

clean:
	(cd psychk; make clean)
	(cd psydot; make clean)
	(cd psyfn; make clean)
	(cd psylib; make clean)
	(cd psylat; make clean)
	(cd psymfg; make clean)
	(cd psyran; make clean)

squeaky:	clean
	rm -f $(BIN)/psyran $(BIN)/psymfg $(BIN)/psylat $(BIN)/psychk $(BIN)/psyfn
	rm $(BIN)/psydot

psymfg/psymfg:	lib/psylib.o
	(cd psymfg; make)

psylat/psylat:	lib/psylib.o
	(cd psylat; make)

psychk/psychk:	lib/psylib.o
	(cd psychk; make)

psyran/psyran:	lib/psylib.o
	(cd psyran; make)

psyfn/psyfn:	lib/psylib.o
	(cd psyfn; make)

psydot/psydot:	lib/psylib.o
	(cd psydot; make)

lib/psylib.o:
	(cd psylib; make )
