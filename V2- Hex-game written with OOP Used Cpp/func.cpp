#include "func.h"

int Hex:: move_count=0;

//Constructors that sets size
Hex::Hex():size1(6),size2(6){/*intentionally empty*/};
Hex::Hex(int size1value):size1(size1value),size2(size1value){/*intentionally empty*/};
Hex::Hex(int size1value,int size2value):size1(size1value),size2(size2value){/*intentionally empty*/};
Hex::Hex(const Hex &cp){	//copy constructor
	last_x_move=cp.last_x_move;
	last_y_move=cp.last_y_move;
	x_counter=cp.x_counter;
	o_counter=cp.o_counter;
	set_size(cp.get_size());
	set_gametype(cp.get_gametype());
	flag1=cp.flag1;
	flag2=cp.flag2;

};
Hex & Hex::operator=(const Hex &cp){	//for provided big three, overloaded asignment operator
	if(this==&cp){
		return *this;
	}
	last_x_move=cp.last_x_move;
	last_y_move=cp.last_y_move;
	x_counter=cp.x_counter;
	o_counter=cp.o_counter;
	set_size(cp.get_size());
	set_gametype(cp.get_gametype());
	flag1=cp.flag1;
	flag2=cp.flag2;

	return *this;
}
Hex::~Hex(){	//Destructor
	for(int i=0; i< get_size(); i++){
		delete []hexCells[i];
		delete []control_table[i];
	}
	delete []control_table;
	delete []hexCells;
	control_table=nullptr;
	hexCells=nullptr;
};

const bool Hex::operator ==(const Hex & other_game) const{
	//Compares two games; if first game is bigger return true, else return false
	if (get_total_move_for_this_class()>other_game.get_total_move_for_this_class())
		return true;
	return false ;
}


void Hex::operator--(){
	//UNDO operation
	int count=get_total_move_for_this_class();

	if (count>0){	//creating new array that have new capacity
		int **temp=new int*[count-1];

		for (int i = 0; i < count-1; i++){
			temp[i]=new int[3];
		}

		for(int i=0;i<count-1;i++){		//loading old datas
			for(int j=0;j<3;j++){
				temp[i][j] = all_move_logs[i][j];
				
				if (j==0){
					last_x_move= all_move_logs[i][j];
				}
				else if (j==1){
					last_y_move=all_move_logs[i][j];
				}
				
			}
		}

		hexCells[all_move_logs[count-1][1]-1][all_move_logs[count-1][0]-1].set_situation(EMPTY);
		//deleting last move
		
		if (count!=1){	//deleting old datas
			for(int i=0; i< count; i++){
				delete []all_move_logs[i];
			}
			delete []all_move_logs;
		}
		all_move_logs=nullptr;
		
		x_counter=0;
		o_counter=0;

		who_is_next();	//resetting counter

		all_move_logs=temp;
	}
}

void Hex::operator--(int){ //two times UNDO
	--*this;
	--*this;
}

//loads the game table,size, gametype and starts the game
void Hex::load_game(string file_path="SAVE.TXT"){
	int x,old_total=get_total_move_for_this_class();//Old total
	
	ifstream load_file;
	load_file.open(file_path);

	if (load_file.is_open()){

		delete_log_array();	//reseting my datas
		reset_table();
		
		x_counter=0;
		o_counter=0;

		load_file>> *this;	//overloaded stream

		for(int i=0;i<get_total_move_for_this_class();i++){ 	//setting loaded moves
			hexCells[all_move_logs[i][1]-1][all_move_logs[i][0]-1].set_situation(all_move_logs[i][2]);
		}

		who_is_next();	//setting counter

		move_count-=old_total;	//setting move counter
		move_count+=get_total_move_for_this_class();

		cout <<"\n\n\nGame Is Loaded\n\n";
		load_file.close();
	}
	else{
		cerr<<"\n\nThere is error ocured oppening file\n\n";
		exit(1);
	}
}

ifstream & operator >> (ifstream &in,Hex &in_table){
	int x,a;

	in>>x;
	in_table.set_size(x);	//size
	in>>x;
	in_table.set_gametype(x);	//gametype
	in>>a;	// a is move count

	in_table.start_table();

	for(int i=0; i<a;i++){
		for(int j=0; j<3; j++){		
			if(in>>x){

				if(j==0){
					in_table.last_x_move=x;
				}
				else if(j==1){
					in_table.last_y_move=x;
				}
				else{
					in_table.o_counter=i+1;
					
					in_table.add_move_log(x);	//loading move logs
				}
			}
		}
	}

	return in;

}

