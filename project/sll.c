#include "sll.h"
void Empty(LIST *l)
{
	l->Cur = l->Head = l->Tail = NULL;
}
int isEmpty(LIST *l)
{
	return (l->Head == NULL);
}
NODE *CreatNODE(DT a)
{
	NODE *p = (NODE *)malloc(sizeof(NODE));
	if (p == NULL)
		return NULL;
	p->x = a;
	p->x.number_act = 0;
	p->x.number_sign = 0;
	p->next = NULL;
	return p;
}
NODE *AddHead(LIST *l, DT a)
{
	if (!FindByUsername(l, a.username))
		return NULL;
	NODE *p = CreatNODE(a);
	if (l->Head == NULL)
		l->Head = l->Tail = p;
	else
	{
		p->next = l->Head;
		l->Head = p;
	}
	return p;
}
NODE *FindByUsername(LIST *l, char *username)
{
	NODE *tmp = l->Head;
	while (tmp != NULL)
	{
		if (strcmp(tmp->x.username, username) == 0)
		{
			return tmp;
		}
		tmp = tmp->next;
	}
	return NULL;
}
NODE *AddTail(LIST *l, DT a)
{
	if (FindByUsername(l, a.username))
		return NULL;
	NODE *p = CreatNODE(a);
	if (l->Head == NULL)
		l->Head = l->Tail = p;
	else
	{
		l->Tail->next = p;
		l->Tail = p;
	}
	l->Cur = p;
	return p;
}
void AddAfter(LIST *l, DT a)
{
	NODE *p = CreatNODE(a);
	if (l->Head == l->Cur)
		AddTail(l, a);
	else
	{
		p->next = l->Cur->next;
		l->Cur->next = p;
	}
	l->Cur = p;
}
void AddBefor(LIST *l, DT a)
{
	NODE *p = CreatNODE(a);
	if (l->Head == l->Cur)
		AddHead(l, a);
	else
	{
		NODE *b = l->Head;
		while (b->next != l->Cur)
			b = b->next;
		l->Cur = p;
		AddAfter(l, a);
	}
}
void Free(LIST *l)
{
	NODE *p = l->Head;
	while (p != NULL)
	{
		l->Head = l->Head->next;
		free(p);
		p = l->Head;
	}
	free(p);
}
void DeleteHeal(LIST *l)
{
	if (l->Head == NULL)
		printf("LIST rong\n");
	else
	{
		NODE *p = l->Head;
		l->Head = l->Head->next;
		l->Cur = l->Head;
		free(p);
	}
}
void DeleteTail(LIST *l)
{
	if (l->Head == NULL)
		printf("LIST rong\n");
	else if (l->Tail == l->Head)
		Free(l);
	else
	{
		NODE *a = l->Tail;
		NODE *c = l->Head;
		while (c->next != l->Tail)
			c = c->next;
		l->Tail = c;
		l->Tail->next = NULL;
		l->Cur = l->Tail;
		free(a);
	}
}
void DeleteCur(LIST *l)
{
	if (l->Head == NULL)
		printf("LIST rong\n");
	else if (l->Tail == l->Head)
		Free(l);
	else if (l->Cur == l->Head)
		DeleteHeal(l);
	else if (l->Cur == l->Tail)
		DeleteTail(l);
	else
	{
		NODE *a = l->Cur;
		NODE *c = l->Head;
		while (c->next != l->Cur)
			c = c->next;
		c->next = l->Cur->next;
		l->Cur = l->Cur->next;
		free(a);
	}
}
void DeletePos(LIST *l, int n)
{
	NODE *p = l->Head;
	for (int i = 0; i < n; i++)
	{
		if (p == NULL)
			break;
		p = p->next;
	}
	l->Cur = p;
	if (p == NULL)
		printf("Khong ton tai vi tri\n");
	else
		DeleteCur(l);
}

void InsertPos(LIST *l, DT p, int n)
{
	NODE *q = l->Head;
	for (int i = 0; i < n; i++)
	{
		if (q == NULL)
			break;
		q = q->next;
	}
	if (q == NULL)
		printf("Khong ton tai vi tri\n");
	else
	{
		if (n == 0)
			AddHead(l, p);
		else if (q == l->Tail)
			AddTail(l, p);
		else
			AddBefor(l, p);
	}
}
LIST *SeverseLIST(LIST *l)
{
	LIST *p = (LIST *)malloc(sizeof(LIST));
	NODE *a = l->Head;
	while (a != NULL)
	{
		AddHead(p, a->x);
		a = a->next;
	}
	return p;
}
void AddSort(LIST *l, DT a)
{
	if (isEmpty(l))
		AddHead(l, a);
	else
	{
		NODE *p = l->Head;
		while (strcmp(p->x.username, a.username) > 0)
		{
			p = p->next;
			if (p == NULL)
				break;
		}
		if (p == l->Head)
			AddHead(l, a);
		else if (p == NULL)
			AddTail(l, a);
		else
		{
			l->Cur = p;
			AddBefor(l, a);
		}
	}
}
void Update(LIST *l, DT a)
{
	NODE *p = l->Head;
	while (p != NULL)
	{
		if (strcmp(p->x.username, a.username) == 0)
		{
			strcpy(p->x.password, a.password);
			p->x.status = a.status;
		}
		p = p->next;
	}
}

void PrintList(LIST *l)
{
	NODE *p = l->Head;
	while (p != NULL)
	{
		printf("%s | %d\n", p->x.username, p->x.status);
		p = p->next;
	}
}