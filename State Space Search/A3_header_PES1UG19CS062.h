#ifndef STATE_H
#define STATE_H

struct state
{
	int a;
	int b;
	int c;
	int fn_index; 
};

typedef struct state state_t;

struct node
{
	state_t st; //state
	char from; //Moved from which jar
	char to;   //Moved to which jar
	struct node* prev;
	struct node* next;
};
typedef struct node node;


struct list
{
	node* head;
	node* tail;
};
typedef struct list list;

void set_state(state_t *ptr_state, int a, int b, int c);

int are_same(const state_t *ptr_lhs, const state_t *ptr_rhs);

void moveAtoB(const state_t* src, state_t *dst,const int A,const int B,const int C);
void moveAtoC(const state_t* src, state_t *dst,const int A,const int B,const int C);
void moveBtoA(const state_t* src, state_t *dst,const int A,const int B,const int C);
void moveBtoC(const state_t* src, state_t *dst,const int A,const int B,const int C);
void moveCtoA(const state_t* src, state_t *dst,const int A,const int B,const int C);
void moveCtoB(const state_t* src, state_t *dst,const int A,const int B,const int C);

void set_movement(node *temp,int index);
void init_list(list *ptr_list);
void add_at_end(list *ptr_list, const state_t *ptr_state,int index);
void remove_at_end(list *ptr_list);
void disp_sol(const list *ptr_list);
int is_repeated(const list *ptr_list, const state_t *ptr_state);



extern void (*move[])(const state_t*, state_t*, const int, const int , const int);
#endif


