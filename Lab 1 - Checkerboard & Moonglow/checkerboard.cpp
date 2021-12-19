// Robert Schenck
// Lab1
// 1/17/20

#include <iostream>

using namespace std;

int main(){

	int row = -1000;
	int column = -1000;
	int cycleSize = -1000;
	int width = -1000;
	int intChar = 0;
	int counter = 1;
	char startingChar;

	cin >> row >> column >> startingChar >> cycleSize >> width;
	if(row == -1000 || column == -1000 || cycleSize == -1000 || width == -1000 || startingChar == '\0'){
		cerr << "usage: checkerboard  - stdin should contain R, C, SC, CS and W\n";
		return 1;
	}

	intChar = (int)startingChar;

	if(row <= 0 || column <= 0 || cycleSize <= 0 || width <= 0 || intChar <= 0 || (intChar + cycleSize) > 127){
		return 1;
	}

	for(int i = 0; i < row; i++){
		for(int k = 0; k < column; k++){
			for(int j = 0; j < width; j++){
				cout << (char)(startingChar + (i + k) % cycleSize);
			}
		}
		cout << '\n';
		if(counter != width){
			i--;
			counter++;
		}else{
			counter = 1;
		}
	}
}
