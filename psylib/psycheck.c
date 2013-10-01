/*
	project	:	psycho 4
	file	:	psycheck.c

	Copyright (c) 1997,2004 Stuart Boutell

	notes	:
		returns if a file is OK, or cack.
 */

#include <ctype.h>	/* for isspace() */
#include <stdio.h>
#include <string.h>
#include <psyfile.h>

int
p_check( file *this, int num_sets, int num_items,int verbose )
{
	int	item, set, item_length;
	line	*l;
	int	stop, warn;
	int     expected;

	expected = num_sets * (num_items+1);

	stop = 0;
	warn = 0;

	if (this->lines < expected)
	{
		if (verbose)
		{
			printf("[%s] (short file - wanted %d lines, found %d lines)\n", this->name, expected, this->lines);
			warn++;
		} else {
			return(0);
		}
	}

	if (this->lines > expected)
	{
		if (verbose)
		{
			printf("[%s] (long file - wanted %d lines, found %d lines)\n", this->name, expected, this->lines);
			warn++;
		} else {
			return(0);
		}
	}

	l = this->line1;

	for(set=0; set<num_sets && this->lines && !stop; set++)
	{
		for (item=0; item<num_items && this->lines && !stop; item++)
		{
			item_length = strlen(l->str);

			/* check for blank lines in sets */
			if ( !item_length || strspn( l->str, "\r\n \t") == item_length)
			{
				if (verbose)
				{
					printf("[%s] (material %d, version %d) blank line\n", this->name, set+1, item+1);
					stop++;
					break;
				} else
					return(0);
			}

			/* check for spaces after set items - they break dotty - ignore
				the very last character of course - it is always a new line! */
			/* nb exception case for dos txt files- allow \r too */
			if ( item_length>1 && 
				isspace((int)(l->str[(item_length-2)]))
					   && l->str[item_length-2]!='\r')
			{
				if (verbose)
				{
					printf("[%s] (material %d, version %d) whitespace at end of line (%x)\n",
						this->name, set+1, item+1, l->str[item_length-2]);
					warn++;
					/* no break - we are only warning */
				} else
					return(0);
			}

			/* advance */
			this->lines--;
			l=l->next;
		}

		if (!stop)
		{
		
			if (this->lines)
			{
				if (strspn( l->str, "\r\n \t") != strlen(l->str))
				{
					if (verbose)
					{
						printf("[%s] (near material %d) non-blank line found (%lu, %lu)\n",
							this->name, set+1,
							strspn( l->str, "\r\n \t"),
							strlen( l->str));
						stop++;
					} else
						return(0);
				}
			} else {
				if (verbose)
				{
					printf("[%s] please add a blank line after the last set...\n",
						this->name);
					stop++;
				} else
					return(0);
			}
		}

		/* advance */
		if (!stop && this->lines)
		{
			this->lines--;
			l=l->next;
		}
	}
	if (!stop && set!=num_sets) 
	{
		if (verbose)
		{
			printf("[%s] found %d materials, but %d were expected.\n",
				this->name, set, num_sets);
		} else
			return(0);

	}
	if (!stop && item!=num_items) 
	{
		if (verbose)
		{
			stop++;
			printf("[%s] last material only had %d versions\n", this->name, item);
		} else
			return(0);

	}
		
	for(; this->lines && !stop; )
	{
		if (strspn( l->str, "\r\n \t") != strlen(l->str))
		{
			if (verbose)
			{
				printf("[%s] non-blank line found at the end\n", this->name);
				stop++;
			} else
				return(0);
		}
		else {
			this->lines--;
			l=l->next;
		}
	}

	if (verbose)
	{
		if (stop || warn)
			printf("[%s] NEEDS REVISIONS\n", this->name );
		else
			printf("[%s] OK\n", this->name );
	}
	return(1);
}