//resets contol table
void Hex::reset_control_table(){
	for (int i = 0; i < get_size(); i++){
		for (int j = 0; j < get_size(); j++){
			control_table[i][j].set_situation(EMPTY);
		}
	}
}

//deletes log array
void Hex::delete_log_array(){
	if (get_total_move_for_this_class()!=0){
		for (int i = 0; i < get_total_move_for_this_class(); i++){
			delete []all_move_logs[i];
		}
		delete []all_move_logs;
	}
	all_move_logs=nullptr;

}

//resets table
void Hex::reset_table(){
	for (int i = 0; i < get_size(); i++){
		delete [] hexCells[i];
		delete [] control_table[i];
	}
	delete [] hexCells;
	delete [] control_table;
	hexCells=nullptr;
	control_table=nullptr;
}


//saves the game table,size, gametype
const void Hex::save_game(string file_path="SAVE.TXT"){
	//saves the game; board size, game type, table
	ofstream save_file ;

	save_file.open((file_path));
	if (save_file.is_open()){

		save_file << *this;	//overloaded stream 

		cout <<"\n\n\nGame Is Saved\n\n";
		save_file.close();

	}
	else{
		cerr<<"\n\nThere is error ocured oppening file\n\n";
		exit(1);
	}
}

ofstream & operator << (ofstream &out, const Hex &out_table){
	out << out_table.get_size()<<endl; 		//writing size
    out << out_table.get_gametype()<<endl;	//writing gametype
    out << out_table.get_total_move_for_this_class()<<endl;	//writing total move
    
    for(int i=0; i< out_table.get_total_move_for_this_class();i++){	//saves table
		for(int j=0;j< 3;j++){
			out << out_table.all_move_logs[i][j]<<" ";
		}
		out<<endl;
	}

    return out; 
}

void Hex::add_move_log(int _situation){
	//	Add's last move to log array
	int count=get_total_move_for_this_class();

	int **temp=new int*[count];	//new size

	for (int i = 0; i < count; i++){	
		temp[i]=new int[3];
	}

	for(int i=0;i<count-1;i++){		//copy old datas
		for(int j=0;j<3;j++){
			temp[i][j] = all_move_logs[i][j];
		}
	}

	temp[count-1][0]=last_x_move;	//add new log
	temp[count-1][1]=last_y_move;
	temp[count-1][2]=_situation;
	
	if (count!=1){	//delete old log datas
		for(int i=0; i< count-1; i++){
			delete []all_move_logs[i];
		}
		delete []all_move_logs;
	}
	all_move_logs=nullptr;

	all_move_logs=temp;
}

void Hex::start_table(){	
	//starts table
	x_counter=0;
	o_counter=0;
	last_x_move=1;
	last_y_move=1;

	Cell **temp1=new Cell*[get_size()];	// creating board
	for(int i=0; i< get_size(); i++){
		temp1[i]=new Cell[get_size()];
	}
	for (int i = 0; i < get_size(); i++){	//sett empty board
		for (int j = 0; j < get_size(); j++){
			temp1[i][j].set_situation(EMPTY);
		}
	}
	hexCells=temp1;

	Cell **temp2=new Cell*[get_size()];	//creating control table
	for(int i=0; i< get_size(); i++){
		temp2[i]=new Cell[get_size()];
	}
	for (int i = 0; i < get_size(); i++){	//sett empty control table
		for (int j = 0; j < get_size(); j++){
			temp2[i][j].set_situation(EMPTY);
		}
	}
	control_table=temp2;
}

//says that who should play and setts counters
void Hex::who_is_next(){
	x_counter=0;
	o_counter=0;	

	for (int i = 0; i < get_size() ; i++){

		for (int j = 0; j < get_size(); j++){
			if (hexCells[i][j].get_situation()==FILLED_X){
				x_counter+=1;
			}
			else if(hexCells[i][j].get_situation()==FILLED_O){
				o_counter+=1;
			}
		}
	}
}

int Hex::get_size() const{
	return size1;
}
void Hex::set_size(int num){
	if(num>5){
		size1=num;
		size2=num;
	}
}

void Hex::set_size(){
	//takes size from user
	char temp_char;
	int size1value,size2value;
	bool flag=true;

	while(flag){

		cout<<"\nPlease Enter the size of the Hex game:(like 6x6): "; 
		cin>>size1value>>temp_char>>size2value;		//Taking size entry
		Hex(size1value,size2value);
		
		if(size1value==size2value && size1value>5){ //Controlling that if taken size is apropriate
			set_size(size1value);
			flag=false;
		}
		else{
			cout<<endl<< "Invalid size, please re enter the size."<<endl;
		}
	}
}

