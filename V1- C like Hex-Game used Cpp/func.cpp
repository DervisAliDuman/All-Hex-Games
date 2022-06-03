#include "func.h"

void start_table(int table[][12],int border){
	//starts table
	int i,j;
	for(i=0;i<border;i++)
		for(j=0;j<border;j++)
			table[i][j]=0;
}
void print_table(int table[][12],int border){
	//prints table in an aproppriate way, A,B,C/1,2,3/X,O/x,o/. etc.
	int i,j,space_count;
	char mychar;
	cout<<" ";
	for(i=0;i<border;i++){
		mychar=i+'A';
		cout<<" "<<mychar;
	}
	cout<<endl;
	for(i=0;i<border;i++){
		cout<<i+1;
		if(i<9){
			for(space_count=0;i>space_count;space_count++)
				cout<<" ";
		}
		else{
			for(space_count=1;i>space_count;space_count++)
				cout<<" ";
		}

		for(j=0;j<border;j++){
			if(table[i][j]==0)
				cout<<" "<<".";	//No move
			if(table[i][j]==1)
				cout<<" "<<"x";	
			if(table[i][j]==2)
				cout<<" "<<"o";
			if(table[i][j]==3)	//Uppercase X
				cout<<" "<<"X";
			if(table[i][j]==4)	//Uppercase O
				cout<<" "<<"O";
		}
		cout<<endl;
	}
}

void reset_control_table(int control_table[][12]){
	//resets every table which you give it
	for(int i=0; i<12;i++)
		for(int j=0; j<12;j++)
			control_table[i][j]=0;
}

int is_legal(int table[][12],int coordinate_x,int coordinate_y,int border){
	//Controlls that if move is legal
	if(coordinate_y>border || coordinate_x>border || coordinate_y<1 || coordinate_x<1)
		return 0;

	if(table[coordinate_y-1][coordinate_x-1]!=0)
		return 0;

	return 1;
}

int is_game_done_for_user1(int table[][12],int control_table[][12], int coordinate_x,int coordinate_y,int border,int *flag1,int *flag2){
	//Checking if game end for user 1 looking for if the last move is ended the game or not
	int i;

	control_table[coordinate_y-1][coordinate_x-1]=1;	//Controlls that where we went
	for(i=1;i<=border;i++){	//if we go top or lowest place
		if(coordinate_y == i && coordinate_x==1)
			*flag1=1;
		if(coordinate_y == i && coordinate_x==border)
			*flag2=1;
	}
	//Moving around recursively 
	if(coordinate_y-2>=0 && control_table[coordinate_y - 2][coordinate_x-1] == 0 && table[coordinate_y-2][coordinate_x-1]==1)
		is_game_done_for_user1(table,control_table,coordinate_x,coordinate_y-1,border,flag1,flag2);
	if(coordinate_y-2>=0 && coordinate_x <border && control_table[coordinate_y-2][coordinate_x]==0 && table[coordinate_y-2][coordinate_x]==1)
		is_game_done_for_user1(table,control_table,coordinate_x+1,coordinate_y-1,border,flag1,flag2);
	if(coordinate_x < border && control_table[coordinate_y-1][coordinate_x]==0 && table[coordinate_y-1][coordinate_x]==1)
		is_game_done_for_user1(table,control_table,coordinate_x+1,coordinate_y,border,flag1,flag2);
	if(coordinate_y < border && control_table[coordinate_y][coordinate_x-1]==0 && table[coordinate_y][coordinate_x-1]==1)
		is_game_done_for_user1(table,control_table,coordinate_x,coordinate_y+1,border,flag1,flag2);
	if(coordinate_y < border && coordinate_x- 2 >= 0 && control_table[coordinate_y][coordinate_x-2]==0 && table[coordinate_y][coordinate_x-2]==1)
		is_game_done_for_user1(table,control_table,coordinate_x-1,coordinate_y+1,border,flag1,flag2);
	if(coordinate_x-2 >= 0 && control_table[coordinate_y - 1][coordinate_x -2]==0 && table[coordinate_y-1][coordinate_x-2]==1)
		is_game_done_for_user1(table,control_table,coordinate_x-1,coordinate_y,border,flag1,flag2);

	return 0;

}

