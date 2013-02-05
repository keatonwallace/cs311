/*
 *file myar.c
 *Authored by Keaton Wallace
 *Created Jan 28 2013
 *Last updated Feb 4 2013
 *
 *This is a reimplementation of the ar unix function
 *
 *All work not my own is creditted to its creators and is used under the fair 
 *use clause for educational purpose.
 *Special thanks to Dan Albert, Shuaiyuan Emory and Michael Kerrick.
*/

#include <sys/stat.h>
#include <fcntl.h>
#include <ar.h>
#include <stdlib.h>
#include "tlpi_hdr.h"
#ifndef BUF_SIZE /* Allow "cc -D" to override definition */
#define BUF_SIZE 1024
#endif
#define SARFNAME 16
#define SARFDATE 12
#define SARFUID 6
#define SARFGID 6
#define SARFMODE 8
#define SARFSIZE 10
#define SARFMAG 2
void errMsg(const char *format, ...);
void errExit(const char *format, ...);
void err_exit(const char *format, ...);
void errExitEN(int errnum, const char *format, ...);
void fatal(const char *format, ...);
void usageErr(const char *format, ...);
void cmdLineErr(const char *format, ...);
/*This function takes one file name and appends it to an archive with the
 *appropriate header information. Inspiration for this function came from 
 *"The Linux Programming Interface" by Michael Kerrisk and from Dan Albert
 *whose code is online at https://github.com/DanAlbert/myar/blob/master/myar.c*/
void append(char* argv[])
{
	int inputFd, outputFd, openFlags;
    mode_t filePerms;
    ssize_t numRead=0;
    char buf[BUF_SIZE];
    /* Open input and output files */
    inputFd = open(argv[2], O_RDONLY);
    if (inputFd == -1)
	    errExit("opening file %s", argv[2]);
    outputFd= openAr(argv);
    if (outputFd == -1)
	    errExit("opening archive %s", argv[3]);
    /*Write the metadata to the start of the file*/
    struct ar_hdr hdr;
    struct stat st;
    stat (argv[2], &st);
    char date[SARFDATE + 1];
    char uid[SARFUID + 1];
    char gid[SARFGID + 1];
    char mode[SARFMODE + 1];
    char size[SARFSIZE + 1];
    /* Create NULL terminated versions of each header value*/
    snprintf(date, SARFDATE + 1, "%-12d", st.st_mtim.tv_sec);
    snprintf(uid, SARFUID + 1, "%-6u", st.st_uid);
    snprintf(gid, SARFGID + 1, "%-6u", st.st_gid);
    snprintf(mode, SARFMODE + 1, "%-8o", st.st_mode);
    snprintf(size, SARFSIZE + 1, "%-10d", st.st_size);
    /* Fill the header*/
    int len = strlen(argv[2]);
    memcpy(hdr.ar_name, argv[2], len);
    hdr.ar_name[len]='/';
    int x;
    for(x= len+1; x<=15; x++){
	    hdr.ar_name[x]= ' ';
    }
    memcpy(hdr.ar_date, date, SARFDATE);
    memcpy(hdr.ar_uid, uid, SARFUID);
    memcpy(hdr.ar_gid, gid, SARFGID);
    memcpy(hdr.ar_mode, mode, SARFMODE);
    memcpy(hdr.ar_size, size, SARFSIZE);
    memcpy(hdr.ar_fmag, ARFMAG, SARFMAG);
    // Write the header
    if (write(outputFd, &hdr, sizeof(struct ar_hdr)) == -1)
	    fatal("couldn't write header");
    /*Write the contents of the file */
    while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
   	    if (write(outputFd, buf, numRead) != numRead)
		    fatal("couldn't write whole buffer");
    write(outputFd, "\n", 1);
    if (numRead == -1)
	    errExit("read");
    /*Close both files*/
    if (close(inputFd) == -1)
	    errExit("close input");
    if (close(outputFd) == -1)
	    errExit("close output");
    exit(EXIT_SUCCESS);
}
/*This function takes an archive and opens it if it exists and initializes it
 *if it does not. This function was heavily influenced by Dan Albert's open
 *source code available at https://github.com/DanAlbert/myar/blob/master/myar.c*/
