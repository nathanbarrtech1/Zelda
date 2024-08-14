#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"
#include <time.h>

//global variables
int b = 1;
int n = 0;
int xDistorter, yDistorter;
int jump = 0;
int xZelda = 8;
int yZelda = 8;
int xSword = 1;
int ySword = 1;
int xWizard;
int yWizard;
char lives = '3';
Timer moveZelda;
bool onlyOnce1 = false;
bool quest = 0;
bool needsDistorter = false;
bool hasDistorter = false;
bool doorOpen = false;
bool hidden = false;
bool active = true;
bool onlyOnce = true;
bool wand = false;
bool wandTaken = false;
bool swordFound = false;
bool hasSword = false;
bool needsSword = false;
bool guardsDead[5][5] = {
    {true, true, true, true, true},
    {true, true, true, true, true},
    {true, true, true, true, true},
    {true, true, true, true, true},
    {true, true, true, true, true}
};

/**
 * The main game state. Must include Player locations and previous locations for
 * drawing to work properly. Other items can be added as needed.
 */
struct {
    int x,y;    // Current locations
    int px, py; // Previous locations
    int has_key;
    bool omnipotentMode;
    // You can add other properties for the player here
} Player;

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possbile return values are defined below.
 */

#define NO_ACTION     0
#define GO_LEFT       1
#define GO_RIGHT      2
#define GO_UP         3
#define GO_DOWN       4
#define OMNIPOTENT    5
#define ACTION_BUTTON 6

