#include <stdio.h>
#include <stdlib.h>
#include "assignment_4.h"

static void inorder(const node_t *root, FILE *fp)
{
	if(root->left_offset != -1){
		node_t current_node;
		fseek(fp, sizeof(tree_t) + (root->left_offset)*sizeof(node_t), SEEK_SET);
		fread(&current_node, sizeof(node_t), 1, fp);
		inorder(&current_node, fp);
	}
	
	printf("%d ", root->key);
	
	if(root->right_offset != -1){
		node_t current_node;
		fseek(fp, sizeof(tree_t) + (root->right_offset)*sizeof(node_t), SEEK_SET);
		fread(&current_node, sizeof(node_t), 1, fp);
		inorder(&current_node, fp);
	}
}

static void preorder(const node_t *root, FILE *fp)
{
	printf("%d ", root->key);
	
	if(root->left_offset != -1){
		node_t current_node;
		fseek(fp, sizeof(tree_t) + (root->left_offset)*sizeof(node_t), SEEK_SET);
		fread(&current_node, sizeof(node_t), 1, fp);
		preorder(&current_node, fp);
	}
	
	if(root->right_offset != -1){
		node_t current_node;
		fseek(fp, sizeof(tree_t) + (root->right_offset)*sizeof(node_t), SEEK_SET);
		fread(&current_node, sizeof(node_t), 1, fp);
		preorder(&current_node, fp);
	}
}

FILE* init_tree(const char* filename)
{
	FILE* fp;
	fp = fopen(filename, "r+");
	if(fp == NULL){   //If file not present, create it and open in w+ mode.
		fp = fopen(filename, "w+"); 
		if(fp == NULL){
			exit(1);
		}
		
		//Initialise the tree header
		tree_t t; 
		t.root = -1;
		t.free_head = -1;
	
		fseek(fp, 0, SEEK_SET);
		fwrite(&t, sizeof(tree_t), 1, fp);
	}
	return fp;
}

void close_tree(FILE *fp)
{
	fclose(fp);
}

void display_inorder(FILE *fp)
{
	tree_t temp;
	fseek(fp, 0, SEEK_SET);
	fread(&temp, sizeof(tree_t), 1, fp);
	
	if(temp.root!=-1){
		node_t root_node;
		fseek(fp, sizeof(tree_t) + temp.root*sizeof(node_t), SEEK_SET);
		fread(&root_node, sizeof(node_t), 1, fp);
		
		inorder(&root_node, fp); //Calling the inorder traversal function
	}
	
	printf("\n");
}

void display_preorder(FILE *fp)
{
	tree_t temp;
	fseek(fp, 0, SEEK_SET);
	fread(&temp, sizeof(tree_t), 1, fp);
	
	if(temp.root != -1){
		node_t root_node;
		fseek(fp, sizeof(tree_t) + temp.root*sizeof(node_t), SEEK_SET);
		fread(&root_node, sizeof(node_t), 1, fp);
		
		preorder(&root_node, fp); //calling the preorder traversal function
	}
	
	printf("\n");
}

//returns position of parent node that the new node will be attached to
static int find_parent_pos(int curr_index, int key, FILE *fp)
{
	node_t current;
	int prev=-1;
	while(curr_index != -1){
		prev = curr_index;
		fseek(fp, sizeof(tree_t) + curr_index*sizeof(node_t), SEEK_SET);
		fread(&current, sizeof(node_t), 1, fp);
		
		if(key > current.key)
			curr_index = current.right_offset;
		else if(key < current.key)
			curr_index = current.left_offset;
		else
			return -1; //When key is already present
	}
	return prev;
}

