#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

#define PORT 8021
#define BUFSIZE 200
#define LISTENQ 5
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))


//int lane_stat[8];
//int vehicles[3][6];
int confd;
struct sockaddr_in servaddr;
char fname[20], buf[BUFSIZE];
int server_fd, fd, opt=1, pid ,valread, size, cnt;
struct stat stat_buf;

void *thread1(void *vargp)
{
	while((valread=read(confd, buf, BUFSIZE-1))>0)
	{
		fflush(stdout);
		buf[valread]='\0';
		printf("%s\n", buf);
		fflush(stdout);
		//printf("SENT\n");
		fflush(stdout);
		if(cnt == 0)
		{
			cnt = 1;
		}
		else if(cnt == 1)
		{
			char *msg = "hellyZ";	
			write(confd,(void *)msg, strlen(msg));
			cnt = 0;
		}
	}
	printf("\n");
}

int main()
{

//----------------------------------------------------------------------------------------------------------------------------------------------
	/*int g=0;
	for(g=0;g<3;g++)
	{
		int k=0;
		for(k=0;k<6;k++)
			vehicles[g][k] = -1;
	} */



	if((server_fd=socket(AF_INET, SOCK_STREAM,0))==0)
	{
		perror("Socket Failed");
		exit(1);
	}

	printf("----------Server: Server started----------\n");
	if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	if(bind(server_fd, (struct  sockaddr *)&servaddr, sizeof(servaddr))<0)
	{
		printf("bind error\n");
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	if(listen(server_fd, LISTENQ)<0)
	{
		printf("listen error\n");
		perror("listen\n");
		exit(EXIT_FAILURE);
	}

	printf("\n____Waiting for Client____\n");
//----------------------------------------------------------------------------------------------------------------------------------------------
	cnt = 0;
	while(1)
	{
		confd=accept(server_fd, (struct sockaddr *) NULL, NULL);
		pthread_t tid1;
		pthread_create(&tid1, NULL, thread1, NULL);
		//pthread_create(&tid2, NULL, thread1, NULL);
		pthread_join(tid1, NULL);
	}
	return 0;
}

