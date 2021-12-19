/*
 * INSTRUCTOR-GIVEN TEMPLATE
 * COSC 140: Lab 7 - Code Processing
 *
 * Description:
 *     Implements a fake interface for allowing customers to redeem points,
 *     kinda like mycokerewards (or rather, what it "was" like). This is to
 *     give the students flexibility with sets and maps, as well as pointers.
 *
 * Author:
 *     Robert Schenck
 */

#include <string>
#include <fstream>
#include <map>
#include <set>
#include "code_processor.hpp"

using namespace std;

// ----------------------------------------------------------------------------
// Code_Processor Function Definitions                                     {{{1
// ----------------------------------------------------------------------------


unsigned int djb_hash(const string &s){
	
	size_t i;
	unsigned int h;

	h = 5381;

	for(i = 0; i < s.size(); i++){
		h = (h << 5) + h + s[i];
	}
	return h;
}

/*
 * New_Prize                                                               {{{2
 *
 * Makes a new prize and inserts it into Prizes
 */

bool Code_Processor::New_Prize(
	const string &id,
	const string &description,
	int points,
	int quantity
) {
	if(Prizes.find(id) != Prizes.end() || points <= 0 || quantity <= 0){
		return false;
	}
	Prize *obj = new Prize;

	obj->id          = id;
	obj->description = description;
	obj->points      = points;
	obj->quantity    = quantity;

	Prizes.insert(make_pair(id, obj));

	return true;
}

/*
 * New_User                                                                {{{2
 *
 * Creates a new user and inserts it into Names
 */

bool Code_Processor::New_User(
	const string &username,
	const string &realname,
	int starting_points
) {
	if(Names.find(username) != Names.end() || starting_points < 0){
		return false;
	}
	User *obj = new User;
	obj->username = username;
	obj->realname = realname;
	obj->points = starting_points;

	Names.insert(make_pair(username, obj));

	return true;
}

/*
 * Delete_User                                                             {{{2
 *
 * Erases user from Names and erase all of the user's phone numbers from Phones
 */

bool Code_Processor::Delete_User(const string &username) {

	if(Names.find(username) == Names.end()){
		return false;
	}
	User *user = Names.find(username)->second;
	Names.erase(Names.find(username));

	set<string>::iterator it;
	for(it = user->phone_numbers.begin(); it != user->phone_numbers.end(); it++){
		if(Phones.find(*it) != Phones.end()){
			Phones.erase(Phones.find(*it));
		}
	}
	delete user;
	return true;
}


/*
 * Add_Phone                                                               {{{2
 *
 * Registers a phone number with a specified user
 */

bool Code_Processor::Add_Phone(
	const string &username,
	const string &phone
) {
	if(Names.find(username) == Names.end()){
		return false;
	}else if(Phones.find(phone) != Phones.end()){
		return false;
	}
	
	Names.find(username)->second->phone_numbers.insert(phone);
	Phones.insert(make_pair(phone, Names.find(username)->second));

	return true;
}

/*
 * Remove_Phone                                                            {{{2
 *
 * Removes a user's phone number from the system
 */

bool Code_Processor::Remove_Phone(
	const string &username,
	const string &phone
) {
	if(Names.find(username) == Names.end()){
		return false;
	}else if(Phones.find(phone) == Phones.end() || Phones.find(phone)->second->username != username){
		return false;
	}
	Phones.erase(Phones.find(phone));
	Names.find(username)->second->phone_numbers.erase(Names.find(username)->second->phone_numbers.find(phone));
	return true;
}

/*
 * Show_Phones                                                             {{{2
 *
 * Shows all the phone numbers associated with the given user
 */

string Code_Processor::Show_Phones(const string &username) const {

	string result = "";
	set<string>::iterator it;
	if(Names.find(username) == Names.end()){
		return "BAD USER";
	}
	set<string> temp = Names.find(username)->second->phone_numbers;
	for(it = temp.begin(); it != temp.end(); it++){
		result += *it;
		result += '\n';
	}
	return result;
}


/*
 * Enter_Code                                                              {{{2
 *
 * User enters a code, use hashing to check if the code is valid and add 10 or 3 points depending on if the codes hash is divisible by 17 or 13
 */

int Code_Processor::Enter_Code(
	const string &username,
	const string &code
) {
	if(Names.find(username) == Names.end()){
		return -1;
	}else if(Codes.find(code) != Codes.end()){
		return -1;
	}

	unsigned int hashedNum = djb_hash(code);
	if(hashedNum % 17 == 0){
		Codes.insert(code);
		Names.find(username)->second->points += 10;
		return 10;
	}else if(hashedNum % 13 == 0){
		Codes.insert(code);
		Names.find(username)->second->points += 3;
		return 3;
	}else{
		return 0;
	}
}

