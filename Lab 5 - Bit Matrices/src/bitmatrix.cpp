#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include "bitmatrix.hpp"
using namespace std;

// Creates the bitmatrix
Bitmatrix::Bitmatrix(int rows, int cols)
{
	if(rows <= 0){
		throw((string) "Bad rows");
	}else if(cols <= 0){
		throw((string) "Bad cols");
	}
	M.clear();
	M.resize(rows, string(cols, '0'));

}
 
// Reads a bitmatrix in from a file
Bitmatrix::Bitmatrix(const string &fn)
{
	cout << "read" << fn << '\n';
	ifstream fin;
	string line, real_line;
	int first, sz;
	unsigned int i;

	fin.open(fn.c_str());
	first = -1;

	if(!fin){
		throw string("Can't open file");
	}
	while(getline(fin, line)){

		sz = 0;
		real_line = "";

		if(line.size() == 0){
			continue;
		}

		for(i = 0; i < line.size(); i++){
			switch(line[i]){
				case ' ':
					break;
				case '0':
				case '1':
					sz++;
					real_line += line[i];
					break;
				default:
					fin.close();
					throw string("Bad file format");
			}
		}

		if(first == -1){
			first = sz;
		}else if(sz != first){
			fin.close();
			throw string("Bad file format");
		}

		M.push_back(real_line);
	}
	fin.close();
}
 
// This returns a copy of a bitmatrix as a pointer probably
Bitmatrix *Bitmatrix::Copy() const
{
	Bitmatrix *copy = new Bitmatrix(Rows(), Cols());

	int i, j;
	for(j = 0; j < Rows(); j++){
		for(i = 0; i < Cols(); i++){
			copy ->Set(j, i, Val(j, i));
		}
	}
	return copy;
}
 
// This writes the bitmatrix to a file
bool Bitmatrix::Write(const string &fn) const
{
	FILE *fout;
	unsigned int i;

	fout = fopen(fn.c_str(), "w");

	if(fout == NULL){
		return false;
	}

	for(i = 0; i < (unsigned int)Rows(); i++){
		fprintf(fout, "%s\n", M[i].c_str());
	}

	fclose(fout);
	return true;
}
 
// This prints the bitmatrix out to standard input
void Bitmatrix::Print(size_t w) const
{
	unsigned int c, r, i, j;

	c = Cols();
	r = Rows();

	for(j = 0; j < r; j++){
		for(i = 0; i < c; i++){
			if(w > 0 && i % w == 0 && i > 0){
				printf(" ");
			}
			printf("%c", Val(j, i));
		}

		if(w > 0 && (j + 1) % w == 0 && j < r - 1){
			printf("\n\n");
		}else{
			printf("\n");
		}
	}
}
 
// This is supposed to write the bitmatrix to a pgm, empahsis on supposed to
bool Bitmatrix::PGM(const string &fn, int p, int border) const
{
	size_t r, c, i, j, k, l, iw, ih;
	size_t x1, x2, y1, y2;
	FILE *fout;

	vector<vector<unsigned char> > cache;

	fout = fopen(fn.c_str(), "w");

	if(fout == NULL){
		return false;
	}

	c = Cols();
	r = Rows();

	iw = border + ((p + border) * c);
	ih = border + ((p + border) * r);

	cache.resize(ih, vector<unsigned char>(iw, 0));

	fprintf(fout, "P2\n%lu %lu\n255\n", iw, ih);
	
	x1 = y1 = border;

	for(j = 0; j < r; j++, y1 += p + border){
		x1 = border;

		for(i = 0; i < c; i++, x1 += p + border){
			x2 = x1 + p;
			y2 = y1 + p;

			for(l = y1; l < y2; l++){
				for(k = x1; k < x2; k++){
					cache[l][k] = (Val(j, i) == '0') ? 0xFF : 0x64;
				}
			}
		}
	}

	for(j = 0; j < ih; j++){
		for(i = 0; i < iw; i++){
			fprintf(fout, "%d\n", cache[j][i]);
		}
	}
	fclose(fout);
	return true;
}

// returns the numbers of rows
int Bitmatrix::Rows() const{

	return M.size();
}
 
