#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

// WARNING: LCD DISPLAY USES P1.0.  Do not modify!!!

#define LED BIT6		/* LED connected to P1.6 */

#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8

#define SWITCHES 15     /* Define switches as bits 0-3 */

char blue = 31, green = 0, red = 31; /* RGB color values */
unsigned char step = 0; /* Step counter for animation */

/* Function to update switch interrupt sensing */
static char switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* Update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);	/* If switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);	/* If switch down, sense up */
  return p2val;
}

/* Initialize switches */
void switch_init()
{  
  P2REN |= SWITCHES;		/* Enable resistors for switches */
  P2IE |= SWITCHES;		/* Enable interrupts from switches */
  P2OUT |= SWITCHES;		/* Pull-ups for switches */
  P2DIR &= ~SWITCHES;		/* Set switches' bits for input */
  switch_update_interrupt_sense();
}

int switches = 0; /* Variable to store switch status */

/* Interrupt handler for switches */
void switch_interrupt_handler()
{
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;
}

// Ball position and movement variables
short drawPos[2] = {1,10}, controlPos[2] = {2, 10};
short colVelocity = 1, colLimits[2] = {1, screenWidth/2};

/* Draw a ball at specified position with given color */
void draw_ball(int col, int row, unsigned short color)
{
  fillRectangle(col-1, row-1, 3, 3, color);
}

/* Update ball position on screen */
void screen_update_ball()
{
  for (char axis = 0; axis < 2; axis ++) 
    if (drawPos[axis] != controlPos[axis]) /* Check if position changed */
      goto redraw;
  return;			/* Nothing to do if position unchanged */
 redraw:
  draw_ball(drawPos[0], drawPos[1], COLOR_BLUE); /* Erase previous ball */
  for (char axis = 0; axis < 2; axis ++) 
    drawPos[axis] = controlPos[axis];
  draw_ball(drawPos[0], drawPos[1], COLOR_WHITE); /* Draw new ball */
}

short redrawScreen = 1; /* Flag to indicate screen redraw */
u_int controlFontColor = COLOR_GREEN; /* Color for text */

/* Watchdog Timer interrupt handler */
void wdt_c_handler()
{
  static int secCount = 0;

  secCount ++;
  if (secCount >= 25) {		/* Approximately 1 second */
   
    {				/* Move the ball */
      short oldCol = controlPos[0];
      short newCol = oldCol + colVelocity;
      if (newCol <= colLimits[0] || newCol >= colLimits[1])
	colVelocity = -colVelocity; /* Change direction at screen edge */
      else
	controlPos[0] = newCol;
    }

    {				/* Update bunny */
      if (switches & SW3) green = (green + 1) % 64; /* Change green component */
      if (switches & SW2) blue = (blue + 2) % 32; /* Change blue component */
      if (switches & SW1) red = (red - 3) % 32; /* Change red component */
      if (step <= 30)
	step ++; /* Increment step counter */
      else
	step = 0; /* Reset step counter */
      secCount = 0; /* Reset second counter */
    }
    if (switches & SW4) return; /* Exit if switch 4 pressed */
    redrawScreen = 1; /* Set flag for screen redraw */
  }
}
  
void update_shape();

void main()
{
  
  P1DIR |= LED;		/**< Green led on when CPU on */
  P1OUT |= LED;
  configureClocks();
  lcd_init();
  switch_init();
  
  enableWDTInterrupts();      /**< Enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */
  
  clearScreen(COLOR_PURPLE); // Set background color to purple
  while (1) {			/* Forever loop */
    if (redrawScreen) {
      redrawScreen = 0; /* Reset redraw flag */
      update_shape(); /* Update shape on screen */
    }
    P1OUT &= ~LED;	/* Turn off LED */
    or_sr(0x10);	/**< CPU OFF */
    P1OUT |= LED;	/* Turn on LED */
  }
}

/* Update the shape on the screen */
void update_shape()
{
  screen_update_ball();
  // Draw bunny directly without using separate functions
  // Draw head
  fillRectangle(31, 69, 64, 45, COLOR_WHITE);
  // Draw left ear
  fillRectangle(31, 39, 16, 30, COLOR_WHITE);
  // Draw right ear
  fillRectangle(79, 39, 16, 30, COLOR_WHITE);
  // Draw inner ear
  fillRectangle(35, 44, 8, 25, COLOR_PINK);
  fillRectangle(83, 44, 8, 25, COLOR_PINK);
  // Draw open eyes
  fillRectangle(43, 84, 10, 10, COLOR_BLUE); // Adjust eyes position and color
  fillRectangle(73, 84, 10, 10, COLOR_BLUE); // Adjust eyes position and color
  // Draw nose
  fillRectangle(59, 94, 6, 6, COLOR_PINK); // Adjust nose position
}

/* Interrupt vector for Port 2 (switches) */
void __interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {	      /* Check if a button caused this interrupt */
    P2IFG &= ~SWITCHES;		      /* Clear pending switch interrupts */
    switch_interrupt_handler();	/* Call switch interrupt handler */
  }
}
