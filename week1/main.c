#include <stdio.h>
#include "sll.h"
#include <string.h>

void insertFromFile(FILE *fp);
void menu();

void main()
{
   FILE *fp;
   fp = fopen("users.txt", "r+");
   if (fp == NULL)
   {
      printf("Mo file khong thanh cong");
      exit(1);
   }
   insertFromFile(fp);
   menu(fp);

   printf("Danh sach tai khoan: ");
   //in danh sach
   printList();

   fclose(fp);
}

void menu(FILE *fp)
{
   printf("\n");
   node *currentUser;
   char username[20];
   char pass[20];
   int choice;
   do
   {
      printf("1.Register\n2.Activate\n3.Sign in\n4.Search\n5.Change password\n6.Sign out\n=============\n0.(Exit Program)\n");
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
            insertFirst(username, pass, 2);
            fclose(fp);

            //ghi lai file tu linklist
            fp = fopen("users.txt", "w");
            saveUsersToFile(fp);
            fclose(fp);
            // mo lai file de doc
            fp = fopen("users.txt", "r+");
         }
         break;
      case 2:
      printf("Nhap username:  ");
      scanf("%s",username);
      printf("Nhap password:  ");
      gets();
      scanf("%s",pass);
      currentUser=find(username);
      if(currentUser==NULL){
         printf("Sai ten tai khoan hoac mat khau!\n");
      }else{
         if(strcmp(currentUser->user.password,pass)!=0){
            printf("Sai ten tai khoan hoac mat khau!\n");
         }else
         {
            if (currentUser->user.status==2){
               printf("ban chua active!!");
            }
            if(currentUser->user.status==1){
               printf("xin chao %s",currentUser->user.username);
            }
            if(currentUser->user.status==0){
               printf("tai khoan cua ban da bi khoa");
            }
            
         }
      }
         break;
      case 3:
         break;
      case 4:
         break;
      case 5:
         break;
      case 6:
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
   while ((fscanf(fp, "%s %s %d", username, pass, &status)) != EOF)
   {
      insertFirst(username, pass, status);
   }
}