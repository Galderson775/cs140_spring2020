// Robert Schenck
// Lab 2 pgminfo
// 1/22/2020

#include <cstdio>
#include <iostream>
#include <iomanip>

using namespace std;

int main(){

	// Variable declarations
	string header;
	int row, column, max, pixel;
	int i = 0;
	int sum = 0;
	double average = 0;

	// Gets the header and other file info using standard input
	cin >> header >> column >> row >> max;

	// Error checks the file info
	if(header != "P2"){
		fprintf(stderr, "Bad PGM file -- first word is not P2\n");
		return -1;
	}else if(column <= 0){
		fprintf(stderr, "Bad PGM file -- No column specification\n");
		return -1;
	}else if(row <= 0){
		fprintf(stderr, "Bad PGM file -- No row specification\n");
		return -1;
	}else if(max != 255){
		fprintf(stderr, "Bad PGM file -- No 255 following the rows and columns\n");
		return -1;
	}

	// Reads in all pixels checking if they are valid
	while(cin >> pixel){
		if(i >= (row * column)){
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
		// Keeps track of the total value of pixels
		sum += pixel;
	}
	
	// Calculates the average
	average = (double)sum / (double)(row * column);

	// Prints out the info
	printf("# Rows: %11d\n", row);
	printf("# Columns: %8d\n", column);
	printf("# Pixels: %9d\n", column * row);
	printf("Avg Pixel:  %7.3f\n", average);
}