int is_game_done_for_user2(int table[][12],int control_table[][12], int coordinate_x,int coordinate_y,int border,int *flag1,int *flag2){
	//Checking if game end for user 2 looking for if the last move is ended the game or not
	int i;
	control_table[coordinate_y-1][coordinate_x-1]=1;	//Controlls that where we went

	for(i=1;i<=border;i++){	//if we go top or lowest place
		if(coordinate_y == 1 && coordinate_x==i)
			*flag1=1;
		if(coordinate_y == border && coordinate_x==i)
			*flag2=1;
	}
	//Moving around recursively 
	if(coordinate_y-2>=0 && control_table[coordinate_y - 2][coordinate_x-1] == 0 && table[coordinate_y-2][coordinate_x-1]==2)
		is_game_done_for_user2(table,control_table,coordinate_x,coordinate_y-1,border,flag1,flag2);
	if(coordinate_y-2>=0 && coordinate_x <border && control_table[coordinate_y-2][coordinate_x]==0 && table[coordinate_y-2][coordinate_x]==2)
		is_game_done_for_user2(table,control_table,coordinate_x+1,coordinate_y-1,border,flag1,flag2);
	if(coordinate_x < border && control_table[coordinate_y-1][coordinate_x]==0 && table[coordinate_y-1][coordinate_x]==2)
		is_game_done_for_user2(table,control_table,coordinate_x+1,coordinate_y,border,flag1,flag2);
	if(coordinate_y < border && control_table[coordinate_y][coordinate_x-1]==0 && table[coordinate_y][coordinate_x-1]==2)
		is_game_done_for_user2(table,control_table,coordinate_x,coordinate_y+1,border,flag1,flag2);
	if(coordinate_y < border && coordinate_x- 2 >= 0 && control_table[coordinate_y][coordinate_x-2]==0 && table[coordinate_y][coordinate_x-2]==2)
		is_game_done_for_user2(table,control_table,coordinate_x-1,coordinate_y+1,border,flag1,flag2);
	if(coordinate_x-2 >= 0 && control_table[coordinate_y - 1][coordinate_x -2]==0 && table[coordinate_y-1][coordinate_x-2]==2)
		is_game_done_for_user2(table,control_table,coordinate_x-1,coordinate_y,border,flag1,flag2);

	return 0;

}

int is_there_danger(int table[][12],int danger_zone[][12],int *old_x,int *old_y,int border){
	//because I did bridge, when we are use bridge tactic in the game, we should be aware of it
	for(int i=0;i<border;i++){
		for(int j=0;j<border;j++){
			if(table[i][j]==1 && danger_zone[i][j]==1){
				danger_zone[i][j]=0;
				if(danger_zone[i][j+1]==1){
					danger_zone[i][j+1]=0;	//that means there is no longer any danger from there
					*old_x=j+2;
					*old_y=i+1;
					return 1;
				}
				else{
					danger_zone[i][j-1]=0;
					*old_x=j;
					*old_y=i+1;
					return 1;
				}
			}
		}
	}
	return 0;
}

int fill_empty_zones(int danger_zone[][12],int *old_x,int *old_y,int border){
	//because I did bridge, when we are use bridge tactic in the game, we should be aware of it
	for(int i=0;i<border;i++){
		for(int j=0;j<border;j++){
			if(danger_zone[i][j]==1){
				danger_zone[i][j]=0;
				*old_x=j+1;
				*old_y=i+1;
				if(danger_zone[i][j+1]==1){	//that means there is no longer any danger from there
					danger_zone[i][j+1]=0;
				}
				else{
					danger_zone[i][j-1]=0;
				}
				return 1;
			}
		}
	}
	return 0;
}

