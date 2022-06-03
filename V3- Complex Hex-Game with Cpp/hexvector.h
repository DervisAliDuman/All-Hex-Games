#ifndef _HEXVECTOR_H
#define _HEXVECTOR_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <stdexcept>

#include "abstract.h"

using namespace std; 

namespace abstractDuman{

	class HexVector : public AbstractHex{
		public:
			HexVector(int sizevalue=6);
	     	AbstractHex& operator=(const AbstractHex &rv);
			HexVector(const HexVector &rv);
			~HexVector() = default;

			Cell& operator()(int y,int x)throw (Memoryerror); //index operator that return cell
			const Cell& operator()(int y,int x) const throw (Memoryerror); //index operator that return cell

		private:
			void Allocator();	//Allocates memory
			vector<vector<Cell> >hexCells;	//cell vektor
	};

}
#endif
