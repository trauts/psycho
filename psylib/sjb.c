/*
 * sjb.c
 *	nice little routines
 */

/* copyright (c) 1994-1997 by Stuart Boutell. All rights reserved. */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sjb.h"

static char viswaits[4] = { '|', '/' , '-' , '\\' };

void
viswait()
{
	static int count=0;
	static int vis=0;

	if (count++ % 1000)
		return;

	fprintf(stderr, "%c%c",viswaits[vis++ % 4], '\b');
	fflush(stderr);
}

void
fast_viswait()
{
	static int vis=0;
	fprintf(stderr, "%c%c",viswaits[vis++ % 4], '\b');
	fflush(stderr);
}

void
version( char *prog, int version, char *date )
{
	char *ptr;

/* strip any leading stuff */
	ptr = rindex( prog, '/' );
	if (!ptr)
		ptr = prog;
	else
		ptr++;

	printf("\n%s %d.%02d (%s)\n", ptr,
		version / 100, version % 100, date);
}

void
copyright( int start, int end )
{
	printf("copyright (c) Stuart Boutell %4d",start);

	if (start==end)
		putchar('.');
	else
		printf("-%4d.", end);
	printf(" All rights reserved.\n");
}

void
usage( char *prog, char *string )
{
	char *ptr;

/* strip any leading stuff */
	ptr = rindex( prog, '/' );
	if (!ptr)
		ptr = prog;
	else
		ptr++;

	printf("usage: %s %s\n\n", ptr, string );
}

void
stop( char *file, int line )
{
	fprintf(stderr,"--- STOP called - %s line %d ---\n\n", file, line);
	exit(1);
}

void
error_stop( char *reason, char *file, int line )
{
	fprintf(stderr,"--- STOP called - reason: %s - %s line %d ---\n\n", reason,
		file, line);
	exit(1);
}
