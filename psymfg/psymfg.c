/*
	project	:	psycho 4
	file	:	psymfg.c

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
	usage( "psymfg", "<n1> <n2> <out> <file> [<file> ...]" );
	printf("  where <n1>   is the number of files\n");
	printf("        <n2>   is the number of important files\n");
	printf("        <out>  is the output file\n");
	printf("        <file> is a filename - up to %d allowed\n", P_FILE_MAX);
	version( "psymfg", PSYMFG_VERSION, PSYMFG_DATE );
	copyright( 1993, PSYMFG_YEAR );
	exit(1);
}

int
main( int argc, char *argv[] )
{
	int	num_files;
	int	num_exp;
	int	num_lines;
	int	i;
	file *last, *this;
	file *outfile;

	if ( argc< 4 )
	{
		p_usage("not enough args");
	}

/* check args */
	num_files = strtol( argv[1], NULL, 0 );
	num_exp  = strtol( argv[2], NULL, 0 );

	if (!num_files ||
		!num_exp)
	{
		p_usage( "zero files is not allowed\n");
	}

	if (num_exp != 1)
	{
		p_usage( "sorry! only one experimental file allowed in this release");
	}

	if ( num_files > P_FILE_MAX )
	{
		p_usage( "too many files" );
	}
	if ( num_exp > num_files )
	{
		p_usage( "experimental files greater than files");
	}
	if ( argc != 4+num_files )
	{
		p_usage( "file count mismatch to files" );
	}

/* set random number generator seed */
	p_reset_rand();

/* now read files */
	num_lines = 0;		/* total number of lines */
	last = files;
	for (i=0; i<num_files; i++)
	{

	/* read */
		this = p_read_file( argv[4+i], i,  (i<num_exp), 0 );
		if (! this )	/* no file! */
		{
			p_usage( "file not found" );
		}


	/* insert */
		if (i)
		{
			last->next = this;
			last = this;

			if (this->lines != files->lines)
				HALT("support for variable length files not available");

		} else {
			last = files = this;
		}
		num_lines += this->lines;
	}

#ifdef	NEVER
	/* now loop the file around */
	this->next = files;
#endif


/* generate the template output file */
	outfile = p_blank_file( argv[3], num_lines );

/* now fill the output file with output */
	this = files;
	for (i=0; i<num_files; i++)
	{
		if (this->reuse.expfile)
		{
			p_set_reuse( this, num_files-1, 1 );	/* set reuse fields */
			p_put_exp( outfile, files ); /* fill with experimental files */
		}
		else
			p_put_fil( outfile, this, num_files-1, 1  ); /* a filler */

		this = this->next;
	}

/* scribble it to the hard disk */
	(void) p_write_file( outfile );

	return(0);
}
