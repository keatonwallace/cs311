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


int sieve(int n, int* primes)
{
	// Variables 'i' and 'k' are iteration variables.
	int i;
	int k;
	// sqrtn is used as a starting guesstimate for maximum iteration value
	int sqrt_n = sqrt(n);
	// numPrimes will tally the number of prime numbers, initaially, we have already counted 2 as a prime number
	int num_primes = 1;
	// Allocating memory for arrays, larger than we possibly need.
	// Array 'numbers' contains all numbers between 1 and n
	int* numbers = (int*) malloc(sizeof(int)*(n - 1));
	// Initializes 'numbers'
	for (i = 0; i < n - 1; i++) {
		numbers[i] = i + 1;
	}
	// Marking '1' and '2' (the first two values in 'numbers') as prime numbers. A negative value implies prime
	numbers[0] = - numbers[0];
	numbers[1] = - numbers[1];
	// Setting the first prime number as '2', so primes may be counted by iterating by 2, starting at 3
	primes[0] = 2;
	// Iterate until the square root on 'n'
	for (k = 2; k < sqrt_n; k+=2) {
		// Mark all numbers which 'k' is a factor of until 'n'
		if (numbers[k] > 0) {
			for (i = 2; i * (k + 1) < n; i++) {
				// Mark a number by turning it negative
				numbers[i * (k + 1) - 1] = -(i * (k + 1));
			}
			// Record our prime number, and add another tally to the total primes found
			primes[num_primes] = k + 1;
			num_primes++;
		}
	}
	// Find all unmarked numbers not yet iterated through, and record them in the primes list
	for (k; k < n - 1; k+=2) {
		if (numbers[k] > 0) {
			primes[num_primes] = k + 1;
			num_primes++;
		}
	}
	// Returns the number of primes, the primes array was modified and accessible outside this function
	return num_primes;
}

void fork()
{
	//on fork, copy-on-write semantics are used
	int pid;
	switch(pid = fork()){
	case -1:
		//oops...
	case 0:
		//child specific code goes here
		//we have lots of execs
		execlp("ls", "ls", (char *)NULL);
	default:
		//parent specific code goes here
	}
	//both parent and child continue executing here
	return 0;
}


int main(int argc, char **argv)
{
	/*add a arg for thread vs process*/
	/* This stuff is from the original prime stuff
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
#define BYTES 536870912
uint8_t bitmap[BYTES];

uint8_t MASK[] = {1 << 0, 1 << 1, 1 << 2, 1 << 3,
                  1 << 4, 1 << 5, 1 << 6, 1 << 7};

//check bit in bitmap
bitmap[BYTE] & MASK[3];

//set bit in bitmap
bitmap[354] | MASK[2];

//is the bit corresponding to 432935 set?
bitmap[432935 / 8] & MASK[432935 % 8];
