#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define MAX 100

typedef struct file
{
   char fileName[MAX];
} file;

typedef struct nodeFile
{
   file item;
   struct nodeFile *next;
} nodeFile;

typedef struct listFile
{
   nodeFile *Head;
   nodeFile *Tail;
   nodeFile *Cur;
} listFile;

//hien thi danh sach
void printListFile(listFile *l)
{
   nodeFile *ptr = l->Head;
   printf("\n[ ");

   //bat dau tu phan dau danh sach
   while (ptr != NULL)
   {
      printf("(%s)", ptr->item.fileName);
      ptr = ptr->next;
   }
   printf(" ]\n");
}

int ListFileSize(listFile *l)
{
   int size = 0;
   nodeFile *p = l->Head;
   if (l->Head != NULL)
   {
      do
      {
         size = size + 1;
         p = p->next;
      } while (p != NULL);
   }
   else
   {
      size = 0;
      return size;
   }
   return size;
}

nodeFile *CreateNodeFile(file item)
{
   nodeFile *p = (nodeFile *)malloc(sizeof(nodeFile));
   if (p == NULL)
      return NULL;
   p->item = item;
   p->next = NULL;
   return p;
}

nodeFile *FindByFileName(listFile *l, char *filename)
{
   nodeFile *tmp = l->Head;
   while (tmp != NULL)
   {
      if (strcmp(tmp->item.fileName, filename) == 0)
      {
         return tmp;
      }
      tmp = tmp->next;
   }
   return NULL;
}

//chen link tai vi tri dau tien
nodeFile *AddHeadFile(listFile *l, file item)
{
   if (!FindByFileName(l, item.fileName))
      return NULL;
   nodeFile *p = CreateNodeFile(item);
   if (l->Head == NULL)
      l->Head = l->Tail = p;
   else
   {
      p->next = l->Head;
      l->Head = p;
   }
   return p;
}

//xoa phan tu dau tien
nodeFile *deleteFirst(listFile *l)
{
   if (l->Head == NULL)
      printf("LIST rong\n");
   else
   {
      nodeFile *p = l->Head;
      l->Head = l->Head->next;
      l->Cur = l->Head;
      free(p);
   }
}

int deleteFileByName(listFile *l, char *filename)
{
   nodeFile *p = l->Head;
   nodeFile *k = l->Head;
   if (l->Head == NULL)
   {
      return 0;
   }
   else
   {
      //sosanh head de p chay truoc,k chay sau
      if (strcmp(p->item.fileName, filename) == 0)
      {
         l->Head = l->Head->next;
         free(p);
      }
      else
      {
         p = p->next;
      }
      while (p->next != NULL)
      {
         if (strcmp(p->item.fileName, filename) == 0)
         {
            k->next = p->next;
            free(p);
            return 1;
         }
         p = p->next;
         k = k->next;
      }
      return 2; //khong co file can tim
   }
}
