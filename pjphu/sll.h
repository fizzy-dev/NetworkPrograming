#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#define MAX 100
typedef struct 
{
    char username[MAX];
    char password[MAX];
    int number_act;
    int number_sign;
    char homepage[MAX];
}DT;
typedef struct NODE
{
    DT x;
    struct NODE *next;
}NODE;
typedef struct LIST
{
    NODE *Head;
    NODE *Tail;
    NODE *Cur;
}LIST;
int isEmpty(LIST *l);
void Empty(LIST *l);
NODE *CreatNODE(DT a);
NODE* AddHead(LIST *l,DT a);
NODE *AddTail(LIST *l,DT a);
NODE *FindByUsername(LIST *l, char *username);
void Update(LIST *l,DT a);
void AddAfter(LIST *l,DT a);
void AddBefor(LIST *l,DT a);
void Free(LIST *l);
void InsertPos(LIST *l, DT p,int n);
void DeleteHeal(LIST *l);
void DeleteTail(LIST *l);
void DeleteCur(LIST *l);
void DeletePos(LIST *l,int x);
LIST *SeverseLIST(LIST *l);
void AddSort(LIST *l,DT a);
void PrintList(LIST *l);
int ListSize(LIST *l);