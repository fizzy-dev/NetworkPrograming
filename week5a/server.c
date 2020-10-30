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
	user user1;
	char password[100];
	char status[100];
	int listenfd, len;
	char only_string[100];
	char only_number[100];
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
	int server_fd, new_socket;
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
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}

	while (1)
	{
		//receive the datagram m
		int n = recv(new_socket, (struct Account *)&user1, sizeof(user1), 0);

		printf("%s\n", user1.username);
		printf("%s\n", user1.password);
		currentUser = find(user1.username);
		int check = checkUser(user1, new_socket);
		int i = 0;

		while (check == 3)
		{
			recv(new_socket, password, sizeof(password), 0);
			puts(password);

			// split data to only string and only number
			if (split(password, only_number, only_string) == 1)
			{
			}
			else
			{
				//puts("Error");
				strcpy(only_number, "Mat khau khong hop le");
				strcpy(only_string, "");
			}

			// send the response to client
			send(new_socket, only_number, sizeof(only_number), 0);
			send(new_socket, only_string, sizeof(only_string), 0);

			strcpy(currentUser->user.password, password);
			fclose(fp);
			//ghi lai file tu linklist
			fp = fopen(filename, "w");
			saveUsersToFile(fp);
			fclose(fp);
			// mo lai file de doc
			fp = fopen(filename, "r+");
		}

		while (check == 1)
		{
			i++;
			recv(new_socket, password, sizeof(password), 0);
			puts(password);
			user checkuser;
			strcpy(checkuser.username,user1.username);
			strcpy(checkuser.password,password);
			check = checkUser(checkuser, new_socket);
			if (i > 1)
			{
				currentUser->user.status = 0;
				fclose(fp);
				//ghi lai file tu linklist
				fp = fopen(filename, "w");
				saveUsersToFile(fp);
				fclose(fp);
				// mo lai file de doc
				fp = fopen(filename, "r+");
				break;
			}
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

int checkUser(user user, int new_socket)
{
	char status[100];
	struct node *finduser = NULL;
	finduser = find(user.username);
	if (finduser == NULL)
	{
		strcpy(status, "Tai khoan khong ton tai");
		send(new_socket, status, sizeof(status), 0);
		return 0;
	}
	else
	{
		if (strcmp(finduser->user.password, user.password) != 0)
		{
			strcpy(status, "Sai mat khau");
			send(new_socket, status, sizeof(status), 0);
			return 1;
		}
		else
		{
			if (finduser->user.status == 0 || finduser->user.status == 2)
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

int split(char *buffer, char *only_number, char *only_string)
{

	// Only number in buffer converts to string only_number
	strcpy(only_string, buffer);
	int k = 0;
	strcpy(only_number, buffer);
	int j = 0;

	// if number, copy to only_number
	// if character, copy to only_string
	int m = 0;
	for (int i = 0; i < 100; i++)
	{
		char ch = only_number[i];
		if (ch == '\0')
			break;
		if (ch >= '0' && ch <= '9')
		{
			only_number[j] = ch;
			j++;
		}
		else if ((ch >= 'a' && ch <= 'z') || (ch == ' '))
		{
			only_string[k] = ch;
			k++;
		}
		else
		{
			return 0;
		}
	}
	only_number[j] = '\0';
	only_string[k] = '\0';
	return 1;
}