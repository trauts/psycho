============================
psycho 5.00 - 01st Oct 2013
============================

Copyright (c) Stuart Boutell 1993,1994,1995,1996,1997,1998,2004,2013. 

This release (version 5.00) is the first under the MIT License.

see LICENSE for the license conditions

email contact: git@stuartb.org

introduction
------------

psycho is a suite of programs for generating psycholinguistic experiment
files/subject booklets from experimental and filler materials.

installation
------------

you're pretty much on your own here - the code is pretty clean ANSI C
developed for in unix/gcc environment - the utilities ought to compile
and run OK on other platforms, but the scripts to drive the programs
will need to be rewritten, obviously. plus, look out for some unix
dependencies in psylib/psyfile.c (getpid, time)

try invoking make for starters...

watch out for broken random number generators on your platform, too.
take a good hard look at the output the first time you port this to
new hardware - and make sure it looks sane, sensible and random...

also, remember to set BIN in the makefile, and to make sure the directory
is in your PATH.

make install should build everything and put the binaries in the BIN
directory.

make clean will tidy up the source directory.
make squeaky will even go and delete the installed binaries.

oh yeah - the CFLAGS settings are probably only OK for gcc, too.

documentation
-------------
read doc/readme.txt for detailed instructions on how to use the psycho
suite. there are also some examples to copy to make the suite usable,
plus a bit of history. the doc/template directory is a good example
to copy from scratch - make a materials subdirectory and use that.

each utility will divulge a bit of usage information if run individually,
too - of variable usefulness.

credits
-------

This suite was developed for Professor Holly Branigan; the fact she is my 
beloved wife has everything to do with it. =)