// returns the number of columns
int Bitmatrix::Cols() const
{
	if(M.size() == 0){
		return 0;
	}
	return M[0].size();
}
 
// returns a value from a specified location in the bitmatrix
char Bitmatrix::Val(int row, int col) const
{
	if(row > Rows() || col > Cols() || col < 0 || row < 0){
		return 'x';
	}else{
		return M[row][col];
	}
}
 
// sets a value in the bitmatrix at a specified location
bool Bitmatrix::Set(int row, int col, char val){
	if(row > Rows() || col > Cols() || col < 0 || row < 0){
		return false;
	}
  if(val != '0' && val != '1'){  
	  return false;
  }else{
	  M[row][col] = val;
	  return true;
  }
}
 
// this swaps rows in the bitmatrix
bool Bitmatrix::Swap_Rows(int r1, int r2)
{
	string temp;
	if((unsigned int)r1 > M.size() || (unsigned int)r2 > M.size()){
		return false;
	}
	temp = M[r1];
	M[r1] = M[r2];
	M[r2] = temp;
	return true;
}
 
// this XOR's rows in the bitmatrix
bool Bitmatrix::R1_Plus_Equals_R2(int r1, int r2)
{
  if(r1 > Rows() || r2 > Rows()){
	  return false;
  }
  for(unsigned int i = 0; i < M[0].size(); i++){
	  if(M[r1][i] == '0' && M[r2][i] == '0'){
		  M[r1][i] = '0';
	  }else if(M[r1][i] == '0' && M[r2][i] == '1'){
		  M[r1][i] = '1';
	  }else if(M[r1][i] == '1' && M[r2][i] == '0'){
		  M[r1][i] = '1';
	  }else if(M[r1][i] == '1' && M[r2][i] == '1'){
		  M[r1][i] = '0';
	  }
  }
  return true;
}
      
// adds two matrices together
Bitmatrix *Sum(const Bitmatrix *a1, const Bitmatrix *a2)
{
	// error checking
	if(a1->Rows() != a2->Rows() || a1->Cols() != a2->Cols()){
		return NULL;
	}
	// new bitmatrix
	Bitmatrix *temp = new Bitmatrix(a1->Rows(), a1->Cols());
	// adds the two bitmatrices together use XOR logic
	for(int i = 0; i < a1->Rows(); i++){
		  for(int j = 0; j < a1->Cols(); j++){
			  if(a1->Val(i, j) == '0' && a2->Val(i, j) == '0'){
				  temp->Set(i, j, '0');
			  }else if(a1->Val(i, j) == '0' && a2->Val(i, j) == '1'){
				  temp->Set(i, j, '1');
			  }else if(a1->Val(i, j) == '1' && a2->Val(i, j) == '0'){
				  temp->Set(i, j, '1');
			  }else if(a1->Val(i, j) == '1' && a2->Val(i, j) == '1'){
				  temp->Set(i, j, '0');
			  }
		  }
	}
	// returns matrix
	return temp;
}
// multiples two bitmatrices
Bitmatrix *Product(const Bitmatrix *a1, const Bitmatrix *a2)
{
	int total;
	// error checking
	if(a1->Cols() != a2->Rows()){
		return NULL;
	}
	// temp bitmatrix
	Bitmatrix *temp = new Bitmatrix(a1->Rows(), a2->Cols());

	// multiples the bitmatrices together
	for(int i = 0; i < a1->Rows(); i++){
		for(int j = 0; j < a2->Cols(); j++){

			total = 0;

			for(int k = 0; k < a2->Rows(); k++){
				total += (((int)a1->Val(i, k)) - 48) * (((int)a2->Val(k, j)) - 48);
			}

			temp->Set(i, j, (char)((total % 2) + 48));
		}
	}
	// returns bitmatrix
	return temp;
}
// creates new bitmatrix out of given rows
Bitmatrix *Sub_Matrix(const Bitmatrix *a1, const vector <int> &rows)
{
	// temp bitmatrix
	Bitmatrix *temp = new Bitmatrix(rows.size(), a1->Cols());

	// error checking
	if(rows.empty()){
		return NULL;
	}
	for(unsigned int i = 0; i < rows.size(); i++){
		if(rows[i] > a1->Rows() - 1){
			return NULL;
		}
	}
	// creates bitmatrix from rows given
	for(unsigned int i = 0; i < rows.size(); i++){
		for(int j = 0; j < a1->Cols(); j++){
			temp->Set(i, j, a1->Val(rows[i], j));
		}
	}

	// returns bitmatrix
	return temp;

}

