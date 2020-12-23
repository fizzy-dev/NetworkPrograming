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
    printf("hello %s\n", currentUser.username);
    printf("0.Show your file\n1.Show all user\n2.Change password\n3.Find file by name\n4.Find file by username\n5.Copy file from other user\n6.Create text file\n7.Upload file from local\n8.Download file\n9.Help\n10.Log out\nYour choice:");
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
    puts("==========================");
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
    // while (!feof(picture))
    // {
    //     n = fread(Sbuf, sizeof(char), siz, picture);
    //     if (n > 0)
    //     {                                           /* only send what has been read */
    //         if ((n = send(sock, Sbuf, siz, 0)) < 0) /* or (better?) send(sock, Sbuf, n, 0) */
    //         {
    //             puts("loi");
    //         }
    //     }
    //     /* memset(Sbuf, 0, sizeof(Sbuf)); useless for binary data */
    // }
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
        puts("=================================");
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
            printf("%s doesnt have file named %s\n", user, file);
            puts("===============================");
            return;
        }
        if (strcmp(status, "copyOk") == 0)
        {
            puts("Copy success!");
            puts("===============================");
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
    if (strcmp(status, "notExist") == 0)
    {
        puts("User not exist!");
        puts("=================================");
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
            printf("%s doesnt have file named %s\n", user, file);
            puts("===============================");
            return;
        }
        if (strcmp(status, "downloadOk") == 0)
        {

            char buff[MAX];
            char filename[MAX];
            recv(sock, buff, sizeof(buff), 0);         //recv size
            recv(sock, filename, sizeof(filename), 0); //recv name
            int siz = atoi(buff);
            printf("%d\n", siz);

            char Rbuffer[siz];
            puts("Reading image byte array");
            int n = 0;
            if ((n = recv(sock, Rbuffer, sizeof(Rbuffer), 0)) < 0)
            {
                puts("loi 2");
            }
            puts("Converting byte array to image");
            FILE *image;
            char filePath[MAX];
            // ./downloads/username/filename
            strcpy(filePath, "./");
            strcpy(filePath, "downloads/");
            strcat(filePath, currentUser.username);
            strcat(filePath, "/");
            //strcat(filePath, "/");
            strcat(filePath, filename);

            image = fopen(filePath, "w");
            fwrite(Rbuffer, sizeof(char), sizeof(Rbuffer), image);
            fclose(image);

            puts("Download success!");
            puts("===============================");
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
    puts("==========================");
}

void helpUser(int sock){
    char buff[MAX];
    recv(sock,buff,sizeof(buff),0);
    puts(buff);
    puts("=================================");
}

void logoutUser(int sock, DT currentUser)
{
    DT new;
    currentUser=new;
    puts("Logged out!");
}