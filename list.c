#include <stdlib.h>
#include <stdio.h>
#include "list.h"


static Node nodeArr[LIST_MAX_NUM_NODES];
static List headArr[LIST_MAX_NUM_HEADS];

//stacks for free nodes and heads
static int freeNodes[LIST_MAX_NUM_NODES];
static int freeHeads[LIST_MAX_NUM_HEADS];
//pointers to the top of the stack
int* nodeSP = freeNodes;
int* headSP = freeHeads;


void push(int** stackPointer, int value){
    (*stackPointer)++;
    *(*stackPointer) = value;
}

int pop(int** stackPointer){
    int val = *(*stackPointer);
    --(*stackPointer);
    return val;
}


//lazy way to fill stacks only on first call
bool first = false;

List* List_create(){
    //on first call, fill the stack with the indices
    if(!first){
        for(int i = LIST_MAX_NUM_NODES-1; i>=0; i--){
            push(&nodeSP, i);
        }
        for(int i = LIST_MAX_NUM_HEADS-1; i >=0; i--){
            push(&headSP, i);
        }
        first = true;
    }
    //if stack is empty, then the stack pointer points to the base (freeNodes/Heads)
    if(headSP == freeHeads){
        printf("heads exhausted\n");
        return NULL;
    }
    int index = pop(&headSP);
    headArr[index].size = 0;
    headArr[index].index = index;
    headArr[index].head = NULL;
    headArr[index].tail = NULL;
    headArr[index].cur = NULL;
    return &headArr[index];
}
int List_count(List* pList){
    return pList->size;
}

void* List_first(List* pList){
    pList->cur = pList->head;
    return pList->head;
}

void* List_last(List* pList){
    pList->cur = pList->tail;
    return pList->tail;
}

void* List_next(List* pList){
    if(!pList->cur){
        if(pList->oob == LIST_OOB_END){
            return pList->cur;
        }
        else{
            pList->cur = pList->head;
            return pList->cur;
        }
    }
    else if(!(pList->cur->next)){
        pList->oob = LIST_OOB_END;
    }
    pList->cur = pList->cur->next;
    return pList->cur;
}

void* List_prev(List* pList){
    if(!pList->cur){
        if(pList->oob == LIST_OOB_END){
            return pList->cur;
        }
        else{
            pList->cur = pList->head;
            return pList->cur;
        }
    }
    if(!(pList->cur->prev)){
        pList->oob = LIST_OOB_START;
    }
    pList->cur = pList->cur->prev;
    return pList->cur;
}

void* List_curr(List* pList){
    return pList->cur;
}

int List_insert_after(List* pList, void* pItem){
    if(nodeSP == freeNodes){
        return -1;
    }
    int index = pop(&nodeSP);
    nodeArr[index].item = pItem;
    nodeArr[index].index = index;
    //if list is empty
    if(pList->size == 0){
        pList->head = &nodeArr[index];
        pList->tail = &nodeArr[index];
        pList->cur = &nodeArr[index];
    }
    //if cur is oob
    else if(!pList->cur){
        if(pList->oob == 0){
            nodeArr[index].next = pList->head;
            pList->head->prev = &nodeArr[index];
            pList->head = &nodeArr[index];
        }
        else if(pList->oob == 1){
            nodeArr[index].prev = pList->tail;
            pList->tail->next = &nodeArr[index];
            pList->tail = &nodeArr[index];
        }
    }
    //if inserting into a normal list
    else{
        //if cur is the last element, cur->next is null, so 
        //no need for a tmp node
        if(!pList->cur->next){
            pList->cur->next = &nodeArr[index];
            nodeArr[index].prev = pList->cur;
            pList->tail = &nodeArr[index];
        }
        else{
            Node* tmp = pList->cur->next;
            pList->cur->next = &nodeArr[index];
            tmp->prev = &nodeArr[index];
            nodeArr[index].prev = pList->cur;
            nodeArr[index].next = tmp;
        }
    }
    pList->size += 1;
    pList->cur = &nodeArr[index];
    return 0;
}

int List_insert_before(List* pList, void* pItem){
     if(nodeSP == freeNodes){
        return -1;
    }
    int index = pop(&nodeSP);
    nodeArr[index].item = pItem;
    nodeArr[index].index = index;
    //if list is empty
    if(pList->size == 0){
        pList->head = &nodeArr[index];
        pList->tail = &nodeArr[index];
        pList->cur = &nodeArr[index];
    }
    //if cur is oob
    else if(!pList->cur){
        if(pList->oob == 0){
            nodeArr[index].next = pList->head;
            pList->head->prev = &nodeArr[index];
            pList->head = &nodeArr[index];
        }
        else if(pList->oob == 1){
            nodeArr[index].prev = pList->tail;
            pList->tail->next = &nodeArr[index];
            pList->tail = &nodeArr[index];
        }
    }
    //if inserting into a normal list
    else{
        if(!pList->cur->prev){
            pList->cur->prev = &nodeArr[index];
            nodeArr[index].next = pList->cur;
            pList->head = &nodeArr[index];
        }
        else{
            Node* tmp = pList->cur->prev;
            pList->cur->prev = &nodeArr[index];
            tmp->next = &nodeArr[index];
            nodeArr[index].next = pList->cur;
            nodeArr[index].prev = tmp;
        }
    }
    pList->size += 1;
    pList->cur = &nodeArr[index];
    return 0;
}

