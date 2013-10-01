/*
	project	:	psycho 4
	file	:	psydot.h

	Copyright (c) 1998 Stuart Boutell

	notes	:
		dotty header
 */

#define	DOT_MODE_DOTS	0
#define	DOT_MODE_QUOTE	1
#define	DOT_MODE_NOUGHT	2

extern void
p_original_dotty( FILE	*stream, char *buffer,
		char	*split_pea, int pea_margin,
		int	haricot);