void AI(int table[][12],int danger_zone[][12], int *old_x,int *old_y,int border){
	//All algorithm of Computer Moves
	int middle,i,j,max_x=0,max_y=0,min_x=border,min_y=border;
	int user_max_x=0,user_max_y=0,user_min_x=border,user_min_y=border;
	int control_table[12][12];
	int flag1=0,flag2=0;

	if(border%2==1)	//finds middle point
		middle = ((border -1)/2)+1;
	else
		middle=border/2;

	if(table[middle-1][middle-1]==0){ //Makes the first move to middle of the board
		*old_x=middle;
		*old_y=middle;
	}
	else if(table[middle-1][middle-1]==1 && table[middle-1][middle]==0){
		//in case of middle of table is filled
		*old_x=middle+1;
		*old_y=middle;
	}
	else{	//All other moves except for first one
		for(i=0;i<border;i++){	//finds maximum point of computer
			for(j=0;j<border;j++){
				if(table[i][j]==2 && i>max_y ){
					max_y=i;
					max_x=j;
				}
				if(table[i][j]==2 && i<min_y ){
					min_y=i;
					min_x=j;
				}
			}
		}

		for(i=0;i<border;i++){	//finds maximum point of user
			for(j=0;j<border;j++){
				if(table[i][j]==1 && i>user_max_y ){
					user_max_y=i;
					user_max_x=j;
				}
				if(table[i][j]==1 && i<user_min_y ){
					user_min_y=i;
					user_min_x=j;
				}
			}
		}

		max_y+=1; max_x+=1;	//increasing values so that I can use them easily
		min_y+=1; min_x+=1;
		user_max_y+=1; user_max_x+=1;
		user_min_y+=1; user_min_x+=1;

		reset_control_table(control_table);	//resetting table

		is_game_done_for_user2(table,control_table,*old_x,*old_y,border,&flag1,&flag2);
		//controlling that if game done, I didn't used it from here but I think someday I will need it here

		if(is_there_danger(table,danger_zone,old_x,old_y,border)){	//fills the danger zones
			//because I did bridge, when we are use bridge tactic in the game, we should be aware of it
			//Done
		}
		else if(user_min_y<min_y){	//Makes move looking for users moves
			*old_y=min_y; 	//go to min point
			*old_x=min_x;

			if(is_legal(table,min_x+1,min_y-2,border)==1 && table[min_y-2][min_x-1]==0 && table[min_y-2][min_x]==0){
				//Making bridge tactic if it is possible then be aware of it
				*old_x=min_x+1;
				*old_y=min_y-2;
				danger_zone[min_y-2][min_x-1]=1;
				danger_zone[min_y-2][min_x]=1;
			}
			else {
				*old_y=min_y;	//go to min point
				*old_x=min_x;
				//Checking around of where we are
				if(table[*old_y-2][*old_x-1]==0 && is_legal(table,*old_x,*old_y-1,border)){
					*old_y-=1;
				}
				else if(table[*old_y-2][*old_x]==0 && is_legal(table,*old_x+1,*old_y-1,border)){
					*old_x+=1;
					*old_y-=1;
				}
				else if(fill_empty_zones(danger_zone,old_x,old_y,border)==1){
					//Done, time to fill empty zones
				}
				else if(table[*old_y-1][*old_x-2]==0 && is_legal(table,*old_x-1,*old_y,border)){
					*old_x-=1;
				}
				else if(table[*old_y-1][*old_x]==0 && is_legal(table,*old_x+1,*old_y,border)){
					*old_x+=1;
				}
				else if(table[*old_y][*old_x-2]==0 && is_legal(table,*old_x-1,*old_y+1,border)){
					*old_x-=1;
					*old_y+=1;
				}
				else if(table[*old_y][*old_x-1]==0 && is_legal(table,*old_x,*old_y+1,border)){
					*old_y+=1;
				}
				else{
					*old_y=max_y;	//go to max point
					*old_x=max_x;
					//Checking around of where we are
					if(table[*old_y][*old_x-2]==0 && is_legal(table,*old_x-1,*old_y+1,border)){
						*old_y+=1;
						*old_x-=1;
					}
					else if(table[*old_y][*old_x-1]==0 && is_legal(table,*old_x,*old_y+1,border)){
						*old_y+=1;
					}
				}
			}
		}
		else{
			*old_y=max_y;	//go to max point
			*old_x=max_x;

			if(is_legal(table,max_x-1,max_y+2,border)==1 && table[max_y][max_x-2]==0 && table[max_y][max_x-1]==0){
				*old_x=max_x-1;
				*old_y=max_y+2;
				danger_zone[max_y][max_x-2]=1;
				danger_zone[max_y][max_x-1]=1;
			}
			else {
				*old_y=max_y;	//go to max point
				*old_x=max_x;
				//Checking around of where we are
				if(table[*old_y][*old_x-2]==0 && is_legal(table,*old_x-1,*old_y+1,border)){
					*old_y+=1;
					*old_x-=1;
				}
				else if(table[*old_y][*old_x-1]==0 && is_legal(table,*old_x,*old_y+1,border)){
					*old_y+=1;
				}
				else if(fill_empty_zones(danger_zone,old_x,old_y,border)==1){
					//Done, time to fill emty zones
				}
				else if(table[*old_y-1][*old_x]==0 && is_legal(table,*old_x+1,*old_y,border)){
					*old_x+=1;
				}
				else if(table[*old_y-1][*old_x-2]==0 && is_legal(table,*old_x-1,*old_y,border)){
					*old_x-=1;
				}
				else if(table[*old_y-2][*old_x-1]==0 && is_legal(table,*old_x,*old_y-1,border)){
					*old_y-=1;
				}
				else if(table[*old_y-2][*old_x]==0 && is_legal(table,*old_x+1,*old_y-1,border)){
					*old_y-=1;
					*old_x+=1;
				}
				else{
					*old_y=min_y;	//go to min point
					*old_x=min_x;
					//Checking around of where we are
					if(table[*old_y-2][*old_x-1]==0 && is_legal(table,*old_x,*old_y-1,border)){
						*old_y-=1;
					}
					else if(table[*old_y-2][*old_x]==0 && is_legal(table,*old_x+1,*old_y-1,border)){
						*old_x+=1;
						*old_y-=1;
					}
				}
			}
		}
	}
}

