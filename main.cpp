/* Jason Shao                       ICS4U                              December 12, 2011
This program uses Allegro.cc package graphics to demonstrate the solution of a Towers of Hanoi
Puzzle. The goal of the game is to move all of the discs of different sizes from one disc to another.
However, a bigger disc cannot fall on top of one smaller in size. The discs on each peg are stored 
in 3 linked lists. The user will be able to choose how many discs he/she wants to solve (1-10). The user
may also determine the game speed. A friendly user interface will allow the user to have the option of 
re-running the program.
*/

#include <iostream>
#include <allegro.h>
#include "Stack.h"
#include "Stack.cpp"

using namespace std;

//Define Colours
#define GREY makecol(205, 205, 205)
#define PINK makecol(255, 0, 250)
#define BLUE makecol(30, 5, 255)
#define RED makecol(255, 50, 50)
#define WHITE makecol(255, 255, 255)

//Create enumerated type for game speed (milliseconds)
enum speed {SLOW = 1000, MEDIUM = 600, FAST = 200};

//Function Prototypes
void move (BITMAP* Discs[], BITMAP* Peg, Stack Pegs[], int num, int sorc, int dest, int Scale, int TotDsc, speed MovSpd, int &MovCoun, int &RecCoun);
void displayPegs (BITMAP* Discs[], BITMAP* Peg, Stack Pegs[], int Scale, int TotDsc, speed MovSpd, int &MovCoun, int &RecCoun);
void Finished (bool &PlayAgain);
    
