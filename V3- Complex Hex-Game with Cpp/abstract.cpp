#include "abstract.h"
#include "hexarray1D.h"
#include "hexvector.h"
#include "hexadapter.h"


namespace abstractDuman{

	AbstractHex::AbstractHex(int sizevalue){ //constructor
		size1=sizevalue;
		size2=sizevalue;
		x_counter=0;
		o_counter=0;
		gametype=0;
		flag1=0;
		flag2=0;
		last_x_move=0;
		last_y_move=0;
	}

	AbstractHex::AbstractHex(const AbstractHex& rv){	//copy constructor
		size1=rv.size1;
		size2=rv.size2;
		setSize(get_size(),get_size());
		x_counter=rv.x_counter;
		o_counter=rv.o_counter;
		gametype=rv.gametype;
		flag1=rv.flag1;
		flag2=rv.flag2;
		last_x_move=rv.last_x_move;
		last_y_move=rv.last_y_move;
		for(int i=0;i<get_size();i++){	//copying hexCells
			for(int j=0;j<get_size();j++){
				(*this)(i,j)=(rv)(i,j);
			}
		}
	}

	AbstractHex& AbstractHex::operator=(const AbstractHex& rv){	 //copy assignment operator overloading
		cout<<"Base Class Copy Assingment called\n";
		if(this==&rv){			//controlling adresses same or not
			return *this;
		}
		size1=rv.size1;
		size2=rv.size2;
		setSize(get_size(),get_size());
		x_counter=rv.x_counter;
		o_counter=rv.o_counter;
		gametype=rv.gametype;
		flag1=rv.flag1;
		flag2=rv.flag2;
		last_x_move=rv.last_x_move;
		last_y_move=rv.last_y_move;
		for(int i=0;i<get_size();i++){	//copying hexCells
			for(int j=0;j<get_size();j++){
				(*this)(i,j)=(rv)(i,j);
			}
		}
		return *this;
	}

	bool AbstractHex::operator==(AbstractHex &rv){ //if they are all hexcells same returns true
		if(get_size() == rv.get_size()){
			for(int i=0;i<get_size();i++){
				for(int j=0;j<get_size();j++){
					if((*this)(i,j).get_situation()!=(rv)(i,j).get_situation()){
						return false;
					}
				}
			}
			return true;
		}
		return false;
	}

	//if somebody wins then that charachter will be bigger size
	void AbstractHex::bigger_size(int end){
		for (int i = 0; i < get_size(); i++){
			for (int j = 0; j < get_size(); j++){
				if (control_table[i][j].get_situation()==1 && (*this)(i,j).get_situation()==end){
					(*this)(i,j).set_situation(end+2);
				}
			}
		}
	}

	//Checking that if size legal
	const int AbstractHex::is_size_legal() const{
		if (get_width()>5 && get_width()==get_height())
			return 1;

		return 0;
	}

	//checks that if move is legal
	const int AbstractHex::is_legal(int x_coordinate,int y_coordinate) const{
		if(x_coordinate>get_size() || y_coordinate>get_size() || y_coordinate<1 || x_coordinate <1 || (*this)(y_coordinate-1,x_coordinate-1).get_situation()!=EMPTY ){
			return 0;
		}
		return 1;
	}

	//GAMEPLAY SECTION THAT HAVE ALL FUNCTIONS THAT WE NEED
	void AbstractHex::playGame(){

		TakeSize();
		
		take_gametype();

		int end=is_game_ended();

		while(end==0){	//if is game ended or not
			print();

			if (x_counter==o_counter){	//player 1 moves
				cout<<"Player1:";
				if (take_command()!=0){
					end=is_game_ended();
					x_counter+=1;
				}
			}
			else if (x_counter>o_counter && get_gametype()==1){ //player2 moves
				cout<<"Player2:";
				if (take_command()!=0){
					end=is_game_ended();
					o_counter+=1;
				}
			}
			else{				//computer moves
				play();
				end=is_game_ended();
				o_counter+=1;
			}
		}

		if (end==1){
			cout<<"\nPLAYER 1 WON\n";
			bigger_size(end);									
		}
		else if(end==2){
			cout<<"\nPLAYER 2 WON\n";
			bigger_size(end);
		}
		else if(end==3){
			cout<<"\nCOMPUTER WON\n";
			bigger_size(end-1);
		}
		
		print();	
	}

