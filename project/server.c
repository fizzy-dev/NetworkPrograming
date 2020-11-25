#include<stdio.h>
#include<string.h>
#include "sll.h"

//them user tu file vao linkedlist
void insertFromFile(LIST *listUser,char *fileName);

void main(){
    LIST *listUser = (LIST *)malloc(sizeof(LIST));
    insertFromFile(listUser,"users.txt");
    PrintList(listUser);
    return;
}

void insertFromFile(LIST *listUser,char *fileName){
    FILE *fp;
    DT user;
    fp=fopen(fileName,"r+");
    if(fp==NULL){
        printf("Loi doc file");
        exit(1);
    }
	while ((fscanf(fp, "%s %s %d", user.username, user.password, &user.status)) != EOF)
	{
		AddTail(listUser,user);
	}
    fclose(fp);
}