void insert_key(int key, FILE *fp)
{
	tree_t t;
	fseek(fp, 0, SEEK_SET);
	fread(&t, sizeof(tree_t), 1, fp);
	
	if(t.root != -1){ //Full nodes are present
		
		int parent_pos = find_parent_pos(t.root, key, fp); //find offset for parent node
		
		if(parent_pos == -1) //Function find_parent_pos returned 1 if key was already present, in which case we do insert
			return;

		if(t.free_head == -1){ //No free nodes present
			
			fseek(fp, 0, SEEK_END); //Since there are no free nodes in between, we need to insert at the end of file
			int pos;
			pos=(ftell(fp) - sizeof(tree_t))/sizeof(node_t); //position to insert at 

			//set the parent's offset
			node_t parent;
			fseek(fp, sizeof(tree_t) + parent_pos*sizeof(node_t), SEEK_SET);
			fread(&parent, sizeof(node_t), 1, fp);
			
			if(key > parent.key)
				parent.right_offset = pos;
			else
				parent.left_offset = pos;
				
			fseek(fp, sizeof(tree_t) + parent_pos*sizeof(node_t), SEEK_SET);
			fwrite(&parent, sizeof(node_t), 1, fp);

			//Insert child node
			node_t child_node;
			child_node.key = key;
			child_node.left_offset = -1;
			child_node.right_offset = -1;
			fseek(fp, sizeof(tree_t) + pos*sizeof(node_t), SEEK_SET);
			fwrite(&child_node, sizeof(node_t), 1, fp);
		}

		else{ //Free nodes present
			node_t child_node;
			int pos = t.free_head; //offset for the first free node
			fseek(fp, sizeof(tree_t) + pos*sizeof(node_t), SEEK_SET);
			fread(&child_node, sizeof(node_t), 1, fp);
			
			t.free_head = child_node.right_offset; //making free_head point to the next free node
			fseek(fp, 0, SEEK_SET);
			fwrite(&t, sizeof(tree_t), 1, fp);
			
			//set parent's offset
			node_t parent;
			fseek(fp, sizeof(tree_t) + parent_pos*sizeof(node_t), SEEK_SET);
			fread(&parent, sizeof(node_t), 1, fp);
			
			if(key > parent.key)
				parent.right_offset = pos;
			else
				parent.left_offset = pos;
			
			fseek(fp, sizeof(tree_t) + parent_pos*sizeof(node_t), SEEK_SET);
			fwrite(&parent, sizeof(node_t), 1, fp);
	
			child_node.key = key;
			child_node.left_offset = -1;
			child_node.right_offset = -1;
			fseek(fp, sizeof(tree_t) + pos*sizeof(node_t), SEEK_SET);
			fwrite(&child_node, sizeof(node_t), 1, fp);
		}
	}

	else{ //No full nodes are present
		if(t.free_head != -1){ //Free nodes are present
			node_t root;
			int pos = t.free_head; //offset for new root
			fseek(fp, sizeof(tree_t) + pos*sizeof(node_t), SEEK_SET);
			fread(&root, sizeof(node_t), 1, fp);

			t.free_head = root.right_offset; //making free_head point to the next free node
			t.root = pos;
			
			fseek(fp, 0, SEEK_SET);
			fwrite(&t, sizeof(tree_t), 1, fp);

			root.key = key;
			root.left_offset = -1;
			root.right_offset = -1;
			fseek(fp, sizeof(tree_t) + pos*sizeof(node_t), SEEK_SET);
			fwrite(&root, sizeof(node_t), 1, fp);
		}
		else{  //No free nodes
			node_t root;
			t.root = 0;
			
			fseek(fp, 0, SEEK_SET);
			fwrite(&t, sizeof(tree_t), 1, fp);
			
			root.key = key;
			root.left_offset = root.right_offset = -1;
			fseek(fp, sizeof(tree_t) , SEEK_SET);
			fwrite(&root, sizeof(node_t), 1, fp);
		}
	}
}

