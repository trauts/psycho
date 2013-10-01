/*
	project	:	psycho 4
	file	:	sjb.h

	Copyright (c) 1997 Stuart Boutell

	notes	:
		declarations for private functions
 */

extern	void	fast_viswait();
extern	void	viswait();

extern	void	version( char *prog, int version, char *date );
extern	void	copyright( int start, int end );
extern	void	usage( char *prog, char *string );

#define	STOP	stop ( __FILE__, __LINE__ )
#define	HALT(x)	error_stop (x, __FILE__, __LINE__ )

extern	void	stop( char *file, int line );
extern	void	error_stop( char *reason, char *file, int line );
