// server program for udp connection
#include <stdio.h>
#include <strings.h>
#include <string.h>
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
//list directory
#include <errno.h>
#include <dirent.h>
//copy file
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

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
	int new_socket = (int)arg;

	// DT receivedAccount;
	// NODE *currentUser, *findUser;
	// char password[100];
	// char status[MAX];

	LIST *listUser = (LIST *)malloc(sizeof(LIST));
	insertFromFile(listUser, "users.txt");
	menuLoginRegister(new_socket, listUser);
}

void menuLoginRegister(int new_socket, LIST *listUser)
{
	while (1)
	{
		DT receivedAccount;
		NODE *currentUser, *findUser;
		char password[100];
		char status[MAX];

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
				//0.show your file
				if (strcmp(status, "showYourFile") == 0)
				{
					findFileByUserName(new_socket, listUser, finduser->x.username);
					continue;
				}
				//1.user muon show user
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
					//printf("%d\n",size);
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
				//2.user muon doi mat khau
				if (strcmp(status, "changePassword") == 0)
				{
					int flag = 0;
					char status[MAX];
					char currentPassword[MAX];
					for (int i = 0; i < 3; i++)
					{
						recv(new_socket, currentPassword, sizeof(currentPassword), 0);
						puts(currentPassword);
						if (strcmp(currentPassword, finduser->x.password) == 0)
						{
							strcpy(status, "correct");
							puts(status);
							send(new_socket, status, sizeof(status), 0);
							break;
						}
						else
						{
							strcpy(status, "incorrect");
							puts(status);
							send(new_socket, status, sizeof(status), 0);
							flag++;
							continue;
						}
					}
					//neu nhap sai mk qua 3 lan
					if (flag >= 3)
					{
						continue;
					}
					else
					{ //doi mk cho user
						char newPassword[MAX];
						recv(new_socket, newPassword, sizeof(newPassword), 0);
						puts(newPassword);
						strcpy(finduser->x.password, newPassword);
						exportUserToFile(listUser, "users.txt");
						puts("Change password success");
					}
					continue;
				}
				//3.Find file by key
				if (strcmp(status, "findByFileName") == 0)
				{
					char keyword[MAX];
					recv(new_socket, keyword, sizeof(keyword), 0);
					puts(keyword);
					findFileByFileName(listUser, keyword, new_socket);
					continue;
				}
				//4.Find file by username
				if (strcmp(status, "findByUserName") == 0)
				{
					char keyword[MAX];
					recv(new_socket, keyword, sizeof(keyword), 0);
					puts(keyword);
					findFileByUserName(new_socket, listUser, keyword);
					continue;
				}
				//5.Copy file from the other user
				if (strcmp(status, "copyFromOtherUser") == 0)
				{
					char user[MAX];
					char file[MAX];
					char status[MAX];
					recv(new_socket, user, sizeof(user), 0);
					NODE *p = FindByUsername(listUser, user);
					if (p == NULL)
					{
						strcpy(status, "notExist");
						send(new_socket, status, sizeof(status), 0);
						continue;
					}
					else
					{
						strcpy(status, "ok");
						send(new_socket, status, sizeof(status), 0);

						recv(new_socket, file, sizeof(file), 0);
						int check = checkFileExist(user, file);
						// puts(file);
						// printf("%d\n", check);
						if (check == 0)
						{
							strcpy(status, "fileNotExist");
							send(new_socket, status, sizeof(status), 0);
							puts(status);
							continue;
						}
						if (check == 1)
						{ //ton tai file
							char from[MAX];
							char to[MAX];
							//./username/filename
							strcpy(to, "");
							strcat(to, "./");
							strcat(to, finduser->x.username);
							strcat(to, "/");
							strcat(to, file);
							// ./username/filename
							strcpy(from, "");
							strcat(from, "./");
							strcat(from, user);
							strcat(from, "/");
							strcat(from, file);
							copy(to, from);
							strcpy(status, "copyOk");
							send(new_socket, status, sizeof(status), 0);
							puts(status);
							continue;
						}
					}
					continue;
				}
				//6.neu user muon tao file
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
				//7.chuc nang uploadfile
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

					// if ((n = recv(new_socket, Rbuffer, siz, 0)) < 0)
					// {
					// 	puts("loi");
					// }

					// puts("Converting byte array to image");
					// FILE *image;
					// char filePath[MAX];
					// // ./username/filename
					// strcpy(filePath, "./");
					// strcat(filePath, finduser->x.username);
					// //strcat(filePath, "/");
					// strcat(filePath, uploadFileName);
					// image = fopen(filePath, "w");
					// fwrite(Rbuffer, sizeof(char), siz, image);
					// fclose(image);

					puts("done");
					continue;
				}
				//8.Download file
				if (strcmp(status, "downloadFile") == 0)
				{
					char user[MAX];
					char file[MAX];
					char status[MAX];
					recv(new_socket, user, sizeof(user), 0);
					NODE *p = FindByUsername(listUser, user);
					if (p == NULL)
					{
						strcpy(status, "userNotExist");
						send(new_socket, status, sizeof(status), 0);
						continue;
					}
					else
					{
						strcpy(status, "userOk");
						send(new_socket, status, sizeof(status), 0);

						recv(new_socket, file, sizeof(file), 0);
						int check = checkFileExist(user, file);
						// puts(file);
						// printf("%d\n", check);
						if (check == 0)
						{
							strcpy(status, "fileNotExist");
							send(new_socket, status, sizeof(status), 0);
							puts(status);
							continue;
						}
						if (check == 1)
						{ //ton tai file
							char from[MAX];
							// ./username/filename
							strcpy(from, "");
							strcat(from, "./");
							strcat(from, user);
							strcat(from, "/");
							strcat(from, file);
							puts(from);

							strcpy(status, "downloadOk");
							send(new_socket, status, sizeof(status), 0);
							puts(status);
							// send file to user
							sendFileToClient(new_socket, from, file);
							continue;
						}
					}
					continue;
				}
				//9.Help
				if (strcmp(status, "help") == 0)
				{
					char buff[MAX];
					strcpy(buff, "comming soon ^^");
					send(new_socket, buff, sizeof(buff), 0);
					continue;
				}
				//10.log out
				if (strcmp(status, "logout") == 0)
				{
					puts(status);
					menuLoginRegister(new_socket, listUser);
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

int checkFileExist(char *user, char *filename)
{
	char path[MAX];
	// ./username
	strcpy(path, "");
	strcat(path, "./");
	strcat(path, user);
	DIR *dirp;
	struct dirent *dp;
	dirp = opendir(path);
	if (!dirp)
	{
		perror("opendir()");
		exit(1);
	}
	while (dp = readdir(dirp))
	{
		if (strcmp(dp->d_name, filename) == 0)
		{
			return 1;
		}
	}
	return 0; //
}
//ls
void findFileByFileName(LIST *l, char *keyword, int new_socket)
{
	NODE *p = l->Head;
	NODE *k = l->Head;
	int numberofFile = 0;
	int size = ListSize(l); //get number of user
	printf("%d\n", size);
	for (int i = 0; i < size; i++)
	{
		char path[MAX];
		strcpy(path, "");

		DIR *dirp;
		// ./username
		strcat(path, "./");
		strcat(path, p->x.username);
		puts(path);
		struct dirent *dp;
		dirp = opendir(path); //filepath
		strcpy(path, "");	  //resetpath
		if (!dirp)
		{
			perror("opendir()");
			exit(1);
		}
		while ((dp = readdir(dirp)))
		{
			//puts(dp->d_name);
			// char *ptr = strchr(dp->d_name, '.');
			// *ptr = 0;
			if (strcmp(dp->d_name, keyword) == 0)
			{
				puts(dp->d_name);
				numberofFile++;
			}
		}
		puts("+===================");
		if (errno)
		{
			perror("readdir()");
			exit(1);
		}
		p = p->next;
	}
	printf("%d\n", numberofFile);
	char buf[MAX];
	sprintf(buf, "%d", numberofFile);
	send(new_socket, buf, sizeof(buf), 0); //send numberoffile to client

	for (int i = 0; i < size; i++) //send file name+user to client
	{
		char path[MAX];
		strcpy(path, "");

		DIR *dirp;
		// ./username
		strcat(path, "./");
		strcat(path, k->x.username);

		struct dirent *dp;
		dirp = opendir(path); //filepath
		strcpy(path, "");	  //resetpath
		if (!dirp)
		{
			perror("opendir()");
			exit(1);
		}
		while ((dp = readdir(dirp)))
		{
			char file[MAX];
			// char *ptr = strchr(dp->d_name, '.');
			// *ptr = 0;
			//users | []
			strcpy(file, dp->d_name);
			strcat(file, " | From user: ");
			strcat(file, k->x.username);

			if (strcmp(dp->d_name, keyword) == 0)
			{
				send(new_socket, file, sizeof(file), 0);
			}
		}
		if (errno)
		{
			perror("readdir()");
			exit(1);
		}
		k = k->next;
	}
	puts("done");
}

void findFileByUserName(int new_socket, LIST *l, char *keyword)
{
	NODE *p = FindByUsername(l, keyword);
	char status[MAX];
	if (p == NULL)
	{
		strcpy(status, "AccountNotExist");
		send(new_socket, status, sizeof(status), 0);
		return;
	}
	int numberofFile = 0;
	char path[MAX];
	strcpy(path, "");
	DIR *dirp;
	// ./username
	strcat(path, "./");
	strcat(path, keyword);
	struct dirent *dp1;
	struct dirent *dp2;
	dirp = opendir(path); //filepath
	if (!dirp)
	{
		perror("opendir()");
		exit(1);
	}
	while ((dp1 = readdir(dirp)))
	{
		numberofFile++;
	}

	char buf[MAX];
	sprintf(buf, "%d", numberofFile);
	send(new_socket, buf, sizeof(buf), 0); //send numberoffile to client

	DIR *dirp1;
	dirp1 = opendir(path);
	while ((dp2 = readdir(dirp1)))
	{
		puts(dp2->d_name);
		strcpy(buf, dp2->d_name);
		send(new_socket, buf, sizeof(buf), 0);
	}
	if (errno)
	{
		perror("readdir()");
		exit(1);
	}
	puts("done!");
}

int copy(const char *to, const char *from)
{
	int fd_to, fd_from;
	char buf[4096];
	ssize_t nread;
	int saved_errno;

	fd_from = open(from, O_RDONLY);
	if (fd_from < 0)
		return -1;

	fd_to = open(to, O_WRONLY | O_CREAT | O_EXCL, 0666);
	if (fd_to < 0)
		goto out_error;

	while (nread = read(fd_from, buf, sizeof buf), nread > 0)
	{
		char *out_ptr = buf;
		ssize_t nwritten;

		do
		{
			nwritten = write(fd_to, out_ptr, nread);

			if (nwritten >= 0)
			{
				nread -= nwritten;
				out_ptr += nwritten;
			}
			else if (errno != EINTR)
			{
				goto out_error;
			}
		} while (nread > 0);
	}

	if (nread == 0)
	{
		if (close(fd_to) < 0)
		{
			fd_to = -1;
			goto out_error;
		}
		close(fd_from);

		/* Success! */
		return 0;
	}

out_error:
	saved_errno = errno;

	close(fd_from);
	if (fd_to >= 0)
		close(fd_to);

	errno = saved_errno;
	return -1;
}

void sendFileToClient(int new_socket, char from[], char file[])
{
	FILE *ptr;
	int siz;
	char buf[MAX];
	ptr = fopen(from, "r");
	if (ptr == NULL)
	{
		puts("mo file that bai");
	}
	fseek(ptr, 0, SEEK_END);
	siz = ftell(ptr);
	printf("%d", siz);
	puts("Sending file size to the client\n");
	sprintf(buf, "%d", siz);
	send(new_socket, buf, sizeof(buf), 0); //send size

	send(new_socket, file, sizeof(file), 0); //send name

	char FileBuf[siz];
	puts("Sending the picture as byte array\n");
	fseek(ptr, 0, SEEK_END);
	siz = ftell(ptr);
	fseek(ptr, 0, SEEK_SET); //Going to the beginning of the file

	while (!feof(ptr))
	{
		fread(FileBuf, sizeof(char), sizeof(FileBuf), ptr);
		send(new_socket, FileBuf, sizeof(FileBuf), 0);
		memset(FileBuf, 0, sizeof(FileBuf));
	}
	puts("done");
}