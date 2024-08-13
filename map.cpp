#include "map.h"

#include "globals.h"
#include "graphics.h"

/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 */
struct Map {
    HashTable* items;
    int w, h;
};

/**
 * Storage area for the maps.
 * This is a global variable, but can only be access from this file because it
 * is static.
 */
static Map map;
static Map map2;
static int active_map;

/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
static unsigned XY_KEY(int X, int Y) {
    unsigned int key = map_width()*Y + X;
    return key;
    // TODO: Fix me!
}

/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned map_hash(unsigned key)
{
    // TODO: Fix me!
        return key % 50;
}

void maps_init()
{
    map.items = createHashTable(map_hash, 50);
    map.w = 51;
    map.h = 51;
}

void maps_init2()
{
    map2.items = createHashTable(map_hash, 50);
    map2.w = 20;
    map2.h = 20;
}

Map* get_active_map()
{
    if (active_map == 0) return &map;
    if (active_map == 1) return &map2;
    return NULL;
}

void set_active_map(int m)
{
    pc.printf("starting to set\n");
    active_map = m;
    pc.printf("setting was successful. Map: %dn", m);
}

void print_map()
{
    // As you add more types, you'll need to add more items to this array.
    char lookup[] = {'W', 'P', 'G', 'V', 'Z', 'E', 'L'};
    for(int y = 0; y < map_height(); y++)
    {
        for (int x = 0; x < map_width(); x++)
        {
            MapItem* item = get_here(x,y);
            if (item) pc.printf("%c", lookup[item->type]);
            else pc.printf(" ");
        }
        pc.printf("\r\n");
    }
}

int map_width()
{
    return get_active_map()->w;
}

int map_height()
{
    return get_active_map()->h;
}

int map_area()
{
    return get_active_map()->w * get_active_map()->h;
}

MapItem* get_north(int x, int y)
{
    // (0,0) is top left corner
    y = y - 1;
    unsigned int key = XY_KEY(x,y);
    return (MapItem*)getItem(get_active_map()->items,key);
}

MapItem* get_south(int x, int y)
{
    y = y + 1;
    unsigned int key = XY_KEY(x,y);
    return (MapItem*)getItem(get_active_map()->items,key);
}

MapItem* get_east(int x, int y)
{
    x = x + 1;
    unsigned int key = XY_KEY(x,y);
    return (MapItem*)getItem(get_active_map()->items,key);
}

MapItem* get_west(int x, int y)
{
    x = x - 1;
    unsigned int key = XY_KEY(x,y);
    return (MapItem*)getItem(get_active_map()->items,key);
}

MapItem* get_here(int x, int y)
{
    unsigned int key = XY_KEY(x,y);
    return (MapItem*)getItem(get_active_map()->items,key);
}


void map_erase(int x, int y)
{
    unsigned int key = XY_KEY(x,y);
    if(get_here(x,y) != NULL){
        deleteItem(get_active_map()->items,key);
    }
    draw_nothing(x, y);
}

void destroy() {destroyHashTable(get_active_map()->items);}

void add_wall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall;
        w1->walkable = false;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}