//takes variable from user and setts the gametype (it is overloaded function)
void Hex::set_gametype(){
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

//gets situation of cell
int Hex::Cell::get_situation()const{
	return situation;
}
//setts situation of cell
void Hex::Cell::set_situation(int num){
	if (situation>=0){
		situation=num;
	}	
}

ostream & operator << (ostream &out, const Hex &out_table){
	//prints table in an aproppriate way, A,B,C/1,2,3/X,O/x,o/. etc.
	int i,j,space_count;
	char mychar;
	out<<" ";
	for(i=0;i<out_table.get_size();i++){
		mychar=i+'A';
		out<<" "<<mychar;
	}
	out<<endl;
	for(i=0;i<out_table.get_size();i++){
		out<<i+1;
		if(i<9){
			for(space_count=0;i>space_count;space_count++)
				out<<" ";
		}
		else{
			for(space_count=1;i>space_count;space_count++)
				out<<" ";
		}

		for(j=0;j<out_table.get_size();j++){
			if(out_table.hexCells[i][j].get_situation()==EMPTY)
				out<<" "<<".";	//No move
			if(out_table.hexCells[i][j].get_situation()==FILLED_X)
				out<<" "<<"x";	
			if(out_table.hexCells[i][j].get_situation()==FILLED_O)
				out<<" "<<"o";
			if(out_table.hexCells[i][j].get_situation()==WINNER_X)	//Uppercase X
				out<<" "<<"X";
			if(out_table.hexCells[i][j].get_situation()==WINNER_O)	//Uppercase O
				out<<" "<<"O";
		}
		out<<endl;
	}
}

//prints size
void Hex::showsize() const{
	cout<<endl<<"Board size is:"<<get_size()<<"x"<<get_size()<<endl;
}

//if somebody wins then that charachter will be bigger size
int Hex::bigger_size(int end){
	int score=0;
	for (int i = 0; i < get_size(); i++){
		for (int j = 0; j < get_size(); j++){
			if (control_table[i][j].get_situation()==1 && hexCells[i][j].get_situation()==end){
				hexCells[i][j].set_situation(end+2);
				score+=1;
			}
		}
	}
	return score;
}

//increases move counter
void Hex::increase_total_move(){
	move_count+=1;
}

//checks that if move is legal
const int Hex::is_legal(int x_coordinate,int y_coordinate) const{
	if(x_coordinate>get_size() || y_coordinate>get_size() || y_coordinate<1 || x_coordinate <1 || hexCells[y_coordinate-1][x_coordinate-1].get_situation()!=EMPTY ){
		return 0;
	}
	return 1;
}

//takes user commands
int Hex::take_command(){
	string selection1,selection2;
	char temp_char;
	int coordinate_x,coordinate_y;

	cin >>selection1;	//taking inputs

	if(selection1.compare("SAVE")==0 || selection1.compare("save")==0){	//saves the game
		cin >>selection2;
		save_game(selection2);
		return 0;																
	}
	else if(selection1.compare("LOAD")==0 || selection1.compare("load")==0){	//loads the game
		cin >>selection2;
		load_game(selection2);
		return 0;																
	}
	else if (selection1.compare("UNDO")==0 || selection1.compare("undo")==0){
		if(gametype==1){
			--*this;		//undo one time
		}
		else{
			(*this)--;		//undo two time for computer too
		}	
		
		return 0;
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

//GAMEPLAY SECTION THAT HAVE ALL FUNCTIONS THAT WE NEED
void Hex::playGame(){
	int score;
	if (is_size_legal()==0){//if size not legal then take again
		set_size();
	}
	
	set_gametype();
	start_table();

	showsize();
	int end=is_game_ended();

	while(end==0){	//if is game ended or not
		cout<<*this;

		if (x_counter==o_counter){	//player 1 moves
			cout<<"Player1:";
			if (take_command()!=0){
				score1=0;
				end=is_game_ended();
				cout<<endl<<"Player1's score = "<<score1<<endl<<endl;
			}
		}
		else if (x_counter>o_counter && get_gametype()==1){ //player2 moves
			cout<<"Player2:";
			if (take_command()!=0){
				score2=0;
				end=is_game_ended();
				cout<<endl<<"Player2's score = "<<score2<<endl<<endl;
			}
		}
		else{				//computer moves
			play();
			score2=0;
			end=is_game_ended();
			cout<<"Copmputer's score = "<<score2<<endl<<endl;
		}
	}

	if (end==1){
		cout<<"\nPLAYER 1 WON\n";
		score=bigger_size(end);
		cout<<"FINALLY PLAYER1'S SCORE IS "<< score<<endl<<endl;
	}
	else if(end==2){
		cout<<"\nPLAYER 2 WON\n";
		score=bigger_size(end);
		cout<<"FINALLY PLAYER2'S SCORE IS "<< score<<endl<<endl;
	}
	else if(end==3){
		cout<<"\nCOMPUTER WON\n";
		score=bigger_size(end-1);
		cout<<"FINALLY COMPUTER'S SCORE IS "<< score<<endl<<endl;
	}
	
	cout<<*this;	
}

//plays game for user
void Hex::play(int x_coordinate,int y_coordinate){
	if (is_legal(x_coordinate,y_coordinate)==1){
		if (x_counter==o_counter){
			hexCells[y_coordinate-1][x_coordinate-1].set_situation(FILLED_X);	//user1
			x_counter+=1;
			last_x_move=x_coordinate;
			last_y_move=y_coordinate;
			add_move_log(1);
		}
		else{
			hexCells[y_coordinate-1][x_coordinate-1].set_situation(FILLED_O);	//user2
			o_counter+=1;
			last_x_move=x_coordinate;
			last_y_move=y_coordinate;
			add_move_log(2);
		}
		
		increase_total_move();
	}
	else{
		cout<<"\nThis move that you want is illegal\n";
	}
}

//plays game for computer. This function have AI 
Hex::Cell Hex::play(){
	char temp_char;
	int middle;
	int old_x,old_y;
	int max_x=0,max_y=0,min_x=get_size(),min_y=get_size();
	int user_max_x=0,user_max_y=0,user_min_x=get_size(),user_min_y=get_size();

	if(get_size()%2==1)	//finds middle point
		middle = ((get_size() -1)/2)+1;
	else
		middle=get_size()/2;

	if(hexCells[middle-1][middle-1].get_situation()==EMPTY){ //Makes the first move to middle of the board
		old_x=middle;
		old_y=middle;
	}
	else if(hexCells[middle-1][middle-1].get_situation()==FILLED_X && hexCells[middle-1][middle].get_situation()==EMPTY){
		//in case of middle of table is filled
		old_x=middle+1;
		old_y=middle;
	}
	else{	//All other moves except for first one

		for(int i=0;i<get_size();i++){	//finds maximum point of computer
			for(int j=0;j<get_size();j++){
				if(hexCells[i][j].get_situation()==FILLED_O && i>max_y ){
					max_y=i;
					max_x=j;
				}
				if(hexCells[i][j].get_situation()==FILLED_O && i<min_y ){
					min_y=i;
					min_x=j;
				}
			}
		}

		for(int i=0;i<get_size();i++){	//finds maximum point of user
			for(int j=0;j<get_size();j++){
				if(hexCells[i][j].get_situation()==FILLED_X && i>user_max_y ){
					user_max_y=i;
					user_max_x=j;
				}
				if(hexCells[i][j].get_situation()==FILLED_X && i<user_min_y ){
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

	hexCells[old_y-1][old_x-1].set_situation(FILLED_O);	//making a move
	temp_char=old_x+'A'-1;
	cout<<"Computer put 'o' at "<<temp_char<<" "<<old_y<<" position \n\n";

	o_counter+=1;
	increase_total_move();

	add_move_log(2);

	Cell temp;
	temp.x_coordinate=old_x;
	temp.y_coordinate=old_y;
	temp.set_situation(FILLED_O);

	return temp;
}

//gets total move of all the time
int Hex::get_total_move(){
	return move_count;
}
//gets gametype, if its 1 that means player vs player
int Hex::get_gametype() const{
	return gametype;
}
//sets gametype
void Hex::set_gametype(int num){
	if (num>0 && num<3){
		gametype=num;
	}
}
//gets width
int Hex::get_width() const{
	return size2;
}
//gets height
int Hex::get_height() const{
	return size1;
}
//Checking that if size legal
int Hex::is_size_legal() const{
	if (get_size()>5 && get_width()==get_height())
		return 1;

	return 0;
}
//gets total move just for that class
const int Hex::get_total_move_for_this_class() const{
	return x_counter+o_counter;
}

//checks that if somebody won the game
int Hex::is_game_ended(){
	flag1=0;
	flag2=0;

	if(x_counter>o_counter){	//Controlling that if time for player 1 or not				
		reset_control_table();
		//resetting control table that controlls if game end
		//then controlling that if actual game is done
		is_game_done_for_user1(last_x_move,last_y_move);

		if(flag1==1 && flag2==1)	//if two flag equal 1, then game ended
			return 1;
	}
	else if(x_counter==o_counter){
		reset_control_table();
		//resetting control table that controlls if game end
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
int Hex::is_game_done_for_user1(int coordinate_x,int coordinate_y){
	score1+=1;
	control_table[coordinate_y-1][coordinate_x-1].set_situation(1);

	for(int i=1;i<=get_size();i++){	//if we go top or lowest place
		if(coordinate_y == i && coordinate_x==1)
			flag1=1;
		if(coordinate_y == i && coordinate_x==get_size())
			flag2=1;
	}
	//Moving around recursively 
	if(coordinate_y-2>=0 && control_table[coordinate_y - 2][coordinate_x-1].get_situation() == 0 && hexCells[coordinate_y-2][coordinate_x-1].get_situation()==FILLED_X)
		is_game_done_for_user1(coordinate_x,coordinate_y-1);
	if(coordinate_y-2>=0 && coordinate_x <get_size() && control_table[coordinate_y-2][coordinate_x].get_situation()==0 && hexCells[coordinate_y-2][coordinate_x].get_situation()==FILLED_X)
		is_game_done_for_user1(coordinate_x+1,coordinate_y-1);
	if(coordinate_x < get_size() && control_table[coordinate_y-1][coordinate_x].get_situation()==0 && hexCells[coordinate_y-1][coordinate_x].get_situation()==FILLED_X)
		is_game_done_for_user1(coordinate_x+1,coordinate_y);
	if(coordinate_y < get_size() && control_table[coordinate_y][coordinate_x-1].get_situation()==0 && hexCells[coordinate_y][coordinate_x-1].get_situation()==FILLED_X)
		is_game_done_for_user1(coordinate_x,coordinate_y+1);
	if(coordinate_y < get_size() && coordinate_x- 2 >= 0 && control_table[coordinate_y][coordinate_x-2].get_situation()==0 && hexCells[coordinate_y][coordinate_x-2].get_situation()==FILLED_X)
		is_game_done_for_user1(coordinate_x-1,coordinate_y+1);
	if(coordinate_x-2 >= 0 && control_table[coordinate_y - 1][coordinate_x -2].get_situation()==0 && hexCells[coordinate_y-1][coordinate_x-2].get_situation()==FILLED_X)
		is_game_done_for_user1(coordinate_x-1,coordinate_y);
	

	return 0;
}
//checks for last move of user2 for if game end or not
int Hex::is_game_done_for_user2(int coordinate_x,int coordinate_y){
	score2+=1;
	control_table[coordinate_y-1][coordinate_x-1].set_situation(1);

	for(int i=1;i<=get_size();i++){	//if we go most right or most left
		if(coordinate_y == 1 && coordinate_x==i)
			flag1=1;
		if(coordinate_y == get_size() && coordinate_x==i)
			flag2=1;
	}
	//Moving around recursively 
	if(coordinate_y-2>=0 && control_table[coordinate_y - 2][coordinate_x-1].get_situation() == 0 && hexCells[coordinate_y-2][coordinate_x-1].get_situation()==FILLED_O)
		is_game_done_for_user2(coordinate_x,coordinate_y-1);
	if(coordinate_y-2>=0 && coordinate_x <get_size() && control_table[coordinate_y-2][coordinate_x].get_situation()==0 && hexCells[coordinate_y-2][coordinate_x].get_situation()==FILLED_O)
		is_game_done_for_user2(coordinate_x+1,coordinate_y-1);
	if(coordinate_x < get_size() && control_table[coordinate_y-1][coordinate_x].get_situation()==0 && hexCells[coordinate_y-1][coordinate_x].get_situation()==FILLED_O)
		is_game_done_for_user2(coordinate_x+1,coordinate_y);
	if(coordinate_y < get_size() && control_table[coordinate_y][coordinate_x-1].get_situation()==0 && hexCells[coordinate_y][coordinate_x-1].get_situation()==FILLED_O)
		is_game_done_for_user2(coordinate_x,coordinate_y+1);
	if(coordinate_y < get_size() && coordinate_x- 2 >= 0 && control_table[coordinate_y][coordinate_x-2].get_situation()==0 && hexCells[coordinate_y][coordinate_x-2].get_situation()==FILLED_O)
		is_game_done_for_user2(coordinate_x-1,coordinate_y+1);
	if(coordinate_x-2 >= 0 && control_table[coordinate_y - 1][coordinate_x -2].get_situation()==0 && hexCells[coordinate_y-1][coordinate_x-2].get_situation()==FILLED_O)
		is_game_done_for_user2(coordinate_x-1,coordinate_y);
}