void actionButton() {
    if (get_east(Player.x,Player.y) -> type == ZELDA || get_west(Player.x,Player.y) -> type == ZELDA ||get_north(Player.x,Player.y) -> type == ZELDA || get_south(Player.x,Player.y) -> type == ZELDA){
        if (jump == 0) {
            delete_guards();
            for (int i = -1; i <= 1; i++)
                for (int j = -1; j <= 1; j++)
                    map_erase(xZelda + i, yZelda + j);
            speech("HAHAHAHAHAHA!!!!", "It won't be this");
            speech("easy again now  ", "that I know you ");
            speech("are after me.You", "won't save her  ");
            
            moveZelda.start();
            while(true) {
                xZelda = 3 + rand() % 45;
                yZelda = 3 + rand() % 45;
                if (abs(xZelda - xWizard) > 10 || abs(yZelda - yWizard) > 10 && abs(xZelda - Player.x) > 10 || abs(yZelda - Player.y) > 10) {
                    add_zelda(xZelda, yZelda);
                    break;
                }
            }
            for (int i = -2; i <= 2; i++)
                for (int j = -2; j <= 2; j++)
                    if ((i + j) % 2 == 1) {
                        add_guard(xZelda + i, yZelda + j);
                        add_guard(xZelda - i, yZelda - j);
                    }
            /*for (int i = -2; i <= 2; i++)
                for (int j = -2; j <= 2; j++)
                    guardsDead[i][j] = false;
            for (int i = -1; i <= 1; i++)
                for (int j = -1; j <= 1; j++)
                    add_guard(xZelda, yZelda);*/
            add_exclamation(xWizard, yWizard - 1);
            jump = 1;
        }
        else {
            uLCD.cls();
            uLCD.textbackground_color(BLACK);
            uLCD.text_string("    YOU SAVED    ",1,5,FONT_7X8,0xFFFF00);
            uLCD.text_string("      ZELDA!      ",1,8,FONT_7X8,0xFFFF00);
            uLCD.text_string(" Congratulations! ",1,11,FONT_7X8,0xFFFF00);
            jump = 2;
        }
    }
    else if (get_east(Player.x,Player.y) -> type == SPACE_DISTORTER || get_west(Player.x,Player.y) -> type == SPACE_DISTORTER || get_north(Player.x,Player.y) -> type == SPACE_DISTORTER || get_south(Player.x,Player.y) -> type == SPACE_DISTORTER) {
        hasDistorter = true;
        needsDistorter = false;
        uLCD.text_string("              ",1,15,FONT_7X8,0xFFFF00);
        add_exclamation(xWizard, yWizard - 1);
        map_erase(xDistorter, yDistorter);
    }
    else if (get_east(Player.x, Player.y) -> type == GUARD && swordFound) {
        draw_sword_animation_h (69, 59, true);
        map_erase(Player.x + 1, Player.y);
        guardsDead[0][Player.y - yZelda + 2] = true;
    }
    else if (get_west(Player.x, Player.y) -> type == GUARD && swordFound) {
        draw_sword_animation_h (57, 59, false);
        map_erase(Player.x - 1, Player.y);
        guardsDead[4][Player.y - yZelda + 2] = true;
    }
    else if (get_north(Player.x, Player.y) -> type == GUARD && swordFound) {
        draw_sword_animation_v (58, 59, false);
        map_erase(Player.x, Player.y - 1);
        guardsDead[Player.x - xZelda + 2][4] = true;
    }
    else if (get_south(Player.x, Player.y) -> type == GUARD && swordFound) {
        draw_sword_animation_v (58, 70, true);
        map_erase(Player.x, Player.y + 1);
        guardsDead[Player.x - xZelda + 2][0] = true;
    }
    else if (get_east(Player.x,Player.y) -> type == WIZARD || get_west(Player.x,Player.y) -> type == WIZARD ||get_north(Player.x,Player.y) -> type == WIZARD){
       //erases the exclamation point above the wizard's head
       map_erase(xWizard, yWizard - 1);
       
       //says this when Zelda has jumped
       if (jump == 1 && !hasDistorter) {
           uLCD.text_string("              ",1,15,FONT_7X8,0xFFFF00);
           speech("There is an item", "that can slow   ");
           speech("the necromancers", "teleportation   ");
           speech("speed. Find it  ", "for a better    ");
           speech("chance at saving", "Zelda.          ");
           needsDistorter = true;
           while (true) {
               xDistorter = 1 + rand() % 50;
               yDistorter = 1 + rand() % 50;
               if (get_here(xDistorter, yDistorter) == NULL) {
                   add_space_distorter(xDistorter,yDistorter);
                   break;
               }
           }
        }
        
        //Wizard's last comment
        //says this when the distorter has been obtained
        else if (jump == 1 && hasDistorter) {
           uLCD.text_string("              ",1,15,FONT_7X8,0xFFFF00);
           speech("He is slower now", "You can defeat  ");
           speech("him and save    ", "Zelda           ");
        }
        
        //Wizard's first comment
       else if (!wandTaken) {
           speech("Zelda's missing?", "They used magic ");
           speech("to hide her.    ", "I need my wand  ");
           speech("to find her, but", "I lost my wand  ");
           speech("in the dungeon. ", "Please find it. ");
           quest = 1;
           add_ladder(xWizard + 2, yWizard);
       }
       
       //Wizard says this when the wand has been obtained
       else if (wandTaken && !swordFound) {
           speech("I can sense her,", "but she is being");
           speech("guarded. Find a ", "sword to fight  ");
           speech("the skeletons.  ", "                ");
           needsSword = true;
           hasSword = false;
       }
       
       //Wizard says this when the sword has been obtained
       else if (wandTaken && swordFound) {
           speech("Now, you can    ", "bravely attack  ");
           speech("the enemy and   ", "save Zelda.     ");
           hasSword = true;
       }
    }
    else if (get_here(Player.x, Player.y) -> type == LADDER && quest == 1) {
        //destroys the main map
        destroy();
        
        //sets map flag
        active = false;
        
        //intializes the maze map
        maps_init2();
        init_main_map2();
    }
    
    //Occurs when standing on the ladder in the maze map and the action button is pressed
    else if (get_here(Player.x, Player.y) -> type == LADDER2) {
        //destroys the maze map
        destroy();
        
        //initializes the main map
        maps_init();
        init_main_map();
        
        //sets map flag
        active = true;
        
        //adds the sword in the stone and zelda as long as they don't interfere with each other, the wizard, and the player
        while (true) {
            xZelda = 3 + rand() % 45;
            yZelda = 3 + rand() % 45;
            if (abs(xZelda - xWizard) > 10 || abs(yZelda - yWizard) > 10 && abs(xZelda - Player.x) > 10 || abs(yZelda - Player.y) > 10 && abs(xSword - xZelda) > 3) {
                add_zelda(xZelda, yZelda);
                break;
            }
        }
        while (true) {
            xSword = 1 + rand() % 50;
            ySword = 1 + rand() % 50;
            if (get_here(xSword, ySword) == NULL) {
                add_sword_in_stone(xSword, ySword);
                break;
            }
        }
        
        //activates the guards
        for (int i = 0; i < 5; i++)
            for (int j = 0; j < 5; j++)
                guardsDead[i][j] = false;
                
        //sets the steps
        b = 0;
        
        //first step
        move_guards(b);
    }
    
    //occurs when the player is next to the wand/sonar
    else if (get_north(Player.x, Player.y) -> type == WAND) {
        //adds the ladder to leave the maze map
        add_ladder2(8, 9);
        
        //sets flags
        wandTaken = true;
        hasSword = true;
        
        //erases the wand from the map
        map_erase(Player.x, Player.y - 1);
        
        //provides encouragement
        uLCD.textbackground_color(BLACK);
        uLCD.text_string("Now, find her!", 1,15,FONT_7X8,0xFFFF00); wait(2);
        uLCD.text_string("              ", 1,15,FONT_7X8,0xFFFF00);
    }
    
    //Occurs when next to the sword in the stone and the button is pressed
    else if (get_east(Player.x,Player.y)  -> type == SWORD_IN_STONE || get_west(Player.x,Player.y)  -> type == SWORD_IN_STONE || get_north(Player.x,Player.y) -> type == SWORD_IN_STONE ||get_south(Player.x,Player.y) -> type == SWORD_IN_STONE) {
        //sets flags
        swordFound = true;
        needsSword = false;
        
        //shows wizard needs to say something
        add_exclamation(xWizard, yWizard - 1);
        
        //determines where do do the animation
        if      (get_east(Player.x,Player.y)  -> type == SWORD_IN_STONE) {add_stone(Player.x + 1, Player.y);}
        else if (get_west(Player.x,Player.y)  -> type == SWORD_IN_STONE) {add_stone(Player.x - 1, Player.y);}
        else if (get_north(Player.x,Player.y) -> type == SWORD_IN_STONE) {add_stone(Player.x, Player.y - 1);}
        else if (get_south(Player.x,Player.y) -> type == SWORD_IN_STONE) {add_stone(Player.x, Player.y + 1);}
    }
    else if (wandTaken && active) {
        //changes tip when the player gets close to Zelda for the first time
        if (abs(xZelda - Player.x) < 5 && abs(yZelda - Player.y) < 5 && jump == 0) hasSword = false;
        
        //occurs when searching for the distorter
        if (needsDistorter) {
            uLCD.textbackground_color(BLACK);
            if(xDistorter > Player.x)       uLCD.text_string("Distorter is e",1,15,FONT_7X8,0xFFFF00);
            else if(xDistorter < Player.x)  uLCD.text_string("Distorter is w",1,15,FONT_7X8,0xFFFF00); wait(1.5);
            if(yDistorter > Player.y)       uLCD.text_string("Distorter is s",1,15,FONT_7X8,0xFFFF00);
            else if(yDistorter < Player.y)  uLCD.text_string("Distorter is n",1,15,FONT_7X8,0xFFFF00); wait(1.5);
                                            uLCD.text_string("              ",1,15,FONT_7X8,0xFFFF00);
        }
        
        //occurs when searching for Zelda
        else if (hasSword) {
            uLCD.textbackground_color(BLACK);
            if(xZelda > Player.x)       uLCD.text_string("Zelda is east ",1,15,FONT_7X8,0xFFFF00);
            else if(xZelda < Player.x)  uLCD.text_string("Zelda is west ",1,15,FONT_7X8,0xFFFF00); wait(1.5);
            if(yZelda > Player.y)       uLCD.text_string("Zelda is south",1,15,FONT_7X8,0xFFFF00);
            else if(yZelda < Player.y)  uLCD.text_string("Zelda is north",1,15,FONT_7X8,0xFFFF00); wait(1.5);
                                        uLCD.text_string("              ",1,15,FONT_7X8,0xFFFF00);
        }
        
        //occurs when searching for the sword
        else if (needsSword) {
            uLCD.textbackground_color(BLACK);
            if(xSword > Player.x)       uLCD.text_string("Sword is east ",1,15,FONT_7X8,0xFFFF00);
            else if(xSword < Player.x)  uLCD.text_string("Sword is west ",1,15,FONT_7X8,0xFFFF00); wait(1.5);
            if(ySword > Player.y)       uLCD.text_string("Sword is south",1,15,FONT_7X8,0xFFFF00);
            else if(ySword < Player.y)  uLCD.text_string("Sword is north",1,15,FONT_7X8,0xFFFF00); wait(1.5);
                                        uLCD.text_string("              ",1,15,FONT_7X8,0xFFFF00);
        }
        else {
            uLCD.textbackground_color(BLACK);
            uLCD.text_string("Find the wizard",0,15,FONT_7X8,0xFFFF00); wait(1.5);
            uLCD.text_string("He can help    ",0,15,FONT_7X8,0xFFFF00); wait(1.5);
            uLCD.text_string("               ",0,15,FONT_7X8,0xFFFF00);
        }
    }

        //occurs when the sonar is take, and it is the map maze
    //Allows player to destroy the blocks around him
    else if (wandTaken && !active){
        if (get_east(Player.x,Player.y) ->  type == WALL2  && Player.x != map_width()  - 1) map_erase(Player.x + 1, Player.y);
        if (get_west(Player.x,Player.y) ->  type == WALL2  && Player.x != 1)                map_erase(Player.x - 1, Player.y);
        if (get_north(Player.x,Player.y) -> type == WALL2  && Player.y != 1)                map_erase(Player.x, Player.y - 1);
        if (get_south(Player.x,Player.y) -> type == WALL2  && Player.y != map_height() - 1) map_erase(Player.x, Player.y + 1);
    }
    
    //occurs when not searching for anything and on the main map
    else if (active) {
        uLCD.textbackground_color(BLACK);
        uLCD.text_string("Find the wizard",0,15,FONT_7X8,0xFFFF00); wait(1.5);
        uLCD.text_string("He can help    ",0,15,FONT_7X8,0xFFFF00); wait(1.5);
        uLCD.text_string("               ",0,15,FONT_7X8,0xFFFF00);
    }
    //occurs when not picking up the wand and on the maze map
    else if (!wand) {
        uLCD.textbackground_color(BLACK);
        uLCD.text_string("The wand is    ",0,15,FONT_7X8,0xFFFF00); wait(1.5);
        uLCD.text_string("hidden.You must",0,15,FONT_7X8,0xFFFF00); wait(1.5);
        uLCD.text_string("stand next to  ",0,15,FONT_7X8,0xFFFF00); wait(1.5);
        uLCD.text_string("it to find it. ",0,15,FONT_7X8,0xFFFF00); wait(1.5);
        uLCD.text_string("               ",0,15,FONT_7X8,0xFFFF00);
    }
}

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the Player position accordingly.
 *
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the player has not moved.
 */