	void AbstractHex::reset(){	//resets game 
		last_x_move=1;
		last_y_move=1;
		o_counter=0;
		x_counter=0;
		for(int i=0;i<get_size();i++){
			for(int j=0;j<get_size();j++){
				(*this)(i,j).set_situation(EMPTY);
			}
		}
		who_is_next();
	}

	//takes user commands
	int AbstractHex::take_command(){
		string selection1,selection2;
		char temp_char;
		int coordinate_x,coordinate_y;

		cin >>selection1;	//taking inputs

		if(selection1.compare("SAVE")==0 || selection1.compare("save")==0){	//saves the game
			cin >>selection2;
			writeToFile(selection2);
			return 0;																
		}
		else if(selection1.compare("LOAD")==0 || selection1.compare("load")==0){	//loads the game
			cin >>selection2;
			readFromFile(selection2);
			return 0;																
		}
		else if(selection1.compare("RESET")==0 || selection1.compare("reset")==0){
			reset();
		}
		else{	//taking input from string
			cin >>selection2;

			stringstream my_selection1(selection1);
			my_selection1>> temp_char;

			stringstream my_selection2(selection2);
			my_selection2>> coordinate_y;

			coordinate_x=temp_char-'A'+1;	//type casting char to int

			if (is_legal(coordinate_x,coordinate_y)==1){	//that means the move is legal
				play(coordinate_x,coordinate_y);
				
				return 1;
			}
			else{
				cout<<"\nInvalid value try again.\n\n";
				return 0;
			}
		}
	}

	AbstractHex::Cell AbstractHex::lastMove() throw (no_last_move_error){ //returns last move
		if(last_x_move==0){
			throw no_last_move_error();
		}
		return (*this)(last_y_move,last_x_move);
	}

	//Allocators contol table
	void AbstractHex::reset_control_table(){
		Cell temp_cell;

		vector<vector<Cell> > temp_2D_cell; 

		temp_cell.set_situation(EMPTY);
		for (int i = 0; i < get_size(); i++) { 
			vector<Cell> inner_vec; 
			for (int j = 0; j < get_size(); j++) { 
				inner_vec.push_back(temp_cell);
			} 
			temp_2D_cell.push_back(inner_vec); 
		}

		control_table=temp_2D_cell;

	}

	//sets size and allocate memory with Allocator function
	bool AbstractHex::setSize(int size1,int size2){
		if(size1==size2 && size1>5){
			this->size1=size1;
			this->size2=size2;
			Allocator();
			return true;
		}
		return false;
		
	}

	void AbstractHex::TakeSize(){
		//Takes size from user
		char temp_char;
		int size1value,size2value;
		bool flag=true;

		while(flag){

			cout<<"\nPlease Enter the size of the Hex game:(like 6x6): "; 
			cin>>size1value>>temp_char>>size2value;		//Taking size entry
			
			if(setSize(size1value,size2value)){ //Controlling that if taken size is apropriate
				flag=false;
			}
			else{
				cout<<endl<< "Invalid size, please re enter the size."<<endl;
			}
		}
	}

	//takes variable from user and setts the gametype (it is overloaded function)
	void AbstractHex::take_gametype(){
		bool flag=true;
		int selection;
		while(flag){

			cout<<endl<<endl;	
			cout<<"1-) Player vs Player (two player)."<< endl;
			cout<<"2-) Player vs Computer (single player)."<<endl<<endl;

			cout<<"Please Select a game type from the menu: ";

			cin>>selection;

			switch(selection){
				case 1: set_gametype(1);	//player vs player
						flag=false;
					break;
				case 2: set_gametype(2);	//player vs computer
						flag=false;
					break;
				default: cout<<"\nInvalid value, try again\n";
			}
		}
	} 

	//reads and sett table 
	void AbstractHex::readFromFile(string file_path){
		int x,old_total=numberofMoves();//Old total

		ifstream load_file;
		load_file.open(file_path);

		if (load_file.is_open()){
			if(load_file >> x){		//loads size
				setSize(x,x);
			}
			if(load_file >> x){		//loads game type
				set_gametype(x);
			}

			for(int i=0;i<get_size();i++){	//loads table
				for(int j=0;j<get_size();j++){
					if(load_file >> x){
						(*this)(i,j).set_situation(x);
					}
				}
			}

			who_is_next();

			/*move_count-=old_total;	//setting move counter
			move_count+=numberofMoves();*/

			cout <<"\n\n\nGame Is Loaded\n\n";
			load_file.close();
		}
		else{
			cerr<<"\n\nThere is error ocured oppening file\n\n";
			exit(1);
		}
	}

