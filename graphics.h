#ifndef GRAPHICS_H
#define GRAPHICS_H


/**
 * Draws the player. This depends on the player state, so it is not a DrawFunc.
 */


/**
 * Takes a string image and draws it to the screen. The string is 121 characters
 * long, and represents an 11x11 tile in row-major ordering (across, then down,
 * like a regular multi-dimensional array). The available colors are:
 *      R = Red
 *      Y = Yellow
 *      G = Green
 *      D = Brown ("dirt")
 *      5 = Light grey (50%)
 *      3 = Dark grey (30%)
 *      Any other character is black
 * More colors can be easily added by following the pattern already given.
 */
int getMaze(int val);
/**
 * DrawFunc functions. 
 * These can be used as the MapItem draw functions.
 */


/**
 * Draw the upper status bar.
 */
void draw_upper_status(int x, int y);

/**
 * Draw the lower status bar.
 */ 
void draw_lower_status();

/**
 * Draw the border for the map.
 */
void draw_border();

void draw_player(int u, int v);
void draw_nothing(int u, int v);
void draw_wall(int u, int v);
void draw_wall2(int u, int v);
void draw_plant(int u, int v);
void draw_stairs(int u, int v);
void draw_wizard(int u, int v);
void draw_ladder(int u, int v);
void draw_ladder2(int u, int v);
void draw_zelda(int u, int v);
void draw_guard(int u, int v);
void draw_wand(int u, int v);
void draw_sword_animation_h(int u, int v, bool east);
void draw_sword_animation_v(int u, int v, bool south);
void draw_sword_animation_east(int u, int v);
void draw_sword_in_stone(int u, int v);
void draw_stone(int u, int v);
void draw_house_wall(int u, int v);
void draw_roof_left(int u, int v);
void draw_roof_middle(int u, int v);
void draw_roof_right(int u, int v);
void draw_door(int u, int v);
void draw_exclamation(int u, int v);
void draw_space_distorter(int u, int v);

#endif // GRAPHICS_H