int main(int argc, char *argv[]){
    allegro_init();     // Initialize Allegro 
	install_keyboard(); // Initialize keyboard routines  
	install_mouse();    // Initialize mouse functions
	
	set_color_depth(desktop_color_depth());             // Set the color depth 
	set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640,480,0,0); // Change our graphics mode to 640x480
	show_mouse(screen);                                 // Display cursor
    
    //Initialize variables, classes, bitmaps
    Stack Pegs[3];                 //Stores the different discs on each peg
    int TotDsc = 5;                //Total Discs (default to 5)
    int Scale;                     //Scaled height of each disc depending on total number of discs
    int i = 0;                     //index variable
    int MovCoun = 0;               //Counter that records number of disc moves
    int RecCoun = 0;               //Counter that records number of times the recursive function is called
    bool SelectComplete = 0;       //Boolean to determine whether the user is finished with the selection process
    bool PlayAgain = 1;            //Boolean to determine whether the user wishes to run the solution again
    speed MovSpd = MEDIUM;         //An enumerated speed type to record the gamespeed
    
    BITMAP* Title;                 //Title Image
    BITMAP* Peg;                   //Picture of each Peg
    BITMAP* DiscInstruc;           //Instruction Pic to choose # of discs
    BITMAP* Discs[2];              //2 discs (yellow and green)
    BITMAP* Speed;                 //Instruction Pic to choose game speed
    
    //Load bitmaps
    Title = load_bitmap("IntroTitle.bmp", NULL);
    Peg = load_bitmap("Peg.bmp", NULL);
    DiscInstruc = load_bitmap("NumOfDiscs.bmp", NULL);
    Discs[0] = load_bitmap("Disc1.bmp", NULL);
    Discs[1] = load_bitmap("Disc2.bmp", NULL);
    Speed = load_bitmap("Speed.bmp", NULL);
    
    //Loop of the entire application until user decides to exit
    while (PlayAgain){
 
        //re-initialization of certain variables after user decides to run the program again  
        MovCoun = 0;            //Reset Moves Counter
        RecCoun = 0;            //Reset Recursive Call Counter
        SelectComplete = 0;    //Reset User's Decision to Start the Solution
        
        //re-initialize the 3 peg stacks
        for (i = 0; i < 3; i++)
            while (!Pegs[i].isEmpty())
                  Pegs[i].pop();
         
        //Loop to allow user to select the number of discs he/she wants to solve
        while(!SelectComplete)
        {
             // Create Buffer Bitmap
             BITMAP* buffer = create_bitmap(screen->w, screen->h);
             clear_bitmap(buffer);
             
             //Display Title
             draw_sprite(buffer, Title, 50, 0);
             
             //Display the 3 pegs 
             for (i = 0 ; i < 3 ; i++)
                 draw_sprite(buffer, Peg, 195*i + 50, 175); 
                 
            //-------------------------------CONTROL NUMBER OF DISCS --------------------------------------------------------    
            //Allow User to select number of discs
            draw_sprite(buffer, DiscInstruc, 185, 60);  //instruction title
            
            //Draw 10 blank grey buttons for num of discs
            for (i = 0; i < 10; i++)
                rectfill(buffer, i*62+15, 100, i*62+60, 135, GREY);        
            
            //Show activity when the mouse is on a button
            if (mouse_y >= 100 && mouse_y <= 135){         //If y coordinate is on a button                     
                int MousePosOnButton = (mouse_x - 15)%62;  //Stores x coordinate relative to the button
                
                //If x coordinate is also on a button
                if (MousePosOnButton >= 0 && MousePosOnButton <= 45){
                    //Colour the button pink to show active button
                    rectfill(buffer, mouse_x - MousePosOnButton, 100, mouse_x - MousePosOnButton + 45, 135, PINK);
                    
                    //If mouse is clicked
                    if (mouse_b & 1)
                        TotDsc = (mouse_x - MousePosOnButton)/62 + 1; //Store number of discs to initialize
                }
            }
            
            //Show activity on button that is already previously selected
            rectfill(buffer, (TotDsc-1)*62 + 15, 100, (TotDsc-1)*62 + 60, 135, PINK);
            
            //Display each button with a selection of number of discs (1 - 10)
            for (i = 0; i < 10; i++)
                textprintf_ex(buffer, font, i*62+35, 114, BLUE, -1, "%d", i+1);             
           
           //----------------------------CONTROL MOVING SPEED --------------------------------------------
           //Allow User to select game speed
           draw_sprite(buffer, Speed, 90, 400); //instruction title
           
           //draw three blank grey buttons for SPEED
           for (i = 0; i < 3; i++)
               rectfill(buffer, i*100+35, 430, i*100+95, 460, GREY);
               
           //Show activity when mouse is on a button
           if (mouse_y >= 430 && mouse_y <= 460){       //mouse y coordinate is on a button
               
               //If the x coordinate is also on a button
               if (mouse_x >= 35 && mouse_x <= 95){
                   rectfill(buffer, 35, 430, 95, 460, PINK);    //Change the first button to pink
                   
                   //If mouse is clicked, change speed to slow
                   if (mouse_b & 1){   
                       MovSpd = FAST; 
                   }
               }
               else if (mouse_x >= 135 && mouse_x <= 195){
                   rectfill(buffer, 135, 430, 195, 460, PINK);  //Change the second button to pink 
                   
                   //If mouse is clicked, change speed to medium
                   if (mouse_b & 1)
                       MovSpd = MEDIUM;
               }
               else if (mouse_x >= 235 && mouse_x <= 295){
                   rectfill(buffer, 235, 430, 295, 460, PINK);  //Change the third button to pink
                   
                   //If mouse is clicked, change speed to fast
                   if (mouse_b & 1)
                       MovSpd = SLOW;
               }
           }
           
           //Show activity on button that has previously been selected
           rectfill(buffer, (MovSpd-200)/4 + 35, 430, (MovSpd-200)/4 + 95, 460, PINK);
           
           //Display each button with a selection of game speed on it
           textprintf_ex(buffer, font, 48, 443, BLUE, -1, "FAST");
           textprintf_ex(buffer, font, 142, 443, BLUE, -1, "MEDIUM");
           textprintf_ex(buffer, font, 249, 443, BLUE, -1, "SLOW");
           
           //-------------------------START HANOI----------------------------------------
           //Display Button: CLICK TO START
           rectfill(buffer, 400, 430, 580, 470, RED);
           textprintf_ex(buffer, font, 432, 445, BLUE, -1, "CLICK TO START"); 
           
           //If mouse clicks the start button
           if (mouse_x >= 400 && mouse_x <= 580 && mouse_y >= 430 && mouse_y <= 470){
                if (mouse_b & 1)
                      SelectComplete = 1; //Selection is complete and breaks out of loop to solve hanoi
           }
            
            //Display buffer to screen
            masked_blit(buffer, screen, 0, 0, 0, 0, screen->w, screen->h);
            
            destroy_bitmap(buffer); //Destroy bitmap as it goes out of scope
        }   
        
        //Initialize disks
        for (i = TotDsc-1; i >= 0; i--)
            Pegs[0].push(i);
        
        //Set height scale of each disc
        Scale = 170/TotDsc; 
        
        //Display starting position of discs
        for (i = 0; i < 3; i++)
             Pegs[i].DisplayStack(Discs, screen, i, Scale, Pegs[i].size(), TotDsc);
        
        allegro_message("YOU ARE ABOUT TO START THE TOWERS OF HANOI SOLUTION! BRACE YOURSELF AND PRESS OK!");
        
        //Solve towers of hanoi
        move(Discs, Peg, Pegs, TotDsc,0, 2, Scale, TotDsc, MovSpd, MovCoun, RecCoun); 
        
        rest(1000);
        Finished(PlayAgain); //determines whether the user wants to exit
    }   
    
    //Destroy bitmaps
    destroy_bitmap(Peg);
    destroy_bitmap(DiscInstruc);
    destroy_bitmap(Discs[0]);
    destroy_bitmap(Discs[1]);
    destroy_bitmap(Speed);
    destroy_bitmap(Title);
    
    return 0;
}
END_OF_MAIN();

