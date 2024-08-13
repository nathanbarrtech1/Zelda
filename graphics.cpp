#include "graphics.h"
#include "globals.h"
#include "graphicsChars.h"

#define YELLOW 0xFFFF00
#define BROWN  0xD2691E
#define DIRT   BROWN

void draw_upper_status(int x, int y)
{
    //Draw upper status bar background and border
    uLCD.filled_rectangle(0,8,127,0,BLACK);
    uLCD.line(0, 8, 128, 8, YELLOW);
    

    //sets colorscheme
    uLCD.textbackground_color(BLACK);
    uLCD.color(YELLOW);
    
    //locates and prints the x coordinate
    uLCD.locate(1,0);
    uLCD.printf("(%d,",x);
    
    //locates and prints the y coordinate
    uLCD.locate(6,0);
    uLCD.printf("%d)",y);
}

void draw_lower_status() {
    //Draw lower status bar border
    uLCD.line(0, 118, 127, 118, YELLOW);
}

//Draw border of actual game map
void draw_border() {
    uLCD.filled_rectangle(0,     9, 127,  11, BLACK); // Top
    uLCD.filled_rectangle(0,    10,   2, 114, BLACK); // Left
    uLCD.filled_rectangle(0,   115, 127, 117, BLACK); // Bottom
    uLCD.filled_rectangle(125,  10, 127, 117, BLACK); // Right
}

void draw_nothing(int u, int v)         {uLCD.filled_rectangle(u, v, u+10, v+10, BLACK);}
void draw_wall(int u, int v)            {uLCD.BLIT(u, v, 11,11, wallBlue);}
void draw_wall2(int u, int v)           {uLCD.BLIT(u, v, 11,11, wallColorful);}
void draw_plant(int u, int v)           {uLCD.BLIT(u, v, 11,11, tree);}
void draw_ladder(int u, int v)          {uLCD.BLIT(u, v, 11,11, ladder);}
void draw_ladder2(int u, int v)         {uLCD.BLIT(u, v, 11,11, ladder2);}
void draw_wand(int u, int v)            {uLCD.BLIT(u, v, 11,11, wand);}
void draw_zelda(int u, int v)           {uLCD.BLIT(u, v, 11,11, zelda);}
void draw_guard(int u, int v)           {uLCD.BLIT(u, v, 11,11, guard);}
void draw_wizard(int u, int v)          {uLCD.BLIT(u, v, 11,11, wizard);}
void draw_sword_in_stone(int u, int v)  {uLCD.BLIT(u, v, 11,11, swordInStone);}
void draw_stone(int u, int v)           {uLCD.BLIT(u, v, 11,11, stone);}
void draw_house_wall(int u, int v)      {uLCD.BLIT(u, v, 11, 11, house_wall);}
void draw_roof_left(int u, int v)       {uLCD.BLIT(u, v, 11, 11, roof_left);}
void draw_roof_middle(int u, int v)     {uLCD.BLIT(u, v, 11, 11, roof_middle);}
void draw_roof_right(int u, int v)      {uLCD.BLIT(u, v, 11, 11, roof_right);}
void draw_door(int u, int v)            {uLCD.BLIT(u, v, 11, 11, door);}
void draw_exclamation(int u, int v)     {uLCD.BLIT(u, v, 11, 11, exclamation);}
void draw_player(int u, int v)          {uLCD.BLIT(u, v, 11,11, link);}
void draw_space_distorter(int u, int v) {uLCD.BLIT(u, v, 11,11, space_distorter);}

