#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct User
{
   char username[20];
   char password[20];
   int status;
} User;

typedef struct node
{
   User user;
   struct node *next;
}node;


//hien thi danh sach
void printList();
void insertFirst(char username[], char password[], int status);

struct node *deleteFirst();
bool isEmpty();
int length();
struct node *find(char *key);

void saveUsersToFile(FILE *fp);

// //xoa mot link voi key da cho
// struct node* deleteKey(int key){

//    //bat dau tu first link
//    struct node* current = head;
//    struct node* previous = NULL;

//    //neu list la trong
//    if(head == NULL){
//       return NULL;
//    }

//    //duyet qua list
//    while(current->key != key){

//       //neu day la last node
//       if(current->next == NULL){
//          return NULL;
//       }else {
//          //luu tham chieu toi link hien tai
//          previous = current;
//          //di chuyen toi next link
//          current = current->next;
//       }

//    }

//    //cap nhat link
//    if(current == head) {
//       //thay doi first de tro toi next link
//       head = head->next;
//    }else {
//       //bo qua link hien tai
//       previous->next = current->next;
//    }

//    return current;
// }

// // ham sap xep
// void sort(){

//    int i, j, k, tempKey, tempData ;
//    struct node *current;
//    struct node *next;

//    int size = length();
//    k = size ;

//    for ( i = 0 ; i < size - 1 ; i++, k-- ) {
//       current = head ;
//       next = head->next ;

//       for ( j = 1 ; j < k ; j++ ) {

//          if ( current->data > next->data ) {
//             tempData = current->data ;
//             current->data = next->data;
//             next->data = tempData ;

//             tempKey = current->key;
//             current->key = next->key;
//             next->key = tempKey;
//          }

//          current = current->next;
//          next = next->next;
//       }
//    }
// }

// // ham dao nguoc list
// void reverse(struct node** head_ref) {
//    struct node* prev   = NULL;
//    struct node* current = *head_ref;
//    struct node* next;

//    while (current != NULL) {
//       next  = current->next;
//       current->next = prev;
//       prev = current;
//       current = next;
//    }

//    *head_ref = prev;
// }
