// Robert Schenck
// 4/10/2019
// Lab 9 part 1

#include <string>
#include <iostream>
#include <stdlib.h>

using namespace std;

void do_enumeration(string &s, int index, int n_ones);

int main(int argc, char *argv[])
{
	string s;
	int length, n_ones;
	length = atoi(argv[1]);
	n_ones = atoi(argv[2]);
	s = "";

	// Error Checking
	if(argc < 3 || argc > 3){
		cerr << "Improper usage -- ./enum [length of strings] [number of ones in each string]\n";
		return 1;
	}

	// Creates beginning string
	for(int i = 0; i < length; i++){
		s += '-';
	}


	do_enumeration(s, 0, n_ones);
}

// Recursive function 
void do_enumeration(string &s, int index, int n_ones){

	// Base case
	if(index == (int)s.size()){
		if(n_ones == 0){
			// Prints out string and returns
			cout << s << '\n';
			return;
		}else{
			return;
		}
	}


	// Traversing to the end of the string
	if(n_ones <= ((int)s.size() - index)){
		s[index] = '0';
		do_enumeration(s, index + 1, n_ones);

		if(n_ones > 0){
			s[index] = '1';
			do_enumeration(s, index + 1, n_ones - 1);
		}
	}else{
		// More ones than elements remaining
		return;
	}
}
