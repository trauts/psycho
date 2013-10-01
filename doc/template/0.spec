#! /bin/sh

sets=24			# this is how many MATERIALS are in each file.
items=4			# this is how many VERSIONS of each MATERIAL you have
subjects=80		# this is how many booklets/randomized sets that are required

#	the following 3 are only used in the 'dot_me' stage
# use
#	preamble=-       for normal operation with no preamble file
#	preamble=-0      to create files in quotes. (this affects 5.dot_me)
preamble=-0
splitstr=". "
splitpoint=-1		# the minus sign is important - it means strip the tag