int List_append(List* pList, void* pItem){
    if(nodeSP == freeNodes){
        return -1;
    }
    int index = pop(&nodeSP);
    nodeArr[index].item = pItem;
    nodeArr[index].index = index;
    //if list is empty:
    if(List_count(pList) == 0){
        pList->tail = &nodeArr[index];
        pList->head = &nodeArr[index];
        pList->cur = &nodeArr[index];
    }
    else{
        pList->tail->next = &nodeArr[index];
        nodeArr[index].prev = pList->tail;
        pList->tail = &nodeArr[index];
        pList->cur = &nodeArr[index];
    }
    pList->size += 1;
    return 0;
}

int List_prepend(List* pList, void* pItem){
    if(nodeSP == freeNodes){
        return -1;
    }
    int index = pop(&nodeSP);
    nodeArr[index].item = pItem;
    nodeArr[index].index = index;

    //if list is empty:
    if(List_count(pList) == 0){
        pList->tail = &nodeArr[index];
        pList->head = &nodeArr[index];
        pList->cur = &nodeArr[index];
    }
    else{
        pList->head->prev = &nodeArr[index];
        nodeArr[index].next = pList->head;
        pList->head = &nodeArr[index];
        pList->cur = &nodeArr[index];
    }
    pList->size += 1;
    return 0;
}

void* List_remove(List* pList){
    if(!pList->cur){
        return NULL;
    }
    //tmp node to return
    Node* tmp = pList->cur;
    pList->cur = List_next(pList);
    //adjusting size now to account for if the list becomes empty
    pList->size -= 1;
    //if node is the head
    if(!tmp->prev){
        pList->head = pList->cur;
        pList->cur->prev = NULL;
        tmp->next = NULL;
        //if removing the node causes the list to be empty
        if(List_count(pList) == 0){
            pList->head = NULL;
            pList->tail = NULL;
            pList->cur = NULL;
        }
    }
    //if node is the tail
    else if(!tmp->next){
        tmp->prev->next = NULL;
        pList->tail = tmp->prev;
        tmp->prev = NULL;
    }
    //otherwise normal deletion
    else{
        pList->cur->next = tmp->next;
        tmp->next->prev = tmp->prev;
        tmp->next = NULL;
        tmp->prev = NULL;
    }
    push(&nodeSP, tmp->index);
    return tmp;
}

void* List_trim(List* pList){
    if(List_count(pList) == 0){
        return NULL;
    }
    Node* tmp = pList->tail;
    //adjusting size now to account for if the list becomes empty
    pList->size -= 1;
    if(List_count(pList) == 0){
        pList->head = NULL;
        pList->tail = NULL;
        pList->cur = NULL;
    }
    else{
        tmp->prev->next = NULL;
        pList->tail = tmp->prev;
        tmp->prev = NULL;
    }
    pList->cur = pList->tail;
    push(&nodeSP, tmp->index);
    return tmp;
}

void List_concat(List* pList1, List* pList2){
    //if lists are normal
    if(List_count(pList1) > 0 && List_count(pList2) > 0){
        pList1->tail->next = pList2->head;
        pList2->head->prev = pList1->tail;
        pList1->tail = pList2->tail;
    }
    //if pList1 is empty
    else if(List_count(pList1) == 0){
        pList1->head = pList2->head;
        pList1->tail = pList2->tail;
        pList1->cur = pList2->head;
    }
    //if pList2 is empty then dont do anything to plist1
    pList2->head = NULL;
    pList2->tail = NULL;
    pList2->cur = NULL;
    pList2->oob = -1;

    pList1->size += pList2->size;
    push(&headSP, pList2->index);
}

void List_free(List* pList, FREE_FN pItemFreeFN){
    pList->cur = pList->head;
    while(pList->cur){
        push(&nodeSP, pList->cur->index);
        (*pItemFreeFN)(pList->cur->item);
        pList->cur = List_next(pList);
    }
    push(&headSP, pList->index);
}

void* List_search(List* pList, COMPARATOR_FN pComparator, void* pComparison){
    int match = 0;
    while(pList->cur){
        match = pComparator(pList->cur->item, pComparison);
        if(match){
            return pList->cur;
        }
        pList->cur = List_next(pList);
    }
    return pList->cur;
}

void print_list(List* pList){
    //temp pointer to store where cur was pointing
    Node* tmp = pList->cur;
    int* ip;
    pList->cur = pList->head;
    while(pList->cur){
        ip = pList->cur->item;
        printf("%d ", *ip);
        pList->cur = pList->cur->next;
    }
    pList->cur = tmp;
}