#define NO_RESULT 0
#define HALF_DRAW 1
#define FULL_DRAW 2
int update_game(int action)
{
    // Save player previous location before updating
    Player.px = Player.x;
    Player.py = Player.y;

    switch(action)
    {
        case ACTION_BUTTON:
            actionButton();
            return FULL_DRAW;
        case GO_UP:      if(go_up())   {return HALF_DRAW;}
        case GO_LEFT:    if(go_left()) {return HALF_DRAW;}
        case GO_DOWN:    if(go_down()) {return HALF_DRAW;}
        case GO_RIGHT:   if(go_right()){return HALF_DRAW;}
        case OMNIPOTENT: Player.omnipotentMode = !Player.omnipotentMode;
        default:        break;
    }
    return NO_RESULT;
}

/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status
 * bars. Unless init is nonzero, this function will optimize drawing by only
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int init)
{
    // Draw game border first
    if(init) draw_border();

    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) // Iterate over columns of tiles
    {
        for (int j = -4; j <= 4; j++) // Iterate over one column of tiles
        {
            // Here, we have a given (i,j)

            // Compute the current map (x,y) of this tile
            int x = i + Player.x;
            int y = j + Player.y;

            // Compute the previous map (px, py) of this tile
            int px = i + Player.px;
            int py = j + Player.py;

            // Compute u,v coordinates for drawing
            int u = (i + 5) * 11 + 3;
            int v = (j + 4) * 11 + 15;

            // Figure out what to draw
            DrawFunc draw = NULL;

            if (init && i == 0 && j == 0) // Only draw the player on init
            {
                draw_player(u, v);
                continue;
            }
            //draws the player only if no occlusion occurs
            else if (i == 0 && j == 0 && !hidden)
            {
                draw_player(u, v);
            }
            else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) // Current (i,j) in the map
            {
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (init || curr_item != prev_item) // Only draw if they're different
                {
                    if (curr_item) // There's something here! Draw it
                    {
                        draw = curr_item->draw;
                    }
                    else // There used to be something, but now there isn't
                    {
                        draw = draw_nothing;
                    }
                }
            }
            else if (x == 0 && y >= 0 && y < map_height())            {draw = draw_wall;}
            else if (x >= 0 && y == 0 && x <= map_width())            {draw = draw_wall;}
            else if (y >= 0 && y <= map_height() && x == map_width()) {draw = draw_wall;}
            else if (x >= 0 && x <= map_width() && y == map_height()) {draw = draw_wall;}
            // If doing a full draw, but we're out of bounds, draw the walls.
            else if (init && active)  {draw = draw_wall;}
            else if (init && !active) {draw = draw_wall2;}
            // Actually draw the tile
            if (draw) draw(u, v);
        }
    }

    // Draw status bars
    if ( (Player.x!=Player.px) || (Player.py != Player.y)) {
        draw_upper_status(Player.x,Player.y);
    }
    draw_lower_status();
}

