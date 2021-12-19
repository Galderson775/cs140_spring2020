// Robert Schenck
// Lab 2 neg
// 1/28/2020

#include <iostream>
#include <cstdio>

using namespace std;

int main(){

	// variable declarations
	string header;
	int row, col, max, pixel;
	int i = 0;

	// gets file header and info
	cin >> header >> col >> row >> max;

	// checks if file header and info is valid
	if(header != "P2"){	
		fprintf(stderr, "Bad PGM file -- first word is not P2\n");
		return -1;
	}else if(col <= 0){
		fprintf(stderr, "Bad PGM file -- No column specifications\n");
		return -1;
	}else if(row <= 0){
		fprintf(stderr, "Bad PGM file -- No row specifications\n");
		return -1;
	}else if( max != 255){
		fprintf(stderr, "Bad PGM file -- No 255 following the rows and columns\n");
		return -1;
	}

	// prints header and file info
	cout << "P2\n" << col << " " << row << '\n' << max << '\n';
	// reads in all the pixels checking if they're valid 
	while(cin >> pixel){
		if(i >= (col * row)){
			fprintf(stderr, "Bad PGM file -- Extra stuff after the pixels\n");
			return -1;
		}
		if(pixel > max || pixel < 0){
			fprintf(stderr, "Bad PGM file -- pixel %d is not a number between 0 and 255\n", i);
			return -1;
		}else if(cin.fail()){
			fprintf(stderr, "Bad PGM file -- pixel %d is not a number between 0 and 255\n", i);
			return -1;
		}
		i++;
		// negates all pixels and prints them out
		pixel = 255 - pixel;
		cout << pixel << '\n'; 
	}


}
