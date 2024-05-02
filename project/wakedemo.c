#include <msp430.h>
#include "buzzer.h"
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

#define LED BIT6
#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8
#define SWITCHES 15

// Define colors
#define COLOR_LIGHT_PINK 0xF6B6
#define COLOR_INDIGO     0x4810

// Declare variables
int switches = 0;
int changeSong = 0;
int redrawScreen = 0;


// Define colors
u_int colorChange = COLOR_LIGHT_PINK;

// Function to update switch interrupt sense
static char switch_update_interrupt_sense(){
  char p2val = P2IN;
  P2IES |= (p2val & SWITCHES);
  P2IES &= (p2val | ~SWITCHES);
  return p2val;
}

// Function to initialize switches
void switch_init(){
  P2REN |= SWITCHES;
  P2IE |= SWITCHES;
  P2OUT |= SWITCHES;
  P2DIR &= ~SWITCHES;
  switch_update_interrupt_sense();
}

// Switch interrupt handler
void switch_interrupt_handler(){
  char p2val = switch_update_interrupt_sense();
  // Define 'switches' here or globally
 switches = ~p2val & SWITCHES;
}

// Arrays and variables for kitty cat's position and movement
short drawPos[2] = {1, 1}, controlPos[2] = {2, 2};
short colVelocity = 1, colLimits[2] = {1, screenWidth/2};
short colLimits_2[2] = {1, screenHeight/2};

// Function to draw the square on the screen
void draw_ball(int col, int row, unsigned short color){
  fillRectangle(col-1, row-1, 10, 10, color); // Draws a square at the specified position
}

/* 
 * Updates the position of the square on the screen.
 * If the position has changed, it erases the square from its previous position
 * by drawing a blue square to match the screen's color.
 * Then, it draws the square at the new position.
 */
void screen_update_ball(){
  for (char axis = 0; axis < 2; axis++)
    if (drawPos[axis] != controlPos[axis]) /* Check if position has changed */
      goto redraw;
  return; /* If no change, nothing to do */
 redraw:
  draw_ball(drawPos[0], drawPos[1], COLOR_BLUE); /* Erase previous square */
  for (char axis = 0; axis < 2; axis++)
    drawPos[axis] = controlPos[axis];
  // Draw the square at the new position with the specified color
  draw_ball(drawPos[0], drawPos[1], colorChange);
}

// State machine changes every second, changing frequencies for songs
int change_state = 0;
#define STATE_MAX 8

// Song functions
int song_one(){
  switch(change_state){
    case 0:
      buzzer_set_period(300); // Note E4
      break;
    case 1:
      buzzer_set_period(400); // Note G4
      break;
    case 2:
      buzzer_set_period(500); // Note B4
      break;
    case 3:
      buzzer_set_period(600); // Note D5
      break;
    case 4:
      buzzer_set_period(700); // Note F#5
      break;
    case 5:
      buzzer_set_period(800); // Note A5
      break;
    case 6:
      buzzer_set_period(900); // Note C6
      break;
    case 7:
      buzzer_set_period(1000); // Note E6
      break;
  }
}

int song_two(){
  switch(change_state){
    case 0:
      buzzer_set_period(500); // Note B4
      break;
    case 1:
      buzzer_set_period(600); // Note D5
      break;
    case 2:
      buzzer_set_period(700); // Note F#5
      break;
    case 3:
      buzzer_set_period(800); // Note A5
      break;
    case 4:
      buzzer_set_period(900); // Note C6
      break;
    case 5:
      buzzer_set_period(1000); // Note E6
      break;
    case 6:
      buzzer_set_period(1100); // Note G6
      break;
    case 7:
      buzzer_set_period(1200); // Note B6
      break;
  }
}

int song_three(){
  switch(change_state){
    case 0:
      buzzer_set_period(400); // Note G4
      break;
    case 1:
      buzzer_set_period(500); // Note B4
      break;
    case 2:
      buzzer_set_period(600); // Note D5
      break;
    case 3:
      buzzer_set_period(700); // Note F#5
      break;
    case 4:
      buzzer_set_period(800); // Note A5
      break;
    case 5:
      buzzer_set_period(900); // Note C6
      break;
    case 6:
      buzzer_set_period(1000); // Note E6
      break;
    case 7:
      buzzer_set_period(1100); // Note G6
      break;
  }
}

