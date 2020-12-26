#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define MAXLINE 1000
#define MAX 100

//sendfile
#include <ctype.h>
#include <errno.h>
#include <sys/stat.h>
#define BUFF_SIZE 1024
#define MSG_DUP_FILE "Error: File is existet on server."
#define MSG_RECV_FILE "Successful transfering."
#define MSG_CLOSE "Cancel file transfer"
#define MSG_RECV "Received."

typedef struct
{
    char username[MAX];
    char password[MAX];
    int number_act;
    int number_sign;
    char homepage[MAX];
} DT;

typedef struct Data
{
    char fileName[MAX];
    char content[MAX];
    DT sender;
} Data;

void main(int argc, char *argv[])
{
    // catch wrong input
    if (argc != 3)
    {
        printf("Please input IP address and port number\n");
        return 0;
    }
    // ip_address of server
    // port number
    char *ip_address = argv[1];
    char *port_number = argv[2];
    int port = atoi(port_number);
    int sock = 0;
    struct sockaddr_in serv_addr;

    // Try catch false when connecting
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, ip_address, &serv_addr.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    //-----------------connected to server------------------------------
    while (1)
    {
        menuLoginRegister(sock);
    }

    //-----------------disconnect to server----------------------------------
    close(sock);
    return;
}

//in menu khi chua dang nhap
void menuLoginRegister(int sock)
{
    int choose;
    char status[MAX];
    printf("1.Singup\n2.Log in\n3.Quit\nYour choice:");
    scanf("%d", &choose);
    switch (choose)
    {
    case 1:
        strcpy(status, "signup");
        send(sock, (char *)&status, sizeof(status), 0);
        registerUser(sock);
        break;
    case 2:
        strcpy(status, "login");
        send(sock, (char *)&status, sizeof(status), 0);
        loginUser(sock);
        break;
    case 3:
        return;
        break;
    default:
        return;
        break;
    }
}

void registerUser(int sock)
{
    // while (1)
    // {
    DT account;
    char buffer[MAX];
    char confirmPassword[MAX];
    char status[MAX];
    int g = 0;
    printf("Input username: ");
    g = scanf("%s", account.username);
    getchar();
    printf("Input password: ");
    g = scanf("%s", account.password);
    getchar();
    printf("Confirm password: ");
    g = scanf("%s", confirmPassword);
    getchar();
    if (strcmp(account.password, confirmPassword) != 0)
    {
        printf("Mat khau khong khop\n");
        //continue;
        registerUser(sock);
    }
    else
    {
        //Gui tai khoan muon dang ky len server
        send(sock, (struct DT *)&account, sizeof(account), 0);
        //nhan thong bao tu server
        recv(sock, status, sizeof(status), 0);
        if (strcmp(status, "ok") == 0)
        {
            puts("dk thanh cong");
            char downloadRepo[MAX];
            strcpy(downloadRepo, "downloads/");
            strcat(downloadRepo, account.username);
            makeDirectory(downloadRepo);
            //break;
            menuLoginRegister(sock);
        }
        if (strcmp(status, "accountExist") == 0)
        {
            puts(status);
            //break;
            menuLoginRegister(sock);
        }
    }
}

void loginUser(int sock)
{
    // while (1)
    // {
    DT account;
    char buffer[100];
    char password[20];
    char status[100];
    int g = 0;
    printf("Input username: ");
    g = scanf("%s", account.username);
    getchar();
    printf("Input password: ");
    g = scanf("%s", account.password);
    getchar();
    send(sock, (struct DT *)&account, sizeof(account), 0);

    // waiting for response
    recv(sock, status, sizeof(status), 0);
    puts(status);
    //Neu sai mat khau
    if (strcmp(status, "wrongPassword") == 0)
    {
        menuLoginRegister(sock);
    }
    if (strcmp(status, "accountNotExist") == 0)
    {
        menuLoginRegister(sock);
    }

    //neu dang nhap thanh cong
    if (strcmp(status, "ok") == 0)
    {
        DT currentUser = account;
        // DT * currentUser = (DT *)malloc(sizeof(DT));
        // (*currentUser)=account;
        while (1)
        {
            loggedInMenu(sock, currentUser);
        }
    }

    puts("-------------------------");
    //}
}