//transports zelda and her guards after she is found
void zelda() {
    //erases previous location of zelda and her guards
    map_erase(xZelda, yZelda);
            
    //randomly draws zelda as long as she would not destroy any necessary items to do so
    while(true) {
        xZelda = 3 + rand() % 45;
        yZelda = 3 + rand() % 45;
        if (get_here(xZelda, yZelda) == NULL) {
            add_zelda(xZelda, yZelda);
            break;
        }
    }
    onlyOnce1 = true;
}

//occurs when the player hits an enemy and dies
void died(char &life) {
    //decreases number of lives
    life = life - 1;
    
    //resets player to starting location
    Player.x = Player.y = 5;
    
    //Print death statement
    uLCD.cls();
    uLCD.textbackground_color(BLACK);
    uLCD.text_string    ("    YOU DIED!    ",1,5,FONT_7X8,0xFFFF00);
    if (lives != '0') {
        uLCD.text_string("Lives Remaining: ",0,7,FONT_7X8,0xFFFF00);
        uLCD.text_string(&life,             17,7,FONT_7X8,0xFFFF00);
        uLCD.text_string("   Try Again     ",1,9,FONT_7X8,0xFFFF00);
    }
    else {
        uLCD.text_string("    GAME OVER    ",1,7,FONT_7X8,0xFFFF00);
    }
    wait_ms(2000);
    uLCD.cls();
}

