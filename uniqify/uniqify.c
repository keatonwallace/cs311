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

pid_t *children;
int **to_children;
int **to_nanny;

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
}

void hire_nanny(int **children)
{
}

void nanny_disciplines(int **children)
{
}

int main(int argc, char **argv)
{
	int num_children = atoi(argv[1]);
	to_children = build_bathroom(num_children);
	to_nanny = build_bathroom(num_children);
	spawn(num_children, to_children, to_nanny);
	return 0;
}

/*todo
 *figure out sig handler
 *spawn suppressor
 *choose/use supressor
 *debug
 */
