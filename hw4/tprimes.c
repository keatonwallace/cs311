/*
 *Created 2/25/2013
 * Authored by Keaton Wallace
 */

/* Compiler directives */
/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>
//#include <signal.h>

/*function protptypes*/
void set_NotPrime( unsigned int n);
void set_bitmap();
void seed_primes();
int check_Prime(unsigned int n);
void thread_sort(void *vp);
void create_threads();
void nevermore(int s);
unsigned long next_seed(unsigned long cur_seed);


typedef char word_t;
enum { BITS_PER_WORD = 8 };
#define WORD_OFFSET(b) ((b) / BITS_PER_WORD)
#define BIT_OFFSET(b)  ((b) % BITS_PER_WORD)
//#define SHM_NAME "/wallacke_primes"

static char* primes;
unsigned long max_num;
int num_threads;
//pid_t *process_array;


void set_bitmap()
{
	primes[0] = 0b01010110;
	unsigned long i;
	for(i = 1; i < (max_num / BITS_PER_WORD); i++)
		primes[i] = 0b01010101;
}

void seed_primes()
{
	unsigned long i;
	unsigned long j = 3;
	unsigned long sqrt_n = sqrt(sqrt(max_num));
	while(j <= sqrt_n + 1){
		for(i=j; i*j <= sqrt(max_num)+1; i++)
			set_NotPrime;
		j++;
		while(!check_Prime(j))
			j++;
	}
}

void set_NotPrime( unsigned int n) {
	primes[WORD_OFFSET(n)] |= (1 << BIT_OFFSET(n));
}

int check_Prime(unsigned int n) {

	word_t bit = primes[WORD_OFFSET(n)] & (1 << BIT_OFFSET(n));
	return bit == 0;
}

void create_threads(){
	unsigned int i;
    
	pthread_t *thread;/* thread object */
	pthread_attr_t attr;/* thread attributes */
    
	thread = malloc(num_threads * sizeof(pthread_t));
    
	/* init thread attribute with defaults */
	pthread_attr_init(&attr);

	/* i is thread ID is passed as argument */
	for (i = 0; i < num_threads; i++) {
		if (pthread_create(&thread[i], &attr, thread_sort, (void*) (i)) != 0){
			perror("Cannot create the thread.");
			exit(EXIT_FAILURE);
		}
	}
    
	/* Wait on threads to finish */
	for (i = 0; i < num_threads; i++) {
		pthread_join(thread[i], NULL);
	}
	free(thread);
}

void thread_sort(void *vp){
	unsigned long i = (unsigned long) vp;
	unsigned long min = i * (max_num / num_threads) + 1;
	/* If we're on the last thread, set the max equal to the max_prime */
	unsigned long max =(i == num_threads - 1) ? max_num : min +(max_num / num_threads) - 1;
	unsigned long j;
	unsigned long k;//This is much faster as long as opposed to unsigned int
	j = 1;
	while ((j = next_seed(j)) != 0) {
		for (k = (min / j < 3) ? 3 : (min / j); (j * k) <= max; k++) {
			set_NotPrime(j * k);
		}
	}
	pthread_exit(EXIT_SUCCESS);
}

unsigned long next_seed(unsigned long cur_seed)
{
	unsigned long i;
	for (i = cur_seed + 1; i <= sqrt(max_num); i++)
		if (check_Prime(i) == 0)
			return i;
	return 0;
}

void nevermore(int s)
{
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
	if(argc != 4){
		printf("[# of threads] [max prime] [print y/n] \n");
		exit(EXIT_FAILURE);
	}
	int num = atoi(argv[1]);
	if(num > 0)
		num_threads = num;
	else{
		printf("invalid number of threads\n");
		exit(EXIT_FAILURE);
	}
	unsigned long max = atol(argv[2]);
	if(max < 1 || max > 4294967296){
		printf("invalid max number, using 4294967296\n");
		max_num = 4294967296;
	}
	else {
		max_num = max;
	}
	char print;
	if (argv[3][0] == 'y' || argv[3][0] == 'n') {
		print = argv[3][0];
	} else {
		printf("Invalid entry for print primes.\n");
		exit(EXIT_FAILURE);
	}
	printf("hello program\n");
	/*	struct sigaction raven;

	raven.sa_handler = nevermore;
	sigemptyset(&raven.sa_mask);
	raven.sa_flags = 0;

	sigaction(SIGQUIT, &raven, NULL);
	sigaction(SIGINT, &raven, NULL);
	sigaction(SIGHUP, &raven, NULL);
	*/
	printf("good thing we didn't do anything stupid here\n");
	//unsigned long primes_size = max_num / BITS_PER_WORD + 1;
	primes = (char *)malloc(sizeof(char)*((max_num/BITS_PER_WORD)+1));
	printf("Primes has been set\n");
	set_bitmap();
	printf("bitmap has been set\n");
	seed_primes();
	printf("seeding complete\n");
	create_threads();
	printf("threads have been created and died\n");
	free(primes);
	return 0;
}


/*
 *todo

 *HIGH PRIORITY* Remove all traces of process version

 *set bitmap --set
 *figure out process --figured
 **make threads --made
 **sieve --done for parent
 **sieve for kiddies --done I think
 *print


 *happy/sad
 **should be something here but I want to get the finding part done first

 */
