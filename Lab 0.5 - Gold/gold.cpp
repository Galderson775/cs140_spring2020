// Robert Schenck
// Lab 0.5
// 1/15/2019
#include <iostream>
#include <string>
#include <cstdio>

using namespace std;

int main(){

	string input;
	int gold = 0;

	while(true){
		if(cin >> input){
			for(int i = 0; i < input.length(); i++){
				if(input[i] <= 'Z' && input[i] >= 'A'){
					gold += (input[i] - 'A') + 1;			
				}
			}
		}else{
			break;
		}
	}
	cout << gold << '\n';

}

