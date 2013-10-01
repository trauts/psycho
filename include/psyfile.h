/*
	project	:	psycho 4
	file	:	psyfile.h

	Copyright (c) 1997 Stuart Boutell

	notes	:
		declarations and structure definitions about files
 */

#define	P_FILE_MAX	5

#define	P_REUSE_VARIANCE	2
#define	P_REUSE_MAX (P_FILE_MAX+P_REUSE_VARIANCE)

typedef	struct	_reuse
{
	int		expfile;			/* true for an experimental file */
								/* if true, then use reuse criteria -
								otherwise, just fill in randomly */

	int		reuse[P_REUSE_MAX];	/* array of reusability */
} reuse;


typedef	struct	_line
{
	char	*str;
	struct	_file	*file;			/* souce file of this line */
	struct	_line	*next;	/* ptr to the next line */
} line;


typedef	struct	_file
{
	char	*name;				/* filename */
	int		index;				/* the file index */

	int		lines;				/* number of file lines */

	line	*line1;				/* ptr to the first line */
	reuse	reuse;				/* reuse descriptor */


/* for use in generation */
	line	*cur;			/* ptr to the current line in use */

	struct _file	*next;			/* ptr to the next file */
} file;


/*
	prototypes
 */
unsigned int p_rand( int limit );
void p_reset_rand(void);

void p_set_reuse( file *f, int base, int variance ); 
void p_put_exp( file *out, file *src );
void p_put_fil( file *out, file *src, int base, int variance );
int p_analyse( file *f );
line * p_new_line( char *contents, file *file );
file * p_blank_file (char *name, int lines);
int p_write_file( file *file );
file	* p_read_file( char *name, int index, int expfile, int wrap);

int	p_check( file *file, int sets, int items, int verbose );