void move (BITMAP* Discs[], BITMAP* Peg, Stack Pegs[], int num, int sorc, int dest, int Scale, int TotDsc, speed MovSpd, int &MovCoun, int &RecCoun){
    int i = 0;                         //index variable
    int intermediate = 3-(sorc+dest);  //calculate intermediate peg knowing source and destination pegs
    
    RecCoun++; //Increment counter that keeps track of number of recursive calls

    //End case when disc required to move is 1
    if (num == 1){
              Pegs[dest].push(Pegs[sorc].pop()); //pop the top disc and pushes it on the intermediate peg
              MovCoun++;    //Increment counter of number of moves
              
              //Display screen animation output (Discs, Pegs, Counters)
              displayPegs(Discs, Peg, Pegs, Scale, TotDsc, MovSpd, MovCoun, RecCoun);
              return;
    }
    
    //Move (n-1) discs from source to intermediate peg
    move(Discs, Peg, Pegs, num-1, sorc, intermediate, Scale, TotDsc, MovSpd, MovCoun, RecCoun);
    
    //Move bottom disc to final destination --------------------------------------------------------------------------
    Pegs[dest].push(Pegs[sorc].pop());    //pop the top disc and pushes it on the intermediate peg
    MovCoun++; //Increment counter of number of moves
    displayPegs(Discs, Peg, Pegs, Scale, TotDsc, MovSpd, MovCoun, RecCoun); //Display screen animation output
    //---------------------------------------------------------------------------------------------------------------    
    
    //Move (n-1) discs from intermediate to final destination
    move (Discs, Peg, Pegs, num-1, intermediate, dest, Scale, TotDsc, MovSpd, MovCoun, RecCoun); 
}
END_OF_FUNCTION(move);

void displayPegs (BITMAP* Discs[], BITMAP* Peg, Stack Pegs[], int Scale, int TotDsc, speed MovSpd, int &MovCoun, int &RecCoun){
      // Create Buffer Bitmap
      BITMAP* buffer = create_bitmap(screen->w, screen->h);
      
      //Output counter of number of moves
      textprintf_ex(buffer, font, 10,10 , WHITE, -1, "Number of moves: %d time(s)", MovCoun);
      textprintf_ex(buffer, font, 10, 30, WHITE, -1, "The recursive function has been called: %d time(s)", RecCoun);

      //Display the 3 pegs 
      for (int i = 0 ; i < 3 ; i++)
          draw_sprite(buffer, Peg, 195*i + 50, 175); 
      
      //Display the discs on each of the peg
      for (int i = 0; i < 3; i++)
          Pegs[i].DisplayStack(Discs, buffer, i, Scale, Pegs[i].size(), TotDsc);
      
      //Output the buffer onto the screen
      masked_blit(buffer, screen, 0, 135, 0, 135, buffer->w, buffer->h);
      
      rest(MovSpd); //Delay the movement for user observation
      
      destroy_bitmap(buffer); //destroys buffer bitmap     
}
END_OF_FUNCTION(displayPegs);

void Finished(bool &PlayAgain){
    //Load Bitmap of finished screen 
    BITMAP* End = load_bitmap("End.bmp", NULL);
    
    //Displays the finished screen
    clear_bitmap(screen);
    draw_sprite(screen, End, 35, 0);
    
    //Waits for user input to determine to whether to exit or continue
    if (readkey() >> 8 == KEY_ESC)
        PlayAgain = 0; //Exits the main function
    
    //clears screen and destroys bitmap    
    clear_bitmap(screen);
    destroy_bitmap(End);
}
END_OF_FUNCTION(Finished);
