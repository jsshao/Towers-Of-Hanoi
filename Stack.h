/* Jason Shao               ICS4U                   November 20th, 2011
Stack Class: It creates a stack using a linked linkest by allocating new memory*/

#ifndef STACK_1
#define STACK_1

struct NODE{
        int num; //the value of the number that is stored into stack
        NODE* next; //address of the next number in the stack
};

class Stack{
           private:
              NODE* head; //pointer to the head of all nodes (top of the stack)
           public:
              Stack();  //constructor to initialize the head
              ~Stack(); //destructor
              
              //modifiers
              void push(int number); //function that pushes a value into the stack (allocates memory for newnode)
              int pop();  //deletes the value at the head and re-assign head
              
              //accessor method
              bool isEmpty(); //determines whether the list of nodes is empty
              int size();     //determines the size of the linked list
              
              //display method
              void DisplayStack(BITMAP* Discs[], BITMAP* Buffer, int PegColumn, int scale, int StackSize, int TotalDiscs); //displays all values in stack
};

#endif
