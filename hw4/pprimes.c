/*
 *Created 2/25/2013
 * Authored by Keaton Wallace
 */
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>

/*function protptypes*/
void set_NotPrime( unsigned int n);
void set_bitmap();
void seed_primes();
void fork();
void *mount_shmem(char *path, int object_size);
int check_Prime(unsigned int n);


typedef char word_t;
enum { BITS_PER_WORD = 8 };
#define WORD_OFFSET(b) ((b) / BITS_PER_WORD)
#define BIT_OFFSET(b)  ((b) % BITS_PER_WORD)
#define SHM_NAME "/wallacke_primes"

static char* primes;
unsigned long max_num;
int num_proc;
pid_t *process_array;


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
		for(i=k; i*k <= sqrt(max_num)+1; i++)
			set_NotPrime;
		j++;
		while(!check_Prime(j))
			j++;
	}
}

void fork()
{
	unsigned int i;
	for (i = 0; i < num_proc; i++) {
		process_array = malloc(sizeof(pid_t) * num_proc);
		pid_t pid;
		switch (pid = fork()) {
		case -1://Oops case
			perror("Forking error");
			exit(EXIT_FAILURE);
		case 0://Child case
			sieve(i);
			exit(EXIT_SUCCESS);
		default://Parent case
			process_array[i] = pid;
			break;
		}
	}
}

void *mount_shmem(char *path, int object_size)
{
	int shmem_fd;
	void *addr;

	shmem_fd = shm_open(path, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);

	if (shmem_fd == -1){
		perror("Failure opening shared memory");
		exit(EXIT_FAILURE);
	}
	if (ftruncate(shmem_fd, object_size) == -1){
		perror("Failure resizing shared memory");
		exit(EXIT_FAILURE);
	}
	/* map the shared memory object */
	addr =
		mmap(NULL, object_size, PROT_READ | PROT_WRITE, MAP_SHARED,
		     shmem_fd, 0);
	if (addr == MAP_FAILED){
		perror("Failure mapping shared memory");
		exit(EXIT_FAILURE);
	}
	return addr;
}

void set_NotPrime( unsigned int n) {
	primes[WORD_OFFSET(n)] |= (1 << BIT_OFFSET(n));
}

int check_Prime(unsigned int n) {

	word_t bit = primes[WORD_OFFSET(n)] & (1 << BIT_OFFSET(n));
	return bit == 0;
}

int main(int argc, char **argv)
{
	if(argc != 5){
		printf("[P or T] [# of P or T] [max prime] [print y/n] \n");
		exit(EXIT_FAILURE);
	}
	char type = argv[1][0];
	if(type != 'p' && type != 'P' && type != 't' && type != 'T'){
		printf("invalid process type, need either p or t\n");
		exit(EXIT_FAILURE);
	}
	int num = atoi(argv[2]);
	if(num > 0)
		num_proc = num;
	else{
		printf("invalid number of p/t\n");
		exit(EXIT_FAILURE);
	}
	unsigned long max;
	if((max = atol(argv[3])) > 0)
		max_num = max;
	else{
		printf("invalid max number\n");
		exit(EXIT_FAILURE);
	}
	char print;
	if (argv[4][0] == 'y' || argv[4][0] == 'n') {
		print = argv[4][0];
	} else {
		printf("Invalid entry for print primes.\n")
		exit(EXIT_FAILURE);
	}
	unsigned long primes_size = max_num / BITS_PER_WORD + 1;
	void *addr = mount_shmem(SHM_NAME, primes_size);
	primes = (unsigned char *) addr;
	set_bitmap();
	seed_primes();
	fork();
	
}

// bitmap stuff
// from rindal

// for the process vertion you will have primes = mmap( , , , , ); somewhere in your code
/*figure out if that is an exact statement*/

/*
 *todo

 *HIGH PRIORITY* Remove all traces of threaded version

 *set bitmap --set
 *is mask necesary? --nope
 *shared memory --shared
 *figure out process
 **spawn --spawned
 **sieve --done for parent
 **sieve for kiddies
 *happy/sad
 **should be something here but I want to get the finding part done first
 *print

 *decided to split into seperate code. Hopefully that is easier.
 *write threaded version of code
 **pthreads
*/
