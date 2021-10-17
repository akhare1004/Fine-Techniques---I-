#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "assignment_2.h"


typedef struct node{
    int data;
    struct node* next;
}node;

typedef struct header{
    int count; //to store the length of the list the header points to.
    node* link;
}header;

static void free_tail(node* t)
{
	node *curr,*prev;
	curr=t->next;
	while(curr!=NULL){
		prev=curr;
		curr=curr->next;
		free(prev);
	}
}

static void print_opponents(node *a,node *b)
{
	node *q,*p;
	q=a;
	p=b;
	
	while(q!=NULL){
		printf("%d ",q->data);
		q=q->next;
	}
	printf("X ");
	while(p!=NULL){
		printf("%d ",p->data);
		p=p->next;
	}
	printf("= ");
}

void find_second_greatest(int *numbers, int length)
{
    //Creating a list of header nodes. The 0th index node will not be used.
    int header_count= ceil(log2(length))+1;
    header header_list[header_count+1]; //I want my index to coincide with the number of elements that header node's list can have, hence taking one extra node. header_list[0] 					//will be unused.
    
    for(int z=0;z<=header_count;z++){
        header_list[z].count=0;
        header_list[z].link=NULL;
    }

    int index; // to keep track of the position of current header node from the list of header nodes

    for(int i=0;i<length;i++){
        
        index=1;

        //allocate memory for node
        node *temp=(node*)malloc(sizeof(node));
        temp->data=numbers[i];
        temp->next=NULL;

        while(1){
            if(header_list[index].count==0){
            	header_list[index].link=temp;
            	header_list[index].count=index;
            	break;
            }
            
            else if(header_list[index].count==index){ //already fully occupied. Hence there will be match
            	
            	print_opponents(header_list[index].link,temp);
            	
            	if(header_list[index].link->data < temp->data){
            		free_tail(header_list[index].link); 
            		header_list[index].link->next=temp->next;
            		temp->next=header_list[index].link;
            		header_list[index].link=NULL;
            		header_list[index].count=0;
            		index++;
            	}
            	else{ //header_list[index].link->data > temp->data
            		free_tail(temp);
            		temp->next=header_list[index].link->next;
            		header_list[index].link->next=temp;
            		temp=header_list[index].link;
            		header_list[index].link=NULL;
            		header_list[index].count=0;
            		index++;	
            	}
            	
            	//To print the resultant set
            	node* q;
            	q=temp;
            	while(q!=NULL){
            		printf("%d ",q->data);
            		q=q->next;
            	}
            	printf("\n");
            }
       }
    }
    
	if(header_list[header_count].count==0){  //If the last header node is empty, it means the number of elements in array were not a power of 2
		
		for(int i=1;i<=header_count-1;i++){
			if(header_list[i].count!=0){
				if(header_list[i+1].count==0){ //if next column is empty, move list to it
					header_list[i+1].link=header_list[i].link;
					header_list[i].link=NULL;
					header_list[i].count=0;
					header_list[i+1].count=i+1;
				}
				else if(header_list[i+1].link->data > header_list[i].link->data){ //if next column is not empty, hold a match
					
					print_opponents(header_list[i+1].link,header_list[i].link);
					
					free_tail(header_list[i].link);
					
					header_list[i].link->next=header_list[i+1].link->next;
					header_list[i+1].link->next=header_list[i].link;
					header_list[i].link=NULL;
					header_list[i].count=0;
					
					node *q;
					q=header_list[i+1].link;
				    	while(q!=NULL){
				    		printf("%d ",q->data);
				    		q=q->next;
				    	}
            				printf("\n");
					
				}
				else{
					print_opponents(header_list[i+1].link,header_list[i].link);
					
					free_tail(header_list[i+1].link);
					
					header_list[i+1].link->next=header_list[i].link->next;
					header_list[i].link->next=header_list[i+1].link;
					header_list[i+1].link=header_list[i].link;
					header_list[i].link=NULL;
					header_list[i].count=0;
					
					node *q;
					q=header_list[i+1].link;
				    	while(q!=NULL){
				    		printf("%d ",q->data);
				    		q=q->next;
				    	}
				    	printf("\n");
				}
			}
		}
	}   
	
	//Find the second greatest element using the list at the last header node
	
	int second_highest;
	node* m;
	m=header_list[header_count].link->next;
	second_highest=m->data;
	while(m!=NULL){
		if(m->data > second_highest)
			second_highest=m->data;
		m=m->next;
	}
	
	printf("\n");
	printf("%d\n",second_highest);
	
}
