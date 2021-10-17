#include <stdio.h>
#include "A3_header_PES1UG19CS062.h"

void set_state(state_t *ptr_state, int a, int b, int c)
{
    ptr_state->a=a;
    ptr_state->b=b;
    ptr_state->c=c;
    ptr_state->fn_index=0;
}



int are_same(const state_t *ptr_lhs, const state_t *ptr_rhs)
{
	return (ptr_lhs->a==ptr_rhs->a)&&(ptr_lhs->b==ptr_rhs->b)&&(ptr_lhs->c==ptr_rhs->c);			
}

void moveAtoB(const state_t* src, state_t *dst,const int A,const int B,const int C)
{
    int amt;             //amount to transfer from A to B
    if (B-src->b<src->a) // If empty space in B lesser than amount in A, amt = empty space in B
        amt=B-src->b;     
    else
        amt=src->a;		 //else amt = amount in A.
    dst->a=src->a - amt;
    dst->b=src->b + amt;
    dst->c=src->c;
}

void moveAtoC(const state_t *src, state_t *dst,const int A,const int B,const int C)
{
    int amt;             //amount to transfer from A to C
    if (C-src->c<src->a) // If empty space in C lesser than amount in A, amt = empty space in C
        amt=C-src->c;     
    else
        amt=src->a;		 //else amt = amount in A.
    dst->a=src->a - amt;
    dst->b=src->b;
    dst->c=src->c + amt;
}

void moveBtoA(const state_t *src, state_t *dst,const int A,const int B,const int C)
{
    int amt;             //amount to transfer from B to A
    if (A-src->a<src->b) // If empty space in A lesser than amount in B, amt = empty space in A
        amt=A-src->a;     
    else
        amt=src->b;		 //else amt = amount in B.
    dst->a=src->a + amt;
    dst->b=src->b - amt;
    dst->c=src->c;
}

void moveBtoC(const state_t *src, state_t *dst,const int A,const int B,const int C)
{
    int amt;             //amount to transfer from B to C
    if (C-src->c<src->b) // If empty space in C lesser than amount in B, amt = empty space in C
        amt=C-src->c;     
    else
        amt=src->b;		 //else amt = amount in B.
    dst->a=src->a;
    dst->b=src->b - amt;
    dst->c=src->c + amt;
}

void moveCtoA(const state_t *src, state_t *dst,const int A,const int B,const int C)
{
    int amt;             //amount to transfer from C to A
    if (A-src->a<src->c) // If empty space in A lesser than amount in C, amt = empty space in A
        amt=A-src->a;     
    else
        amt=src->c;		 //else amt = amount in C.
    dst->a=src->a + amt;
    dst->b=src->b;
    dst->c=src->c - amt;
}

void moveCtoB(const state_t *src, state_t *dst,const int A,const int B,const int C)
{
    int amt;             //amount to transfer from C to B
    if (B-src->b<src->c) // If empty space in B lesser than amount in C, amt = empty space in B
        amt=B-src->b;     
    else
        amt=src->c;		 //else amt = amount in C.
    dst->a=src->a;
    dst->b=src->b + amt;
    dst->c=src->c - amt;
}

void (*move[])(const state_t*, state_t*,const int, const int , const int) = {
		moveAtoB,
        moveAtoC,
        moveBtoA,
        moveBtoC,
        moveCtoA,
        moveCtoB
};

void init_list(list *ptr_list)
{
	ptr_list->head = NULL;
	ptr_list->tail = NULL;
}

void set_movement(node* temp,int index){
	switch(index){
		case 0:
			temp->from='A';
			temp->to='B';
			break;
		case 1:
			temp->from='A';
			temp->to='C';
			break;
		case 2:
			temp->from='B';
			temp->to='A';
			break;
		case 3:
			temp->from='B';
			temp->to='C';
			break;
		case 4:
			temp->from='C';
			temp->to='A';
			break;
		case 5:
			temp->from='C';
			temp->to='B';
			break;
		default:
			temp->from='z';
			temp->to='z';	
	}
}
void add_at_end(list *ptr_list, const state_t *ptr_state,int index)
{
	node* temp = (node*)malloc(sizeof(node));
	temp->st = *ptr_state;
	set_movement(temp,index);
	
	// empty list
	if(ptr_list->head == NULL)
	{
		temp->next = NULL;
		temp->prev = ptr_list->tail;
		ptr_list->head = temp;
		ptr_list->tail = temp;
	}
	// non-empty list
	else
	{
		temp->next = NULL;
		temp->prev = ptr_list->tail;
		ptr_list->tail->next = temp;
		ptr_list->tail = temp;
	}
}

void remove_at_end(list *ptr_list)
{
	// empty list
	if(ptr_list->head == NULL)
	{
		printf("empty list\n");
	}
	// one node list
	else if(ptr_list->head == ptr_list->tail)
	{
		node* temp = ptr_list->tail;
		ptr_list->tail = temp->prev;
		ptr_list->head = NULL;
		free(temp);
	}
	// multi node list
	else
	{
		node* temp = ptr_list->tail;
		ptr_list->tail = temp->prev;
		ptr_list->tail->next = NULL;
		free(temp);
	}    
}

int is_repeated(const list *ptr_list, const state_t *ptr_state)
{
	node* temp = ptr_list->head;
	while(temp)
	{	
		if (are_same(&temp->st, ptr_state))
			return 1;
		temp = temp->next;
	}
	return 0;
}

void disp_sol(const list *ptr_list)
{
	node* temp = ptr_list->head->next;
	while(temp)
	{	
		printf("%c %c\n",temp->from,temp->to);
		temp = temp->next;
	}
	printf("\n");
}
