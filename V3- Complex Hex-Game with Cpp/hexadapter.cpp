#include "abstract.h"
#include "hexarray1D.h"
#include "hexvector.h"
#include "hexadapter.h"

namespace abstractDuman{

	template<template<typename ...>class T>
	HexAdapter<T>::HexAdapter(int sizevalue) : AbstractHex(sizevalue){	//constructor
		hexCells.resize(get_size()*get_size());
		for (int i = 0; i < get_size(); i++)
	    	hexCells.at(i).resize(get_size());

	    for (int i = 0; i < get_size(); i++){
	    	for (int j = 0; j < get_size(); j++){
	    		hexCells.at(i).at(j).set_situation(EMPTY);
	    	}
	    }
	}


	template<template<typename ...>class T>
	AbstractHex& HexAdapter<T>::operator=(const AbstractHex &rv){	//copy assignment
		cout<<"HexAdapter Class Copy Assingment called\n";
		AbstractHex::operator=(rv);
		return *this;
	}

	template<template<typename ...>class T>
	HexAdapter<T>::HexAdapter(const HexAdapter &rv):AbstractHex(rv){}


	template<template<typename ...>class T>
	AbstractHex::Cell& HexAdapter<T>::operator()(int y,int x) throw (Memoryerror){ //index operator that return cell
		if(x>=0 && y>=0 && x<get_size() && y<get_size() ){
			return hexCells.at(y).at(x);
		}
		throw Memoryerror();	 														///EXEPTİON
	}
	 
	template<template<typename ...>class T>
	const AbstractHex::Cell& HexAdapter<T>::operator()(int y,int x)const throw (Memoryerror) { //index operator that return cell
		if(x>=0 && y>=0 && x<get_size() && y<get_size() ){
			return hexCells.at(y).at(x);
		}
		throw Memoryerror();	 														///EXEPTİON
	}

	template<template<typename ...>class T>
	void HexAdapter<T>::Allocator(){	//Allocates memory
		x_counter=0;
		o_counter=0;
		last_x_move=1;
		last_y_move=1;

		hexCells.resize(get_size()*get_size());
		for (int i = 0; i < get_size(); i++)
	    	hexCells.at(i).resize(get_size());
	    
	    for (int i = 0; i < get_size(); i++){
	    	for (int j = 0; j < get_size(); j++){
	    		hexCells.at(i).at(j).set_situation(EMPTY);
	    	}
	    }

		reset_control_table();
	}

}