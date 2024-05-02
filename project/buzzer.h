#ifndef buzzer_included
#define buzzer_included

void buzzer_init();
void buzzer_set_period(short cycles);

// Function prototypes for custom functions
void fillCircle(int x0, int y0, int r, unsigned int color);
void drawLine(int x0, int y0, int x1, int y1, unsigned int color);
void drawArc(int x0, int y0, int r, int start_angle, int end_angle, int thickness, unsigned int color);

#endif // included
