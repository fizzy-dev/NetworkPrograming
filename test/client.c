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

    send_server_image(sock);
}
int send_server_image(int sock)
{
    int n = 0;
    int siz = 0;
    FILE *picture;
    char buf[50];
    char *s = "";

    puts("Getting image size");
    picture = fopen("./test.png", "r");
    if (picture == NULL)
    {
        puts("KHONG MO DUOC FILE");
    }
    fseek(picture, 0, SEEK_END);
    siz = ftell(picture);
    printf("%d", siz); // Output 880

    puts("Sending picture size to the server");
    sprintf(buf, "%d", siz);
    if ((n = send(sock, buf, sizeof(buf), 0)) < 0)
    {
        puts("loi 1");
    }

    char Sbuf[siz];
    puts("Sending the picture as byte array");
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