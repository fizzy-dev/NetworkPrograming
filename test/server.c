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
    user user1;
    char password[100];
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
    recv_client_image(new_socket);
}
int recv_client_image(int sock)
{

    int n = 0;

    puts("Reading image size");
    char buf[50];
    int siz = 0;
    if ((n = recv(sock, buf, sizeof(buf), 0) < 0))
    {
        puts("loi");
    }
    siz = atoi(buf);
    printf("%d", siz); // 880 output

    char Rbuffer[siz];
    puts("Reading image byte array");
    n = 0;
    if ((n = recv(sock, Rbuffer, sizeof(Rbuffer), 0)) < 0)
    {
        puts("loi 2");
    }

    puts("Converting byte array to image");
    FILE *image;
    image = fopen("recu.png", "w");
    fwrite(Rbuffer, sizeof(char), sizeof(Rbuffer), image);
    fclose(image);
    puts("done");
}