void draw_sword_animation_h(int u, int v, bool east) {
        int multiplier;
        if (east) multiplier =  1;
        else      multiplier = -1;
    
        for (int i = 0; i < 11; i++) {
            if (i < 6 || i == 10)
                uLCD.pixel(u + i * multiplier, v + 5, 0xD5D5D5);
            if (i >= 1) {
                if (i <= 5 || i == 7 | i == 8 || i == 10) uLCD.pixel(u + (i - 1) * multiplier, v + 4, 0xD5D5D5);
                if (i <= 6)                               uLCD.pixel(u + (i - 1) * multiplier, v + 5, 0xD5D5D5);
                if (i <= 7 || i == 10)                    uLCD.pixel(u + (i - 1) * multiplier, v + 6, 0xD5D5D5);
            }
            if (i >= 7)  {
                uLCD.pixel(u + (i - 7) * multiplier, v + 2, 0xFFB200);
                uLCD.pixel(u + (i - 7) * multiplier, v + 3, 0xFFB200);
                uLCD.pixel(u + (i - 7) * multiplier, v + 4, 0xFF0000);
                uLCD.pixel(u + (i - 7) * multiplier, v + 5, 0xFFB200);
                uLCD.pixel(u + (i - 7) * multiplier, v + 6, 0xFF0000);
                uLCD.pixel(u + (i - 7) * multiplier, v + 7, 0xFFB200);
                uLCD.pixel(u + (i - 7) * multiplier, v + 8, 0xFFB200);
            }
            if (i >= 8)  {
                uLCD.pixel(u + (i - 8) * multiplier, v + 2, 0x000000);
                uLCD.pixel(u + (i - 8) * multiplier, v + 3, 0xFFB200);
                uLCD.pixel(u + (i - 8) * multiplier, v + 4, 0xFFB200);
                uLCD.pixel(u + (i - 8) * multiplier, v + 5, 0xFF0000);
                uLCD.pixel(u + (i - 8) * multiplier, v + 6, 0xFFB200);
                uLCD.pixel(u + (i - 8) * multiplier, v + 7, 0xFFB200);
                uLCD.pixel(u + (i - 8) * multiplier, v + 8, 0x000000);
            }
            if (i >= 9)  {
                uLCD.pixel(u + (i - 9) * multiplier, v + 2, 0x000000);
                uLCD.pixel(u + (i - 9) * multiplier, v + 3, 0x000000);
                uLCD.pixel(u + (i - 9) * multiplier, v + 4, 0xFFB200);
                uLCD.pixel(u + (i - 9) * multiplier, v + 5, 0xFFB200);
                uLCD.pixel(u + (i - 9) * multiplier, v + 6, 0xFFB200);
                uLCD.pixel(u + (i - 9) * multiplier, v + 7, 0x000000);
                uLCD.pixel(u + (i - 9) * multiplier, v + 8, 0x000000);
            }
            wait_ms(80);
       }
}

void draw_sword_animation_v(int u, int v, bool south) {
    int multiplier;
    if (south) multiplier =  1;
    else       multiplier = -1;
    
    for (int i = 0; i < 11; i++) {
        uLCD.pixel(u + 5, v + i * multiplier, 0xD5D5D5);
        if (i >= 1) {
            if (i <= 4 || i == 6) uLCD.pixel(u +  4, v + (i - 1) * multiplier, 0xD5D5D5);
                                  uLCD.pixel(u +  5, v + (i - 1) * multiplier, 0xD5D5D5);
                                  uLCD.pixel(u +  6, v + (i - 1) * multiplier, 0xD5D5D5);
        }
        
        if (i >= 7) {
            if (i == 7 || i == 9)             uLCD.pixel(u +  2, v + (i - 6) * multiplier, 0xFFB200);
            if (i == 7 || i == 11 || i == 13) uLCD.pixel(u +  3, v + (i - 6) * multiplier, 0xFFB200);
            uLCD.pixel(u +  4, v + (i - 6) * multiplier, 0xFF0000);
            uLCD.pixel(u +  5, v + (i - 6) * multiplier, 0xFFB200);
            uLCD.pixel(u +  6, v + (i - 6) * multiplier, 0xFF0000);
            uLCD.pixel(u +  7, v + (i - 6) * multiplier, 0xFFB200);
            uLCD.pixel(u +  8, v + (i - 6) * multiplier, 0xFFB200);
        }
        if (i >= 8) {
            if (i == 7 || i == 9)             uLCD.pixel(u +  2, v + (i - 7) * multiplier, 0x000000);
            if (i == 7 || i == 11 || i == 13) uLCD.pixel(u +  3, v + (i - 7) * multiplier, 0xFFB200);
            uLCD.pixel(u +  4, v + (i - 7) * multiplier, 0xFFB200);
            uLCD.pixel(u +  5, v + (i - 7) * multiplier, 0xFF0000);
            uLCD.pixel(u +  6, v + (i - 7) * multiplier, 0xFFB200);
            uLCD.pixel(u +  7, v + (i - 7) * multiplier, 0xFFB200);
            uLCD.pixel(u +  8, v + (i - 7) * multiplier, 0x000000);
        }
        if (i >= 9) {
            if (i == 7 || i == 9)             uLCD.pixel(u +  2, v + (i - 8) * multiplier, 0x000000);
            if (i == 7 || i == 11 || i == 13) uLCD.pixel(u +  3, v + (i - 8) * multiplier, 0x000000);
            uLCD.pixel(u +  4, v + (i - 8) * multiplier, 0xFFB200);
            uLCD.pixel(u +  5, v + (i - 8) * multiplier, 0xFFB200);
            uLCD.pixel(u +  6, v + (i - 8) * multiplier, 0xFFB200);
            uLCD.pixel(u +  7, v + (i - 8) * multiplier, 0x000000);
            uLCD.pixel(u +  8, v + (i - 8) * multiplier, 0x000000);
        }
        wait_ms(80);
    }
}

int getMaze(int val) {return maze[val];}