	const void AbstractHex::writeToFile(string file_path)const{
		//saves the game; board size, game type, table
		ofstream save_file ;

		save_file.open(file_path);
		if (save_file.is_open()){

			save_file <<get_size()<<endl;	//prints size
			save_file <<get_gametype()<<endl;	//prints game type

			for(int i=0; i< get_size();i++){	//saves table
				for(int j=0;j< get_size();j++){
					save_file<< (*this)(i,j).get_situation();
					save_file<<endl;
				}
			}

			cout <<"\n\n\nGame Is Saved\n\n";
			save_file.close();

		}
		else{
			cerr<<"\n\nThere is error ocured oppening file\n\n";
			exit(1);
		}
	}

	const void AbstractHex::print() const {
		//prints table in an aproppriate way, A,B,C/1,2,3/X,O/x,o/. etc.
		int i,j,space_count;
		char mychar;
		cout<<" ";
		for(i=0;i<get_size();i++){
			mychar=i+'A';
			cout<<" "<<mychar;
		}
		cout<<endl;
		for(i=0;i<get_size();i++){
			cout<<i+1;
			if(i<9){
				for(space_count=0;i>space_count;space_count++)
					cout<<" ";
			}
			else{
				for(space_count=1;i>space_count;space_count++)
					cout<<" ";
			}

			for(j=0;j<get_size();j++){
				if((*this)(i,j).get_situation()==EMPTY)
					cout<<" "<<".";	//No move
				if((*this)(i,j).get_situation()==FILLED_X)
					cout<<" "<<"x";	
				if((*this)(i,j).get_situation()==FILLED_O)
					cout<<" "<<"o";
				if((*this)(i,j).get_situation()==WINNER_X)	//Uppercase X
					cout<<" "<<"X";
				if((*this)(i,j).get_situation()==WINNER_O)	//Uppercase O
					cout<<" "<<"O";
			}
			cout<<endl;
		}
	}

	//gets situation of cell
	int AbstractHex::Cell::get_situation()const{
		return situation;
	}
	//setts situation of cell
	void AbstractHex::Cell::set_situation(int num){
		situation=num;
	}
	//gets size
	int AbstractHex::get_size() const{
		return size1;
	}
	//gets total move just for that class
	const int AbstractHex::numberofMoves() const{
		return x_counter+o_counter;
	}
	//gets gametype, if its 1 that means player vs player
	int AbstractHex::get_gametype() const{
		return gametype;
	}
	//sets gametype
	void AbstractHex::set_gametype(int num){
		gametype=num;
	}

	//gets width
	int AbstractHex::get_width() const{
		return size2;
	}
	//gets height
	int AbstractHex::get_height() const{
		return size1;
	}

	//says that who should play
	void AbstractHex::who_is_next(){	
		x_counter=0;
		o_counter=0;

		for (int i = 0; i < get_size() ; i++){
			for (int j = 0; j < get_size(); j++){
				if ((*this)(i,j).get_situation()==FILLED_X){
					x_counter+=1;
				}
				else if((*this)(i,j).get_situation()==FILLED_O){
					o_counter+=1;
				}
			}
		}
	}

	//returns if the game end true
	//it seems like they are same but they are not
	bool AbstractHex::isEnd(){
		if(is_game_ended()==0){
			return false;
		}
		return true;
	}

	//checks that if somebody won the game
	int AbstractHex::is_game_ended(){
		flag1=0;
		flag2=0;

		if(x_counter==o_counter){	//Controlling that if time for player 1 or not			
			reset_control_table();
			//Allocatorting control table that controlls if game end
			//then controlling that if actual game is done
			is_game_done_for_user1(last_x_move,last_y_move);

			if(flag1==1 && flag2==1)	//if two flag equal 1, then game ended
				return 1;
		}
		else if(x_counter>o_counter){
			reset_control_table();
			//Allocatorting control table that controlls if game end
			//then controlling that if actual game is done
			is_game_done_for_user2(last_x_move,last_y_move);

			if(flag1==1 && flag2==1 && get_gametype()==1)	//if two flag equal 1, then game ended
				return 2;
			else if(flag1==1 && flag2==1)
				return 3;
		}
		return 0;

	}

