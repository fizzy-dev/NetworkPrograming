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
        /* code */
    }
    if (strcmp(status, "accountPassword") == 0)
    {
        /*code*/
    }

    //neu dang nhap thanh cong
    if (strcmp(status, "ok") == 0)
    {
        DT currentUser = account;
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
    printf("hello %s\n", currentUser.username);
    printf("1.Show all user\n2.Change password\n3.Find file by name\n4.Find file by username\n5.Copy file from other user\n6.Create text file\n7.Upload file from local\n8.Download file\n9.Help\n10.Log out\nYour choice:");
    int choice;
    char status[MAX];
    scanf("%d", &choice);
    switch (choice)
    {
    case 1:
        strcpy(status, "showAllUsers");
        break;
    case 2:
        strcpy(status, "changePassword");
        send(sock, (char *)&status, sizeof(status), 0);
        break;
    case 3:
        strcpy(status, "findByFileName");
        send(sock, (char *)&status, sizeof(status), 0);
        break;
    case 4:
        strcpy(status, "findByUserName");
        send(sock, (char *)&status, sizeof(status), 0);
        break;
    case 5:
        strcpy(status, "copyFromOther");
        send(sock, (char *)&status, sizeof(status), 0);
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
        strcpy(status, "userCreateFile");
        send(sock, (char *)&status, sizeof(status), 0);
        break;
    case 9:
        strcpy(status, "downloadFile");
        send(sock, (char *)&status, sizeof(status), 0);
        break;
    case 10:
        strcpy(status, "logout");
        send(sock, (char *)&status, sizeof(status), 0);
        break;
    default:
        break;
    }
}

//dinh dang file
// typedef struct Data
// {
//     char fileName[MAX];
//     char content[MAX];
//     DT sender;
// } Data;

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
    //gui file name len server
    send(sock, filePath, sizeof(filePath), 0);
    puts("Getting image size");
    picture = fopen(filePath, "r");
    if (picture == NULL)
    {
        puts("Mo file khong thanh cong\n");
    }
    fseek(picture, 0, SEEK_END);
    siz = ftell(picture);
    printf("%d", siz);

    puts("Sending picture size to the server\n");
    sprintf(buf, "%d", siz);
    if ((n = send(sock, buf, sizeof(buf), 0)) < 0)
    {
        puts("loi 1");
    }

    char Sbuf[siz];
    puts("Sending the picture as byte array\n");
    fseek(picture, 0, SEEK_END);
    siz = ftell(picture);
    fseek(picture, 0, SEEK_SET); //Going to the beginning of the file

    while (!feof(picture))
    {
        fread(Sbuf, sizeof(char), sizeof(Sbuf), picture);
        if ((n = send(sock, Sbuf, sizeof(Sbuf), 0)) < 0)
        {
            puts("loi2");
        }
        memset(Sbuf, 0, sizeof(Sbuf));
    }
}