#include <stdio.h>
#include "A3_header_PES1UG19CS062.h"

#define DEBUG 0
int main()
{
	
	state_t start;
	state_t goal;
    
	int A,B,C; // Capacities of jug A,B and C
	int start_A,start_B,start_C; // Starting values
	int goal_A,goal_B,goal_C; // Target values
	
	
    scanf("%d %d %d",&A,&B,&C);
	scanf("%d %d %d",&start_A,&start_B,&start_C);
	scanf("%d %d %d",&goal_A,&goal_B,&goal_C);
	printf("\n");

	set_state(&start, start_A,start_B,start_C);
	set_state(&goal, goal_A,goal_B,goal_C);
	
	list mylist;
	init_list(&mylist);
	add_at_end(&mylist, &start,-1);
	
	int soln = 0; //1 when solution found.
	
	state_t *temp;
	temp = (state_t*)malloc(sizeof(state_t));
	int num_sol=0; // No. of solutions
	
	while(mylist.tail!=NULL)
	{
		move[mylist.tail->st.fn_index](&mylist.tail->st,temp,A,B,C);
		if(!is_repeated(&mylist, temp))
		{
			add_at_end(&mylist,temp,mylist.tail->st.fn_index);
			soln = are_same(temp, &goal); //if temp is same as goal, returns 1,i.e, soln =1 (soultuion found)
			
			if(soln)
			{
				disp_sol(&mylist);
    			num_sol++;
				remove_at_end(&mylist);
				
				while((mylist.tail!=NULL) && (++mylist.tail->st.fn_index == 6)) //backtracking to find more solutions
    			{
    				remove_at_end(&mylist);
    			}
			}
		}
		else //backtracking 
		{
			while((mylist.tail!=NULL) && (++mylist.tail->st.fn_index == 6))
    		{
    			remove_at_end(&mylist);
			}
		}
	}
	printf("%d", num_sol);		// total number of solutions
	
}

