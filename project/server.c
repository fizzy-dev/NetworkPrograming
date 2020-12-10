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
//test
#include <unistd.h>
#include <pthread.h>
#define MAX_THREADS 6

#define MAXLINE 1000

//them user tu file vao linkedlist
void insertFromFile(LIST *listUser, char *fileName);
//xu li user
void *clientHandler(void *arg);

void main(int argc, char *argv[])
{

    // catch wrong input
	char status[100];
	int listenfd, len;


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
	int server_fd;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	// Forcefully attaching socket to the port
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

    pthread_t tid;
    while (1)
    {
        int new_socket;
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        pthread_create(&tid, NULL, &clientHandler, (void *)new_socket);
    }

}

void *clientHandler(void *arg)
{
	int new_socket = (int)arg;
    DT receivedAccount;
    NODE *currentUser;
	char password[100];

    LIST *listUser = (LIST *)malloc(sizeof(LIST));
    insertFromFile(listUser, "users.txt");
    PrintList(listUser);
	while (1)
	{
		//receive the datagram m
		int n = recv(new_socket, (struct Account *)&receivedAccount, sizeof(receivedAccount), 0);

		printf("%s\n", receivedAccount.username);
		printf("%s\n", receivedAccount.password);
		currentUser = FindByUsername(listUser,receivedAccount.username);
		int check = checkUser(receivedAccount, new_socket,listUser);
	}
}

int checkUser(DT user, int new_socket,LIST* listUser)
{
	char status[100];
	struct NODE *finduser = NULL;
	finduser = FindByUsername(listUser,user.username);
	if (finduser == NULL)
	{
		strcpy(status, "Tai khoan khong ton tai");
		send(new_socket, status, sizeof(status), 0);
		return 0;
	}
	else
	{
		if (strcmp(finduser->x.password, user.password) != 0)
		{
			strcpy(status, "Sai mat khau");
			send(new_socket, status, sizeof(status), 0);
			return 1;
		}
		else
		{
			if (finduser->x.status == 0 || finduser->x.status == 2)
			{
				strcpy(status, "Account not ready");
				send(new_socket, status, sizeof(status), 0);
				return 2;
			}
			else
			{ //ok
				strcpy(status, "OK");
				send(new_socket, status, sizeof(status), 0);
				return 3;
			}
		}
	}
}


void insertFromFile(LIST *listUser, char *fileName)
{
    FILE *fp;
    DT user;
    fp = fopen(fileName, "r+");
    if (fp == NULL)
    {
        printf("Loi doc file");
        exit(1);
    }
    while ((fscanf(fp, "%s %s %d", user.username, user.password, &user.status)) != EOF)
    {
        AddTail(listUser, user);
    }
    fclose(fp);
}