int player_vs_player_menu(int border){	//2 Player game
	int table[12][12];
	int control_table[12][12];
	int end=0;
	char temp_char;
	int coordinate_x,coordinate_y;
	int counter =1;
	int flag1=0,flag2=0;
	int i,j;

	start_table(table,border);	//Starting table
	print_table(table,border);	//Printing table with appropriate way

	//GAMEPLAY

	while(end==0){
		if(counter%2==1)	//Controlling that if time for player 1 or not
			cout<<"\nPlayer1 "<<": ";
		else if(counter%2==0)	//Controlling that if time for player 2 or not
			cout<<"\nPlayer2 "<<": ";

		cin>>temp_char >>coordinate_y;	//Taking coordinates from user

		coordinate_x=temp_char-'A'+1;	//type casting char to int

		if(is_legal(table,coordinate_x,coordinate_y,border)==1){
			if(counter%2==1)	//making moove if it is legal	
				table[coordinate_y-1][coordinate_x-1]=1;
			else if(counter%2==0)
				table[coordinate_y-1][coordinate_x-1]=2;

			print_table(table,border);

			flag1=0;
			flag2=0;

			if(counter%2==1){	//Controlling that if time for player 1 or not
				reset_control_table(control_table);
				//resetting control table that controlls if game end
				//then controlling that if actual game is done
				is_game_done_for_user1(table,control_table,coordinate_x,coordinate_y,border,&flag1,&flag2);

				if(flag1==1 && flag2==1)	//if two flag equal 1, then game ended
					end=1;
			}
			else if(counter%2==0){
				reset_control_table(control_table);
				//resetting control table that controlls if game end
				//then controlling that if actual game is done
				is_game_done_for_user2(table,control_table,coordinate_x,coordinate_y,border,&flag1,&flag2);

				if(flag1==1 && flag2==1)	//if two flag equal 1, then game ended
					end=2;
			}
			counter+=1;	//thats mean is move accepted and time for other player move
		}
		else
			cout<<"Not Legal Try AGAİN \n";
	}

	if(end==1){	//That means player 1 won the game
		cout<<"\n\nPlayer1 won the game\n";
		for(i=0;i<border;i++){	//increasing letters of the winning path
			for(j=0;j<border;j++){
				if(table[i][j]==1 && control_table[i][j]==1)
					table[i][j]=3;
			}
		}
		print_table(table,border);
	}
	else if(end==2){	//That means player 2 won the game
		cout<<"\n\nPlayer2 won the game\n";
		for(i=0;i<border;i++){	//increasing letters of the winning path
			for(j=0;j<border;j++){
				if(table[i][j]==2 && control_table[i][j]==1)
					table[i][j]=4;
			}
		}
		print_table(table,border);
	}

	return 0;
}

