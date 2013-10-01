/*
	project	:	psycho 4
	file	:	psylat.c

	Copyright (c) 1997 Stuart Boutell

	notes	:
		this file performs the latin square file generation 
		from the materials.
 */

#include	<limits.h>
#include	<time.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>
#include	<stdio.h>

#include	<sys/stat.h>	/* for mkdir */
#include	"sjb.h"

#include	"psy.h"
#include	"psyfile.h"

static	file 	*files = NULL;

static	file	*
p_latin_copy( file *f, int sets, int items )
{
	char	*thisname;
	file	*chain;
	file	*this, *last;
	int	i;

	char	*name;
	name = strrchr( f->name, '/');
	if (!name)
		name = f->name;

	last = chain = 0;

	for(i=0; i<items; i++)
	{

/* make name */
		thisname = malloc(strlen(name) + strlen("latin_x")+1);
		if (!thisname)
			HALT("malloc");
		sprintf(thisname, "latin%s-%c", name, 'A'+i);

/* make blank */
		this = p_blank_file( thisname, sets );

/* hook in */
		if (chain)
		{
			last->next = this;
			last = this;
		} else {
			chain = last = this;
		}
	}

/* now fill them in ! */
	{
		int		offset;
		int		 j,k;
		line	*src_l;
		line	*dest_l;

		this=chain;

		for (i=0; i<items; i++, this = this->next)
		{
			src_l =f->line1;
			dest_l=this->line1;
			offset=i % items;

			for (j=0; j<sets; j++)
			{
				for (k=0; k<offset; k++)
					src_l=src_l->next;

				/* the copy ! */
				dest_l->str = src_l->str;

				offset = (offset+1) % items;
				dest_l = dest_l->next;

				for (; k<(items+1); k++)
					src_l  = src_l->next;	/* skip on */
			}
		}
	}
	return(chain);
}



static	void
p_usage(char *msg)
{
	printf("\nerror: %s\n\n", msg );
	usage( PSYLAT_PROG, "<sets> <items> <file> [<file> ...]" );
	printf("  where <sets>  is the number of sets in a file\n");
	printf("        <items> is the number of items per set\n");
	printf("        <file> is a filename to check\n\n");
	printf("the latin squared output is always put in the 'latin' subdirectory\n");
	version( PSYLAT_PROG, PSYLAT_VERSION, PSYLAT_DATE );
	copyright( 1993, PSYLAT_YEAR );
	exit(1);
}

int
main( int argc, char *argv[] )
{
	int	i, num_sets, num_items, fail;
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
		if (!files)
		{
			last = files = this;
		} else
		{
			last->next = this;
			last = this;
		} 
	}

/* now check the files */
	fail=0;
	for(this=files; this; this=this->next)
	{
		if (!p_check( this, num_sets, num_items, 0))
		{
			printf("%s: %s contains errors - run psychk on it.\n", PSYLAT_PROG,this->name);
			fail++;
		}
	}
	if	(fail)
		return(1);

/* now latin square them */
	mkdir("latin", 0755);

	for(this=files; this; this=this->next)
	{
		file *new;

		new = p_latin_copy( this, num_sets, num_items );

		for (; new; new=new->next)
		{
			p_write_file( new );
		}
	}

	return (0);
}
