// Robert Schenck
// Lab 2 bigwhite
// 1/27/2020

#include <iostream>
#include <cstdio>
#include <sstream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[]){

	// Variable declarations
	int rows = 0;
	int columns = 0;
	int zero = 0;
	istringstream ss;
	string file;
	ofstream fout;

	// Checks to see if correct amount of arguements given
	if(argc != 3){
		fprintf(stderr, "usage: bigwhite rows cols\n");
		return -1;
	}

	// gets output file
	cin >> file;

	// converts the command line arguments to ints and checks if they're valid
	for(int i = 1; i < argc; i++){
		ss.clear();
		ss.str(argv[i]);
		if(i == 1){
			if(ss >> rows){
				if(rows <= 0){
					fprintf(stderr, "usage: bigwhite rows cols\n");
				}
			}else{
				fprintf(stderr, "usage: bigwhite rows cols\n");
			}
		}else if(i == 2){
			if(ss >> columns){
				if(columns <= 0){
					fprintf(stderr, "usage: bigwhite rows cols\n");
				}
			}else{
				fprintf(stderr, "usage: bigwhite rows cols\n");
			}
		}
	}

	// opens output file and checks if it is opened
	fout.open(file);
	if(fout.fail()){
		return -1;
	}

	// Prints the pgm file to the file and closes
	fout << "P2\n" << columns << " " << rows << '\n' << "255\n";
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			fout << "255 ";
		}
		fout << '\n';
	}
	fout.close();
	cout << "P2\n" << columns << " " << rows << '\n' << "255\n";
	for(int i = 0; i < rows; i++){
		for(int j = 0; j < columns; j++){
			cout << "255\n";
		}
	}
}
