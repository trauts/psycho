#! /bin/sh

sets=32			# this is how many MATERIALS are in each file.
items=4			# this is how many VERSIONS of each MATERIAL you have
# NB! Subjects must be divisible by ITEMS
subjects=64		# this is how many sets of booklets you get (items x subjects booklets made)


#	the following 3 are only used in the 'dot_me' stage
preamble=-
splitstr=". "
splitpoint=-1		# the minus sign is important - it means strip the tag
