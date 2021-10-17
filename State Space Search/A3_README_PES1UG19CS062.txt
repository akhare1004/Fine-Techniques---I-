STATE SPACE SEARCH: WATER JUG PROBLEM
HOW TO COMPILE:
gcc A3_impl_PES1UG19CS062.c A3_client_PES1UG19CS062.c
Any other standard compilation method should work too.

DATA STRUCTURES USED:
Array of pointers to functions, to implement function callback.
Doubly Linked List behaving as Stack.
Each node in the list contains 5 things. 
Pointers to next and previous nodes. 
State structure.
 Character variables ‘from’ and ‘to’ that are used to keep track for movements made.
The state structure has 4 fields.
       Integers a, b, c to hold the amount in the respective jars. 
       Integer fn_index for index of corresponding pointer in array of pointers to functions.
       
KEY TAKEAWAYS FROM ASSIGNMENT:
Proper usage and advantages of function callbacks.
Concept of backtracking in State Space Search

