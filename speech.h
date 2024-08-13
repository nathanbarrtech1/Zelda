#ifndef SPEECH_H
#define SPEECH_H

//Display a speech bubble.
void speech(char* line1, char* line2);

//Allow speech to appeax for a time
static void speech_bubble_wait();

//Draw a speech bubble
static void draw_speech_bubble();

//Draw the text
static void draw_speech_line(const char* line, int lineNum);

//erases the text
static void erase_speech_bubble();

#endif // SPEECH_H