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
//make directory
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
//test
#include <unistd.h>
#include <pthread.h>
#define MAX_THREADS 6

#define MAXLINE 1000

//dinh dang file
typedef struct Data
{
	char fileName[MAX];
	char content[MAX];
	DT sender;
} Data;

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
	char status[MAX];
	int new_socket = (int)arg;
	DT receivedAccount;
	NODE *currentUser, *findUser;
	char password[100];

	LIST *listUser = (LIST *)malloc(sizeof(LIST));
	insertFromFile(listUser, "users.txt");
	puts(listUser->Head->x.username);
	puts("after insert");
	//PrintList(listUser);

	while (1)
	{
		recv(new_socket, (char *)&status, sizeof(status), 0);
		puts(status);
		//neu nguoi dung signup
		if (strcmp(status, "signup") == 0)
		{
			int n = recv(new_socket, (struct Account *)&receivedAccount, sizeof(receivedAccount), 0);
			findUser = FindByUsername(listUser, receivedAccount.username);
			//Neu da ton tai username -> bao loi cho client
			if (findUser != NULL)
			{
				strcpy(status, "accountExist");
				send(new_socket, status, sizeof(status), 0);
				continue;
			}
			//Neu username kha dung -> luu tai khoan vao database-> send ok to client+ tao thu muc tren server
			else
			{
				AddTail(listUser, receivedAccount);
				//PrintList(listUser);
				exportUserToFile(listUser, "users.txt");
				strcpy(status, "ok");
				send(new_socket, status, sizeof(status), 0);
				//tao folder tren server cho moi user
				char folderPath[50];
				// ./username
				strcpy(folderPath, "./");
				strcat(folderPath, receivedAccount.username);
				//puts(folderName);
				makeDirectory(folderPath);
				continue;
			}
		}
		//neu nguoi dung login
		if (strcmp(status, "login") == 0)
		{
			//receive the datagram
			int n = recv(new_socket, (struct Account *)&receivedAccount, sizeof(receivedAccount), 0);
			printf("%s\n", receivedAccount.username);
			printf("%s\n", receivedAccount.password);
			currentUser = FindByUsername(listUser, receivedAccount.username);
			//kiem tra cac truong hop dang nhap <sai mk> <tk khong ton tai> <dang nhap thanh cong>
			int check = checkUser(receivedAccount, new_socket, listUser);
			//continue;
		}
	}
}

