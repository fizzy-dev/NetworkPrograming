// udp client driver program
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

typedef struct user
{
    char username[20];
    char password[20];
} user;

// Driver code
int main(int argc, char *argv[])
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

    while (1)
    {

        user user1;
        char buffer[100];
        char password[20];
        char status[100];
        int g = 0;
        printf("Input username: ");
        g = scanf("%s", user1.username);
        getchar();
        printf("Input password: ");
        g = scanf("%s", user1.password);
        getchar();
        send(sock, (struct user *)&user1, sizeof(user1), 0);

        // waiting for response
        recv(sock, status, sizeof(status), 0);
        puts(status);
        int i = 0;
        //neu nhap sai ma khau
        while (strcmp(status, "Sai mat khau") == 0)
        {
            i++;
            printf("Input password: ");
            g = scanf("%s", password);
            getchar();

            send(sock, password, sizeof(password), 0);

            recv(sock, status, sizeof(status), 0);
            puts(status);
            if (i > 1)
            {
                puts("Tai khoan cua ban bi tam khoa vi da nhap qua 3 lan!");
                break;
            }
        }

        if (strcmp(status, "OK") == 0)
        {
            int input;
            char bye[100];
            printf("hello,%s\n", user1.username);
            do
            {
                printf("1.Doi password\n2.Dang xuat\nInput:");
                scanf("%d", &input);
                if (input == 1)
                {
                    printf("Newpassword:");
                    scanf("%s", password);
                    getchar();
                    send(sock, password, sizeof(password), 0);
                    puts("------------------------------------------");
                    // waiting for response
                    recv(sock, status, sizeof(status), 0);
                    puts(status);
                    recv(sock, status, sizeof(status), 0);
                    puts(status);
                }
                if (input == 2)
                {
                    printf("nhap 'bye' de dang xuat!:");
                    scanf("%s", bye);
                    if (strcmp(bye, "bye") != 0)
                    {
                        printf("hix,ban nhap sai!!!!\n");
                    }
                    else
                    {
                        break;
                    }
                }
            } while (1);
        }

        puts("-------------------------");
    }
    // close the descriptor
    close(sock);
    return 0;
}