void delete_key(int key, FILE *fp)
{
	tree_t t;
	
	fseek(fp, 0, SEEK_SET);
	fread(&t, sizeof(tree_t), 1, fp);
	
	int curr_index = t.root;
	
	int parent_pos = -1; //offset of parent of node to be deleted
	int delete_at_pos = -1; //offset of node to be deleted

	node_t current; //node to delete
	
	if(t.root == -1)  //empty tree
		return;

	//find node to be deleted
	while(curr_index != -1){
		fseek(fp, sizeof(tree_t) + curr_index*sizeof(node_t), SEEK_SET);
		fread(&current, sizeof(node_t), 1, fp);
		
		if(key > current.key){
			parent_pos = curr_index;
			curr_index = current.right_offset;
		}
		else if(key < current.key){
			parent_pos = curr_index;
			curr_index = current.left_offset;
		}
		else{  //key == current.key
			delete_at_pos = curr_index;
			break;
		}
	}
	
	if(delete_at_pos == -1) //key not found
		return;

	if(current.left_offset!=-1 && current.right_offset!=-1){ //node has two subtrees
		int par_inorder_succ = -1; //offset of parent of inorder successor
		int in_suc_pos = current.right_offset; //offset of inorder successor
		
		node_t inorder_succ; //inorder successor node
		fseek(fp, sizeof(tree_t) + in_suc_pos*sizeof(node_t), SEEK_SET);
		fread(&inorder_succ, sizeof(node_t), 1, fp);

		//finding parent of inorder successor and inorder successor position
		while(inorder_succ.left_offset != -1){
			par_inorder_succ = in_suc_pos;
			in_suc_pos = inorder_succ.left_offset;
			fseek(fp, sizeof(tree_t) + in_suc_pos*sizeof(node_t), SEEK_SET);
			fread(&inorder_succ, sizeof(node_t), 1, fp);	
		}

		if(par_inorder_succ != -1){
			node_t node_p; //parent of inorder successor
			fseek(fp, sizeof(tree_t) + par_inorder_succ*sizeof(node_t), SEEK_SET);
			fread(&node_p, sizeof(node_t), 1, fp);

			node_p.left_offset = inorder_succ.right_offset;
			fseek(fp, sizeof(tree_t) + par_inorder_succ*sizeof(node_t), SEEK_SET);
			fwrite(&node_p, sizeof(node_t), 1, fp);
		}

		else{
			current.right_offset = inorder_succ.right_offset;
		}

		current.key = inorder_succ.key;
		fseek(fp, sizeof(tree_t) + delete_at_pos*sizeof(node_t), SEEK_SET);
		fwrite(&current, sizeof(node_t), 1, fp);

		//adding inorder successor to the free list
		inorder_succ.right_offset = t.free_head;
		fseek(fp, sizeof(tree_t) + in_suc_pos*sizeof(node_t), SEEK_SET);
		fwrite(&inorder_succ, sizeof(node_t), 1, fp);
		t.free_head = in_suc_pos;
		fseek(fp, 0, SEEK_SET);
		fwrite(&t, sizeof(tree_t), 1, fp);
	}
	else{   //Either one or no subtrees
		int k; 
		if(current.left_offset == -1){
			k = current.right_offset;
		}
		else{
			k = current.left_offset;
		}

		if(parent_pos == -1){  //node to be deleted is the root
			current.right_offset = t.free_head;
			t.free_head = t.root;
			fseek(fp, sizeof(tree_t) + delete_at_pos*sizeof(node_t), SEEK_SET);
			fwrite(&current, sizeof(node_t), 1, fp);

			t.root = k;
			
			fseek(fp, 0, SEEK_SET);
			fwrite(&t, sizeof(tree_t), 1, fp);
			return;
		}

		node_t parent_node; //parent of node to be deleted
		fseek(fp, sizeof(tree_t) + parent_pos*sizeof(node_t), SEEK_SET);
		fread(&parent_node, sizeof(node_t), 1, fp);

		//updating parent's offset 
		if(parent_node.left_offset == delete_at_pos){
			parent_node.left_offset = k;
			fseek(fp, sizeof(tree_t) + parent_pos*sizeof(node_t), SEEK_SET);
			fwrite(&parent_node, sizeof(node_t), 1, fp);
		}
		else{
			parent_node.right_offset = k;
			fseek(fp, sizeof(tree_t) + parent_pos*sizeof(node_t), SEEK_SET);
			fwrite(&parent_node, sizeof(node_t), 1, fp);
		}

		current.right_offset = t.free_head;
		t.free_head = delete_at_pos;
		fseek(fp, sizeof(tree_t) + delete_at_pos*sizeof(node_t), SEEK_SET);
		fwrite(&current, sizeof(node_t), 1, fp);
		
		fseek(fp, 0, SEEK_SET);
		fwrite(&t, sizeof(tree_t), 1, fp);
	}
}