void add_wall2(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = WALL2;
    w1->draw = draw_wall2;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_plant(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLANT;
    w1->draw = draw_plant;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_wizard(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = WIZARD;
    w1->draw = (DrawFunc) draw_wizard;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_ladder(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = LADDER;
    w1->draw = (DrawFunc) draw_ladder;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_ladder2(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = LADDER2;
    w1->draw = (DrawFunc) draw_ladder2;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_zelda(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = ZELDA;
    w1->draw = (DrawFunc) draw_zelda;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_guard(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = GUARD;
    w1->draw = (DrawFunc) draw_guard;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_wand(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = WAND;
    w1->draw = (DrawFunc) draw_wand;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_sword_in_stone(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = SWORD_IN_STONE;
    w1->draw = (DrawFunc) draw_sword_in_stone;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_stone(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = STONE;
    w1->draw = (DrawFunc) draw_stone;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_exclamation(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = EXCLAMATION;
    w1->draw = (DrawFunc) draw_exclamation;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_space_distorter(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = SPACE_DISTORTER;
    w1->draw = (DrawFunc) draw_space_distorter;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_house(int x, int y)
{
    map_erase(x - 1, y);          /*HOUSE              HOUSE                   HOUSE*/              map_erase(x + 3, y);
    add_plant(x - 1, y + 1);      /*HOUSE              HOUSE                   HOUSE*/              add_plant(x + 3, y + 1);
    add_plant(x - 1, y + 2);      /*HOUSE              HOUSE                   HOUSE*/              add_plant(x + 3, y + 2);
    map_erase(x - 1, y + 3); add_plant(x, y + 3); map_erase(x + 1, y + 3); add_plant(x + 2, y + 3); map_erase(x + 3, y + 3);
    
    
    
    
    
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = ROOF_LEFT;
    w1->draw = (DrawFunc) draw_roof_left;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
    
    MapItem* w2 = (MapItem*) malloc(sizeof(MapItem));
    w2->type = ROOF_MIDDLE;
    w2->draw = (DrawFunc) draw_roof_middle;
    w2->walkable = true;
    w2->data = NULL;
    void* val2 = insertItem(get_active_map()->items, XY_KEY(x + 1, y), w2);
    if (val2) free(val2); // If something is already there, free it
    
    MapItem* w3 = (MapItem*) malloc(sizeof(MapItem));
    w3->type = ROOF_RIGHT;
    w3->draw = (DrawFunc) draw_roof_right;
    w3->walkable = true;
    w3->data = NULL;
    void* val3 = insertItem(get_active_map()->items, XY_KEY(x + 2, y), w3);
    if (val3) free(val3); // If something is already there, free it
    
    MapItem* w4 = (MapItem*) malloc(sizeof(MapItem));
    w4->type = DOOR;
    w4->draw = (DrawFunc) draw_door;
    w4->walkable = true;
    w4->data = NULL;
    void* val4 = insertItem(get_active_map()->items, XY_KEY(x + 1, y + 2), w4);
    if (val4) free(val4); // If something is already there, free it
    
    MapItem* w5 = (MapItem*) malloc(sizeof(MapItem));
    w5->type = HOUSE_WALL;
    w5->draw = (DrawFunc) draw_house_wall;
    w5->walkable = true;
    w5->data = NULL;
    void* val5 = insertItem(get_active_map()->items, XY_KEY(x, y + 1), w5);
    if (val5) free(val5); // If something is already there, free it
    
    MapItem* w6 = (MapItem*) malloc(sizeof(MapItem));
    w6->type = HOUSE_WALL;
    w6->draw = (DrawFunc) draw_house_wall;
    w6->walkable = true;
    w6->data = NULL;
    void* val6 = insertItem(get_active_map()->items, XY_KEY(x, y + 2), w6);
    if (val6) free(val6); // If something is already there, free it
    
    MapItem* w7 = (MapItem*) malloc(sizeof(MapItem));
    w7->type = HOUSE_WALL;
    w7->draw = (DrawFunc) draw_house_wall;
    w7->walkable = true;
    w7->data = NULL;
    void* val7 = insertItem(get_active_map()->items, XY_KEY(x + 1, y + 1), w7);
    if (val7) free(val7); // If something is already there, free it
    
    MapItem* w8 = (MapItem*) malloc(sizeof(MapItem));
    w8->type = HOUSE_WALL;
    w8->draw = (DrawFunc) draw_house_wall;
    w8->walkable = true;
    w8->data = NULL;
    void* val8 = insertItem(get_active_map()->items, XY_KEY(x + 2, y + 1), w8);
    if (val8) free(val8); // If something is already there, free it
    
    MapItem* w9 = (MapItem*) malloc(sizeof(MapItem));
    w9->type = HOUSE_WALL;
    w9->draw = (DrawFunc) draw_house_wall;
    w9->walkable = true;
    w9->data = NULL;
    void* val9 = insertItem(get_active_map()->items, XY_KEY(x + 2, y + 2), w9);
    if (val9) free(val9); // If something is already there, free it
    /*for (int i = 1; i <= 2; i++) {
        for (int j = 0; j <= 2; j++) {
            if (i != 1 && j != 2) {
                MapItem* w5 = (MapItem*) malloc(sizeof(MapItem));
                w5->type = HOUSE_WALL;
                w5->draw = (DrawFunc) draw_house_wall;
                w5->walkable = true;
                w5->data = NULL;
                void* val5 = insertItem(get_active_map()->items, XY_KEY(x + i, y + j), w5);
                if (val5) free(val5); // If something is already there, free it
            }
        }
    }*/
}