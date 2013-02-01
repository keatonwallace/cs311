/*append, tlpi_hdr.h, get_num.h, error_functions.h,
error_functions.c all come directly from the the book
and I take no credit for it*/

#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"
#ifndef BUF_SIZE /* Allow "cc -D" to override definition */
#define BUF_SIZE 1024
#endif

void errMsg(const char *format, ...);
void errExit(const char *format, ...);
void err_exit(const char *format, ...);
void errExitEN(int errnum, const char *format, ...);
void fatal(const char *format, ...);
void usageErr(const char *format, ...);
void cmdLineErr(const char *format, ...);

void append(argv*[])
{
	int inputFd, outputFd, openFlags;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];
    if (argc != 3 || strcmp(argv[1], "--help") == 0)
	    usageErr("%s old-file new-file\n", argv[0]);
    /* Open input and output files */
    inputFd = open(argv[1], O_RDONLY);
    if (inputFd == -1)
	    errExit("opening file %s", argv[1]);
    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
	    S_IROTH | S_IWOTH; /* rw-rw-rw- */
    outputFd = open(argv[2], openFlags, filePerms);
    if (outputFd == -1)
	    errExit("opening file %s", argv[2]);
    /* Transfer data until we encounter end of input or an error */
    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
	    if (write(outputFd, buf, numRead) != numRead)
		    fatal("couldn't write whole buffer");
    if (numRead == -1)
	    errExit("read");
    if (close(inputFd) == -1)
	    errExit("close input");
    if (close(outputFd) == -1)
	    errExit("close output");
    exit(EXIT_SUCCESS);

}

int main(int argc, char *argv[])
{
	if(strcmp(argv[1], "-q")==0)
		append(argv[]);
	if(strcmp(argv[1], "-x")==0)
		extract();
	if(strcmp(argv[1], "-t")==0)
		concisetoc();
	if(strcmp(argv[1], "-v")==0)
		verbosetoc();
	if(strcmp(argv[1], "-d")==0)
		delete();
	if(strcmp(argv[1], "-d")==0)
		regular();
	return 0;
}
