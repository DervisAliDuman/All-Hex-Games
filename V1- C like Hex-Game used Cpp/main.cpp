#include "func.h"

int main(){
	int size1,size2;
	char temp_char ;

	cout<<endl<<endl<<"Welcome to The Hex Game "<<endl<<endl; 	//Headline

	while(true){

		cout<<"Please Enter the size of the Hex game:(like 6x6): "; 
		cin>>size1>>temp_char>>size2;		//Taking size entry

		if(size1==size2 && size1>5 && size1<13){ //Controlling that if taken size is apropriate
			game_mode_menu(size1);
		}
		else{
			cout<<endl<< "Invalid size, please re enter the size."<<endl;
		}
	}
	return 1;
}

















