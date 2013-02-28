/*
 *Created 2/25/2013
 * Authored by Keaton Wallace
 */
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>

typedef char word_t;
enum { BITS_PER_WORD = 8 };
#define WORD_OFFSET(b) ((b) / BITS_PER_WORD)
#define BIT_OFFSET(b)  ((b) % BITS_PER_WORD)

static char* primes;
unsigned long max_num;
int num_threads;
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
	for (i = 0; i < num_threads; i++) {
		process_array = malloc(sizeof(pid_t) * num_threads);
		pid_t pid;
		switch (pid = fork()) {
		case -1://Oops case
			perror("Forking error");
			exit();
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

	if (shmem_fd == -1)
		puke_and_exit("Failed to open shared memory object\n");

	if (ftruncate(shmem_fd, object_size) == -1)
		puke_and_exit("Failed to resize shared memory object\n");

	/* map the shared memory object */
	addr =
		mmap(NULL, object_size, PROT_READ | PROT_WRITE, MAP_SHARED,
		     shmem_fd, 0);

	if (addr == MAP_FAILED)
		puke_and_exit("Failed to map shared memory object\n");

	return addr;
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
		num_threads = num;
	else{
		printf("invalid number of p/t\n");
		exit(EXIT_FAILURE);
	}
	unsigned int max;
	if((max = atoi(argv[3])) > 0)
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

	/* This stuff is from the original prime stuff from dave
	// Variable 'n' is the maximum value all determined prime numbers can be
	//doesn't matter using max int
	int x = atoi(argv[1]);
	// primeArray declared outside sieve method call, afterward will contain all primes found under x
	//this needs to happen in shared memory
	int prime_array[x];
	// result will contain the number of primes found under x, returned by the sieve method call
	int result = sieve(x, prime_array);
	// Print number of primes
	//for testing purposes
	printf("%d\n", result);
	// Print list of primes
	for (i = 1; i < result+1; i++) {
		printf("%d\t", prime_array[i-1]);
		// Rows of 10, please!
		if (i % 10 == 0)
			printf("\n");
	}
	printf("\n");
	return 0;
	*/
}

// bitmap stuff
// from rindal

// for the process vertion you will have primes = mmap( , , , , ); somewhere in your code
/*figure out if that is an exact statement*/

// for the thread vertion you will have primes = (char*)malloc(sizeof(char)*(4294967296/BITS_PER_WORD));


void set_NotPrime( unsigned int n) { 
	primes[WORD_OFFSET(n)] |= (1 << BIT_OFFSET(n));
}

void set_Prime(unsigned int n) {// you should never use this but i added it anyways
	primes[WORD_OFFSET(n)] &= ~(1 << BIT_OFFSET(n)); 
}

int check_Prime(unsigned int n) {
	
	word_t bit = primes[WORD_OFFSET(n)] & (1 << BIT_OFFSET(n));
	return bit == 0; 
}

/*from class check over later*/
//#define BYTES 536870912
//uint8_t bitmap[BYTES];

//uint8_t MASK[] = {1 << 0, 1 << 1, 1 << 2, 1 << 3,
//                  1 << 4, 1 << 5, 1 << 6, 1 << 7};

//check bit in bitmap
bitmap[BYTE] & MASK[3];

//set bit in bitmap
//bitmap[354] | MASK[2];

//is the bit corresponding to 432935 set?
//bitmap[432935 / 8] & MASK[432935 % 8];

/*
 *todo
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
 *write threaded version of code
 **pthreads
*/
