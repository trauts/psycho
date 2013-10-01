/*
	project	:	psycho 4
	file	:	psydot.c

	Copyright (c) 1997,1998 Stuart Boutell

	notes	:
		this does the "dotting"
 */

#include	<ctype.h>
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
#include	"psydot.h"

static	file 	*files = NULL;

static	int	p_line = 0;


#define	DOT_PAGE_TRUNCATE	79
#define	DOT_PAGE_LENGTH		88

static	void
p_put_newline (FILE *stream)
{
	fputs ("\n", stream);
	p_line ++;
}

static	void
p_put_page (FILE *stream)
{
	for (; p_line % DOT_PAGE_LENGTH;)
	{
		p_put_newline (stream);
	}
}

static	void
p_put_dotless_line (FILE *stream, char *line)
{
	fputs (line, stream);
	p_line++;
}

static	void
p_dot_out( file *preamble, file *file,
	char *splitchar, int splitpoint,
	int tagless, int mode )
{
	char	*basename;
	char	*dotname;

	FILE	*stream;
	line	*outline;
	char	*outchar;
	int		totchar;


/* make dotfile filename */
	basename = strrchr( file->name, '/');
	if (!basename)		/* no slash */
		basename = file->name;

	dotname = malloc(strlen(basename) + strlen("dotted.dot")+1);
	if (!dotname)
		HALT("malloc");
	sprintf(dotname, "dotted%s.dot", basename);

/* open for write */
	stream = fopen (dotname, "w");
	if (!stream)
	{
		perror(dotname);
		STOP;
	}

/*	dump the preamble */
	if (preamble)
	{
		for (outline = preamble->line1; outline; outline=outline->next)
		{
			p_put_dotless_line (stream, outline->str);
		}
		p_put_page (stream);
	}

/*	strip the tags, if needs be */
	if (tagless)
	{
		for (outline = file->line1; outline; outline=outline->next)
		{
			outchar = outline->str;

			for ( ; *outchar && !((*outchar==' ')||(*outchar=='\t')); outchar++)
				;

			if ( *outchar )
				outchar++;

			if ( !*outchar )
				outline->str = "";
			else
				outline->str = outchar;	/* dirty */
		}
	}

/* fill a buffer with the ENTIRE file (!) */
	totchar = 0;
	for (outline=file->line1; outline; outline=outline->next)
	{
		totchar += strlen( outline->str );
	}

	outchar = malloc( totchar );
	if (!outchar)
	{
		HALT("malloc of large number");
	}
	
	*outchar = '\0';
	for (outline=file->line1; outline; outline=outline->next)
	{
		strcat( outchar, outline->str );
	}

	p_original_dotty( stream, outchar, splitchar, splitpoint, mode );

	(void) free (outchar);

	(void) fclose (stream);
}

static	void
p_usage(char *msg)
{
	printf("\nerror: %s\n\n", msg );
	usage( PSYDOT_PROG, "<splitstring> <splitpoint> <preamble> <file> [<file> ...]" );
	printf("        <splitstring> is the string to split at\n");
	printf("        <splitpoint> is the point in the <splitstring> to do the split\n");
	printf("                     (add a '-' to the <splitpoint> to delete the tag)\n");
	printf("        <preamble> is a filename of a textfile to add as preamble\n");
	printf("                     (use '-' to mean no preamble required)\n");
	printf("                     (use '-q' to mean no preamble, and no dots and enquote)\n");
	printf("                     (use '-0' to mean no preamble, and no dots)\n");
	printf("        <file> is a filename to dot\n");
	printf("NB the dotted output is found in the dotted directory\n\n");
	version( PSYDOT_PROG, PSYDOT_VERSION, PSYDOT_DATE );
	copyright( 1994, PSYDOT_YEAR );
	exit(1);
}

int
main( int argc, char *argv[] )
{
	int	i;
	file *last, *this, *preamble;

	int		 dot_mode = DOT_MODE_DOTS;
	int		 remove_tag;
	int		 splitpoint;
	char	*splitstr;


	if ( argc< 4 )
	{
		p_usage("not enough args");
	}

	splitstr  = argv[1];
	splitpoint= strtol( argv[2], 0, 0 );

	if (splitpoint<0)
	{
		splitpoint = (-splitpoint);
		remove_tag = 1;
	} else
		remove_tag = 0;

	if (splitpoint> strlen(splitstr))
	{
		p_usage("<splitpoint> must be within <splitstr>");
	}
/* now read files */

	if (argv[3][0] != '-')
	{
		preamble = p_read_file( argv[3], 0, 0, 0);
		if (! preamble )
		{
			p_usage("preamble not found");
		}
	} else 
	{
		preamble = 0;
		switch (argv[3][1])
		{
			case	'q':
				dot_mode = DOT_MODE_QUOTE;
				break;

			case	'0':
				dot_mode = DOT_MODE_NOUGHT;
				break;

			default:
				break;
		}
	}

	last = files;
	for (i=4; i<argc; i++)
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

/*
	if this fails, we are really buggered.

	however, i'm feeling peverse, and so we won't check the
	return code.

	-uts, 1st june 1998
*/
	mkdir("dotted", 0755);

	for(this = files; this; this=this->next)
	{
		p_dot_out( preamble, this, splitstr, splitpoint, remove_tag, dot_mode );
	}
	return (0);
}
