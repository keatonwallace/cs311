/*
 *created 3/12/13
 *authored by Keaton Wallace
 *all code not written by me is creditted to the creator and is used under fair
 *use laws for educational purpose
 *
 */

#include<sys/types.h>/* basic system data types */
#include<sys/socket.h>/* basic socket definitions */
#include<sys/time.h>/* timeval{} for select() */
#include<time.h>/* timespec{} for pselect() */
#include<netinet/in.h>/* sockaddr_in{} and other Internet defns */
#include<arpa/inet.h>/* inet(3) functions */
#include<errno.h>
#include<fcntl.h>/* for nonblocking */
#include<netdb.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<strings.h>     /* for bzero */
#include<sys/stat.h>/* for S_xxx file mode constants */
#include<sys/uio.h>/* for iovec{} and readv/writev */
#include<unistd.h>
#include<sys/wait.h>
#include<sys/select.h>

#define LISTENQ 1024
#define MAXLINE 4096
#define MAXSOCKADDR 128
#define BUFFSIZE 4096

#define SERV_PORT 6549
#define SERV_PORT_STR "6549"

int main(int argc, char **argv)
{
	pid_t pid;
	switch(pid = fork()){
	case -1:
		perror("couldn't fork child process in compute");
		exit(EXIT_FAILURE);
	case 0:
		//ops_per_sec and server comm should go here
		find_perfection();
		break;
	case 1:
		pid_t process_pid = pid;
		break;
	}

	int sockfd;
	struct sockaddr_in servaddr;
	char recvline[MAXLINE];

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);

	connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	
	
	return 0;
}

unsigned int ops_a_sec()
{
	unsigned int i;
	unsigned int j = 7;
	clock_t begin, end;
	begin = clock();
	for(i = 1; i < 100000000; i++){
		i % j;
	}
	end = clock();
	unsigned int ops_per_sec = (int) 100000000/(end - begin);
	return 15 * ops_per_sec;
}

int is_perfect(unsigned int n)
{
	if(n == 1) //1 works, but is not a perfect number
		return 0;
	int i;
	int sum = 1;
	for(i=2; i<sqrt(n); i++){
		if((n % i) == 0){
			sum += i;
			sum += n/i;
		}
	}
	if(sum == n) 
		return 1;
	return 0;
}


/*todo
 *number of mods a sec --done
 *find perfect numbers in range 
 **is_perfect --done
 **check range with is_perfect
 *send perfect number to server
 *handle kill signal from server
 */
