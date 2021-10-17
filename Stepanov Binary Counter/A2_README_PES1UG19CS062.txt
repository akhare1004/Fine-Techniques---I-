Data Structures Used:
	I used 2 structures. Nodes and header nodes.
	The node structure contains 2 properties. 
		An integer holding the value in that node. 
		A pointer to a node
	For the header node structure, which I have named as just header, I used 2 properties
		An integer, ]count, holding the number of nodes in the linked list that the header node points to. It is 0 if it does not point to anything.
		A pointer to a node.
	
	I statically created an array of header nodes with size equal to the log of the number of elements in the given array+1. I created an extra header node because I wanted header_list[1] to have 1 element in its corresponding linked list, header_list[2] to have 2 and so on for my own convenience.
	
	I used a linked list of nodes in which the first element is the winner of last round, followed by all the elements it had defeated in previous rounds.
	
HOW TO COMPILE:
	I have used math.h, so you will need to use the -lm flag during compilation.

KEY TAKEAWAY:
	I learnt an efficient technique, namely The Stepanov Binary Counter, to find out the second greatest element in a given array of unique integer, with the 		number of comparisions reduced to n + logn -2 when n is an exact power of 2.
	
	After doing this assignment, I also feel a lot more comfortable working with pointers, and writing my own data structures. 
	
	This assignment has made me appreciate time-space tradeoff, i.e, reducing the time of execution by using data structures that make the task more efficient even if it takes extra space. 
