/*
	project	:	psycho 4
	file	:	psyfile.c

	Copyright (c) 1993-1997 Stuart Boutell

	notes	:
		this file provides general file operations, and random number
		services
 */

#include	<limits.h>
#include	<time.h>
#include	<stdlib.h>
#include	<string.h>
#include	<unistd.h>
#include	<stdio.h>

#include	"sjb.h"
#include	"psyfile.h"

void
p_set_reuse( file *f, int base, int variance )
{
	int	i, lines, quart, nineth;

	lines	= f->lines;
	quart	= lines/4;
	nineth	= lines/9;

	for (i=0; i<P_REUSE_MAX; i++)
		f->reuse.reuse[i] = 0;

	switch(variance)
	{
		case	0:		/* files appear regularly, every BASE ticks */
			f->reuse.reuse[base] = lines;
			break;

		case	1:
			if (!quart)
				HALT ("too few lines for variance of 1");
			if ((base+1)>=P_REUSE_MAX)
				HALT ("P_REUSE_MAX exceeded (varilength problem?)");
			f->reuse.reuse[base-1] = quart;
			f->reuse.reuse[base]   = lines - quart - quart; /* catch rounding */
			f->reuse.reuse[base+1] = quart;
			break;
		case	2:
			if (!nineth)
				HALT ("too few lines for variance of 2");
			if ((base+2)>=P_REUSE_MAX)
				HALT ("P_REUSE_MAX exceeded (varilength problem?)");
			f->reuse.reuse[base-2] = nineth;
			f->reuse.reuse[base-1] = 2*nineth;
			f->reuse.reuse[base]   = lines - (6*nineth);	/* NB this catches rounding */
			f->reuse.reuse[base+1] = 2*nineth;
			f->reuse.reuse[base+2] = nineth;
			break;
		default:
			HALT("unknown variance");
			break;
	}

#ifdef	DEBUG
	for (i=0; i<P_REUSE_MAX; i++)
		printf("[%s] [%d] %d\n", f->name, i, f->reuse.reuse[i] );
#endif
}

static	int	rand_init = 0;

unsigned int
p_rand( int limit )
{
	if (!rand_init)
		HALT("p_rand called without p_reset_rand");

	if (limit>0)
		return( ((unsigned int)random() % limit) );
	else
		return( 0 );
}

void
p_reset_rand(void)
{
	unsigned int	seed;

	rand_init++;

	seed = (unsigned int) time( (time_t *) 0 ) + (((unsigned int)getpid())<<8);
	srandom( seed );

#ifdef	RAND_MAX	/* POSIX systems have this - which is nice */
	seed += p_rand( RAND_MAX );
#else				/* skanky pre POSIX systems do not. */
	seed += p_rand( 1024000 );
#endif
	srandom( seed );
}

void
p_put_exp( file *out, file *src )
{
	int		i, this, remaining;
	line	*out_l, *src_l;

	out_l = out->line1;
	src_l = src->line1;

	for( remaining=src->lines; remaining !=0 ; remaining-- )
	{
		this = p_rand(remaining);

/*	find this number of spaces to insert */
		for ( i=0; i<P_REUSE_MAX; i++)
		{
			if (this < src->reuse.reuse[i])
				break;

			this -= src->reuse.reuse[i];
		}
		if (i==P_REUSE_MAX)
		{
			HALT("overran");
		}
		(src->reuse.reuse[i]) --;

		if (remaining == src->lines)
		{	/* 1st time around, we allow (randomly) less lines */
			i = p_rand(i);
		}

/* advance the i lines, and then insert the src line */
		for (; i ; i--)
			out_l=out_l->next;

		if (!src_l)
			HALT("src barf 1");

		if (out_l->str)		/* overwrite! */
			HALT("overwright barf");

		out_l->str  = src_l->str;
		out_l->file = src_l->file;

/* advance the source, and dest */
		src_l = src_l -> next;
		out_l = out_l -> next;
	}

	if (src_l)
		HALT("src barf 2");
}

void
p_put_fil( file *out, file *src, int base, int variance )
{
	int		i, remaining;
	line	*out_l, *src_l;

	out_l = out->line1;
	src_l = src->line1;

	for( remaining=src->lines; remaining !=0 ; remaining-- )
	{
		i = base - variance + p_rand(variance*2);		/* smallish */

	/* skip  this far on */
		for (; i ; i--)
			out_l = out_l->next;

	/* now skip to free line */
		for (; out_l->str ;)
			out_l = out_l->next;

	/* write it away */
		out_l->str  = src_l->str;
		out_l->file = src_l->file;

	/* advance the source, and dest (for what it is worth) */
		src_l = src_l -> next;
		out_l = out_l -> next;
	}

	if (src_l)
		HALT("src barf 2");
}

