#ifndef _HEXARRAY1D_H
#define _HEXARRAY1D_H

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

	class HexArray1D : public AbstractHex{
		public:
			HexArray1D(int sizevalue=6);
			AbstractHex& operator=(const AbstractHex &rv);
			HexArray1D(const HexArray1D &rv);
	     	~HexArray1D();

			Cell& operator()(int y,int x)throw (Memoryerror); //index operator that return cell
			const Cell& operator()(int y,int x) const throw (Memoryerror); //index operator that return cell
		private:
			void Allocator();
			Cell *hexCells;	//cell vektor
	};

}
#endif