int openAr(char* argv[])
{
	struct stat st;
	Boolean exists;
	int outputFd;
	/*Check if the archive already exists*/
	if (stat(argv[3], &st) == 0) {
		exists = TRUE;
	} else {
		exists = FALSE;
	}
	int openFlags = O_CREAT | O_WRONLY | O_APPEND;
    int filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
	    S_IROTH | S_IWOTH; /* rw-rw-rw- */
    outputFd = open(argv[3], openFlags, filePerms);
    if(exists == TRUE){
	    if(checkAr(outputFd)==-1){
		    errExit("bad ar header");
	    }
    }
    else{
	    if(writeArHeader(outputFd)== -1)
		    errExit("couldn't write ar header");
    }
    return outputFd;
}
/*This function takes an archive and indicates if it has a valid header. Credit to Dan Albert for inspiration. It is as of now unimplemented and always returns true*/
int checkAr(int outputFd)
{
	char hdr[SARMAG];
	int start;
	int isGood;
	start = lseek(outputFd, 0, SEEK_CUR);
	lseek(outputFd, 0, SEEK_SET);
	return 0;
}
/*This function takes a new archive and attaches the start value for an archive.*/
int writeArHeader(int outputFd)
{
	lseek(outputFd, 0, SEEK_SET);
	if(write(outputFd, ARMAG, SARMAG)== -1)
		return -1;
	return 0;
}
void extract(char* argv[])
{
	//find write file in usual mannor,
	//send to delete
}
void delete(char* argv[])
{
	int arFd, newFd, pos, dataSize;
	char *t;
	size_t nextValue;
	size_t osize;
	struct ar_hdr hdr;
	struct stat arStat;
	char filename[18];
	ssize_t numRead=0;
	char buf[BUF_SIZE];
	memcpy(filename, argv[3], strlen(argv[3]));
	printf(filename);
	printf("\n");
	arFd = open(argv[3], O_RDONLY);
	printf("old fd %d\n", arFd);
	stat(argv[3], &arStat);
	unlink(argv[3]);
	newFd = open(filename, O_WRONLY | O_CREAT, arStat.st_mode);
	printf("new fd %d\n", newFd);
	write(newFd, arFd, 8);
	pos = lseek(arFd, 0, SEEK_CUR);
	printf("pos in ar %d\n", pos);
	while (pos<arStat.st_size)
	{
		read(arFd, hdr.ar_name, sizeof(hdr.ar_name));
		read(arFd, hdr.ar_date, sizeof(hdr.ar_date));
		read(arFd, hdr.ar_uid, sizeof(hdr.ar_uid));
		read(arFd, hdr.ar_gid, sizeof(hdr.ar_gid));
		read(arFd, hdr.ar_mode, sizeof(hdr.ar_mode));
		read(arFd, hdr.ar_size, sizeof(hdr.ar_size));
		read(arFd, hdr.ar_fmag, sizeof(hdr.ar_fmag));
		nextValue = string2ll(hdr.ar_size, sizeof(hdr.ar_size));
		osize = nextValue;
		t = index(hdr.ar_name, '/');
		if (t<hdr.ar_date)
			*t='\0';
		if(strcmp(hdr.ar_name, argv[2]) == 0)
			pos = lseek(arFd,(osize+1)&(~1),SEEK_CUR);
		else{
			lseek(arFd, -60, SEEK_CUR);
			while ((numRead = read(arFd, buf, hdr.ar_size+60)) > 0)
				if (write(newFd, buf, numRead) != numRead)
					fatal("couldn't write whole buffer");
		}
		pos = lseek(arFd,(osize+1)&(~1),SEEK_CUR);
		//do weird t thing, if hdr.ar_name == argv[2]
		//skip data
		//else lseek back 60 and copypasta till size+60
		//update pos
	}
	close(arFd);
	close(newFd);
}