//Occurs when the player is moving upwards
int go_up() {
    //adds the wand/sonar to the maze map when next to the location it will be placed
    if(!active && !wand && abs(Player.x - 10) <= 2 && abs(Player.y - 9) <= 2) {
        add_wand(10,9);
        wand = true;
    }

    //sets the previos player position to the current player position
    Player.py = Player.y;
    
    //occurs when on the first map and zelda has not yet been moved
    //this adds and moves the guards
    if (active && jump == 0) move_guards(b);
    
    //occurs when the player is running into an enemy; it kills the player
    if (get_north(Player.x, Player.y) -> type == GUARD) {
        died(lives);
        return 0;
    }
    
    //occurs when an item is not walkable and omniscient mode is not on or conditions for the house
    else if (get_north(Player.x,Player.y) -> walkable == NULL && Player.omnipotentMode || (get_north(Player.x, Player.y) -> type == HOUSE_WALL && (get_here(Player.x, Player.y) == NULL || get_here(Player.x, Player.y) -> type == PLANT))) {
        hidden = false;
        return Player.py;
    }
    
    //more conditions for the house
    else if (Player.omnipotentMode && (get_north(Player.x, Player.y) -> type == ROOF_MIDDLE || get_north(Player.x, Player.y) -> type == ROOF_LEFT || get_north(Player.x, Player.y) -> type == ROOF_RIGHT)) {
        hidden = true;
        return Player.py;
    }
    
    //occurs when the player is able to move in the desired direction
    else {
        //hides player when entering the house
        if (get_here(Player.x, Player.y) -> type == DOOR) hidden = true;
        Player.y = Player.y - 1;
        return Player.y;
    }
}

