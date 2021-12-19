// Robert Schenck
// Lab 3 Intermediate
// 2/5/2020
#include "pgm.hpp"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

bool Pgm::Read(const std::string &file){

	ifstream fin;
	string s;
	size_t i, j, r, c, v;

	fin.open(file.c_str());
	if (fin.fail()) return false;
  
	if (!(fin >> s)) return false;
	if (s != "P2") return false;
	if (!(fin >> c) || c <= 0) return false;
	if (!(fin >> r) || r <= 0) return false;
	if (!(fin >> i) || i != 255) return false;

	Pixels.resize(r);
	for (i = 0; i < r; i++) {
		Pixels[i].clear();
		for (j = 0; j < c; j++) {
			if (!(fin >> v) || v > 255) return false;
				Pixels[i].push_back(v);
		}
	}
	if (fin >> s) return false;
	fin.close();
	return true;
}

bool Pgm::Write(const std::string &file) const{

	// Varaibles
	ofstream fout;
	int i = 1;

	// Error checking
	if(Pixels.size() <= 0){
		return false;
	}else if (Pixels.at(0).size() <= 0){
		return false;
	}

	// Opens file to write to
	fout.open(file.c_str());
	if(fout.fail()){
		return false;
	}

	// Prints out header of pgm file
	fout << "P2" << '\n' << Pixels.at(0).size() << " " << Pixels.size() << '\n' << 255 << '\n';
	// Loops through the 2d vector and prints out the pixels
	for(unsigned int r = 0; r < Pixels.size(); r++){
		for(unsigned int c = 0; c < Pixels.at(r).size(); c++){
			if(i % 20 == 0 || (r == Pixels.size() - 1 && c == Pixels.at(r).size() - 1)){
				fout << Pixels.at(r).at(c) << '\n';
			}else{
				fout << Pixels.at(r).at(c) << " ";
			}
			i++;
		}
	}
	// Closes file and returns true
	fout.close();
	return true;
}

bool Pgm::Create(size_t r, size_t c, size_t pv){
	// Error checking
	if(pv > 255){
		return false;
	}
	// Resizes 2d vector with given parameters
	Pixels.clear();
	Pixels.resize(r, vector<int>(c, pv));
	return true;
}

bool Pgm::Clockwise(){
	// Error checking
	if(Pixels.size() == 0 || Pixels.at(0).size() == 0){
		return false;
	}
	// Temporary vectors
	vector<vector<int> > temp1;
	vector<vector<int> > temp2;
	vector<int> tempPixel;
	// Signed integer for comparisons
	int signedRow = Pixels.size();
	// rotates the image
	for(unsigned int i = 0; i < Pixels.at(0).size(); i++){
		for(unsigned int j = 0; j < Pixels.size(); j++){
			tempPixel.push_back(Pixels.at(j).at(i));
		}
		temp1.push_back(tempPixel);
		tempPixel.clear();
	}
	// flips the image
	for(unsigned int i = 0; i < Pixels.at(0).size(); i++){
		for(int j = signedRow - 1; j >= 0; j--){
			tempPixel.push_back(temp1.at(i).at(j));
		}
		temp2.push_back(tempPixel);
		tempPixel.clear();
	}
	// overwrites pixels with the flipped and rotated image
	Pixels = temp2;
	return true;
}

bool Pgm::Cclockwise(){
	// Calls clockwise 3 times
	for(int i = 0; i < 3; i++){
		if(!Clockwise()){
			return false;
		}
	}
	return true;

}

bool Pgm::Pad(size_t w, size_t pv){
	// Error checking
	if(pv > 255){
		return false;
	}
	// Creates a temporary 2d vector to create the padding
	vector<vector<int> > temp;

	// Resizes the temporary 2d vector with the given additional width and height
	temp.clear();
	temp.resize(Pixels.size() + (2 * w), vector<int>(Pixels.at(0).size() + (2 * w), pv));
	// Recreates image in the middle of new vector
	for(unsigned int rows = 0; rows < Pixels.size(); rows++){
		for(unsigned int columns = 0; columns < Pixels.at(0).size(); columns++){
			temp.at(rows + w).at(columns + w) = Pixels.at(rows).at(columns);
		}
	}
	// Sets pixel vector equal to temp vector
	Pixels = temp;
	return true;
}

bool Pgm::Panel(size_t r, size_t c){
	// Error checking
	if(r <= 0 || c <= 0){
		return false;
	}
	// Stores original width and height
	unsigned int width = Pixels.at(0).size();
	unsigned int height = Pixels.size();
	
	// Create a temporary 2d vector to create the padding
	vector<vector<int> > temp;

	// Resizes the temp 2d vector with the given additional width and height
	temp.clear();
	temp.resize(height * r, vector<int>(width * c));

	// Creates the image in the new temp vector col * row numbers of times
	for(unsigned int i = 0; i < temp.size(); i++){
		for(unsigned int j = 0; j < temp.at(0).size(); j++){
			temp.at(i).at(j) = Pixels.at(i % height).at(j % width);
		}
	}

	// Sets pixel vector equal to temp vector
	Pixels = temp;
	return true;
}

bool Pgm::Crop(size_t r, size_t c, size_t rows, size_t cols){
	// Error checking
	if(Pixels.size() == 0 || Pixels.at(0).size() == 0){
		return false;
	}
	if((r + rows) > Pixels.size() || (c + cols) > Pixels.at(0).size()){
		return false;
	}

	// temporary 2D array
	vector<vector<int> > temp;

	// resizes the array to the size of the new image
	temp.clear();
	temp.resize(rows, vector<int>(cols));
	// crops the image
	for(unsigned int i = 0; i < rows; i++){
		for(unsigned int j = 0; j < cols; j++){
			temp.at(i).at(j) = Pixels.at(i + r).at(j + c);
		}
	}

	// overwrites pixel array with temp array
	Pixels = temp;
	return true;
}
        
