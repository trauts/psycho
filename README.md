psycho
======

A suite of programs for generating psycholinguistic experiment
files from experimental and filler materials.

Introduction
------------

Some psychology experiments rely on the presentation of stimulae (materials) to many experimental subjects;
in order to prevent the ordering of these materials from affecting the experimental results, it is important that
the materials are suitably randomised, and that experimental stimulae are mixed in with suitable control stimulae (controls).

In addition, there are often a number of conditions being tested, and generating these experimental material sets for multiple subjects
is time consuming, and error prone.

This suite of software supports generating random experimental subject sets from materials and fillers, using techniques such as
latin-squares; it also supports making sure that materials from a given set aren't reused too frequently; this helps to ensure the
experimental materials are distributed evenly.



Installation
------------

You're pretty much on your own here - the code is pretty clean ANSI C
developed for in unix/gcc environment (including cygwin, and mingw).

The utilities ought to compile and run OK on other platforms, but the
scripts to drive the programs may need to be rewritten, obviously - plus,
look out for some unix dependencies in psylib/psyfile.c (getpid, time)


Watch out for broken random number generators on your platform, too.
take a good hard look at the output the first time you port this to
new hardware - and make sure it looks sane, sensible and random...

also, remember to set BIN in the makefile, and to make sure the directory
is in your PATH.

make install should build everything and put the binaries in the BIN
directory.

make clean will tidy up the source directory.
make squeaky will even go and delete the installed binaries.

oh yeah - the CFLAGS settings are probably only OK for gcc, too.

Documentation and examples
--------------------------

read doc/readme.txt for detailed instructions on how to use the psycho
suite. there are also some example template experiments to copy to make the suite usable,
plus a bit of history. the doc/template directory is a good example
to copy from scratch - make a materials subdirectory and use that.

each utility will divulge a bit of usage information if run individually,
too - of variable usefulness.


