/*
 * INSTRUCTOR-GIVEN TEMPLATE
 * COSC 140: Lab 6 - Candy Crush
 *
 * Description:
 *     Implements a 1D variation of Candy Crush where you crush candy and all
 *     adjacent candies of the same kind.
 *
 * Author:
 *     Robert Schenck
 */

//Our secret weapon
#include <sstream>
#include <list>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include "candyCrush.hpp"

using namespace std;

// ----------------------------------------------------------------------------
// CandyCrush Class Methods                                                {{{1
// ----------------------------------------------------------------------------

/*
 * candyCrush(const string &)                                              {{{2
 *
 * Sets up the candyCrush instance via a file at "inputFile". This file will
 * specify the rules of the game, as well as points given for a specific number
 * of candies being broken in a row.
 */

candyCrush::candyCrush(const string &inputFile) {
	// Variables
	int seed;
	ifstream fin;
	istringstream ss;
	string line;
	string firstFlavor;
	int temp;
	string temp2;
	int totalProb = 0;
	int lineCount = 1;

	// Open input file and check for bad file
	fin.open(inputFile.c_str());
	if(fin.fail()){
		fprintf(stderr, "/home/bvanderz/cs140/Labs/Lab6/Gradescript-Examples/input/052.dat: No such file or directory\n");
		exit(0);
	}

	// reads in seed and checks for bad seed
	if(!(fin >> seed)){
		fin >> temp2;
		fprintf(stderr, "line %d: Bad seed for the random number generator\n\terroneous input was: bvz\n", lineCount);
		exit(0);
	}
	// seeds the rand() function
	srand(seed);

	// read ins row length and checks for bad input
	if(!(fin >> rowLength)){
		fprintf(stderr, "line %d: Bad row length--must be an integer\n\terroneous input was: plank\n", lineCount);
		exit(0);
	}else if(rowLength < 1 || rowLength > 100){
		fprintf(stderr, "line %d: The row length you entered, %d, must be from 1-100\n", lineCount, rowLength);
		exit(0);
	}
	lineCount++;
	// gets the first flavor cause weird stuff
	fin >> firstFlavor;
	// gets the rest of the flavors
	getline(fin, line);
	ss.clear();
	ss.str(line);
	flavors.push_back(firstFlavor);

	// pushes back the flavors
	while(ss >> temp2){
		flavors.push_back(temp2);
	}

	lineCount++;
	// reads in all probabilities and point values while checking for bad input
	while(fin >> temp){
		if(temp > 100 || temp < 0){
			fprintf(stderr, "Line %d: The probability you entered, %d, must be 0-100\n", lineCount, temp);
			exit(0);
		}
		totalProb += temp;
		temp = 0;
		if(totalProb > 100){
			fprintf(stderr, "Line %d: The cumulative probability exceeds 100. It must be exactly 100 when all probabilities have been entered\n", lineCount);
			exit(0);
		}
		probabilities.push_back(totalProb);
		fin >> temp;
		lineCount++;
		if(temp < 1){
			fprintf(stderr, "Line %d: points, %d, is negative. It must be non-negative\n", lineCount, temp);
			exit(0);
		}
		points.push_back(temp);
	}
	// checks if total prob is less than 100
	if(totalProb < 100){
		fprintf(stderr, "The probabilities you entered must sum to 100 but their sum was %d\n", totalProb);
		exit(0);
	}

	// checks the validity of the amount of prob and points added
	if(probabilities.size() < rowLength || points.size() < rowLength){
		fprintf(stderr, "You entered %d pairs of probabilities and points. You must enter the same number of pairs as the rowLength, which is %d\n", probabilities.size(), rowLength);
		exit(0);
	}
	// close file
	fin.close();
	// fill candy list
	fillCandyList();

}

list<string> candyCrush::fillCandyList(){

	int candyNum;
	int probNum;

	while(candy.size() != rowLength){
		// determines which candy to use
		candyNum = rand() % flavors.size();
		// determines how many candies to use
		probNum = rand() % 100;
		// sees how many candies to add
		for(unsigned int i = 0; i < probabilities.size(); i++){
			if(probNum < probabilities[i]){
				candy.resize(candy.size() + (i + 1), flavors[candyNum]);
				probNum = 101;
			}
		}
		// if list is bigger than row length then shrink it
		if(candy.size() > rowLength){
			candy.resize(rowLength);
		}
	}
	return candy;
}

/*
 * getRowLength                                                            {{{2
 *
 * Gets the length of amount of candy in the row...
 */

int candyCrush::getRowLength() const {
	return rowLength;
}

/*
 * getScore                                                                {{{2
 *
 * Gets the current score in-game.
 */

int candyCrush::getScore() const {
	return score;
}

/*
 * printCandy                                                              {{{2 
 * Prints candy to stdout.
 */

void candyCrush::printCandy() const {
	list<string>::const_iterator it;
	int counter = 0;

	for(it = candy.begin(); it != candy.end(); it++){
		printf("%-10s", it->c_str());
		counter++;
		if(counter == 8){
			printf("\n");
			counter = 0;
		}
	}
	printf("\n");

}

/* 
 * play                                                                    {{{2
 *
 * Make a play in the game. The candy destroyed is determined by "choice".
 */

int candyCrush::play(int choice) {

	int scoreThisTurn;
	list<string>::iterator left;
	list<string>::iterator current = candy.begin();
	list<string>::iterator right;

	// increments current iterator to player choice
	for(unsigned int i = 0; i < choice; i++){
		current++;
	}

	// searches for first candy that does not match the candy the user chose left of the current iterator
	for(left = current; left != candy.begin(); left--){
		if(left->c_str() != current->c_str()){
			left++;
			break;
		}
	}

	// searches for first cady that does not match the candy the user chose right of the current iterator
	for(right = current; right != candy.end(); right++){
		if(right->c_str() != current->c_str()){
			break;
		}
	}
	// deletes candies
	candy.erase(left, right);
	// scores this turn
	scoreThisTurn = points[(rowLength - candy.size()) - 1];
	// adds to total point value
	score += scoreThisTurn;
	// fills list of candy
	fillCandyList();
	return scoreThisTurn;

}

