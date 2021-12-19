// Robert Schenck
// Lab 1
// 1/21/2020
#include <iostream>
#include <string>

using namespace std;

int main(){
	
	double i;
	double count = 0;
	double score = 0;
	double average = 0;
	string badInput;
	string name;

	while(true){
		if(cin >> i){
			score += i;
		}else if(cin.eof()){
			break;
		}else{
			cin.clear();
			cin >> badInput;
			if(badInput == "NAME"){
				cin >> name;
			}else if(badInput == "AVERAGE"){
				while(true){
					if(cin >> i){
						average += i;
						count++;
					}else{
						break;
					}
				}
				if(average > 0){
					average /= count;
					count = 0;
					score += average;
					average = 0;
				}
			}
		}
	}
	cout << name << " " << score << '\n';

}
