#include "abstract.h"
#include "hexarray1D.h"
#include "hexvector.h"
#include "hexadapter.h"
#include "hexadapter.cpp"

using namespace abstractDuman;

namespace {
	bool are_any_game_valid(AbstractHex *Games[],int game_count){
		//function that look if there are any wrong value on table
		bool flag=true;

		for(int i=0; i<game_count;i++){
			flag=true;
			for (int j = 0;j < Games[i]->get_size() ; j++){
				for (int k = 0;k < Games[i]->get_size(); k++){
					if((*Games[i])(j,k).get_situation() == EMPTY || (*Games[i])(j,k).get_situation() == FILLED_X || (*Games[i])(j,k).get_situation() == FILLED_O || (*Games[i])(j,k).get_situation() == WINNER_X || (*Games[i])(j,k).get_situation() == WINNER_O){
						
					}
					else{
						flag=false;
					}
				}
			}
			if(flag)
				return flag;
		}
		return flag;
	}
} 

int main(){

	cout<<endl<<endl;

	HexArray1D game111,game222;
	HexVector game333;
	HexAdapter<deque> game444;
	HexAdapter<vector> game555;

	AbstractHex *game11=&game111,*game22=&game222,*game33=&game333,*game44=&game444,*game55=&game555;
	AbstractHex *games[5];

	game444=game333;
	game222=game333; //As you can see, you can assign different type games for deepcopying
	game333=*game55; //It will print which assignment operator called on terminal
					 //you can do it same thing with pointers as well

	games[0]=game11;
	games[1]=game22;
	games[2]=game33;
	games[3]=game44;
	games[4]=game55;

	int lastmoveX,lastmoveY;

	try{
		game11->lastMove();
		cout<<endl<<"last Move coordinate Y : "<< lastmoveY <<" coordinate X: "<< lastmoveX << endl;
	}
	catch(exception &b){
		cerr<<endl<<b.what()<<endl;
	}

	if(are_any_game_valid(games,5)){
		cout<<"\nThere is at least one game valid\n\n";

		game11->playGame();
	}
	else{
		cout<<"\nThere are no valid game\n\n";
	}

	try{
		(*game11)(1,2).get_situation();
	}
	catch (exception &a){
		cerr<<endl<<a.what()<<endl;
	}

	try{
		(*game11)(-1,5).get_situation();
	}
	catch (exception &a){
		cerr<<endl<<a.what()<<endl;
	}

	if(are_any_game_valid(games,5)){
		cout<<"\nThere is at least one game valid\n\n";
		game22->playGame();
	}
	else{
		cout<<"\nThere are no valid game\n\n";
	}

	if((*game11)==(*game22)){
		cout<<"game1 and game2 are equal";
	}
	else{
		cout<<"game1 and game2 are not equal";
	}


	if(are_any_game_valid(games,5)){
		cout<<"\nThere is at least one game valid\n\n";
		game33->playGame();
	}
	else{
		cout<<"\nThere are no valid game\n\n";
	}

	if(are_any_game_valid(games,5)){
		cout<<"\nThere is at least one game valid\n\n";
		game44->playGame();
	}
	else{
		cout<<"\nThere are no valid game\n\n";
	}

	if(are_any_game_valid(games,5)){
		cout<<"\nThere is at least one game valid\n\n";
		game55->playGame();
	}
	else{
		cout<<"\nThere are no valid game\n\n";
	}
	
	return 0;
}
