/*
	project	:	psycho 4
	file	:	psyran.c

	Copyright (c) 1997,1998 Stuart Boutell

	notes	:
		this file performs the internal randomisation
		of the materials.
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

/*
	shuffle items from item offsets in different sets with
	each other
 */
static	file *
p_random_copy(	file *f, int num_sets, int num_items, int subject )
{
	file *result;

/*
	do internal shuffle of original - keeps on happening, so should be fun..
 */
 	{
 		char	*hold;
 		line	*src_l;
 		line	*dst_l;
 		int	i,j,k;

 		src_l = dst_l = f->line1;

	/* do num_sets loops */
 		for (i=0; i<num_sets; i++)
 		{
	 	/* of swaps of every item in every set with another one */
	 		for (j=0; j<(num_sets*num_items); j++)
	 		{
	 			dst_l = src_l;			/* start from ourselves */

	 			k= 1+p_rand( num_sets / num_items );	/* who to swap with */
	 			if (k == (num_sets/num_items) )
	 				k--;

				k = k * num_items;
	 			for (; k; k--)
	 				dst_l=dst_l->next;

	 			if (dst_l == src_l)
	 			{
	 				printf("missed me\n");
	 			} else {
	 				hold = src_l->str;
	 				src_l->str = dst_l->str;
	 				dst_l->str = hold;
	 			}
	 			src_l = src_l ->next;	/* src skips forward */
	 		}
	 	}
	}
/*
	now make a copy, doing shuffles on the way
 */
	{
		line	*src_l;
		line	*dst_l;
		int		j;

		unsigned int	swp_src, swp_dst;

		char	**dups;
		char	*dup_tmp;

		dups = malloc( sizeof(char *)*num_sets );
		if (!dups)
			HALT("malloc");

		result = p_blank_file( "", num_sets );

		src_l=f->line1;
		dst_l=result->line1;

		/* copy in num_items objects */
		for(j=0; j<num_sets; j++)
		{
			dups[j]	= src_l->str;
			src_l	= src_l->next;
		}

		/* do some swaps */
		for (j=4+p_rand(num_sets*2); j; j--)
		{
			/* swap a pair */
			swp_src=p_rand(num_sets-1);
			swp_dst=swp_src + 1 + p_rand(num_sets-1-swp_src);

			if (swp_src >= num_sets)
				HALT("swap error (source)");

			if (swp_dst >= num_sets)
				HALT("swap error (destination)");

			dup_tmp = dups[swp_src];
			dups[swp_src] = dups[swp_dst];
			dups[swp_dst] = dup_tmp;
		}

		/* lay them down */
		for(j=0; j<num_sets; j++)
		{
			dst_l->str = dups[j];
			dst_l	= dst_l->next;
		}
	}

	return (result);
}

static	void
p_usage(char *msg)
{
	printf("\nerror: %s\n\n", msg );
	usage( PSYRAN_PROG, "<sets> <items> <subjects> <file> [<file> ...]" );
	printf("  where <sets>     is the number of sets in a file\n");
	printf("        <items>    is the number of items per set\n");
	printf("        <subjects> is the number of subjects to create\n");
	printf("        <file>     is a latin-squered filename to make from\n\n");
	printf("the randomised is always put in the 'random' subdirectory\n");
	printf("\n");
	printf("NB! the input files are actually assumed to be single item files\n");
	printf(" (ie the output of the latin square) - so therefore they will only\n");
	printf(" be allowed to have <sets> number of objects in them...\n");
	version( PSYRAN_PROG, PSYRAN_VERSION, PSYRAN_DATE );
	copyright( 1993, PSYRAN_YEAR );
	exit(1);
}

int
main( int argc, char *argv[] )
{
	int	i, num_sets, num_items, num_subjects, subject;
	file *last, *this;

	if ( argc< 4 )
	{
		p_usage("not enough args");
	}

	num_sets  = strtol( argv[1], 0, 0 );
	num_items = strtol( argv[2], 0, 0 );
	if (num_sets % num_items)
	{
		p_usage("number of sets MUST be divisible by number of items");
	}
	num_subjects = strtol( argv[3], 0, 0 );
	if (num_subjects % num_items)
	{
		p_usage("number of subjects MUST be divisible by number of items");
	}

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
		p_usage("zero subjects not very clever");
	}

/* now read files */
	last = files;
	for (i=4; i<argc; i++)
	{

	/* read */
		this = p_read_file( argv[i], 0, 0, 1 );	/* NB! WRAP THE FILE */
		if (! this )	/* no file! */
		{
			p_usage( "file not found" );
		}

		if (this->lines != num_sets)
		{
			printf("%s : %d lines - expecting %d\n",
				this->name, this->lines, num_sets );
			p_usage ("file was different size than expected");
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

	p_reset_rand();

/* now shuffle them */
	mkdir("random", 0755);

	for(subject=0; subject<num_subjects; subject++)
	{
		for(this=files; this; this=this->next)
		{
			file *new;
			char *name;
			char *newname;

			new = p_random_copy( this, num_sets, num_items, subject );


		/* make name */
			name = strrchr( this->name, '/' );
			if (!name)
				name=this->name;
			else
				name++;	/* skip da slash */

			newname = malloc(strlen(name) + strlen("random/000-")+1);
			if (!newname)
				HALT("malloc");
			sprintf(newname, "random/%03d-%s", subject, name);
			new->name = newname;

			p_write_file( new );
		}

		fast_viswait();		/* show some progress to user */
	}

	return (0);
}
