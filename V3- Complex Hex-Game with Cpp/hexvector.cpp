#include "abstract.h"
#include "hexvector.h"
#include "hexarray1D.h"
#include "hexadapter.h"

namespace abstractDuman{

	HexVector::HexVector(int sizevalue) :AbstractHex(sizevalue){
		vector<vector<Cell> > temp_2D_cell; 

		Cell temp_cell;
		temp_cell.set_situation(EMPTY);

		for (int i = 0; i < get_size(); i++) { 
			vector<Cell> inner_vec; 
			for (int j = 0; j < size2; j++) { 
				inner_vec.push_back(temp_cell);
			} 
			temp_2D_cell.push_back(inner_vec); 
		}
		hexCells=temp_2D_cell;
	}

	AbstractHex& HexVector::operator=(const AbstractHex &rv){ //copy assignment
		cout<<"HexVector Class Copy Assingment called\n";
		AbstractHex::operator=(rv);
		return *this;
	}

	HexVector::HexVector(const HexVector &rv):AbstractHex(rv){/*EMPTY*/}

	AbstractHex::Cell& HexVector::operator()(int y,int x) throw (Memoryerror){ //index operator that return cell
		if(x>=0 && y>=0 && x<get_size() && y<get_size() ){
			return hexCells[y][x];
		}
		throw Memoryerror();	 														///EXEPTİON
	}
	 
	const AbstractHex::Cell& HexVector::operator()(int y,int x)const throw (Memoryerror) { //index operator that return cell
		if(x>=0 && y>=0 && x<get_size() && y<get_size() ){
			return hexCells[y][x];
		}
		throw Memoryerror();	 														///EXEPTİON
	}

	void HexVector::Allocator(){  //Allocate memory
		x_counter=0;
		o_counter=0;
		last_x_move=1;
		last_y_move=1;

		vector<vector<Cell> > temp_2D_cell; 

		Cell temp_cell;
		temp_cell.set_situation(EMPTY);

		for (int i = 0; i < get_size(); i++) { 
			vector<Cell> inner_vec; 
			for (int j = 0; j < size2; j++) { 
				inner_vec.push_back(temp_cell);
			} 
			temp_2D_cell.push_back(inner_vec); 
		}
		hexCells=temp_2D_cell;

		reset_control_table();
	}

}
