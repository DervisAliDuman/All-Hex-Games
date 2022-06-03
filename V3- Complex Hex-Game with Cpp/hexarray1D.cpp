#include "abstract.h"
#include "hexarray1D.h"
#include "hexvector.h"
#include "hexadapter.h"

namespace abstractDuman{

	HexArray1D::HexArray1D(int sizevalue):AbstractHex(sizevalue){
		hexCells = (Cell*) malloc(get_size()*get_size()*sizeof(Cell));
		for(int i=0;i<get_size();i++){
			for(int j=0;j<get_size();j++){
				(*this)(i,j).set_situation(EMPTY);
			}
		}
	}

	HexArray1D::HexArray1D(const HexArray1D &rv):AbstractHex(rv){/*EMPTY*/}

	AbstractHex& HexArray1D::operator=(const AbstractHex &rv){ //copy assignment
		cout<<"HexArray1D's copy assingment operator called\n";
		AbstractHex::operator=(rv);
		return *this;
	}

	HexArray1D::~HexArray1D(){ //destructor
		if(hexCells)
			free(hexCells);
	}

	void HexArray1D::Allocator(){ //Allocate memory
		x_counter=0;
		o_counter=0;
		last_x_move=1;
		last_y_move=1;

		Cell *temp1=(Cell*) malloc(get_size()*get_size()*sizeof(Cell));	// creating board

		for (int i = 0; i < get_size()*get_size(); i++){	//sett empty board
				temp1[i].set_situation(EMPTY);
		}
		if(hexCells){
			free (hexCells);
		}
		hexCells=temp1;

		reset_control_table();
	}

	AbstractHex::Cell& HexArray1D::operator()(int y,int x) throw (Memoryerror){ //index operator that return cell
		if(x>=0 && y>=0 && x<get_size() && y<get_size() ){
			return hexCells[y*get_size()+x];
		}
		throw Memoryerror();														///EXEPTİON
	}

	const AbstractHex::Cell& HexArray1D::operator()(int y,int x) const throw (Memoryerror){ //index operator that return cell
		if(x>=0 && y>=0 && x<get_size() && y<get_size() ){
			return hexCells[y*get_size()+x];
		}
		throw Memoryerror();														///EXEPTİON
	}


}