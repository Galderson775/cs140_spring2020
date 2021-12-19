/*
 * INSTRUCTOR-GIVEN TEMPLATE
 * COSC 140: Lab 9B - Shape Shifter!
 *
 * Description:
 *     A simple introduction to recursion. Brute force through all possible
 *     combinations of a puzzle until a valid solution is found. If none are
 *     discovered, print nothing. If a solution does exist, print out the
 *     pieces and the positions where they would solve the puzzle.
 *
 * Author:
 *     Robert Schenck
 */

//C++
#include <iostream>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <algorithm>
#include <sstream>

//C
#include <stdio.h>
#include <stdlib.h>

using namespace std;

// ----------------------------------------------------------------------------
// Class/Struct Function Prototypes                                        {{{1
// ----------------------------------------------------------------------------

/*
 * Class for ShapeShifter. Has a grid of the initial setup that you will try to
 * put pieces into.
 */


class ShapeShifter {	
	public:
		
		bool read_grid_and_shapes(int argc, char **argv);
		bool apply_shape(unsigned int index, unsigned int r, unsigned int c);
		bool find_solution(unsigned int index);
		bool all_one();
		void print_solution() const;
		
	protected:
		/* This is the grid. I have this to be a vector of 0's and 1's, because it makes it
		   it easy to print the grid out. */
		vector<string> Grid;

		/* These are the shapes, in the order in which they appear in standard input. */
		vector<vector<string> > Shapes;

		/* These two vectors hold the starting positions of the shapes, both as you
		   are finding a solution, and when you are done finding a solution. */

		vector<int> Solution_Rows;
		vector<int> Solution_Cols;
};

// Checks if a zero is in Grid

bool ShapeShifter::all_one(){

	for(unsigned int i = 0; i < Grid.size(); i++){
		for(unsigned int j = 0; j < Grid[0].size(); j++){
			if(Grid[i][j] == '0'){
				return false;
			}
		}
	}
	return true;
}

// Reads in the Grid and the shapes

bool ShapeShifter::read_grid_and_shapes(int argc, char* argv[]){

	string line, shape;
	vector<string> temp;
	for(int i = 1; i < argc; i++){
		Grid.push_back(argv[i]);
	}

	while(getline(cin, line)){
		for(unsigned int i = 0; i < line.size(); i++){
			if(line[i] == ' '){
				temp.push_back(shape);
				shape = "";
			}else{
				shape += line[i];
			}
		}
		temp.push_back(shape);
		Shapes.push_back(temp);
		temp.clear();
	}
	return true;
}

/*
 * apply_shape                                                             {{{2
 *
 * Inserts a shape into the game grid. The shape is in "shapes" at index
 * "index" and it is inserted at the top-left corner, but shifted by
 * "r" and "c". If the shape shifted by "r" and "c" goes out of the game board,
 * make sure nothing happens. This function may be used twice to "undo" a shape
 * apply.
 *
 * (ex. Assume the given grid "100 101 000" and shape "111".
 *
 *    apply_shape at (x: 0, y: 0)      apply_shape at (x: 0, y: 1)
 *      100   111   011                  100         100
 *      101 +     = 101                  101 + 111 = 010
 *      000         000                  000         000
 */

bool ShapeShifter::apply_shape(unsigned int index, unsigned int r, unsigned int c){

	for(unsigned int i = 0; i < Shapes[index].size(); i++){
		if(Shapes[index].size() > Grid.size() - r || Shapes[index][i].size() > Grid[0].size() - c){
			return false;
		}
	}
	for(unsigned int row = 0; row < Shapes[index].size(); row++){
		for(unsigned int col = 0; col < Shapes[index][row].size(); col++){
			if(Shapes[index][row][col] == '1' && Grid[row + r][col + c] == '1'){
				Grid[row + r][col + c] = '0';
			}else if((Shapes[index][row][col] == '1' && Grid[row + r][col + c] == '0') || (Shapes[index][row][col] == '0' && Grid[row + r][col + c] == '1')){
				Grid[row + r][col + c] = '1';
			}
		}
	}
	return true;
}

/*
 * find_solution                                                           {{{2
 *
 * Recursively traverse and try to find a solution. If called with no argument,
 * it'll start at the very first shape in the "shapes" vector and go through
 * each of them trying to apply in every possible position until a solution is
 * found. Returns TRUE upon a solution being found, and FALSE otherwise.
 */

bool ShapeShifter::find_solution(unsigned int index) {
	
	for(unsigned int row = 0; row < Grid.size(); row++){
		for(unsigned int col = 0; col < Grid[0].size(); col++){

			if(apply_shape(index, row, col)){
				Solution_Rows.insert(Solution_Rows.begin(), row);
				Solution_Cols.insert(Solution_Cols.begin(), col);
				if(index < Shapes.size()){
					find_solution(index + 1);
				}else{
					if(all_one()){
						return true;
					}
				}
			}
		}
	}
	return false;
}

/*
 * print_solution                                                          {{{2
 *
 * Goes through the shapes vector and prints each shape and the coordinates
 */

void ShapeShifter::print_solution() const{
	
	for(unsigned int rows = 0; rows < Shapes.size(); rows++){
		for(unsigned int cols = 0; cols < Shapes.size(); cols++){
			cout << Shapes[rows][cols] << ' ';
		}
		cout << Solution_Rows[rows] << ' ' << Solution_Cols[rows] << '\n';
	}
	
}

// ----------------------------------------------------------------------------
// Main Function                                                           {{{1
// ----------------------------------------------------------------------------

int main(int argc, char** argv) {

	// Creates ShapeShifter object
	ShapeShifter shapeShifter;
	// Reads the Grid and Shapes
	shapeShifter.read_grid_and_shapes(argc, argv);
	// Finds a solution
	shapeShifter.find_solution(0);
	// Prints the solution
	shapeShifter.print_solution();

	return 0;
}