int player_vs_computer_menu(int border){ //Single player game
	int table[12][12];
	int control_table[12][12];
	int danger_zone[12][12];
	int end=0;
	char temp_char;
	int coordinate_x,coordinate_y;
	int old_x,old_y;
	int counter =1;
	int flag1=0,flag2=0;
	int i,j;

	reset_control_table(danger_zone);

	start_table(table,border); 	//starting table
	print_table(table,border);	//Printing table to terminal

	//GAMEPLAY

	while(end==0){
		flag1=0; //reseting flag values (Theese values controlls if game ended) 
		flag2=0;
		if(counter%2==1){	//Controlling that if time for user or not
			cout<<"\nPlayer "<<": ";

			cin>>temp_char >>coordinate_y;	//Taking coordinates from user
			coordinate_x=temp_char-'A'+1;	//Typecasting

			if(is_legal(table,coordinate_x,coordinate_y,border)==1){
				//if coordinates is legal then make that move to table
				table[coordinate_y-1][coordinate_x-1]=1;

				print_table(table,border);	

				reset_control_table(control_table);	//resetting control table that controlls if game end
									//then controlling that if actual game is done
				is_game_done_for_user1(table,control_table,coordinate_x,coordinate_y,border,&flag1,&flag2);

				if(flag1==1 && flag2==1)	//if two flag equal 1, then game ended
					end=1;
				
				counter+=1;	//thats mean is move accepted and time for other player move
			}
			else
				cout<<"Not Legal Try AGAİN \n";
		}
		else{ //Time for Computer make a move

			AI(table,danger_zone,&old_x,&old_y,border);
			//It's algorith that give us coordinates

			table[old_y-1][old_x-1]=2;

			print_table(table,border);
			temp_char = old_x+64;	//typecasting for user undertanding

			cout<<"Computer put 'o' at "<< temp_char<<" "<<old_y <<" point"<<endl;
			//giving information about move of computer
			flag1=0;
			flag2=0;

			reset_control_table(control_table);
			//resetting control table that controlls if game end
			//then controlling that if actual game is done
			is_game_done_for_user2(table,control_table,old_x,old_y,border,&flag1,&flag2);

			if(flag1==1 && flag2==1)	//if two flag equal 1, then game ended
				end=2;

			counter+=1;	//thats mean is move accepted and time for other player move

		}
	}

	if(end==1){	//That means player won the game
		cout<<"\n\nPlayer won the game\n";
		for(i=0;i<border;i++){	//increasing letters of the winning path
			for(j=0;j<border;j++){
				if(table[i][j]==1 && control_table[i][j]==1)
					table[i][j]=3;
			}
		}
		print_table(table,border);
	}
	else if(end==2){	//That means computer won the game
		cout<<"\n\nComputer won the game\n";
		for(i=0;i<border;i++){	//increasing letters of the winning path
			for(j=0;j<border;j++){
				if(table[i][j]==2 && control_table[i][j]==1)
					table[i][j]=4;
			}
		}
		print_table(table,border);
	}

	return 0;
}

int game_mode_menu(int border){
	int selection;
	while(true){

		cout<<endl<<endl;	//Menu Statement
		cout<<"1-) Player vs Player (two player)."<< endl;
		cout<<"2-) Player vs Computer (single player)."<<endl<<endl;
		cout<<"3-) Go back to the size selection."<<endl<<endl;

		cout<<"Please Select an operation from the menu: ";
		cin>>selection;

		cout<<endl<<endl;

		switch(selection){
			case 1: player_vs_player_menu(border); 	//2 Player game
				break;
			case 2: player_vs_computer_menu(border);  //Single player game
				break;
			case 3: return 0;
				break;
		}
	}
}


