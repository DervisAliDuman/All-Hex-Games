#ifndef _HEXADAPTER_H
#define _HEXADAPTER_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <stdexcept>
#include <iterator>
#include <deque> 

#include "abstract.h"

using namespace std; 
namespace abstractDuman{

template<template<typename...> class T>
	class HexAdapter:public AbstractHex{
		public:
			HexAdapter(int sizevalue=6);
			AbstractHex& operator=(const AbstractHex &rv);
			HexAdapter(const HexAdapter &rv);
		 	~HexAdapter() = default;

			Cell& operator()(int y,int x) throw (Memoryerror); //index operator that return cell
			const Cell& operator() (int y,int x)const throw (Memoryerror); //index operator that return cell
		private:
			void Allocator();	//ALlocates memory
			T<T<Cell>> hexCells;
	   
	};
}
#endif
