/*
	project	:	psycho 4
	file	:	psyolddot.c

	Copyright (c) 1994-1997 Stuart Boutell

	notes	:
		this is actually the original "dotty" program, much mangled.
 */

#include <stdio.h>
#include <string.h>

#include "psydot.h"

#define	PAGE_TRUNCATE 	 79
#define	PAGE_LENGTH 	 88

#define LINE_LENGTH 76

#define	MAGIC_PEA	'@'

int		line = 0;

static	void
newline(FILE *stream)
{
	fputc('\n', stream);
	line++;
}

static	void
dots( FILE *stream, int num )
{
  for(; num; num--)
    fputc('.', stream);
  newline(stream);
}


void
p_original_dotty(
		FILE	*stream, char *buffer,
		char	*split_pea, int pea_margin,
		int	haricot)
{
	int	line_pos = 0;
	int	split_pea_size = 0;
	char  *pea;

	split_pea_size = strlen(split_pea);
	
	switch (haricot)
	{
		case	DOT_MODE_QUOTE:
			fputc('\"',stream);
			line_pos++;
			break;
		default:
			break;
	}

	for( pea = buffer; *pea; )
	{
		if ( *pea == '\n' )
		{	/* split required HERE */
			switch (haricot)
			{
				case	DOT_MODE_DOTS:
				{
					if ( line_pos < LINE_LENGTH )
						dots(stream, LINE_LENGTH-line_pos);
					else
						newline(stream);
					newline(stream);
					dots(stream, LINE_LENGTH);
					newline(stream);
					line_pos=0;
					break;
				}
				case	DOT_MODE_QUOTE:
				{
					fputc('\"', stream);
					newline(stream);
					fputc('\"', stream);
					line_pos=1;
					break;
				}
				default:
					newline(stream);
					line_pos=0;
					break;
			}
			pea++;
      /* check for MAGIC_PEA, and put newlines in if we would truncate */
			if ((*pea == MAGIC_PEA) && (line % PAGE_LENGTH)>=PAGE_TRUNCATE )
      		{
				pea++;  /* skip magic pea */
				while (line % PAGE_LENGTH)
					newline(stream);
			}
		}
		else if (split_pea && !strncmp(pea, split_pea, split_pea_size) )
		{	/* split required HERE */
			int i;

			/* put bit of split string header */
			for( i=pea_margin; i; i-- )
			{
				fputc(*pea++,stream);
				line_pos++;
			}

/* and SKIP the separator */
			pea++;

			switch (haricot)
			{
				case	DOT_MODE_DOTS:
				{
					if ( line_pos < LINE_LENGTH )
						dots(stream, LINE_LENGTH-line_pos);
					else
						newline(stream);
					newline(stream);
					dots(stream, LINE_LENGTH);
					newline(stream);
					line_pos=0;
					break;
				}
				case	DOT_MODE_QUOTE:
				{
					fputc('\"', stream);
					newline(stream);
					fputc('\"', stream);
					line_pos=1;
					break;
				}
				default:
					newline(stream);
					line_pos=0;
					break;
			}
		}
		else
		if (*pea == MAGIC_PEA)
		{
			pea++;
		} else
		{
			fputc(*pea++,stream);
			line_pos++;
		}
	}
}
