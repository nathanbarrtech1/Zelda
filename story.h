#ifndef STORY_H
#define STORY_H

void actionButton();
void draw_game(int init);
int update_game(int action);
void move_guards(int &b);
void delete_guards() ;
int go_right();
int go_left();
int go_down();
int go_up();
void died(char &life);
void zelda();
int get_action(GameInputs inputs);

#endif