// Inverts the bitmatrix
Bitmatrix *Inverse(const Bitmatrix *m)
{
	// error checks if the matrix is a square
	if(m->Cols() != m->Rows()){
		return NULL;
	}
	// creates a copy bitmatrix and an inverse bitmatrix
	Bitmatrix *m_copy = m->Copy();
	Bitmatrix *inv = new Bitmatrix(m->Rows(), m->Cols());
	int index = 0;
	int found = 0;
	// for loops for lower half of bitmatrix
	for(int i = 0; i < inv->Rows(); i++){
		for(int j = 0; j < inv->Cols(); j++){
			if(j == index){
				inv->Set(i, j, '1');
			}else{
				inv->Set(i, j, '0');
			}
		}
		index++;
	}

	for(int i = 0; i < m->Rows(); i++){
		if(m_copy->Val(i, i) == '0'){
			for(int j = i + 1; j < m->Rows(); j++){
				if(m_copy->Val(j, i) == '1'){
					inv->Swap_Rows(i, j);
					m_copy->Swap_Rows(i, j);
					found = 1;
					break;
				}
			}
			if(found == 0){
				return NULL;
			}
			found = 0;
		}
		for(int k = i + 1; k < m->Rows(); k++){
			if(m_copy->Val(k, i) == '1'){
				inv->R1_Plus_Equals_R2(k, i);
				m_copy->R1_Plus_Equals_R2(k, i);
			}
		}
	}
	// for loop for upper half of bitmatrix
	for(int i = m->Rows() - 1; i >= 0; i--){
		for(int j = i + 1; j < m->Rows(); j++){
			if(m_copy->Val(i, j) == '1'){
				inv->R1_Plus_Equals_R2(i, j);
				m_copy->R1_Plus_Equals_R2(i, j);
			}
		}
	}
	// return bitmatrix
	return inv;
}

unsigned int djb_hash(const string &s){

	size_t i;
	unsigned int h;

	h = 5381;
	for(i = 0; i < s.size(); i++){
		h = (h << 5) + h + s[i];
	}
	return h;
}


// construcotr for a hash table
BM_Hash::BM_Hash(int size)
{
	if(size <= 0){
		throw((string) "Bad size");
	}
	Table.resize(size);
}

// stores values in the hash table
bool BM_Hash::Store(const string &key, Bitmatrix *bm)
{
	cout << "store " << key << '\n';
	unsigned int hash, i, sz_out, sz_in;
	HTE _push;

	sz_out = Table.size();
	hash = djb_hash(key);
	vector<HTE> &chain = Table[hash % sz_out];
	sz_in = chain.size();

	for(i = 0; i < sz_in; i++){
		if(chain[i].key == key){
			break;
		}
	}

	if(i == sz_in){
		_push.key = key;
		_push.bm = bm;

		chain.push_back(_push);
	}else{
		return false;
	}
	return true;

}
 
// retreives values from the hash table
Bitmatrix *BM_Hash::Recall(const string &key) const
{
	unsigned int hash, i, sz_out, sz_in;

	sz_out = Table.size();
	hash = djb_hash(key);
	const vector<HTE> &chain = Table[hash % sz_out];
	sz_in = chain.size();

	for(i = 0; i < sz_in; i++){
		if(chain[i].key == key){
			return chain[i].bm;
		}
	}
	return NULL;
}
 
// puts all the values in the hash table into a vector
vector <HTE> BM_Hash::All() const
{
  vector <HTE> rv;
  unsigned int i, j, sz_out, sz_in;

  sz_out = Table.size();

  for(i = 0; i < sz_out; i++){
	  sz_in = Table[i].size();
	  for(j = 0; j < sz_in; j++){
		  rv.push_back(Table[i][j]);
	  }
  }
  return rv;
}
