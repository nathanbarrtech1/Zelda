#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"
#include <time.h>

/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion. Note: using the similar
 * procedure you can init the secondary map(s).
 */
void init_main_map()
{
    //sets the active map to the main map
    set_active_map(0);
    
    //retrieves the main map
    Map* map = get_active_map();
    
    //sets the starting position
    Player.x = Player.y = 5;
    
    //populates the map with trees
    for(int i = map_width() + 3; i < map_area(); i += 39) {add_plant(i % map_width(), i / map_width());}

    //randomly places the wizard in a localized area near the player
    while(true) {
        xWizard = 1 + rand() % 20;
        yWizard = 1 + rand() % 20;

        if ((xWizard > 15 && yWizard > 5) || (xWizard > 5 && yWizard > 15)) {
            add_wizard(xWizard, yWizard);
            break;
        }
    }
    //places a house two blocks to the left of the wizard
    add_house(xWizard - 4,yWizard - 2);
    
    //shows the wizard has a quest
    add_exclamation(xWizard,yWizard - 1);

    //creates the boundaries of the map
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height(),   HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width(),    0,              VERTICAL,   map_height());
}

void init_main_map2()
{
    //sets the active map to the maze map
    set_active_map(1);
    
    //retrieves the maze map
    Map* map = get_active_map();
    
    //sets the starting position
    Player.x = Player.y = 1;
    
    //builds the maze
    for (int i = 0; i < 400; i++) {if (getMaze(i) != 0) add_wall2(i / 20, i % 20);}
}