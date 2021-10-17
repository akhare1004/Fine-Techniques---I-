#ifndef assignment1_h
#define assignment1_h

extern char *p;
extern int size;

void allocate(int n); //function to allocate memory for the global array p, n number of bytes long.

void* mymalloc(int size); //function to allocate a block of size "size" from p

void myfree(void *b); //free the block pointed to by the parameter

void print_book(); // prints bytes used by the bookkeeping structure

void display_mem_map(); // print the memory array p in the following format
// start_addr   block_in_bytes  encoded_status

// sample output for the following sanity_client:

// allocate(100);
// print_book();
//
// char *b = (char *)mymalloc(30 * sizeof(char));
// display_mem_map();
// printf("\n");
//
// char *c = (char *)mymalloc(30 * sizeof(char));
// display_mem_map();
// printf("\n");
	
// 16
// 0	16	0
// 16	30	1
// 46	16	0
// 62	38	2
//
// 0	16	0
// 16	30	1
// 46	16	0
// 62	38	1

// refer to the sample output screenshots in the google doc too.

#endif