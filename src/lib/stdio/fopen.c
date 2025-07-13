/*
 *	NMH's Simple C Compiler, 2011,2012
 *	fopen()
 */
#define _ELFCLIB_
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#pragma             extrn Cfdopen
#pragma             extrn Copen
#pragma             extrn Cerrno

int _openmode(char *mode) {
	if ('a' == *mode) return O_CREAT | O_APPEND;
	else if ('w' == *mode) return O_CREAT | O_TRUNC;
	else if ('r' == *mode++) {
		if ('+' == *mode)
		  return O_OPEN;
		else 
			return O_RDONLY;
	}
	return EOF;
}

FILE *fopen(char *path, char *mode) {
	int	fd;
	int flags;
	FILE	*f;
	/* mode for everything but read only */
	int  iomode = _FREAD | _FWRITE;
	
	flags = _openmode(mode);
	
	if (flags == EOF) {
		errno = EIO;  
		return NULL;
	}
		
	fd = open(path, flags);
	if (fd == EOF) { 
		/* if read-only then not found error */
		if (flags == O_RDONLY)
			errno = ENOENT;
		/* otherwise, then file access error */
		else 
			errno = EACCESS;
		return NULL;
	}
	if (flags == O_RDONLY)
		iomode = _FREAD;
		
	return fdopen(fd, iomode);
}
