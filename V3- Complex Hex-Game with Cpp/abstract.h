#ifndef _ABSTRACT_H
#define _ABSTRACT_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <stdexcept>
#include <iterator>
#include <deque> 

using namespace std;

namespace abstractDuman{

enum Situation {EMPTY,FILLED_X,FILLED_O,WINNER_X,WINNER_O};

class Memoryerror: public exception{
	virtual const char* what() const throw(){
		return "EXEPTİON: Trying to reach position that you can not reach";
	}
};

class no_last_move_error: public exception{
	virtual const char* what() const throw(){
		return "EXEPTİON: There is no last move for this game";
	}
};

class AbstractHex{
	public:
		class Cell{	//cell class
			public:
				int get_situation() const;	//getter setter for situation
				void set_situation(int num);
				int x_coordinate,y_coordinate;
				
			private:
				int situation;	//if the cell is filled or empty
		};
		void playGame(); //GAMEPLAY SECTION THAT HAVE ALL FUNCTIONS THAT WE NEED
		bool setSize(int size1,int size2);
		void play();	//plays computerr
		void play(int x_coordinate,int y_coordinate); //plays user
		bool isEnd();
		Cell lastMove() throw (no_last_move_error);
		bool operator==(AbstractHex &rv); //if they are all hexcells same returns true
		int get_gametype() const;
		int get_size() const;
		int get_width() const;
		int get_height() const;
		const void print() const; //print game on terminal
		virtual Cell& operator()(int y,int x) throw (Memoryerror) = 0 ; //index operator that return cell
		virtual const Cell& operator()(int y,int x)const throw (Memoryerror)  = 0; //index operator that return cell

	protected:
		AbstractHex(int sizevalue=6);
		AbstractHex(const AbstractHex& rv) ;
     	AbstractHex& operator=(const AbstractHex& rv);
     	virtual ~AbstractHex() = default;
     	//you may think why i am doing this kind of things in protected
     	//because this class already has pure virtual function. So, i think that is allright to protect

		virtual void Allocator() = 0; //allocate memory virtually
		
		void readFromFile(string file_path); //load game
		const void writeToFile(string file_path) const; //save game
		void reset(); //reset hexcels and game
		int is_game_ended();
		int is_game_done_for_user1(int coordinate_x,int coordinate_y); //recursive function that controlls game ended or not for user 1
		int is_game_done_for_user2(int coordinate_x,int coordinate_y);
		const int numberofMoves() const;
		void reset_control_table(); //resets control table
		const int is_legal(int x_coordinate,int y_coordinate) const; //checks that if move is legal
		void bigger_size(int end); //if somebody wins then that charachter will be bigger size
		void who_is_next(); //reset counter
		void TakeSize(); //takes size from user
		void take_gametype(); //takes gametype from user
		void set_gametype(int num);
		int take_command(); //takes user commands from user
		const int is_size_legal() const; //Checking that if size legal
		

		int size1,size2;	
		int x_counter,o_counter;
		int gametype;
		int flag1,flag2;
		int last_x_move,last_y_move;

		vector<vector<Cell> > control_table;  //cell vektor but controller
};

}


#endif
