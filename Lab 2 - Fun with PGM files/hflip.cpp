// Robert Schenck
// Lab 2 hflip
// 1/28/2020

#include <iostream>
#include <cstdio>
#include <vector>

using namespace std;

int main(){

	// variable declarations
	string header;
	int row, col, max, extra;
	int i = 0;
	vector<vector<int> > pixels;
	
	// gets file header and info
	cin >> header >> col >> row >> max;

	// checks if file header and info are valid
	if(header != "P2"){
		fprintf(stderr, "Bad PGM file -- first word is not P2\n");
		return -1;
	}else if(col <= 0){
		fprintf(stderr, "Bad PGM file -- No column specification\n");
		return -1;
	}else if(row <= 0){
		fprintf(stderr, "Bad PGM file -- No row specification\n");
		return -1;
	}else if(max != 255){
		fprintf(stderr, "Bad PGM file -- No 255 following the rows and columns\n");
		return -1;
	}

	// resizes 2D vector to appropriate size 
	pixels.resize(row);
	for(int i = 0; i < pixels.size(); i++){
		pixels.at(i).resize(col);
	}

	// reads in all pixels checking if they're valid
	for(int r = 0; r < pixels.size(); r++){
		for(int c = 0; c < pixels.at(r).size(); c++){
			cin >> pixels.at(r).at(c);
			if(pixels.at(r).at(c) > max || pixels.at(r).at(c) < 0){
				fprintf(stderr, "Bad PGM file -- pixel %d is not a number between 0 and 255\n", i);
				return -1;
			}else if(cin.fail()){
				fprintf(stderr, "Bad PGM file -- pixel %d is not a number between 0 and 255\n", i);
				return -1;
			}
			i++;
		}
	}

	if(cin >> extra){
		fprintf(stderr, "Bad PGM file -- Extra stuff after the pixels\n");
		return -1;
	}

	// prints header and file info
	cout << "P2\n" << col << " " << row << '\n' << max << '\n';

	// prints pixels flipped across y axis
	for(int r = 0; r < pixels.size(); r++){
		for(int c = 0; c < pixels.at(r).size(); c++){
			cout << pixels.at(r).at(col - 1 - c ) << '\n';
		}
	}
}
