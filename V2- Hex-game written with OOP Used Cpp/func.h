#ifndef _FUNC_H
#define _FUNC_H

#include <iostream> 
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
  
using namespace std; 

enum Situation {EMPTY,FILLED_X,FILLED_O,WINNER_X,WINNER_O};

class Hex {  
	public:
		Hex();//constructors that takes size 
		explicit Hex(int size1value);
		explicit Hex(int size1value,int size2value);
		//Big three is provided
		Hex(const Hex &cp);
		Hex & operator=(const Hex &cp);
		~Hex();				

		class Cell{	//cell class
			public:
				int get_situation() const;	//getter setter for situation
				void set_situation(int num);
				int x_coordinate,y_coordinate;
				
			private:
				int situation;	//if the cell is filled or empty
		};

		void playGame();//Gameplay									

		void start_table();	//starts table and control table
		int get_size() const;			
		int get_width() const;//gets width
		int get_height() const;//gets height
		int get_gametype() const;	//gets gametype
		const int get_total_move_for_this_class() const;	//Gets total move for this class
		void showsize() const;	//prints size

		int take_command();//takes user commands
		void play(int x_coordinate,int y_coordinate);	//plays game for user
		Cell play();//plays game for computer. This function have AI 

		const void save_game(string file_path) ;	//saves the game

		void load_game(string file_path);	//loads the game

		void operator--();	//For UNDO command
		void operator--(int);	//calls UNDO 2 times
								
		friend ostream & operator << (ostream &out, const Hex &out_table);	//prints table

		static int get_total_move();	//ALL total moves that made
		static void increase_total_move();		//increases total move for all the times

		const bool operator ==(const Hex & other_game) const;

	private:
		void who_is_next();
		void set_gametype();//takes variable from user and setts the gametype (it is overloaded function)
		void set_gametype(int num);//sets gametype
		inline int bigger_size(int end);	//increase charachter that who win
		const int is_legal(int x_coordinate,int y_coordinate) const;
		void add_move_log(int situation);
		void set_size();	//setts size1 		
		void set_size(int num);
		void reset_control_table();//resets contol table
		void reset_table();//resets table
		int is_size_legal() const;//checks that if move is legal
		void delete_log_array();	//deletes all log datas

		inline int is_game_ended();		//checks that if somebody won the game	
		int is_game_done_for_user1(int coordinate_x,int coordinate_y);	//checks for last move of user1 for if game end or not
		int is_game_done_for_user2(int coordinate_x,int coordinate_y);	//checks for last move of user2 for if game end or not

		friend ofstream & operator << (ofstream &out, const Hex &out_table);//reading from a file
		friend ifstream & operator >> (ifstream &in,Hex &in_table);	//writing in a file
		
		Cell **hexCells;	
		Cell **control_table;
		int **all_move_logs; //[i][j] -> i represents movements, j represent for 0: coordinate_x
							//j represent for 1: coordinate_o and  j represent for 2: Cell situation
		int size1,size2;	
		int x_counter,o_counter;
		int gametype;
		int flag1,flag2;
		int score1,score2;
		int last_x_move,last_y_move;

		static int move_count;
		
};

#endif