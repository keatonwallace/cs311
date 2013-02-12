/*
 *Created 2/11/2013
 *Authored by Keaton Wallace
 *
*/

#include <pipe.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
//exec lp
//char* NULL
//./a.out<in>out

/*spawn is a function that takes int as a  parameter and spawns that number of
 *child processes.
 */

void spawn(int num_spawn, int **into, int **outof)
{
	pit_t pid;
	int x;
	for (x = 0; x < num_spawn; x++){
		build_toilet(into[x]);
		build_toilet(outof[x]);
		switch((result = fork())){
		case -1:
			//in parent, oops
			perror("forking went awry (use a condom kids)");
			break;
		case 0:
			//child case
			close(into[i][1]);
			//stopping here for now
			FILE *output = fdopen(pipefds[1], "w");
			fputs(buf, 10, output);
			break;
		default:
			//parent case -- result holds pid of child
			close(pipefds[1]);
			FILE *input = fdopen(pipefds[0], "r");
			fgets(buf, 10, input);

			break;

			}
}

void build_toilet(int *fdar)
{
	if(pipe(fdar) == -1)
		perror("couldn't build a pipe");
}

int main(int argc, char **argv)
{
	int num_childs = atoi(argv[1]);
	int result;
	int pipefds[2];
	if(pipe(pipefds) != 0){
		perror("Pipes are screwed up, call a plumber");
		exit(-1);
	}
	for(int i = 0; i < num_of_proc; i++)
	{
		switch((result = fork())){
		case -1:
			//in parent, oops
			break;
		case 0:
			//child case
			close(pipefds[0]);
			FILE *output = fdopen(pipefds[1], "w");
			fputs(buf, 10, output);
			break;
		default:
			//parent case -- result holds pid of child
			close(pipefds[1]);
		FILE *input = fdopen(pipefds[0], "r");
		fgets(buf, 10, input);

		break;
	}
	
	return 0;
}
