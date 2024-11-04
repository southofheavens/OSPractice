#include "../include/list.h"
#include <stdio.h>

struct Link* MakeInt(int value)
{
    struct Link* lnk = (struct Link*)malloc(sizeof(struct Link));
    lnk->prev = 0;
    lnk->succ = 0;
    lnk->elem = value;
    return lnk;
}

void Init(struct List* lst)
{
    assert(lst);
    lst->head = lst->tail = 0;
    lst->size = 0;
}

struct List* Create(void)
{
    struct List* lst = (struct List*)malloc(sizeof(struct List));
    Init(lst);
    return lst;
}

void Clear(struct List* lst)
{
    assert(lst);
    struct Link* curr = lst->head;
    while(curr)
    {
        struct Link* next = curr->succ;
        free(curr);
        curr = next;
    }
    lst->head = lst->tail = 0;
    lst->size = 0;
}

void Destroy(struct List* lst)
{
    assert(lst);
    Clear(lst);
    free(lst);
}

void PushBack(struct List* lst, struct Link* p)
{
    assert(lst);
    struct Link* tail = lst->tail;
    if (tail)
    {
        tail->succ = p;
        p->prev = tail;
    }
    else
    {
        lst->head = p;
        p->prev = 0;
    }
    lst->tail = p;
    p->succ = 0;
    lst->size++;
}

void PopBack(struct List* lst)
{
    assert(lst);
    if (lst->tail)
    {
        if (lst->tail->prev)
        {
            struct Link* newTail = lst->tail->prev;
            free(lst->tail);
            lst->tail = newTail;
            lst->tail->succ = 0;
        }
        else
        {
            free(lst->tail);
            lst->head = lst->tail = 0;
        }
    }
    lst->size--;
}

void PushFront(struct List* lst, struct Link* p)
{
    assert(lst);
    struct Link* head = lst->head;
    if (head)
    {
        head->prev = p;
        p->succ = head;
    }
    else
    {
        lst->tail = p;
        p->succ = 0;
    }
    lst->head = p;
    p->prev = 0;
    lst->size++;
}

void PopFront(struct List* lst)
{
    assert(lst);
    if (lst->head)
    {
        if (lst->head->succ)
        {
            struct Link* newHead = lst->head->succ;
            free(lst->head);
            lst->head = newHead;
            lst->head->prev = 0;
        }
        else
        {
            free(lst->head);
            lst->head = lst->tail = 0;
        }
    }
    lst->size--;
}

struct Link* Insert(struct List* lst, struct Link* aft, struct Link* p)
{
    assert(lst);
    if (aft == 0) // список пуст
    {
        lst->head = lst->tail = p;
        p->prev = 0;
        p->succ = 0;
    }
    else
    {
        if (aft->succ)
        {
            p->succ = aft->succ;
            aft->succ->prev = p;
        }
        else {
            lst->tail = p;
            p->succ = 0;
        }
        p->prev = aft;
        aft->succ = p;
    }
    lst->size++;
    return p;
}

struct Link* Erase(struct List* lst, struct Link* p)
{
    assert(lst);
    if (p == 0) {
        return 0;
    }
    
    lst->size--;
    if (p == lst->head)
    {
        if (p->succ)
        {
            lst->head = p->succ;
            free(p);
            lst->head->prev = 0;
            return lst->head;
        }
        else
        {
            free(lst->head);
            lst->head = lst->tail = 0;
            return 0;
        }
    }
    else if (p == lst->tail)
    {
        if (p->prev)
        {
            lst->tail = p->prev;
            free(p);
            lst->tail->succ = 0;
            return 0;
        }
        else
        {
            free(lst->head);
            lst->head = lst->tail = 0;
            return 0;
        }
    }
    else
    {
        struct Link* removable = p;
        struct Link* remSucc = p->succ;
        p->succ->prev = p->prev;
        p->prev->succ = p->succ;
        free(removable);
        return remSucc;
    }
}

struct Link* Advance(struct Link* p, int n)
{
    while (n != 0)
    {
        assert(p);
        if (n < 0)
        {
            p=p->prev;
            n++;
        }
        else
        {
            p=p->succ;
            n--;
        }
    }
    return p;
}
