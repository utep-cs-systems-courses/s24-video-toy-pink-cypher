#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "buzzer.h"

#define LED BIT6
#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8
#define SWITCHES 15

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

// Function to draw Bibi the kitty cat
void draw_kitty_cat(int x, int y, u_int color){
  fillCircle(x, y, 15, color); // Head
  fillCircle(x - 7, y + 5, 3, color); // Left ear
  fillCircle(x + 7, y + 5, 3, color); // Right ear
  fillCircle(x - 5, y - 2, 2, COLOR_BLACK); // Left eye
  fillCircle(x + 5, y - 2, 2, COLOR_BLACK); // Right eye
  drawLine(x - 2, y + 3, x + 2, y + 3, COLOR_BLACK); // Nose
  drawArc(x, y + 5, 7, 180, 360, 2, COLOR_BLACK); // Mouth
}

// Function to update the kitty cat's position on screen
void screen_update_kitty_cat(){
  for (char axis = 0; axis < 2; axis++)
    if (drawPos[axis] != controlPos[axis])
      goto redraw;
  return;
 redraw:
  draw_kitty_cat(drawPos[0], drawPos[1], COLOR_BLUE); // Erase previous position
  for (char axis = 0; axis < 2; axis++)
    drawPos[axis] = controlPos[axis];
  draw_kitty_cat(drawPos[0], drawPos[1], colorChange); // Draw Kitty Cat in new position
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
