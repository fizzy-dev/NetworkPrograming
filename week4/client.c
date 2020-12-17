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
    // ip_address : get ip from argv
    // port : get port from argv
    // buffer : data get from server
    // message: data send to server
    char *ip_address = argv[1];
    char *port_number = argv[2];
    int port = atoi(port_number);

    int sockfd, n;
    struct sockaddr_in servaddr;

    // clear servaddr
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_addr.s_addr = inet_addr(ip_address);
    servaddr.sin_port = htons(port);
    servaddr.sin_family = AF_INET;

    // create datagram socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    // connect to server
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("\n Error : Connect Failed \n");
        exit(0);
    }

    while (1)
    {

        user user1;
        char buffer[100];
        char password[20];
        int g = 0;
        printf("Input username: ");
        g = scanf("%s", user1.username);
        getchar();
        printf("Input password: ");
        g = scanf("%s", user1.password);
        getchar();
        sendto(sockfd, (struct user *)&user1, MAXLINE, 0, (struct sockaddr *)&servaddr, sizeof(servaddr));

        // waiting for response
        recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)NULL, NULL);
        puts(buffer);
        int i = 0;
        //neu nhap sai ma khau
        while (strcmp(buffer, "Sai mat khau") == 0)
        {
            i++;
            printf("Input password: ");
            g = scanf("%s", password);
            getchar();

            sendto(sockfd, password, MAXLINE, 0, (struct sockaddr *)&servaddr, sizeof(servaddr));

            recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)NULL, NULL);
            puts(buffer);
            if (i > 1)
            {
                puts("Tai khoan cua ban bi tam khoa vi da nhap qua 3 lan!");
                break;
            }
        }

        if (strcmp(buffer, "OK") == 0)
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
                    sendto(sockfd, password, MAXLINE, 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
                    puts("------------------------------------------");
                    // waiting for response
                    recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)NULL, NULL);
                    puts(buffer);
                    recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)NULL, NULL);
                    puts(buffer);
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
    close(sockfd);
    return 0;
}