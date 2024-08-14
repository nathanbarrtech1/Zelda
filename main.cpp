// Project includes
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"
#include <time.h>
#include <story.h>

/**
 * Program entry point! This is where it all begins.
 * This function orchestrates all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main()
{
    //bool loop = true;
    //while(loop) {
        uLCD.cls();
        //initialize omnipotentMode to not omnipotent
        //unfortunately, I put every condition in backwards
        Player.omnipotentMode = true;
        
        //timer created for seeding purposes
        Timer randTime;
        
        // First things first: initialize hardware
        ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    
        //starts the time for seeding purposes
        randTime.start();
        
        //creates a start menu
        while(true){
            uLCD.textbackground_color(BLACK);
            uLCD.text_string("   THE LEGEND   ",1,5,FONT_7X8,0xFFFF00);
            uLCD.text_string("    OF ZELDA    ",1,7,FONT_7X8,0xFFFF00);
    
            GameInputs in = read_inputs();
            if(!in.b1 || !in.b2 || !in.b3){
                break;
            }
        }
        uLCD.cls();
        //sets the seed
        srand(randTime.read_ms());
        wait(1);
    
        //prints the "backstory" and mission
        uLCD.textbackground_color(BLACK);
        uLCD.text_string("    ZELDA HAS    ",1,5,FONT_7X8,0xFFFF00);
        uLCD.text_string(" BEEN KIDNAPPED! ",1,7,FONT_7X8,0xFFFF00);
        uLCD.text_string("    SAVE HER!    ",1,9,FONT_7X8,0xFFFF00);
        wait(2);
    
        //describes the player's main way of understanding what to do next
        uLCD.cls();
        speech("For tips, press ", "action button   ");
        // Initialize the maps
        maps_init();
        //maps_init2();
        init_main_map();
        //init_main_map2();
    
        // Initialize game state
        set_active_map(0);
        Player.x = Player.y = 5;
    
        // Initial drawing
        draw_game(true);
    
        // Main game loop
        while(1)
        {
            // Timer to measure game update speed
            Timer t; t.start();
    
            // Actually do the game update:
            // 1. Read inputs
            GameInputs in = read_inputs();
    
            // 2. Determine action (get_action)
            int current_action  = get_action(in);
    
            // 3. Update game (update_game)
            int current_update = update_game(current_action);
    
            // 3b. Check for game over
            //Occurs when the player died and lost
            if (lives == '0') {
                uLCD.cls();
                uLCD.textbackground_color(BLACK);
                uLCD.text_string("   YOU FAILED    ",1,5,FONT_7X8,0xFFFF00);
                uLCD.text_string("    TO SAVE      ",1,7,FONT_7X8,0xFFFF00);
                uLCD.text_string("    ZELDA...     ",1,9,FONT_7X8,0xFFFF00);
                destroy();
                wait(3);
                break;
            }
            
            //Occurs when the player saved Zelda and beat the game
            if (jump == 2) {
                destroy();
                wait(3);
                break;
            }
    
            // 4. Draw frame (draw_game)
            //fully redraws when the action button is pressed
            if      (current_update == FULL_DRAW || onlyOnce1) {
                draw_game(true);
                onlyOnce1 = false;
            }
            
            //does a quick draw when the player moves
            else if (current_update == HALF_DRAW) draw_game(false);
    
            // 5. Frame delay
            t.stop();
            int dt = t.read_ms();
            if (dt < 100) wait_ms(100 - dt);
        }
}