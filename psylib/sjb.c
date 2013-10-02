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
	printf("\nThe MIT License (MIT)\n\n");

	printf("Copyright (c) Stuart Boutell %4d",start);

	if (start==end)
		putchar('.');
	else
		printf("-%4d.\n\n", end);


	printf("Permission is hereby granted, free of charge, to any person obtaining a copy of\n");
	printf("this software and associated documentation files (the \"Software\"), to deal in\n");
	printf("the Software without restriction, including without limitation the rights to\n");
	printf("use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of\n");
	printf("the Software, and to permit persons to whom the Software is furnished to do so,\n");
	printf("subject to the following conditions:\n");
	printf("\n");
	printf("The above copyright notice and this permission notice shall be included in all\n");
	printf("copies or substantial portions of the Software.\n");
	printf("\n");
	printf("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n");
	printf("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS\n");
	printf("FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR\n");
	printf("COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER\n");
	printf("IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN\n");
	printf("CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n\n");

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
