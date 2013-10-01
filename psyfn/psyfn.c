/*
	project	:	psycho 4
	file	:	psyfn.c

	Copyright (c) 1997 Stuart Boutell

	notes	:
		this file generates the horribe filename lines
 */

#include	<limits.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>
#include	<stdio.h>
#include	"sjb.h"
#include	<sys/stat.h>

#include	"psy.h"

static	void
p_usage(char *msg)
{
	printf("\nerror: %s\n\n", msg );
	usage( PSYFN_PROG, "<sets> <items> <subjects> <binary> <file> [<file> ...]" );
	printf("  where <sets>     is the number of sets in a file\n");
	printf("        <items>    is the number of items per set\n");
	printf("        <subjects> is the number of subjects\n");
	printf("        <binary>   is the executable to involke\n");
	printf("        <file>     is the material filenames\n");
	version( PSYFN_PROG, PSYFN_VERSION, PSYFN_DATE );
	copyright( 1997, PSYFN_YEAR );
	exit(1);
}

int
main( int argc, char *argv[] )
{
	char	cmdline[1024];
	int	num_sets, num_items, num_subjects;
	int	i, subj, item;
	char	*binary;

	if ( argc< 5 )
	{
		p_usage("not enough args");
	}

	num_sets  = strtol( argv[1], 0, 0 );
	num_items = strtol( argv[2], 0, 0 );
	num_subjects = strtol( argv[3], 0, 0 );
	binary		= argv[4];

	if (! num_sets) 
	{
		p_usage("zero set counts not allowed");
	}

	if (! num_items) 
	{
		p_usage("zero item counts not allowed");
	}

	if (! num_subjects) 
	{
		p_usage("zero subject counts not allowed");
	}

	mkdir ("output", 0755);

	for (subj=0; subj<num_subjects; subj++)
	{
		for (item=0; item<num_items; item++)
		{
			sprintf(cmdline, "%s %d 1 output/%03d-%c ", binary, argc-5,
				subj, 'A'+item);
			for (i=5; i<argc; i++)
			{
				char	tmp[256];
				char	*fn;
				fn=strrchr( argv[i], '/');
				if (fn)
					fn++;
				else
					fn=argv[i];	/* should never happen(TM) */

				sprintf(tmp, "random/%03d-%s-%c ", subj, fn, 'A'+item );
				strcat (cmdline, tmp);
			}
			if (system(cmdline) != 0)
			{
				printf("\n\noops: %s\n", cmdline);
				HALT("system() faled");
			}
			fast_viswait();
		}
	}
	return (0);
}
