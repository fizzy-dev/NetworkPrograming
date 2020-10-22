// server program for udp connection
#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "sll.h"
#define MAXLINE 1000

typedef struct user
{
	char username[20];
	char password[20];
} user;

// Driver code
int main(int argc, char *argv[])
{
	FILE *fp;
	node *findUser = NULL;
	node *currentUser = NULL;
	char filename[20] = "users.txt";
	fp = fopen(filename, "r+");
	if (fp == NULL)
	{
		printf("Mo file khong thanh cong");
		exit(1);
	}
	insertFromFile(fp);
	// catch wrong input

	if (argc == 1)
	{
		printf("Please input port number\n");
		return 0;
	}
	// port : port get from argv
	// buffer : data get from client
	// only_string : message send to client ( string only )
	// only_number : message send to client ( number only )
	char *port_number = argv[1];
	int port = atoi(port_number);
	user user1;
	char password[100];
	char status[100];
	int listenfd, len;
	struct sockaddr_in servaddr, cliaddr;
	bzero(&servaddr, sizeof(servaddr));
	// Create a UDP Socket
	listenfd = socket(AF_INET, SOCK_DGRAM, 0);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(port);
	servaddr.sin_family = AF_INET;

	// bind server address to socket descriptor
	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	while (1)
	{
		//receive the datagram m
		len = sizeof(cliaddr);
		int n = recvfrom(listenfd, (struct user *)&user1, sizeof(user1),
						 0, (struct sockaddr *)&cliaddr, &len); //receive message from server

		printf("%s\n", user1.username);
		printf("%s\n", user1.password);
		int check=checkUser(user1.username,user1.password,listenfd,status,cliaddr);

		while(check==1)
		{
			recvfrom(listenfd, password, sizeof(password),0, (struct sockaddr *)&cliaddr, &len);
			puts(password);
			check=checkUser(user1.username,password,listenfd,status,cliaddr);
	    }
	}
	fclose(fp);
}

void insertFromFile(FILE *fp)
{
	char username[20];
	char pass[20];
	int status;
	while ((fscanf(fp, "%s %s %d", username, pass, &status)) != EOF)
	{
		insertFirst(username, pass, status);
	}
}

int checkUser(char *username, char *password, int listenfd, char *status, struct sockaddr_in cliaddr)
{
	struct node *finduser = NULL;
	finduser = find(username);
	if (finduser == NULL)
	{
		strcpy(status, "Tai khoan khong ton tai");
		sendto(listenfd, status, MAXLINE, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
		return 0;
	}
	else
	{
		if (strcmp(finduser->user.password, password) != 0)
		{
			strcpy(status, "Sai mat khau");
			sendto(listenfd, status, MAXLINE, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
			return 1;
		}
		else
		{
			if (finduser->user.status == 0 || finduser->user.status == 2)
			{
				strcpy(status, "Account not ready");
				sendto(listenfd, status, MAXLINE, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
				return 2;
			}
			else
			{ //ok
				strcpy(status, "OK");
				sendto(listenfd, status, MAXLINE, 0, (struct sockaddr *)&cliaddr, sizeof(cliaddr));
				return 3;
			}
		}
	}
}
