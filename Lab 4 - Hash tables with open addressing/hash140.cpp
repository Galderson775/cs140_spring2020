// Robert Schenck
// Lab 4

#include <string>
#include <vector>
#include <iostream>
#include <cstdio>
#include <sstream>
#include "hash140.h"
#include <stdlib.h>

using namespace std;

// Constructor
HashTable::HashTable(int table_size, string function, string collision){
	if(function == "Last7"){
		Fxn = 0;
	}else if(function == "XOR"){
		Fxn = 1;
	}

	if(collision == "Linear"){
		Coll = 0;
	}else if(collision == "Double"){
		Coll = 1;
	}

	nkeys = 0;
	tmp = 0;
	vector<string> keyst(table_size);
	vector<string> valst(table_size);
	keys = keyst;
	vals = valst;
}

// Gets the last 7 digits of the key and returns the hexadecimal value
unsigned int Last7(string &key){
	istringstream ss;
	unsigned int index;

	if(key.length() > 7){
		ss.clear();
		ss.str(key.substr(key.length() - 8, 7));
		ss >> index;
	}else{
		ss.clear();
		ss.str(key);
		ss >> index;
	}
	return index;
}

// XOR the entire key in 7 digit increments
unsigned int XOR(string &key){
	istringstream ss;
	unsigned int index;
	unsigned int xorTotal = 0;

	if(key.length() > 7){
		for(unsigned int i; i < key.length() - 1; i+=7){
			if((key.length() - 1) - i < 0){
				ss.clear();
				ss.str(key.substr(i, i + 7));
			}else{
				ss.clear();
				ss.str(key);
			}
			ss >> index;
			xorTotal = xorTotal ^ index;
		}
	}
}

// Returns the total number of probes
int HashTable::Total_Probes(){
	return tmp;
}

// Prints out the hash table
void HashTable::Print(){
	for(unsigned int i = 0; i < keys.size(); i++){
		if(vals[i] != ""){
			cout << "    " << i + 1 << ' ' << keys[i] << ' ' << vals[i] << '\n';
		}
	}
}

// Finds a value in the hash table given a key
string HashTable::Find(string &key){
	tmp++;
	for(unsigned int i = 0; i < keys.size(); i++){
		if(key == keys.at(i)){
			return vals[i];
		}
	}
	return "";
}

// Adds a value to the hash table
void HashTable::Add_Hash(string & key, string &val){
	int keyInserted = 0;
	unsigned int index = 0;
	unsigned int index2 = 0;
	
	if(Fxn == 0){
		index = Last7(key);
	}else if(Fxn == 1){
		index = XOR(key);
	}

	if(nkeys == keys.size()){
		cerr << "Hash Table Full\n";
		exit(420);
	}

	if(Coll == 0){
		for(unsigned int i = 0; i < ((keys.size() - 1) - (index % 10)); i++){
			tmp++;
			if(keys.at((index % 10) + i) == ""){
				keys.at((index % 10) + i) =  key;
				vals.at((index % 10) + i) = val;
				keyInserted = 1;
				nkeys++;
				break;
			}
			if(keyInserted == 0){
				cerr << "Couldn't put " << key << " into the table\n";
				exit(420);
			}
		}
	}else if(Coll == 1){
		if(Fxn == 0){
			index2 = XOR(key);
		}else if(Fxn == 1){
			index2 = Last7(key);
		}

		if(index2 % keys.size() == 0){
 		index2 = 1;
		}
		for(unsigned int i = 0; i < keys.size() - 1; i++){
			tmp++;
			if(keys.at((index + (index2 * i)) % keys.size()) == ""){
				keys.at((index + (index2 * i)) % keys.size()) = key;
				vals.at((index + (index2 * i)) % keys.size()) = val;
				keyInserted = 1;
				nkeys++;
				break;
			}
		}
		if(keyInserted == 0){
			cerr << "Couldn't put " << key << " into the table\n";
			exit(420);
		}
	}
}
