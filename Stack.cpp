/*Jason Shao                ICS4U               December 12th, 2011
Stack Class Implementations: It includes a display method that displays the values using bitmaps 
*/
#include <iostream>
#include <allegro.h>

using namespace std;

Stack::Stack(){
     head = 0; //initializes the head pointer to create an empty linked list
}

Stack::~Stack(){
     //Deallocates all of the nodes created from memory
     NODE* node = head, *temp;
     
     while (node){ //traverse through list
          temp = node;
          node = node->next;
          delete temp; //deallocation
     }
}

void Stack::push(int number){
     //puts the number at the head of the list
     NODE *newNode;
     newNode = new NODE;    //allocates new memory
     newNode->num = number; //assign value
     newNode->next = head;  //store address of next node in the linked list
     head = newNode;        //re-assign the head
}

int Stack::pop(){
    if (head){
       NODE* node = head; 
       int temp = head->num; //remembers value at the top of the linked list
       head = head->next;      //re-assign head
       delete node;            //deallocates memory of the node at the old head
       return temp;
    }
}

bool Stack::isEmpty(){
    return (!head); //determines whether the list is empty
}

int Stack::size(){
    int counter = 0;   //counter used to record how many nodes there are
    NODE* node = head; //pointer used to traverse the list
    
    while(node!=NULL){
        counter++;           //Increment counter while the node is not NULL
        node = node->next;   //Increment the traversing pointer
    }
    return counter; //Returns how many nodes there are (size of the list)
}

void Stack::DisplayStack(BITMAP* Discs[], BITMAP* Buffer, int PegColumn, int scale, int StackSize, int TotalDiscs){
    //Initialize variables
    NODE *node = head;          //pointer used to traverse through the linked list     
    int counter =0;              //keeps track of how many discs there are
    int DiscSize;               //Size of the disc on the peg
    int DiscX;                  //X-coordinate Position of the Disc
    int DiscY;                  //Y-coordinate Position of the Disc
    int DiscWidth;              //The width of the disc
    int DiscHeight = scale;     //The height of the disc
    
    while(node!=NULL){                
        DiscSize = node->num;   //DiscSize value is stored
        
        DiscX = 195*PegColumn+ 55 + (TotalDiscs-DiscSize)*(10+10-TotalDiscs)/2; //X-coordinate based on the disc's width
        DiscY = 369-scale*(StackSize-counter);                  //Y-coordinate based on position in stack
        DiscWidth = 130-(TotalDiscs-DiscSize)*(10+10-TotalDiscs);               //Width calculated by stretching the BITMAP
        
        //Outputs the image on the buffer
        masked_stretch_blit(Discs[DiscSize%2], Buffer, 0, 0, Discs[DiscSize%2]->w, 30, DiscX, DiscY, DiscWidth, DiscHeight);
        
        //increment counter and the traversing pointer
        node = node->next;
        counter++;
    }
    return;
}