//Occurs when the player is moving downwards
int go_down() {
    //adds the wand/sonar to the maze map when next to the location it will be placed
    if(!active && !wand && abs(Player.x - 10) <= 2 && abs(Player.y - 9) <= 2) {
        add_wand(10,9);
        wand = true;
    }
    
    //sets the previos player position to the current player position
    Player.py = Player.y;
    
    //occurs when on the first map and zelda has not yet been moved
    //this adds and moves the guards
    if (active && jump == 0) move_guards(b);
    
    //occurs when the player is running into an enemy; it kills the player
    if (get_south(Player.x, Player.y) -> type == GUARD) {
        died(lives);
        return 0;
    }
    
    //occurs when an item is not walkable and omniscient mode is not on or conditions for the house
    else if(get_south(Player.x,Player.y) -> walkable == NULL && Player.omnipotentMode || ((get_south(Player.x, Player.y) == NULL || get_south(Player.x, Player.y) -> type == PLANT) && get_here(Player.x, Player.y) -> type == HOUSE_WALL)) {
        return Player.py;
    }
    
    //more conditions for the house
    else if (Player.omnipotentMode && (get_south(Player.x, Player.y) -> type == ROOF_MIDDLE || get_south(Player.x, Player.y) -> type == ROOF_LEFT || get_south(Player.x, Player.y) -> type == ROOF_RIGHT)) {
        if (get_here(Player.x, Player.y) -> type == HOUSE_WALL) hidden = true;
        return Player.py;
    }
    
    //occurs when the player is able to move in the desired direction
    else{
        //occurs when the door is open and we are standing in the doorway
        //allows player to move out of the house
        if (get_here(Player.x, Player.y) -> type == DOOR && doorOpen) {
            hidden = false;
            doorOpen = false;
        }
        
        //occurs when wanting to exit but the door is not open
        //opens the door
        else if (get_here(Player.x, Player.y) -> type == DOOR) {
            hidden = false;
            doorOpen = true;
            return Player.py;
        }
        Player.y = Player.y + 1;
        return Player.y;
    }
}
//Occurs when the player is moving to the left
int go_left() {
    //adds the wand/sonar to the maze map when next to the location it will be placed
    if(!active && !wand && abs(Player.x - 10) <= 2 && abs(Player.y - 9) <= 2) {
        add_wand(10,9);
        wand = true;
    }
    
    if (get_here(Player.x - 2, Player.y - 1) -> type == WAND && onlyOnce) {
        add_wall2(Player.x - 1, Player.y);
        map_erase(Player.x - 2, Player.y);
        speech("That would be a ", "little..too easy");
        onlyOnce1 = true;
        onlyOnce = false;
    }
    //sets the previos player position to the current player position
    Player.px = Player.x;
    
    //occurs when on the first map and zelda has not yet been moved
    //this adds and moves the guards
    if (active && jump == 0) move_guards(b);
    
    //occurs when the player is running into an enemy; it kills the player
    if (get_west(Player.x, Player.y) -> type == GUARD) {
        died(lives);
        return 0;
    }
    
    //occurs when an item is not walkable and omniscient mode is not on or conditions for the house
    else if(get_west(Player.x,Player.y) -> walkable == NULL && Player.omnipotentMode) {
        return Player.px;
    }
    
    //more conditions for the house
    else if (Player.omnipotentMode && ((get_west(Player.x, Player.y) -> type == HOUSE_WALL && (get_here(Player.x, Player.y) == NULL || get_here(Player.x, Player.y) -> type == PLANT)) || (get_here(Player.x, Player.y) -> type == HOUSE_WALL && (get_west(Player.x, Player.y) == NULL || get_west(Player.x, Player.y) -> type == PLANT)) || get_west(Player.x, Player.y) -> type == ROOF_RIGHT)) {
        if (get_here(Player.x, Player.y) -> type == HOUSE_WALL) hidden = true;
        return Player.px;
    }
    
    //occurs when the player is able to move in the desired direction
    else{
        //hides player when entering the house
        if (get_here(Player.x, Player.y) -> type == DOOR) hidden = true;
        Player.x = Player.x - 1;
        return Player.x;
    }
}
//Occurs when the player is moving to the right
int go_right() {
    //adds the wand/sonar to the maze map when next to the location it will be placed
    if(!active && !wand && abs(Player.x - 10) <= 2 && abs(Player.y - 9) <= 2) {
        add_wand(10,9);
        wand = true;
    }
    
    //sets the previos player position to the current player position
    Player.px = Player.x;
    
    //occurs when on the first map and zelda has not yet been moved
    //this adds and moves the guards
    if (active && jump == 0) move_guards(b);
    
    //occurs when the player is running into an enemy; it kills the player
    if (get_east(Player.x, Player.y) -> type == GUARD) {
        died(lives);
        return 0;
    }
    
    //occurs when an item is not walkable and omniscient mode is not on or conditions for the house
    else if(get_east(Player.x,Player.y) -> walkable == NULL && Player.omnipotentMode) {
        return Player.px;
    }
    
    //more conditions for the house
    else if (Player.omnipotentMode && ((get_east(Player.x, Player.y) -> type == HOUSE_WALL && (get_here(Player.x, Player.y) == NULL || get_here(Player.x, Player.y) -> type == PLANT)) || (get_here(Player.x, Player.y) -> type == HOUSE_WALL && (get_east(Player.x, Player.y) == NULL || get_east(Player.x, Player.y) -> type == PLANT)) || get_east(Player.x, Player.y) -> type == ROOF_LEFT)) {
        if (get_here(Player.x, Player.y) -> type == HOUSE_WALL) hidden = true;
        return Player.px;
    }
    
    //occurs when the player is able to move in the desired direction
    else {
        //hides player when entering the house
        if (get_here(Player.x, Player.y) -> type == DOOR) hidden = true;
        Player.x = Player.x + 1;
        return Player.x;
    }
}