int song_four(){
  switch(change_state){
    case 0:
      buzzer_set_period(600); // Note D5
      break;
    case 1:
      buzzer_set_period(700); // Note F#5
      break;
    case 2:
      buzzer_set_period(800); // Note A5
      break;
    case 3:
      buzzer_set_period(900); // Note C6
      break;
    case 4:
      buzzer_set_period(1000); // Note E6
      break;
    case 5:
      buzzer_set_period(1100); // Note G6
      break;
    case 6:
      buzzer_set_period(1200); // Note B6
      break;
    case 7:
      buzzer_set_period(1300); // Note D7
      break;
  }
}

int song_five(){
  switch(change_state){
    case 0:
      buzzer_set_period(500);
      break;
    case 1:
      buzzer_set_period(400);
      break;
    case 2:
      buzzer_set_period(600);
      break;
    case 3:
      buzzer_set_period(300);
      break;
    case 4:
      buzzer_set_period(700);
      break;
    case 5:
      buzzer_set_period(200);
      break;
    case 6:
      buzzer_set_period(800);
      break;
    case 7:
      buzzer_set_period(100);
      break;
  }
}

// Function to update the state every second
int sec(){
  change_state++;
  if(change_state >= STATE_MAX){
    change_state = 0;
  }
}

// Function to handle interrupt
void wdt_c_handler(){
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;
  short oldCol_x = controlPos[0];
  short oldCol_y = controlPos[1];
  static int sec_count = 0;
  sec_count++;
  if(sec_count == 100){
    sec_count = 0;
    sec();
  }

   // Conditions for switch presses
  // Condition for SW4
  if(switches & SW4){
    colorChange = COLOR_LIGHT_PINK;
    changeSong = 4;
    short newCol = oldCol_x + colVelocity;
    if(!(newCol <= colLimits[0]) || !(newCol >= colLimits[1])){
      redrawScreen = 1;
      controlPos[0] = newCol;
    }
  }
 // Condition for SW3
  if(switches & SW3){
    colorChange = COLOR_INDIGO;
    changeSong = 6;
    short newCol = oldCol_x - colVelocity; // Move left
    if (!(newCol <= colLimits[0]) || !(newCol >= colLimits[1])){
      redrawScreen = 1;
      controlPos[0] = newCol;
    }
  }

  if(switches & SW2){
    colorChange = COLOR_LIGHT_BLUE;
    changeSong = 7;
    short newCol = oldCol_y + colVelocity; // Move down
    redrawScreen = 1;
    controlPos[1] = newCol;
  }

  // Condition for SW1
  if(switches & SW1){
    colorChange = COLOR_SEA_GREEN;
    changeSong = 8;
    short newCol = oldCol_y - colVelocity; // Move up
    if (!(newCol <= colLimits_2[0]) || !(newCol >= colLimits_2[1])){
      redrawScreen = 1;
      controlPos[1] = newCol;
    }
  }
}

// Main function
void main(){
  P1DIR |= LED;
  P1OUT |= LED;
  configureClocks();
  lcd_init();
  switch_init();
  buzzer_init();
  enableWDTInterrupts();
  or_sr(0x8);
  clearScreen(COLOR_BLUE);
  
  while (1) {
    if(redrawScreen) {
      redrawScreen = 0;
      screen_update_kitty_cat();
    }
    switch(changeSong){
      case 1:
        song_one();
        break;
      case 2:
        song_two();
        break;
      case 3:
        song_three();
        break;
      case 4:
        song_four();
        break;
      case 5:
        song_five();
        break;
      // Add more cases for other songs as needed
    }
    P1OUT &= ~LED;
    or_sr(0x10);
    P1OUT |= LED;
  }
}

// Function to update the Kitty Cat on the screen
void update_kitty_cat(){
  screen_update_kitty_cat();
}

// Interrupt vector for switch inputs
void __interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {
    P2IFG &= ~SWITCHES;
    switch_interrupt_handler();
  }
}
