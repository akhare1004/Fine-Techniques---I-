#include <stdio.h>
#include <stdlib.h>
#include "assignment_1.h"

char *p = NULL;
int size;
static int max_size;

struct book_keeper
{
	char marker; //marker = '0' if free, marker = '1' if allocated
	int size;
	char *mem;
	struct book_keeper *next;
};
typedef struct book_keeper book_keeper;

static book_keeper *head = NULL; //head pointer to list of books

void allocate(int n)
{
	//mallocing n bytes
	p = (char *)malloc(n*sizeof(char));
	size=n;
	head = (book_keeper *)p;

	//initialising head pointer
	head->marker = '0';
	head->size = n - sizeof(book_keeper);
	head->mem = (char *)(p + sizeof(book_keeper));
	head->next = NULL;

	max_size = n; //to be used while implementing best_fit fit policy
}

void* mymalloc(int size)
{
	book_keeper *temp = head;
	
	//will point to free book_keeper according to best_fit fit policy
	book_keeper *free_bk = NULL;

	//difference between temp->size and size which is minimum so far
	int best_fit = max_size;

	while(temp != NULL)
	{
		//best_fit fit policy where min(temp->size - size which is >= 0) is given
		//this will result in best_fit fit by definition
		if((temp->marker == '0') && (temp->size >= size) && (temp->size - size < best_fit))
		{
			best_fit = temp->size - size;
			free_bk = temp;
		}
		temp = temp->next;
	}

	//if best_fit fit book_keeper not found
	if(free_bk == NULL)
		return NULL;

	//if mem is sufficient for both data asked and to create another book_keeper keeping info
	if((size + sizeof(book_keeper)) < free_bk->size)
	{
		//steps for allocating:
		book_keeper *new_bk = (book_keeper *)(free_bk->mem + size); //new book_keeper keeping info of free space after allocation
		free_bk->marker = '1'; //since it is now allocated
		new_bk->marker = '0'; //since it is the new reduced free block
		new_bk->size = free_bk->size - ((size + sizeof(book_keeper))); //amount of free space after allocation
		free_bk->size = size; //amount of allocated mem
		new_bk->mem = free_bk->mem + (size + sizeof(book_keeper)); //pointer to the reduced free block

		//inserting book_keeper new_bk after free_bk in the list of books
		new_bk->next = free_bk->next;
		free_bk->next = new_bk;
	}
	//mem sufficient for data asked alone
	else
	{
		//steps for allocating:
		//just giving the entire block (along with extra mem)
		free_bk->marker = '1';
	}

	//returning pointer to allocated block of mem of size "size" or more(in case mem is sufficient for data asked alone)  bytes
	return free_bk->mem;
}

void myfree(void *b)
{
	//if b is NULL
	if(b == NULL)
		return;

	book_keeper *current = head;
	book_keeper *prev = NULL;

	//searching for book_keeper which stores info about b
	while(current != NULL)
	{		
		if((current->marker == '1') && (current->mem == b))
		{
			break;
		}
		prev = current;
		current = current->next;
	}

	//b was never allocated or it was invalid
	if(current == NULL)
		return;

	book_keeper *current_next = current->next;
	
	//checking if a merge is possible b/w prev and current
	if((prev != NULL) && (prev->marker == '0'))
	{
		prev->size += (current->size + sizeof(book_keeper)); //increasing prev's size
		prev->next = current->next; //deleting current from list of books
		current = prev; //having current point to merged ie prev
	}
	else
	{
		current->marker = '0'; //current is freed
	}

	//if current_next is NULL, then there is no other work to be done
	if(current_next == NULL)
		return;

	//but if current_next is present, then it should be checked for merging with current
	if(current_next->marker == '0')
	{
		current->size += (current_next->size + sizeof(book_keeper)); //increasing current's size
		current->next = current_next->next; //deleting current_next from list of books
	}
	return;
}

void print_book() //In the documentation given to us, this function is named print_book_size. But I have followed the header file's declaration here
{
	printf("%ld\n", sizeof(book_keeper)); //prints the size of book_keeper keeping structure
} 

void display_mem_map()
{
	book_keeper *temp = head;
	
	while(temp != NULL)
	{
		printf("%ld\t%d\t0\n", (char *)temp-p, sizeof(book_keeper)); //info about book_keeper
		if(temp->marker == '1')
		{
			printf("%ld\t%d\t1\n", temp->mem-p, temp->size); //if allocated
		}
		else
		{
			printf("%ld\t%d\t2\n", temp->mem-p, temp->size); //if free
		}
		temp = temp->next;
	}
}