int checkUser(DT user, int new_socket, LIST *listUser)
{
	puts(listUser->Head->x.username);
	puts("after login");
	char status[100];
	struct NODE *finduser = NULL;
	finduser = FindByUsername(listUser, user.username);
	//khong ton tai user
	if (finduser == NULL)
	{
		strcpy(status, "accountNotExist");
		send(new_socket, status, sizeof(status), 0);
		return 0;
	}
	else
	{
		//sai mat khau
		if (strcmp(finduser->x.password, user.password) != 0)
		{
			strcpy(status, "wrongPassword");
			send(new_socket, status, sizeof(status), 0);
			return 1;
		}
		//dang nhap thanh cong
		else
		{
			Data sentData;
			FILE *fp;
			strcpy(status, "ok");
			send(new_socket, status, sizeof(status), 0);
			while (1)
			{
				//nhan request tu user
				recv(new_socket, (char *)&status, sizeof(status), 0);
				//user muon show user
				if (strcmp(status, "showAllUsers") == 0)
				{
					puts("list size :");
					int size = 0;
					//lay kich co linkedlist
					size = ListSize(listUser);
					//gui so user cho client
					char buf[MAX];
					sprintf(buf, "%d", size);
					send(new_socket, buf, sizeof(buf), 0);
					//gui tung username ve cho client
					int i = 0;
					NODE *k = listUser->Head;
					for (i = 0; i < size; i++)
					{
						send(new_socket, k->x.username, sizeof(k->x.username), 0);
						puts(k->x.username);
						k = k->next;
					}
					puts("done!");
					continue;
				}

				if (strcmp(status, "changePassword") == 0)
				{
					int flag=0;
					char status[MAX];
					char currentPassword[MAX];
					for (int i = 0; i < 3; i++)
					{
						recv(new_socket, currentPassword, sizeof(currentPassword), 0);
						puts(currentPassword);
						if (strcmp(currentPassword, finduser->x.password) == 0)
						{
							strcpy(status,"correct");
							puts(status);
							send(new_socket,status,sizeof(status),0);
							break;
						}
						else
						{
							strcpy(status,"incorrect");
							puts(status);
							send(new_socket,status,sizeof(status),0);
							flag++;
							continue;
						}
					}
					//neu nhap sai mk qua 3 lan
					if(flag>=3){
						continue;
					}else{ //doi mk cho user
						char newPassword[MAX];
						recv(new_socket,newPassword,sizeof(newPassword),0);
						puts(newPassword);
						strcpy(finduser->x.password,newPassword);
						exportUserToFile(listUser,"users.txt");
						puts("Change password success");
					}
					continue;
				}
				//neu user muon tao file
				if (strcmp(status, "userCreateFile") == 0)
				{
					recv(new_socket, (struct Data *)&sentData, sizeof(sentData), 0);
					printf("data content :%s\n", sentData.content);
					char path[MAX];
					// ./username/filename.txt
					strcpy(path, "./");
					strcat(path, sentData.sender.username);
					strcat(path, "/");
					strcat(path, sentData.fileName);
					strcat(path, ".txt");
					fp = fopen(path, "w");
					puts(sentData.content);
					fprintf(fp, "%s", sentData.content);
					fclose(fp);
					continue;
				}
				//chuc nang uploadfile
				if (strcmp(status, "userUploadFile") == 0)
				{
					const char client_filepath[MAX];
					//nhan filepath tu client
					recv(new_socket, client_filepath, sizeof(client_filepath), 0);
					puts(client_filepath);
					puts("---------------------------");
					const char ch = '/';
					char *uploadFileName;
					uploadFileName = strrchr(client_filepath, ch);
					printf("%s", uploadFileName);
					int n = 0;
					puts("Reading image size");
					char buf[50];
					int siz = 0;
					if ((n = recv(new_socket, buf, sizeof(buf), 0) < 0))
					{
						puts("loi");
					}
					siz = atoi(buf);
					printf("%d", siz); // 880 output
					char Rbuffer[siz];
					puts("Reading image byte array");
					n = 0;
					if ((n = recv(new_socket, Rbuffer, sizeof(Rbuffer), 0)) < 0)
					{
						puts("loi 2");
					}
					puts("Converting byte array to image");
					FILE *image;
					char filePath[MAX];
					// ./username/filename
					strcpy(filePath, "./");
					strcat(filePath, finduser->x.username);
					//strcat(filePath, "/");
					strcat(filePath, uploadFileName);
					image = fopen(filePath, "w");
					fwrite(Rbuffer, sizeof(char), sizeof(Rbuffer), image);
					fclose(image);
					puts("done");
					continue;
				}
			}
			return 3;
		}
	}
}

//import user from file to linkedlist
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
	while ((fscanf(fp, "%s %s", user.username, user.password)) != EOF)
	{
		AddTail(listUser, user);
	}
	fclose(fp);
}

//server make directory for each user when signup
void makeDirectory(char *folderPath)
{
	struct stat st = {0};
	if (stat(folderPath, &st) == -1)
	{
		mkdir(folderPath, 0700);
	}
}

//export linkedlist user to file txt
void exportUserToFile(LIST *l, char *fileName)
{
	FILE *fp;
	NODE *currentUser;
	fp = fopen(fileName, "w");
	if (fp == NULL)
	{
		printf("Loi doc file");
		exit(1);
	}
	currentUser = l->Head;
	if (currentUser == NULL)
	{
		return;
	}
	while (currentUser != NULL)
	{
		fprintf(fp, "%s %s\n", currentUser->x.username, currentUser->x.password);
		currentUser = currentUser->next;
	}
	fclose(fp);
}