void regular(char* argv[])
{
}
/*This function takes an archive and prints information about the files stored therein. In terse mode indicated by verbose equalling -1 just file names are printed. In verbose mode additional file information is presented. Based closely on the work by shuaiyuan.emory available at http://code.google.com/p/yuanshuai/source/browse/trunk/cs551/hw%231/myar.c?r=156*/
static int print_table(int argc, char *argv[], int verbose)
{
	int ar_fd;
	int pos;
	long st_mode;
	char * t;
	size_t need_to_write;
	size_t o_file_size;
	struct ar_hdr p_hdr;
	struct stat arfileStat;
	/*Open the archive*/
	if((ar_fd=open(argv[2], O_RDONLY)) < 0)
		fatal("can not open ar file: %s\n", argv[2]);
	if (stat (argv[2], &arfileStat)<0)
		fatal("can not open ar file: %s\n", argv[2]);
	/*This line skips the !<arch> thing at the start of the archive*/
	pos =lseek(ar_fd,8,SEEK_CUR);
	while (pos < arfileStat.st_size)
	{
		/*Read in the file info based on the size of the struct fields*/
		read(ar_fd, p_hdr.ar_name, sizeof(p_hdr.ar_name));
		read(ar_fd, p_hdr.ar_date, sizeof(p_hdr.ar_date));
		read(ar_fd, p_hdr.ar_uid, sizeof(p_hdr.ar_uid));
		read(ar_fd, p_hdr.ar_gid, sizeof(p_hdr.ar_gid));
		read(ar_fd, p_hdr.ar_mode, sizeof(p_hdr.ar_mode));
		read(ar_fd, p_hdr.ar_size, sizeof(p_hdr.ar_size));
		read(ar_fd, p_hdr.ar_fmag, sizeof(p_hdr.ar_fmag));
		/*Clean up some details*/
		need_to_write = string2ll(p_hdr.ar_size, sizeof(p_hdr.ar_size));
		o_file_size = need_to_write;
		t = index(p_hdr.ar_name, '/');
		/*Make sure the name is null terminated*/
		if (t<p_hdr.ar_date)
			*t='\0';
		if (verbose == 0)
		{
			int tmp;
			st_mode = string2int8(p_hdr.ar_mode, sizeof(p_hdr.ar_mode));
			/*Print the mode in octal*/
			printf( (st_mode & S_IRUSR) ? "r" : "-");
			printf( (st_mode & S_IWUSR) ? "w" : "-");
			printf( (st_mode & S_IXUSR) ? "x" : "-");
			printf( (st_mode & S_IRGRP) ? "r" : "-");
			printf( (st_mode & S_IWGRP) ? "w" : "-");
			printf( (st_mode & S_IXGRP) ? "x" : "-");
			printf( (st_mode & S_IROTH) ? "r" : "-");
			printf( (st_mode & S_IWOTH) ? "w" : "-");
			printf( (st_mode & S_IXOTH) ? "x" : "-");
			/*Ouput formatting*/
			tmp = string2int(p_hdr.ar_uid, sizeof(p_hdr.ar_uid));
			printf("%6d", tmp);//uid
			printf("/");
			tmp = string2int(p_hdr.ar_gid, sizeof(p_hdr.ar_gid));
			printf("%6d", tmp);//gid
			tmp = string2int(p_hdr.ar_size, sizeof(p_hdr.ar_size));
			printf("%7d ", tmp);//size                                          
			time_t file_time = (time_t)string2ll(p_hdr.ar_date, sizeof(p_hdr.ar_date));
			printstr(ctime(&file_time)+4, 12);
			printf(" ");
			printstr(ctime(&file_time)+20, 4);
			printf(" ");                                    
			}
		printf("%s\n", p_hdr.ar_name);                                  
		pos =lseek(ar_fd,(o_file_size+1)&(~1),SEEK_CUR);
	}
	/*Don't forget to close the file*/
	close(ar_fd);
	return 0;
}
/*This function, courtesy of shuaiyuan.emory available at http://code.google.com/p/yuanshuai/source/browse/trunk/cs551/hw%231/myar.c?r=156 takes a string and converts it to long long integer.*/
static long long int string2ll(char *str, int str_len)
{
	char tmp[128];
	memcpy(tmp, str, str_len);
	tmp[str_len]='\0';
	return atoll(tmp);              
}
/*This function, courtesy of shuaiyuan.emory available at http://code.google.com
/p/yuanshuai/source/browse/trunk/cs551/hw%231/myar.c?r=156 takes a string and co
nverts it to an integer.*/
static int string2int(char *str, int str_len)
{
	char tmp[128];
	memcpy(tmp, str, str_len);
	tmp[str_len]='\0';
	return atoi(tmp);               
}
/*This function, courtesy of shuaiyuan.emory available at http://code.google.com
/p/yuanshuai/source/browse/trunk/cs551/hw%231/myar.c?r=156 takes a string and co
nverts it to long integer.*/
static long  string2int8(char *str, int str_len)
{
	char tmp[128];
	memcpy(tmp, str, str_len);
	tmp[str_len]='\0';
	return strtol(tmp, NULL, 8);
                
}
/*This function, courtesy of shuaiyuan.emory available at http://code.google.com
/p/yuanshuai/source/browse/trunk/cs551/hw%231/myar.c?r=156 takes a string null terminates it and prints it.*/
void printstr(char *str, int len)
{
	char temp[128];
	memcpy(temp, str, len);
	temp[len]='\0';
	printf(temp);
}
/*This is main. Main is boring but functional*/
int main(int argc, char *argv[])
{
  	if(strcmp(argv[1], "-t")==0)
		print_table(argc, argv, -1);
	else if(strcmp(argv[1], "-q")==0)
		append(argv);
	else if(strcmp(argv[1], "-x")==0)
		extract(argv);
	//else if(strcmp(argv[1], "-t")==0)
	//	concisetoc(argv);
	else if(strcmp(argv[1], "-v")==0)
		print_table(argc, argv, 0);
	else if(strcmp(argv[1], "-d")==0)
		delete(argv);
	else if(strcmp(argv[1], "-A")==0)
		regular(argv);
	else
		printf("Did you forget the param?");
	return 0;
}
