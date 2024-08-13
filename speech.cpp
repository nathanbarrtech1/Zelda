#include "speech.h"

#include "globals.h"
#include "hardware.h"

#define TOP    0
#define BOTTOM 1

//Draw a white speech bubble
void draw_speech_bubble() {uLCD.rectangle(6, 23, 119, 40, WHITE);}

//Allow speech to appeax for 1.8 seconds
void speech_bubble_wait() {wait(1.8);}

//Draw the text
void draw_speech_line(char* line, int lineNum)
{
    uLCD.textbackground_color(BLACK);
    uLCD.text_string(line, 1,3 + lineNum,FONT_7X8,0xFFFF00);
}

//erases the text
static void erase_speech_bubble() {
    uLCD.filled_rectangle(6, 23, 119, 40, BLACK);
}

//function to be called that draws the speech and bubble and waits
void speech(char* line1, char* line2)
{
    draw_speech_bubble();
    draw_speech_line(line1, TOP);
    draw_speech_line(line2, BOTTOM);
    speech_bubble_wait();
    erase_speech_bubble();
}