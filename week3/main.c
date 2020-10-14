#include "sll.h"
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

void insertFromFile(FILE *fp);
void menu(FILE *fp, char filename[]);

void main()
{
   FILE *fp;
   char filename[20] = "users.txt";

   fp = fopen(filename, "r+");
   if (fp == NULL)
   {
      printf("Mo file khong thanh cong");
      exit(1);
   }
   insertFromFile(fp);
   menu(fp, filename);

   printf("Danh sach tai khoan: ");
   //in danh sach
   printList();

   fclose(fp);
}

void menu(FILE *fp, char filename[])
{
   printf("\n");
   node *currentUser = NULL;
   node *findUser = NULL;
   char username[20];
   char pass[20];
   int choice;
   char IpPattern[] = "^([0-9]{1,3}\\.){3}[0-9]{1,3}$";
   char DomainPattern[] = "^[a-zA-Z1-9_-]+(\\.[A-Za-z0-9]+){1,2}$";
   do
   {
      if (currentUser != NULL)
      {
         printf("-------------\nHi,%s\n------------\n", currentUser->user.username);
      }
      printf("1.Register\n2.Activate\n3.Sign in\n4.Search\n5.Change password\n6.Sign out\n7.Homepage with domainname\n8.Homepage with ip address\n=============\n0.(Exit Program)\n");
      printf("Your choice:");
      scanf("%d", &choice);
      switch (choice)
      {
      case 1:
         printf("Nhap username  :");
         scanf("%s", username);
         if (find(username) != NULL)
         {
            printf("Tai khoan da ton tai! \n");
         }
         else
         {
            printf("Nhap password  :");
            gets();
            scanf("%s", pass);
            insertFirst(username, pass, 2, "");
            printf("Dang ki tai khoan thanh cong!!\n");
            fclose(fp);

            //ghi lai file tu linklist
            fp = fopen(filename, "w");
            saveUsersToFile(fp);
            fclose(fp);
            // mo lai file de doc
            fp = fopen(filename, "r+");
         }
         break;
      case 2:
         printf("Nhap username:  ");
         scanf("%s", username);
         printf("Nhap password:  ");
         gets();
         scanf("%s", pass);
         currentUser = find(username);
         if (currentUser == NULL)
         {
            printf("Sai ten tai khoan hoac mat khau!\n");
         }
         else
         {
            if (strcmp(currentUser->user.password, pass) != 0)
            {
               currentUser = NULL;
               printf("Sai ten tai khoan hoac mat khau!\n");
            }
            else
            {

               //Neu tai khoan da active
               if (currentUser->user.status == 1)
               {
                  printf("xin chao %s,tai khoan da duoc kich hoat truoc do!!!\n", currentUser->user.username);
               }

               //neu tai khoan bi khoa
               if (currentUser->user.status == 0)
               {
                  printf("tai khoan cua ban dang bi khoa,lien he admin de mo khoa tai khoan\n");
               }

               // Neu tai khoan chua active
               if (currentUser->user.status == 2)
               {
                  int i = 0;
                  char code[20];
                  while (i < 3)
                  {
                     printf("Nhap ma kich hoat:   ");
                     scanf("%s", code);
                     if (strcmp(code, "LTM121216") == 0)
                     {
                        currentUser->user.status = 1;
                        fclose(fp);
                        fp = fopen(filename, "w");

                        //ghi lai file tu linklist
                        fp = fopen(filename, "w");
                        saveUsersToFile(fp);
                        fclose(fp);
                        // mo lai file de doc
                        printf("Account activated!!\n");
                        fp = fopen(filename, "r+");
                        break;
                     }
                     else
                     {
                        printf("Ban nhap code chua chinh xac!!\n");
                        i++;
                     }
                  }
                  if (i >= 3)
                  {
                     currentUser->user.status = 0;
                     fclose(fp);
                     fp = fopen(filename, "w");

                     //ghi lai file tu linklist
                     fp = fopen(filename, "w");
                     saveUsersToFile(fp);
                     fclose(fp);
                     // mo lai file de doc
                     fp = fopen(filename, "r+");
                     printf("Tai khoan cua ban da bi khoa vi nhap sai qua 3 lan!!\n");
                  }
               }
            }
         }
         break;
      case 3:
         printf("Nhap username:  ");
         scanf("%s", username);
         currentUser = find(username);
         if (currentUser == NULL)
         {
            printf("Tai khoan khong ton tai!\n");
         }
         else
         {
            int i = 0;
            while (i < 3)
            {
               printf("Nhap password:  ");
               scanf("%s", pass);
               if (strcmp(currentUser->user.password, pass) == 0)
               {
                  printf("Logged in\n");
                  break;
               }
               else
               {
                  printf("Sai mat khau! con %d lan thu!\n", (2 - i));
                  i++;
               }
               if (i >= 3)
               {
                  currentUser->user.status = 0;
                  fclose(fp);
                  //ghi lai file tu linklist
                  fp = fopen(filename, "w");
                  saveUsersToFile(fp);
                  fclose(fp);
                  // mo lai file de doc
                  fp = fopen(filename, "r+");
                  printf("Tai khoan cua ban da bi khoa vi nhap sai mk 3 lan \n");
               }
            }
         }

         break;
      case 4:
         if (currentUser == NULL)
         {
            printf("Ban phai dang nhap moi su dung duoc chung nang nay\n");
         }
         else
         {
            printf("Nhap username:  ");
            scanf("%s", username);
            findUser = find(username);
            if (findUser == NULL)
            {
               printf("Nguoi dung khong ton tai\n");
            }
            else
            {
               if (findUser->user.status == 0)
               {
                  printf("%s | Blocked\n", findUser->user.username);
               }
               if (findUser->user.status == 1)
               {
                  printf("%s | Activated\n", findUser->user.username);
               }
               if (findUser->user.status == 2)
               {
                  printf("%s | Inactivated\n", findUser->user.username);
               }
            }
         }
         break;
      case 5:
         if (currentUser == NULL)
         {
            printf("Ban phai dang nhap de su dung chuc nang nay!!!\n");
         }
         else
         {
            printf("Nhap mat khau cu:  ");
            scanf("%s", pass);
            if (strcmp(pass, currentUser->user.password) != 0)
            {
               printf("Ban nhap sai mat khau!\n");
            }
            else
            {
               printf("Nhap mat khau moi:  ");
               scanf("%s", currentUser->user.password);
               fclose(fp);
               fp = fopen(filename, "w");
               saveUsersToFile(fp);
               fclose(fp);
               fp = fopen(filename, "r+");
               printf("Ban da thay doi mat khau thanh cong!\n");
            }
         }
         break;
      case 6:
         if (currentUser == NULL)
         {
            printf("Ban chua dang nhap!\n");
         }
         else
         {
            currentUser = NULL;
            printf("Ban da dang xuat\n");
         }
         break;
      case 7:
         if (currentUser == NULL)
         {
            printf("ban chua dang nhap\n");
            break;
         }
         if (match(currentUser->user.homepage, IpPattern) == 1) //Neu nguoi dung nhap vao ip
         {
            ip_to_hostname(currentUser->user.homepage);
         }
         else
         {
            if (match(currentUser->user.homepage, DomainPattern) == 1)
            {
               printf("%s\n", currentUser->user.homepage);
            }else{
               printf("Sai ip hoac domain,vui long kiem tra lai\n");
            }
         }
         break;
      case 8:
         if (currentUser == NULL)
         {
            printf("ban chua dang nhap\n");
            break;
         }
         if (match(currentUser->user.homepage, DomainPattern) == 1) //Neu nguoi dung nhap vao ip
         {
            hostname_to_ip(currentUser->user.homepage);
         }
         else
         {
            if (match(currentUser->user.homepage, IpPattern) == 1)
            {
               printf("%s\n", currentUser->user.homepage);
            }else{
               printf("sai ip hoac domain,vui long kiem tra lại\n");
            }
         }
         break;
      default:
         break;
      }

   } while (choice != 0);
   printf("\n");
}
void insertFromFile(FILE *fp)
{
   char username[20];
   char pass[20];
   int status;
   char homepage[50];
   while ((fscanf(fp, "%s %s %d %s", username, pass, &status, homepage)) != EOF)
   {
      if(homepage=='\n'){
         strcpy(homepage," ");
      }
      insertFirst(username, pass, status, homepage);
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
      return;
   }
   addr_list = (struct in_addr **)he->h_addr_list;
   for (i = 0; addr_list[i] != NULL; i++)
   {
      //Return the first one;
      printf("%s\n", inet_ntoa(*addr_list[i]));
   }
   printf("Alias:\n");
   if (he->h_aliases == NULL)
   {
      printf("Null");
   }
   for (int i = 0; he->h_addr_list[i] != NULL; i++)
   {
      printf("\t%s\n", he->h_aliases[i]);
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
   printf("%s\n", he->h_name);
   printf("Alias:\n");
   if (he->h_aliases == NULL)
   {
      printf("Null");
   }
   for (int i = 0; he->h_addr_list[i] != NULL; i++)
   {
      printf("\t%s\n", he->h_aliases[i]);
   }
}