	//checks for last move of user1 for if game end or not
	int AbstractHex::is_game_done_for_user1(int coordinate_x,int coordinate_y){

		control_table[coordinate_y-1][coordinate_x-1].set_situation(1);

		for(int i=1;i<=get_size();i++){	//if we go top or lowest place
			if(coordinate_y == i && coordinate_x==1)
				flag1=1;
			if(coordinate_y == i && coordinate_x==get_size())
				flag2=1;
		}
		//Moving around recursively 
		if(coordinate_y-2>=0 && control_table[coordinate_y - 2][coordinate_x-1].get_situation() == 0 && (*this)(coordinate_y-2,coordinate_x-1).get_situation()==FILLED_X)
			is_game_done_for_user1(coordinate_x,coordinate_y-1);
		if(coordinate_y-2>=0 && coordinate_x <get_size() && control_table[coordinate_y-2][coordinate_x].get_situation()==0 && (*this)(coordinate_y-2,coordinate_x).get_situation()==FILLED_X)
			is_game_done_for_user1(coordinate_x+1,coordinate_y-1);
		if(coordinate_x < get_size() && control_table[coordinate_y-1][coordinate_x].get_situation()==0 && (*this)(coordinate_y-1,coordinate_x).get_situation()==FILLED_X)
			is_game_done_for_user1(coordinate_x+1,coordinate_y);
		if(coordinate_y < get_size() && control_table[coordinate_y][coordinate_x-1].get_situation()==0 && (*this)(coordinate_y,coordinate_x-1).get_situation()==FILLED_X)
			is_game_done_for_user1(coordinate_x,coordinate_y+1);
		if(coordinate_y < get_size() && coordinate_x- 2 >= 0 && control_table[coordinate_y][coordinate_x-2].get_situation()==0 && (*this)(coordinate_y,coordinate_x-2).get_situation()==FILLED_X)
			is_game_done_for_user1(coordinate_x-1,coordinate_y+1);
		if(coordinate_x-2 >= 0 && control_table[coordinate_y - 1][coordinate_x -2].get_situation()==0 && (*this)(coordinate_y-1,coordinate_x-2).get_situation()==FILLED_X)
			is_game_done_for_user1(coordinate_x-1,coordinate_y);
		

		return 0;
	}
	//checks for last move of user2 for if game end or not
	int AbstractHex::is_game_done_for_user2(int coordinate_x,int coordinate_y){
		control_table[coordinate_y-1][coordinate_x-1].set_situation(1);

		for(int i=1;i<=get_size();i++){	//if we go most right or most left
			if(coordinate_y == 1 && coordinate_x==i)
				flag1=1;
			if(coordinate_y == get_size() && coordinate_x==i)
				flag2=1;
		}
		//Moving around recursively 
		if(coordinate_y-2>=0 && control_table[coordinate_y - 2][coordinate_x-1].get_situation() == 0 && (*this)(coordinate_y-2,coordinate_x-1).get_situation()==FILLED_O)
			is_game_done_for_user2(coordinate_x,coordinate_y-1);
		if(coordinate_y-2>=0 && coordinate_x <get_size() && control_table[coordinate_y-2][coordinate_x].get_situation()==0 && (*this)(coordinate_y-2,coordinate_x).get_situation()==FILLED_O)
			is_game_done_for_user2(coordinate_x+1,coordinate_y-1);
		if(coordinate_x < get_size() && control_table[coordinate_y-1][coordinate_x].get_situation()==0 && (*this)(coordinate_y-1,coordinate_x).get_situation()==FILLED_O)
			is_game_done_for_user2(coordinate_x+1,coordinate_y);
		if(coordinate_y < get_size() && control_table[coordinate_y][coordinate_x-1].get_situation()==0 && (*this)(coordinate_y,coordinate_x-1).get_situation()==FILLED_O)
			is_game_done_for_user2(coordinate_x,coordinate_y+1);
		if(coordinate_y < get_size() && coordinate_x- 2 >= 0 && control_table[coordinate_y][coordinate_x-2].get_situation()==0 && (*this)(coordinate_y,coordinate_x-2).get_situation()==FILLED_O)
			is_game_done_for_user2(coordinate_x-1,coordinate_y+1);
		if(coordinate_x-2 >= 0 && control_table[coordinate_y - 1][coordinate_x -2].get_situation()==0 && (*this)(coordinate_y-1,coordinate_x-2).get_situation()==FILLED_O)
			is_game_done_for_user2(coordinate_x-1,coordinate_y);
	}

