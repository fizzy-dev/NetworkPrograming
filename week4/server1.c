/* Echo server using UDP */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int sockfd, rcvBytes, sendBytes;
socklen_t len;
char buff[BUFF_SIZE + 1];
struct sockaddr_in servaddr, cliaddr;
//Step 1: Construct socket
if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
{
    perror("Error: ");
    return 0;
}
//Step 2: Bind address to socket
bzero(&servaddr, sizeof(servaddr));
servaddr.sin_family = AF_INET;
servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
servaddr.sin_port = htons(SERV_PORT);
if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)))
{
    perror("Error: ");
    return 0;
}
printf("Server started.");

//Step 3: Communicate with client
for ( ; ; ) {
len = sizeof(cliaddr);
rcvBytes = recvfrom(sockfd, buff, BUFF_SIZE, 0,
(struct sockaddr *) &cliaddr, &len);
if(rcvBytes < 0){
perror("Error: ");
return 0;
}
buff[recvBytes] = '\0';
printf("[%s:%d]: %s", inet_ntoa(cliaddr.sin_addr),
ntohs(cliaddr.sin_port), mesg);
sendBytes = sendto(sockfd, buff, rcvBytes, 0,
(struct sockaddr *) &cliaddr, len);
if(sendBytes < 0){
perror("Error: ");
return 0;
}
}