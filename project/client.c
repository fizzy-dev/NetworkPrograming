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
    int status;
    int number_act;
    int number_sign;
    char homepage[MAX];
} DT;

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

    menuLoginRegister(sock);

    //-----------------disconnect to server----------------------------------
    close(sock);
    return;
}

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
    while (1)
    {
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
            continue;
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
            }
            if (strcmp(status, "accountExist") == 0)
            {
                puts(status);
            }
        }
    }
}

void loginUser(int sock)
{
    while (1)
    {
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

        puts("-------------------------");
    }
}