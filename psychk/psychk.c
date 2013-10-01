/*
	project	:	psycho 4
	file	:	psychk.c

	Copyright (c) 1997 Stuart Boutell

	notes	:
		this file performs the metafile generation and ultimately the
		experimental files.
 */

#include	<limits.h>
#include	<time.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>
#include	<stdio.h>
#include	"sjb.h"

#include	"psy.h"
#include	"psyfile.h"

static	file 	*files = NULL;

static	void
p_usage(char *msg)
{
	printf("\nerror: %s\n\n", msg );
	usage( PSYCHK_PROG, "<sets> <items> <file> [<file> ...]" );
	printf("  where <sets>  is the number of sets in a file\n");
	printf("        <items> is the number of items per set\n");
	printf("        <file> is a filename to check\n");
	version( PSYCHK_PROG, PSYCHK_VERSION, PSYCHK_DATE );
	copyright( 1997, PSYCHK_YEAR );
	exit(1);
}

int
main( int argc, char *argv[] )
{
	int	i, num_sets, num_items;
	file *last, *this;

	if ( argc< 3 )
	{
		p_usage("not enough args");
	}

	num_sets  = strtol( argv[1], 0, 0 );
	num_items = strtol( argv[2], 0, 0 );

	if (! num_sets) 
	{
		p_usage("zero set counts not allowed");
	}

	if (! num_items) 
	{
		p_usage("zero item counts not allowed");
	}

/* now read files */
	last = files;
	for (i=3; i<argc; i++)
	{

	/* read */
		this = p_read_file( argv[i], 0, 0, 0 );
		if (! this )	/* no file! */
		{
			p_usage( "file not found" );
		}

	/* insert */
		if (i!=3)
		{
			last->next = this;
			last = this;
		} else {
			last = files = this;
		}
	}

/* now check the files */
	for(this=files; this; this=this->next)
	{
		p_check( this, num_sets, num_items, 1 );	/* do the check, be verbose */
	}
	return (0);
}