int
p_analyse( file *f )
{
	int		flines, lines;
	line	*l;

	lines = 0;
	flines = f->lines;

	for (l=f->line1; l; l=l->next)
	{
		lines++;
		printf("[%s] %s\n", f->name, l->str?l->str:"NULL" );
	}

	if(flines != lines)
		HALT("mismatched lines");

	return flines;
}

line *
p_new_line( char *contents, file *file )
{
	char	*str;
	line	*new;

	new = malloc (sizeof(line));
	if (!new)
	{
		return 0;
	}

	if (contents)
	{
		str = malloc (strlen(contents) + 1);
		if (!str)
		{
			free (new);
			return 0;
		}

		strcpy( str, contents );
		new->str = str;
	} else {
		new->str = 0;		/* to be filled in later */
	}

	new->file = file;
	new->next = 0;
	return (new);
}

file *
p_blank_file (char *name, int lines)
{
	int		i;
	line	*l, *last;
	file	*f;

	f = malloc (sizeof (file));
	if (!f)
		return (0);

/* initialise bits and bobs */
	f->index=0;			/* output file has no index */
	f->name = name;		/*	the output file name */

	f->lines= 0;		/* no lines yet */
	f->line1= 0;		/* so no pointer to a line */

	f->cur  = 0;		/* line used in generation */
	f->next = 0;		/* pointer to next file */

	f->reuse.expfile = 0;
	for (i=0; i<P_REUSE_MAX; i++)
		f->reuse.reuse[i] = 0;

/* make the new (blank) lines */
	last = 0;
	for (i=0; i<lines; i++)
	{
		l=p_new_line( 0, 0 );

		if (f->lines)
		{
			last->next = l;
		} else {
			f->line1 = l;
		}

		f->lines++;
		last = l;
	}

/*	AND tie around in a loop */
	last->next = f->line1;

	return(f);
}

int
p_write_file( file *file )
{
	int		i;
	line	*l;
	FILE	*stream;

/* open the stream */
	stream = fopen( file->name, "w" );
	if (!stream)
	{
		perror(file->name);
		return(0);
	}

	l = file->line1;
	for (i=0; i< file->lines; i++)
	{
		fprintf( stream, "%s", l->str?l->str:"<NULL>\n" );
		l = l->next;
	}

	fclose (stream);
	return (1);
}

/* read a file in */
file	*
p_read_file( char *name, int index, int expfile, int wrap)
{
	int	i;
	FILE	*stream;
	file	*f;
	line	*l, *last_line;
	char	*linebuf;

/* open the stream */
	stream = fopen( name, "r" );
	if (!stream)
	{
		return (0);
	}

/* get the internal descriptor */
	f = malloc (sizeof (file));
	if (!f)
	{
		fclose( stream );
		return (0);
	}

/* initialise bits and bobs */
	f->name = name;		/* should be OK to use the passed name... */
	f->index = index;

	f->lines= 0;		/* no lines yet */
	f->line1= 0;		/* so no pointer to a line */

	f->cur  = 0;		/* line used in generation */
	f->next = 0;		/* pointer to next file */

	f->reuse.expfile = expfile;
	for (i=0; i<P_REUSE_MAX; i++)
		f->reuse.reuse[i] = 0;

/* read the file */
	linebuf = malloc( 10240 );	/* that better be enough. */
	if (!linebuf)
	{
		free (f);
		fclose (stream);
		return (0);
	}

	last_line = 0;

	while( fgets( linebuf, 10240, stream ) )
	{
		l = p_new_line( linebuf, f );
		if (!l)
		{
			free (linebuf);
			free (f);
			fclose (stream);
			return (0);
		}

		if (f->lines)
		{
			last_line->next = l;
		} else {
			f->line1 = l;
		}

	/* inc counter and last */
		f->lines ++;
		last_line = l;
	}

	if (!f->lines)
		HALT("empty file");
	
	if (wrap)
	{
		last_line->next = f->line1;
	}

	free (linebuf);
	fclose ( stream );
	return (f);
}

