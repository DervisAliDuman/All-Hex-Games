#ifndef _FUNC_H
#define _FUNC_H

#include <iostream>
using namespace std;

//Initializing part of functions

//starts table
void start_table(int table[][12],int border);
//prints table
void print_table(int table[][12],int border);
//resets control table
void reset_control_table(int control_table[][12]);
//controlls that if the move is legal
int is_legal(int table[][12],int coordinate_x,int coordinate_y,int border);
//controlls if player 1 (x) won the game
int is_game_done_for_user1(int table[][12],int control_table[][12], int coordinate_x,int coordinate_y,int border,int *flag1,int *flag2);
//controlls if player 2 (o) won the game
int is_game_done_for_user2(int table[][12],int control_table[][12], int coordinate_x,int coordinate_y,int border,int *flag1,int *flag2);
//there is danger zones that occures when we do bridge using AI
int is_there_danger(int table[][12],int danger_zone[][12],int *old_x,int *old_y,int border);
//Fills last blocks
int fill_empty_zones(int danger_zone[][12],int *old_x,int *old_y,int border);
//Mind of computer
void AI(int table[][12],int danger_zone[][12], int *old_x,int *old_y,int border);
//they are menues
int player_vs_player_menu(int border);
int player_vs_computer_menu(int border);
int game_mode_menu(int border);

#endif
