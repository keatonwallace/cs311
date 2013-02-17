/*
 *Created 2/11/2013
 *Authored by Keaton Wallace
 *
*/
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
//exec lp
//char* NULL
//./a.out<in>out

pid_t *children;
int **to_children;
int **to_nanny;
int num_children = 0;

/*spawn is a function that takes int as a  parameter and spawns that number of
 *child processes.
 */
void spawn(int num_spawn, int **into, int **outof)
{
	pid_t pid;
	int x;
	for (x = 0; x < num_spawn; x++){
		build_toilet(into[x]);
		build_toilet(outof[x]);
		switch((pid = fork())){
		case -1:
			//in parent, oops
			perror("forking went awry (use a condom kids)");
			break;
		case 0:
			//child case
			close(STDIN_FILENO);
			close(STDOUT_FILENO);
			dup2(into[x][0], STDIN_FILENO);
			break_toilet(outof[x][0]);			      
			dup2(outof[x][1], STDOUT_FILENO);
			int i, j;
			for (i = 0; i < x + 1; i++) {
				for ( j = 0; j < 2; j++) {
					break_toilet(into[i][j]);
					break_toilet(outof[i][j]);
				}
			}
			execlp("sort", "sort", (char *)NULL);
			break;
		default:
			//parent case -- result holds pid of child
			children[x] = pid;
			break_toilet(into[x][0]);
			break_toilet(outof[x][1]);
			break;
		}
	}
}

void build_toilet(int *fdar)
{
	if(pipe(fdar) == -1)
		perror("couldn't build a pipe");
}

void break_toilet(int fd)
{
	if(close(fd) == -1)
		perror("couldn't close pipe");
}

int **build_bathroom(int num)
{
	int **toilets = malloc(sizeof(int *) * (num));
	int x;
	for (x = 0; x < num; x++){
		toilets[x] = malloc(sizeof(int) * 2);
	}
	return toilets;
}

void destroy_bathroom(int num_toilets, int **bathroom)
{
	int x;
	for(x = 0; x < num_toilets; x++){
		free(bathroom[x]);
	}
	free(bathroom);
}

void hire_nanny(int **children)
{
	pid_t pid;
	int x;
	switch(pid = fork()){
	case -1:
		//error case
		perror("couldn't spawn supressor");
		break;
	case 0:
		//child case
		//write later
		break;
	case 1:
		//parent case
		//kill nanny after children are disciplined
		waitpid(pid, NULL, 0);
		break;
	}
}

void nanny_disciplines(int **children)
{
}

void nevermore(int errno)
{
	int x;
	if(children != NULL){
		//need num_children for this
		for(x = 0; x < num_children; x++){
			kill(children[x], SIGQUIT);
		}
	}
	int childPid = 0;
	while ((childPid = wait(NULL)) != -1)
		continue;
	if (errno != ECHILD) /* An unexpected error... */
		perror("waiting for children in signal handler");
	if(to_children != NULL)
		destroy_bathroom(num_children, to_children);
	if(to_nanny != NULL)
		destroy_bathroom(num_children, to_nanny);
	exit(1);
}

int main(int argc, char **argv)
{
	num_children = atoi(argv[1]);
	struct sigaction quoth_the_raven;
	quoth_the_raven.sa_handler = nevermore;
	sigemptyset(&quoth_the_raven.sa_mask);
	quoth_the_raven.sa_flags = 0;
	sigaction(SIGQUIT, &quoth_the_raven, NULL);
	sigaction(SIGINT, &quoth_the_raven, NULL);
	sigaction(SIGHUP, &quoth_the_raven, NULL);
	to_children = build_bathroom(num_children);
	to_nanny = build_bathroom(num_children);
	spawn(num_children, to_children, to_nanny);
	// parse of some kind
	hire_nanny(to_nanny);
	//murder children -from book
	int childPid = 0;
	while ((childPid = wait(NULL)) != -1)
		continue;
	if (errno != ECHILD) /* An unexpected error... */
		perror("waiting for children to die");
	destroy_bathroom(num_children, to_children);
	destroy_bathroom(num_children, to_nanny);
	free(children);
	return 0;
}

/*todo
 *figure out sig handler --qouth_the_raven nevermore
 *spawn suppressor --nanny has been hired
 *free allocated arrays --bathroom has been destroyed
 *choose/use supressor
 *parser
 *debug

void RRParser(int numsorts, int **outPipe)
{
    int i;
    char word[MAX_WORD_LEN];

    //Close read end of pipes
    for(i = 0; i < numsorts; i++){
        closePipe(outPipe[i][0]);
    }

    //fdopen() all output pipes
    FILE *outputs[numsorts];
    for(i=0; i < numsorts; i++){
        outputs[i] = fdopen(outPipe[i][1], "w");
        if(outputs[i] == NULL)
            printf("Error: could not create output stream.\n");
    }

    //Distribute words to them
    i = 0;
    while(scanf("%[^,]%*c,", word) != EOF){
        strtoupper(word);
        fputs(word, outputs[i % numsorts]); //round robin
        fputs("\n", outputs[i % numsorts]); //sort needs newline
        i++;
    }

    //Flush the streams:
    for(i=0; i < numsorts; i++){
        if(fclose(outputs[i]) == EOF)
            printf("Error closing stream.\n");
    }
}
 */