//in menu khi da dang nhap
void loggedInMenu(int sock, DT currentUser)
{
    puts("=======================================");
    printf("hello %s\n", currentUser.username);
    puts("---------------------------------------");
    printf("0.Show your file\n1.Show all user\n2.Change password\n3.Find file by name\n4.Find file by username\n5.Copy file from other user\n6.Create text file\n7.Upload file from local\n8.Download file\n9.Help\n10.Log out\n");
    puts("=======================================");
    printf("Your choice:");
    int choice;
    char status[MAX];
    scanf("%d", &choice);
    switch (choice)
    {
    case 0:
        strcpy(status, "showYourFile");
        send(sock, status, sizeof(status), 0);
        showYourFile(sock);
        loggedInMenu(sock, currentUser);
    case 1:
        strcpy(status, "showAllUsers");
        send(sock, (char *)&status, sizeof(status), 0);
        showAllUsers(sock);
        loggedInMenu(sock, currentUser);
        break;
    case 2:
        strcpy(status, "changePassword");
        send(sock, (char *)&status, sizeof(status), 0);
        changePassword(sock, currentUser);
        loggedInMenu(sock, currentUser);
        break;
    case 3:
        strcpy(status, "findByFileName");
        send(sock, (char *)&status, sizeof(status), 0);
        findByFileName(sock, currentUser);
        loggedInMenu(sock, currentUser);
        break;
    case 4:
        strcpy(status, "findByUserName");
        send(sock, (char *)&status, sizeof(status), 0);
        findByUserName(sock, currentUser);
        loggedInMenu(sock, currentUser);
        break;
    case 5:
        strcpy(status, "copyFromOtherUser");
        send(sock, (char *)&status, sizeof(status), 0);
        copyFileFromOtherUser(sock, currentUser);
        loggedInMenu(sock, currentUser);
        break;
    case 6:
        strcpy(status, "userCreateFile");
        send(sock, (char *)&status, sizeof(status), 0);
        userCreateFile(sock, currentUser);
        loggedInMenu(sock, currentUser);
        break;
    case 7:
        strcpy(status, "userUploadFile");
        send(sock, status, sizeof(status), 0);
        uploadExistFile(sock, currentUser);
        loggedInMenu(sock, currentUser);
        break;
    case 8:
        strcpy(status, "downloadFile");
        send(sock, (char *)&status, sizeof(status), 0);
        downloadFile(sock, currentUser);
        loggedInMenu(sock, currentUser);
        break;
    case 9:
        strcpy(status, "help");
        send(sock, (char *)&status, sizeof(status), 0);
        helpUser(sock);
        loggedInMenu(sock, currentUser);
        break;
    case 10:
        strcpy(status, "logout");
        send(sock, (char *)&status, sizeof(status), 0);
        logoutUser(sock, currentUser);
        menuLoginRegister(sock);
        break;
    default:
        break;
    }
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
//show all current user file
void showYourFile(int sock)
{
    char buff[MAX];
    recv(sock, buff, sizeof(buff), 0);
    int numberOfFile = atoi(buff);
    printf("All Your File:\n");
    for (int i = 0; i < numberOfFile; i++)
    {
        recv(sock, buff, sizeof(buff), 0);
        if (strcmp(buff, ".") == 0 || strcmp(buff, "..") == 0)
        {
            continue; // bo 2 file . va ..
        }
        puts(buff);
    }
}
//nguoi dung gui file len sever
void userCreateFile(int sock, DT currentUser)
{
    Data sentData;
    printf("Input file name:\n");
    scanf("%s", sentData.fileName);
    getchar();
    printf("Input content:\n");
    fgets(sentData.content, sizeof(sentData.content), stdin);
    //scanf("%s", sentData.content);
    //getchar();
    strcpy(sentData.sender.username, currentUser.username);
    //sentData.sender = currentUser;
    send(sock, (struct Data *)&sentData, sizeof(sentData), 0);
}
//upload from local
void uploadExistFile(int sock, DT currentUser)
{
    char filePath[MAX];
    int n = 0;
    int siz = 0;
    FILE *picture;
    char buf[50];
    char *s = "";
    printf("Nhap duong dan den file:\n");
    scanf("%s", filePath);
    getchar();
    client_send_file(sock, filePath);
}

void showAllUsers(int sock)
{
    char username[MAX];
    int size;
    char buf[MAX];
    recv(sock, buf, sizeof(buf), 0);
    size = atoi(buf);
    printf("Co tat ca %d nguoi dung\n", size);
    int i;
    for (i = 0; i < size; i++)
    {
        recv(sock, username, sizeof(username), 0);
        puts(username);
    }
}

void copyFileFromOtherUser(int sock, DT currentUser)
{
    char user[MAX];
    char file[MAX];
    char status[MAX];
    puts("Copy file from:");
    scanf("%s", user);
    send(sock, user, sizeof(user), 0); //gui user len sever

    recv(sock, status, sizeof(status), 0);
    if (strcmp(status, "notExist") == 0)
    {
        puts("User not exist!");
        return;
    }
    if (strcmp(status, "ok") == 0)
    {
        puts("what file?:");
        scanf("%s", file);
        send(sock, file, sizeof(file), 0);

        recv(sock, status, sizeof(status), 0); //check if file exist
        if (strcmp(status, "fileNotExist") == 0)
        {
            printf("%s doesn't have any file named %s\n", user, file);
            return;
        }
        if (strcmp(status, "copyOk") == 0)
        {
            puts("Copy success!");
            return;
        }
    }
}

void changePassword(int sock, DT currentUser)
{
    char currentPassword[MAX];
    char status[MAX];
    int flag = 0;
    for (int i = 0; i < 3; i++)
    {
        puts("Input current password:");
        scanf("%s", currentPassword);
        //gui mat khau cu len server
        send(sock, currentPassword, sizeof(currentPassword), 0);
        //server tra ket qua so sanh voi mat khau cu
        recv(sock, status, sizeof(status), 0);
        if (strcmp(status, "incorrect") == 0)
        {
            puts("sai mk");
            flag++;
            continue;
        }
        if (strcmp(status, "correct") == 0)
        {
            char newPassword[MAX];
            puts("Input new password:");
            scanf("%s", newPassword);
            getchar();
            send(sock, newPassword, sizeof(newPassword), 0);
            puts("doi mk thanh cong");
            break;
        }
    }
}

void findByFileName(int sock, DT currentUser)
{
    char keyword[MAX];
    puts("Input file name");
    scanf("%s", keyword);
    //gui keyword
    send(sock, keyword, sizeof(keyword), 0);

    //recv number of file
    char buff[MAX];
    recv(sock, buff, sizeof(buff), 0);
    int numberOfFile = atoi(buff);
    printf("Found %d result :\n", numberOfFile);
    for (int i = 0; i < numberOfFile; i++)
    {
        recv(sock, buff, sizeof(buff), 0);
        puts(buff);
    }
}

void downloadFile(int sock, DT currentUser)
{
    char user[MAX];
    char file[MAX];
    char status[MAX];
    puts("Download file from user?:");
    scanf("%s", user);
    send(sock, user, sizeof(user), 0); //gui user len sever

    recv(sock, status, sizeof(status), 0);
    if (strcmp(status, "userNotExist") == 0)
    {
        puts("User not exist!");
        return;
    }

    if (strcmp(status, "userOk") == 0)
    {
        puts("what file?:");
        scanf("%s", file);
        send(sock, file, sizeof(file), 0);

        recv(sock, status, sizeof(status), 0); //check if file exist
        if (strcmp(status, "fileNotExist") == 0)
        {
            printf("%s doesnt have file named %s\n", user, file);
            return;
        }
        if (strcmp(status, "downloadOk") == 0)
        {
            char dest[MAX];
            strcpy(dest, "downloads/");
            strcpy(dest, currentUser.username);
            client_recv_file(sock, dest, currentUser);

            // char buff[MAX];
            // char filename[MAX];
            // recv(sock, buff, sizeof(buff), 0);         //recv size
            // recv(sock, filename, sizeof(filename), 0); //recv name
            // int siz = atoi(buff);
            // printf("%d\n", siz);

            // char Rbuffer[siz];
            // puts("Reading image byte array");
            // int n = 0;
            // if ((n = recv(sock, Rbuffer, sizeof(Rbuffer), 0)) < 0)
            // {
            //     puts("loi 2");
            // }
            // puts("Converting byte array to image");
            // FILE *image;
            // char filePath[MAX];
            // // ./downloads/username/filename
            // strcpy(filePath, "./");
            // strcpy(filePath, "downloads/");
            // strcat(filePath, currentUser.username);
            // strcat(filePath, "/");
            // //strcat(filePath, "/");
            // strcat(filePath, filename);

            // image = fopen(filePath, "w");
            // fwrite(Rbuffer, sizeof(char), sizeof(Rbuffer), image);
            // fclose(image);

            puts("Download success!");
            return;
        }
    }
}

void findByUserName(int sock, DT currentUser)
{
    char keyword[MAX];
    puts("Input username:");
    scanf("%s", keyword);
    send(sock, keyword, sizeof(keyword), 0);

    char buff[MAX];
    recv(sock, buff, sizeof(buff), 0);
    if (strcmp(buff, "AccountNotExist") == 0)
    {
        puts("Account not exist!");
        return;
    }
    int numberOfFile = atoi(buff);
    printf("Tim thay %d file cua user %s:\n", (numberOfFile - 2), keyword);
    for (int i = 0; i < numberOfFile; i++)
    {
        recv(sock, buff, sizeof(buff), 0);
        if (strcmp(buff, ".") == 0 || strcmp(buff, "..") == 0)
        {
            continue; // bo 2 file . va ..
        }
        puts(buff);
    }
}

void helpUser(int sock)
{
    char buff[MAX];
    recv(sock, buff, sizeof(buff), 0);
    puts(buff);
}

void logoutUser(int sock, DT currentUser)
{
    DT new;
    currentUser = new;
    puts("Logged out!");
}

//client send file

char *extract_file_name(char *file_path)
{
    int i;
    int n = strlen(file_path);
    char *file_name;
    for (i = n - 1; i >= 0; --i)
    {
        if (file_path[i] == '/')
            break;
    }

    if (i == 0) //current directory so that no '/'
        return file_path;

    file_name = (char *)malloc((n - i) * sizeof(char));
    memcpy(file_name, &file_path[i + 1], n - i);

    return file_name;
}

int client_send_file(int client_sock, char *file_path)
{
    struct stat st;
    char recv_data[BUFF_SIZE];
    char sendbuff[BUFF_SIZE];
    int bytes_sent, bytes_received;
    FILE *fp;
    int nLeft, idx;
    char *file_name = NULL;
    off_t file_size = 0;
    char file_size_str[65];
    size_t result = 0;

    if (file_path[0] == '\0')
    { // enter an empty string
        printf("Sending file ended. Exiting.\n");
        bytes_sent = send(client_sock, file_path, 1, 0);
        if (bytes_sent <= 0)
            printf("Connection closed!\n");
        return 1;
    }

    // check if file exists
    if (stat(file_path, &st) == -1)
    { // Not exists
        fprintf(stderr, "Error: File not found.\n");
        bytes_sent = send(client_sock, MSG_CLOSE, strlen(MSG_CLOSE), 0); //echo error message
        if (bytes_sent <= 0)
            printf("Connection closed!\n");
        return -1;
    }

    file_name = extract_file_name(file_path);
    printf("Uploading file to server: %s\n", file_name);
    bytes_sent = send(client_sock, file_name, strlen(file_name), 0);
    if (bytes_sent <= 0)
    {
        printf("Connection closed!\n");
        return -1;
    }

    // confirm that server received file name and check file status on server side
    bytes_received = recv(client_sock, recv_data, BUFF_SIZE - 1, 0);
    if (bytes_received <= 0)
    {
        printf("Connection closed!\n");
        return -1;
    }
    else
        recv_data[bytes_received] = '\0';

    printf("%s\n", recv_data);
    if (strcmp(recv_data, MSG_DUP_FILE) == 0) //file was found on server, duplicate file
        return -1;
    bzero(recv_data, sizeof(recv_data));

    file_size = st.st_size;
    sprintf(file_size_str, "%lu", file_size);
    bytes_sent = send(client_sock, file_size_str, strlen(file_size_str), 0);
    if (bytes_sent <= 0)
    {
        printf("Connection closed!\n");
        return -1;
    }

    //open file and send data
    if ((fp = fopen(file_path, "rb")) == NULL)
    {
        fprintf(stderr, "Open file error.\n");
        exit(1);
    }
    int loop_size = file_size;
    nLeft = file_size % BUFF_SIZE; // cuz file size is not divisible by BUFF_SIZE

    while (loop_size > 0)
    {
        idx = 0;

        result += fread(sendbuff, 1, nLeft, fp); // use fread instead of fgets because fgets stop reading if newline is read
        while (nLeft > 0)
        {
            // Assume s is a valid, connected stream socket
            bytes_sent = send(client_sock, &sendbuff[idx], nLeft, 0);
            if (bytes_sent <= 0)
            {
                // Error handler
                printf("Connection closed.Trying again.\n");
            }
            nLeft -= bytes_sent;
            idx += bytes_sent;
        }

        bzero(sendbuff, sizeof(sendbuff));
        loop_size -= BUFF_SIZE; // decrease unfinished bytes
        nLeft = BUFF_SIZE;      // reset nLeft
    }

    if (result != file_size)
    {
        printf("Error reading file.\n");
        return -1;
    }

    bytes_received = recv(client_sock, recv_data, BUFF_SIZE - 1, 0);
    if (bytes_received <= 0)
    {
        printf("Connection closed!\n");
        return -1;
    }
    else
        recv_data[bytes_received] = '\0';

    printf("%s\n", recv_data);
    if (strcmp(recv_data, MSG_RECV_FILE) != 0) //if cannot receive last message, file transfer is interrupted
        return -1;

    // clean
    fclose(fp);
    free(file_name);
    return 0;
}

//client receive file

void clean_and_restore(FILE **fp)
{
    if (*fp != NULL)
        fclose(*fp);
    chdir("../..");
}

int client_recv_file(int conn_sock, char *dir_name, DT currentUser)
{
    struct stat st;
    int size_file;
    char recv_data[BUFF_SIZE];
    int bytes_sent, bytes_received;
    //char dir_name[] = "destination";
    char *tok;
    FILE *fp = NULL;
    int nLeft, idx;

    // // check if directory exists, if not mkdir
    // if (stat(dir_name, &st) == -1)
    // {
    // 	tok = strtok(dir_name, "/");
    // 	while (tok != NULL)
    // 	{
    // 		mkdir(tok, 0700); //config permissions by changing 2nd argument
    // 		chdir(tok);		  // chdir ~ cd
    // 		tok = strtok(NULL, "/");
    // 	}
    // }
    // else
    // 	chdir(dir_name);

    //receives file name
    bytes_received = recv(conn_sock, recv_data, BUFF_SIZE - 1, 0);
    if (bytes_received <= 0)
    {
        printf("Connection closed\n");
        clean_and_restore(&fp);
        return -1; //meet error, aborted
    }
    else
        recv_data[bytes_received] = '\0'; // check with client send format

    if (recv_data[0] == '\0')
    {
        printf("Receiving data from client end. Exiting.\n");
        clean_and_restore(&fp);
        return 1;
    }

    if (strcmp(recv_data, MSG_CLOSE) == 0)
    { //file not found on client
        printf("No file found on client. Transmission aborted.\n");
        clean_and_restore(&fp);
        return -1;
    }

    // check if file exists, if not create new one, else return error
    // already at destination folder
    // echo to client
    char dest[MAX];
    //currentuser/file
    strcpy(dest, "./");
    strcpy(dest, "downloads/");
    strcat(dest, currentUser.username);
    strcat(dest, "/");
    strcat(dest, recv_data);
    puts(dest);
    if (stat(dest, &st) == -1)
    { // file does not exist
        fp = fopen(dest, "wb");
        if (fp == NULL)
        {
            printf("File path error\n");
            clean_and_restore(&fp);
            return -1;
        }
        bytes_sent = send(conn_sock, MSG_RECV, strlen(MSG_RECV), 0); //echo that received file name and no duplicate file on server
        if (bytes_sent <= 0)
        {
            printf("Connection closed\n");
            clean_and_restore(&fp);
            return 1; //meet error, aborted
        }
    }
    else
    {
        printf("Duplicate file.\n");
        bytes_sent = send(conn_sock, MSG_DUP_FILE, strlen(MSG_DUP_FILE), 0);
        if (bytes_sent <= 0)
        {
            printf("Connection closed\n");
        }
        clean_and_restore(&fp);
        return 1;
    }

    printf("File name: %s\n", recv_data);
    bzero(recv_data, bytes_received); //empty buffer

    //receives file size
    bytes_received = recv(conn_sock, recv_data, BUFF_SIZE - 1, 0);
    if (bytes_received <= 0)
    {
        printf("Connection closed\n");
        clean_and_restore(&fp);
        return 1; //meet error, aborted
    }
    else
        recv_data[bytes_received] = '\0'; // check with client send format

    size_file = atoi(recv_data);

    printf("File size: %s\n", recv_data);
    bzero(recv_data, bytes_received); //empty buffer

    nLeft = size_file % BUFF_SIZE; // cuz file size is not divisible by BUFF_SIZE
    int loop_size = size_file;

    while (loop_size > 0)
    {
        idx = 0; // reset idx

        while (nLeft > 0)
        {
            bytes_received = recv(conn_sock, &recv_data[idx], nLeft, 0); // read at missing data index
            if (bytes_received <= 0)
            {
                // Error handler
                printf("Connection closed. Trying again.\n");
            }
            idx += bytes_received; // if larger then socket size
            nLeft -= bytes_received;
        }

        fwrite(recv_data, 1, idx, fp); //idx is the real length of recv_data
        bzero(recv_data, sizeof(recv_data));
        loop_size -= BUFF_SIZE; // decrease unfinished bytes   		-
        nLeft = BUFF_SIZE;      // reset nLeft
    }

    // echo to client that transmission sucessfully completed
    bytes_sent = send(conn_sock, MSG_RECV_FILE, strlen(MSG_RECV_FILE), 0);
    if (bytes_sent <= 0)
    {
        printf("Connection closed\n");
        clean_and_restore(&fp);
        return 1; //meet error, aborted
    }

    // sucessful block
    fclose(fp);
    //chdir("../.."); //return original folder

    return 0;
}