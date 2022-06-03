#include "func.h"

int main(){
	Hex game1(7,7),game2(8),game4(9),game5(6,6);
	Hex game3(game1);
	game5=game2;

	cout<<endl <<endl<<"Total move is "<<game1.get_total_move()<<endl; 

	game1.playGame();

	cout<<endl <<endl<<"Total move is "<<game1.get_total_move()<<endl; 

	game2.playGame();

	cout<<endl <<endl<<"Total move is "<<game1.get_total_move()<<endl;

	if (game1==game2){
		cout<<"\ngame1 has more filled cell than game2\n";
	}
	else if (game2==game1){
		cout<<"\ngame2 has more filled cell than game1\n";
	}
	else{
		cout<<"\ngame1 and game2 has equal filled cell count\n";
	}

	game3.playGame();

	cout<<endl <<endl<<"Total move is "<<game1.get_total_move()<<endl;
	if (game2==game3){
		cout<<"\ngame2 has more filled cell than game3\n";
	}
	else if(game3==game2) {
		cout<<"\ngame3 has more filled cell than game2\n";
	}
	else{
		cout<<"\ngame2 and game3 has equal filled cell count\n";
	}

	game4.playGame();

	if (game3==game4){
		cout<<"\ngame2 has more filled cell than game3\n";
	}
	else if(game4==game3) {
		cout<<"\ngame3 has more filled cell than game2\n";
	}
	else{
		cout<<"\ngame2 and game3 has equal filled cell count\n";
	}

	cout<<endl <<endl<<"Total move is "<<game1.get_total_move()<<endl;

	game5.playGame();

	cout<<endl <<endl<<"Total move is "<<game1.get_total_move()<<endl; 
	

	return 1;

 }