//deletes the moving guards; useful when changing the rotation
void delete_guards() {
        if (abs(Player.x - xZelda) <= 5 && abs(Player.y - yZelda) <= 5 && wand && active) {
            for (int i = -2; i <= 2; i++) {
            for (int j = -2; j <= 2; j++) {
                if (abs(i) > 1 || abs(j) > 1) map_erase(xZelda + j, yZelda + i);
            }
        }
        onlyOnce1 = true;
        }
}

//keeps track of the steps the player takes and moves the guards one rotation every three steps
void move_guards(int &b) {
    delete_guards();
    
    //places the new guards
    for (int i = -2; i <= 2; i++) {
        for (int j = -2; j <= 2; j++) {
            //occurs when the first rotation is valid
            if (((i + j) % 2 == 0) && (b % 6 >= 3) && !guardsDead[i + 2][j + 2] && (abs(i) > 1 || abs(j) > 1)) add_guard(xZelda + i, yZelda + j);
            
            //occurs when the second rotation is valid; places guards on bottom and right sides
            if (((i + j) % 2 == 1) && (b % 6 <  3) && !guardsDead[i + 2][j + 2] && (abs(i) > 1 || abs(j) > 1)) add_guard(xZelda + i, yZelda + j);
            
            //occurs when the second rotation is valid; places guards on top and left sides
            if (((i + j) % 2 == 1) && (b % 6 <  3) && !guardsDead[2 - i][2 - j] && (abs(i) > 1 || abs(j) > 1)) add_guard(xZelda - i, yZelda - j);

            //places immobile guards in the inner section
            if (b == 0 && abs(i) + abs(j) != 0 && !guardsDead[i + 2][j + 2] && abs(i) <= 1 && abs(j) <= 1)     add_guard(xZelda + i, yZelda + j);
        }
    }
    //increments step counter
    b++;
}

//returns the action based on the accelerometer and buttons
int get_action(GameInputs inputs)
{
    //used for timing for moving Zelda
    //placed here because this function always occurs
    uLCD.color(0xFFFF00);
    int timeRemaining;
    
    //35 seconds if tool is found, 20 seconds if not
    if (hasDistorter) timeRemaining = 35 - moveZelda.read();
    else              timeRemaining = 20 - moveZelda.read();
    if (jump == 1) {
        uLCD.textbackground_color(BLACK);
        uLCD.locate(1, 15);
        if (timeRemaining >= 10) uLCD.printf("Time Left: %d ", timeRemaining);
        if (timeRemaining < 10)  uLCD.printf("Time Left: %d ", timeRemaining);
    }
    if (timeRemaining <= 0) {
        zelda();
        moveZelda.start();
    }
    
    //read buttons first
    if(!inputs.b2){
        return OMNIPOTENT;
    }
    if(!inputs.b1){
        return ACTION_BUTTON;
    }
    //read accelerometer
    //inputs look weird because mbed is sideways
    if (inputs.ax > -0.3)       {return GO_UP;}
    else if (inputs.ax < -0.7) {return GO_DOWN;}
    if (inputs.ay < -0.3)      {return GO_RIGHT;}
    else if (inputs.ay > 0.3)  {return GO_LEFT;}
    else                       {return NO_ACTION;}
}