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
#define MAX_WORD_LEN 100
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
		suppressor_process(children);
		_exit(EXIT_SUCCESS);
		break;
	case 1:
		//parent case
		//kill nanny after children are disciplined
		waitpid(pid, NULL, 0);
		break;
	}
}

//void nanny_disciplines(int **children)
//{
//}

void word_vomit(int numsorts, int **outPipe)
{ //Round Robin parser
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
		strtolower(word);
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

/* Converts a string to lowercase */
char *strtolower(char *str){
	int i;
	for(i=0;i<sizeof(str);i++){
		str[i] = tolower(str[i]);
	}
	return str;
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

struct word_counter {
	char word[MAX_WORD_LEN];
	int count;
};

//this cannot stay. For testing purposes.
void suppressor_process(int **in_pipe)
{
	int i;
	char **words;
	FILE *inputs[num_children];
	struct word_counter *cur_word = malloc(sizeof(struct word_counter));
	int alpha;//index of alpha word in pipe
	int null_count = 0; //Increments if output from pipe is NULL (meaning EOF)

	/* initialize word array */
	words = malloc(num_children * sizeof(char *));
	for (i = 0; i < num_children; i++) {
		words[i] = malloc(MAX_WORD_LEN * sizeof(char));
	}

	/* fdopen in_pipes and get first batch of words to initialize cur_word with */
	for (i = 0; i < num_children; i++) {
		inputs[i] = fdopen(in_pipe[i][0], "r");
		if (fgets(words[i], MAX_WORD_LEN, inputs[i]) == NULL){
			words[i] = NULL;
			null_count++;
		}
	}
	/* Find the lowest alphabetical word in the array */
	alpha = alpha_index(num_children, words);

	/* Make this word our current word with count 1 */
	strncpy(cur_word->word, words[alpha], MAX_WORD_LEN);
	cur_word->count = 1;

	while (null_count < num_children) {
		if (fgets(words[alpha], MAX_WORD_LEN, inputs[alpha]) == NULL) {
			words[alpha] = NULL;
			null_count++;
		}
		alpha = alpha_index(num_children, words);
		if (alpha == -1) /* Meaning that the entire array was NULL */
			break;
		/* If the next word is the same as the current one, increment count */
		if (!strcmp(cur_word->word, words[alpha])) {
			cur_word->count++;
		} else {
			/* If it's a new word, print the last one and set current to the new one */
			printf("%d %s", cur_word->count, cur_word->word);
			strncpy(cur_word->word, words[alpha], MAX_WORD_LEN);
			cur_word->count = 1;
		}
	}

	/* Print last word */
	printf("%d %s", cur_word->count, cur_word->word);

	/* Free words array */
	for (i = 0; i < num_children; i++) {
		free(words[i]);
	}
	free(words);
	free(cur_word);

	/* Close inputs */
	for (i = 0; i < num_children; i++) {
		fclose(inputs[i]);
	}
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
	word_vomit(num_children, to_children);
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
 *parser --words have been vomitted
 *debug

 */
