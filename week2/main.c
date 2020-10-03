#include <stdio.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <regex.h>
#include <sys/types.h>
#include <string.h>

//validate data from input
int match(const char *string, const char *pattern);
//domain to ip
void hostname_to_ip(char *hostname);
void ip_to_hostname(const char *ip);

int main(int argc, char *argv[])

{
    char IpPattern[] = "^([0-9]{1,3}\\.){3}[0-9]{1,3}$";
    char DomainPattern[] = "^[a-zA-Z1-9_-]+(\\.[A-Za-z0-9]+){1,2}$";
    if (argc !=2)
    {
        printf("Input a domain or ipaddress!!");
    }
    else
    {
        if (match(argv[1], IpPattern) == 1) //Neu nguoi dung nhap vao dia chi ip
        {
            ip_to_hostname(argv[1]);
        }
        else
        {
            if (match(argv[1], DomainPattern) == 1) //Neu nguoi dung nhap vao domain name
            {
                char *hostname = argv[1];
                hostname_to_ip(hostname);

            }
            else  //neu nguoi dung nhap vao khong phai ip hay domain name
            {
                printf("Sai dia chi ip hoac domain name\n");
            }
        }
    }
}

int match(const char *string, const char *pattern)
{
    regex_t re;
    if (regcomp(&re, pattern, REG_EXTENDED | REG_NOSUB) != 0)
        return 0;
    int status = regexec(&re, string, 0, NULL, 0);
    regfree(&re);
    if (status != 0)
        return 0;
    return 1;
}

/*
	Get ip from domain name
 */

void hostname_to_ip(char *hostname)
{
    struct hostent *he;
    struct in_addr **addr_list;
    int i;
    if ((he = gethostbyname(hostname)) == NULL)
    {
        // get the host info
        herror("gethostbyname");
        return ;
    }
    addr_list= (struct in_addr **) he->h_addr_list;
    for (i = 0; addr_list[i] != NULL; i++)
    {
        //Return the first one;
        printf("%s\n",inet_ntoa(*addr_list[i]));
    }
    printf("Alias:\n");
    if(he->h_aliases==NULL){
        printf("Null");
    }
    for(int i = 0; he->h_addr_list[i] != NULL; i++) {
         printf("\t%s\n",he->h_aliases[i]);
    }
}

void ip_to_hostname(const char *ip)
{
    struct hostent *he;
    int i;
    struct in_addr addr;
    inet_aton(ip, &addr);
    if ((he = gethostbyaddr(&addr, sizeof(addr), AF_INET)) == NULL)
    {
        // get the host info
        herror("gethostbyname");
        return;
    }
    printf("%s\n",he->h_name);
    printf("Alias:\n");
    if(he->h_aliases==NULL){
        printf("Null");
    }
    for(int i = 0; he->h_addr_list[i] != NULL; i++) {
         printf("\t%s\n",he->h_aliases[i]);
    }
}