/*
 * Text_Code                                                               {{{2
 *
 * Same as enter code but the users account is identified by number instead of username
 */

int Code_Processor::Text_Code(
	const string &phone,
	const string &code
) {
	if(Phones.find(phone) == Phones.end()){
		return -1;
	}
	if(Codes.find(code) != Codes.end()){
		return -1;
	}
	unsigned int hashedNum = djb_hash(code);
	if(hashedNum % 17 == 0){
		Codes.insert(code);
		Phones.find(phone)->second->points += 10;
		return 10;
	}else if(hashedNum % 13 == 0){
		Codes.insert(code);
		Phones.find(phone)->second->points += 3;
		return 3;
	}else{
		return 0;
	}
}

/*
 * Mark_Code_Used                                                          {{{2
 *
 * Marks a code as used if it is valid and is not in Codes
 */

bool Code_Processor::Mark_Code_Used(const string &code) {

	unsigned int hashedNum = djb_hash(code);
	bool notUsed;
	if(hashedNum % 17 == 0 || hashedNum % 13 == 0){
		notUsed = true;
	}else{
		notUsed = false;
	}
	if(Codes.find(code) != Codes.end() || notUsed == false){
		return false;
	}
	Codes.insert(code);
	return true;
}


/*
 * Balance                                                                 {{{2
 *
 * returns a user's balance
 */

int Code_Processor::Balance(const string &username) const {

	if(Names.find(username) == Names.end()){
		return -1;
	}else{
		return Names.find(username)->second->points;
	}
}

/*
 * Redeem_Prize                                                            {{{2
 *
 * redeems a prize for the user if the user has enough points
 */

bool Code_Processor::Redeem_Prize(
	const string &username,
	const string &prize
) {

	if(Names.find(username) == Names.end()){
		return false;
	}else if(Prizes.find(prize) == Prizes.end()){
		return false;
	}else if(Names.find(username)->second->points < Prizes.find(prize)->second->points){
		return false;
	}
	Names.find(username)->second->points -= Prizes.find(prize)->second->points;
	Prizes.find(prize)->second->quantity -= 1;
	if(Prizes.find(prize)->second->quantity == 0){
		Prizes.erase(Prizes.find(prize));
	}
	return true;
}


/*
 * ~Code_Processor                                                         {{{2
 *
 * Destructor to call delete on users and prizes
 */

Code_Processor::~Code_Processor() {

	map<string, User *>::iterator userIt;
	map<string, Prize *>::iterator prizeIt;
	for(userIt = Names.begin(); userIt != Names.end(); userIt++){
		delete userIt->second;
	}
	for(prizeIt = Prizes.begin(); prizeIt != Prizes.end(); prizeIt++){
		delete prizeIt->second;
	}
}

/*
 * Write                                                                   {{{2
 *
 * Writes the state of the program to a file
 */

bool Code_Processor::Write(const string &filename) const {

	ofstream out;
	out.open(filename.c_str());
	if(!(out.is_open())){
		return false;
	}
	map<string, Prize *>::const_iterator prizeIt;
	map<string, User *>::const_iterator namesIt;
	map<string, User *>::const_iterator phonesIt;
	set<string>::const_iterator numbersIt;
	set<string>::const_iterator codesIt;
	for(prizeIt = Prizes.begin(); prizeIt != Prizes.end(); prizeIt++){
		out << "PRIZE " << prizeIt->second->id << " " << prizeIt->second->points << " " << prizeIt->second->quantity << " " << prizeIt->second->description << '\n';
	}
	for(namesIt = Names.begin(); namesIt != Names.end(); namesIt++){
		out << "ADD_USER " << namesIt->second->username << " " << namesIt->second->points << " " << namesIt->second->realname << '\n';
	}
	for(phonesIt = Phones.begin(); phonesIt != Phones.end(); phonesIt++){
		for(numbersIt = phonesIt->second->phone_numbers.begin(); numbersIt != phonesIt->second->phone_numbers.end(); numbersIt++){
			out << "ADD_PHONE " << phonesIt->second->username << " " << *numbersIt << '\n';
		}
	}
	for(codesIt = Codes.begin(); codesIt != Codes.end(); codesIt++){
		out << "MARK_USED " << *codesIt << '\n';
	}
	out.close();
	return true;
}