	//plays game for user
	void AbstractHex::play(int x_coordinate,int y_coordinate){
		if (is_legal(x_coordinate,y_coordinate)==1){
			if (x_counter==o_counter){
				(*this)(y_coordinate-1,x_coordinate-1).set_situation(FILLED_X);	//user1
			}
			else{
				(*this)(y_coordinate-1,x_coordinate-1).set_situation(FILLED_O);	//user2
			}
			last_x_move=x_coordinate;
			last_y_move=y_coordinate;
		}

		//increase_total_move();
	}
	//plays game for computer. This function have AI 
	void AbstractHex::play(){
		char temp_char;
		int middle;
		int old_x,old_y;
		int max_x=0,max_y=0,min_x=get_size(),min_y=get_size();
		int user_max_x=0,user_max_y=0,user_min_x=get_size(),user_min_y=get_size();

		if(get_size()%2==1)	//finds middle point
			middle = ((get_size() -1)/2)+1;
		else
			middle=get_size()/2;

		if((*this)(middle-1,middle-1).get_situation()==EMPTY){ //Makes the first move to middle of the board
			old_x=middle;
			old_y=middle;
		}
		else if((*this)(middle-1,middle-1).get_situation()==FILLED_X && (*this)(middle-1,middle).get_situation()==EMPTY){
			//in case of middle of table is filled
			old_x=middle+1;
			old_y=middle;
		}
		else{	//All other moves except for first one

			for(int i=0;i<get_size();i++){	//finds maximum point of computer
				for(int j=0;j<get_size();j++){
					if((*this)(i,j).get_situation()==FILLED_O && i>max_y ){
						max_y=i;
						max_x=j;
					}
					if((*this)(i,j).get_situation()==FILLED_O && i<min_y ){
						min_y=i;
						min_x=j;
					}
				}
			}

			for(int i=0;i<get_size();i++){	//finds maximum point of user
				for(int j=0;j<get_size();j++){
					if((*this)(i,j).get_situation()==FILLED_X && i>user_max_y ){
						user_max_y=i;
						user_max_x=j;
					}
					if((*this)(i,j).get_situation()==FILLED_X && i<user_min_y ){
						user_min_y=i;
						user_min_x=j;
					}
				}
			}

			max_y+=1; max_x+=1;	//increasing values so that I can use them easily
			min_y+=1; min_x+=1;
			user_max_y+=1; user_max_x+=1;
			user_min_y+=1; user_min_x+=1;
			
			
			if(user_min_y<min_y){	//Makes move looking for users moves
				old_y=min_y; 	//go to min point
				old_x=min_x;
					
					//Checking around of where we are
					if(is_legal(old_x,old_y-1)){
						old_y-=1;

					}
					else if(is_legal(old_x+1,old_y-1)){
						old_x+=1;
						old_y-=1;
					}
					else if(is_legal(old_x-1,old_y)){
						old_x-=1;
					}
					else if(is_legal(old_x+1,old_y)){
						old_x+=1;
					}
					else if(is_legal(old_x-1,old_y+1)){
						old_x-=1;
						old_y+=1;
					}
					else if(is_legal(old_x,old_y+1)){
						old_y+=1;
					}
					else{
						old_y=max_y;	//go to max point
						old_x=max_x;
						//Checking around of where we are
						if(is_legal(old_x-1,old_y+1)){
							old_y+=1;
							old_x-=1;
						}
						else if(is_legal(old_x,old_y+1)){
							old_y+=1;
						}
						
					}

			}
			else{
				old_y=max_y;	//go to max point
				old_x=max_x;
				
					//Checking around of where we are
					if(is_legal(old_x-1,old_y+1)){
						old_y+=1;
						old_x-=1;
					}
					else if( is_legal(old_x,old_y+1)){
						old_y+=1;
					}
					else if(is_legal(old_x+1,old_y)){
						old_x+=1;
					}
					else if(is_legal(old_x-1,old_y)){
						old_x-=1;
					}
					else if( is_legal(old_x,old_y-1)){
						old_y-=1;
					}
					else if( is_legal(old_x+1,old_y-1)){
						old_y-=1;
						old_x+=1;
					}
					else{
						old_y=min_y;	//go to min point
						old_x=min_x;
						//Checking around of where we are
						if(is_legal(old_x,old_y-1)){
							old_y-=1;
						}
						else if(is_legal(old_x+1,old_y-1)){
							old_x+=1;
							old_y-=1;
						}
					}
			}
		}

		last_x_move=old_x;
		last_y_move=old_y;

		(*this)(old_y-1,old_x-1).set_situation(FILLED_O);	//making a move
		temp_char=old_x+'A'-1;
		cout<<"Computer put 'o' at "<<temp_char<<" "<<old_y<<" position \n\n";

		//increase_total_move();

	}

}
