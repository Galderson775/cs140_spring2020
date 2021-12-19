/*
 * INSTRUCTOR-GIVEN TEMPLATE
 * COSC 140: Lab 8 - A Doubly-Linked List Implementation for Strings
 *
 * Description:
 *     Implements a doubly linked list to store std::strings. It uses a
 *     sentinel node and pointers to ensure bi-directional traversal. It is
 *     also circular, as the sentinel has access to the last and first
 *     elements.
 *
 * Author:
 *     Robert Schenck
 */

#include <iostream>
#include "dlist.hpp"

using namespace std;
// ----------------------------------------------------------------------------
// Constructors & Destructors                                              {{{1
// ----------------------------------------------------------------------------

/*
 * Dlist()                                                                 {{{2
 *
 * Default Constructor
 */

Dlist::Dlist() {
	
	size = 0;
	sentinel = new Dnode;
	sentinel->s = "";
	sentinel->flink = sentinel;
	sentinel->blink = sentinel;
}

/*
 * Dlist(const Dlist &d)                                                   {{{2
 *
 * Copy Constructor. This is a "deep copy". In other words, elements are copied
 * over from "d" by making Dnodes and coping over the elements, one-by-one.
 * They are not copied by copying pointers.
 */

Dlist::Dlist(const Dlist &d) {

	size = 0;
	sentinel = new Dnode;
	sentinel->s = "";
	sentinel->flink = sentinel;
	sentinel->blink = sentinel;
	*this = d;
}

/*
 * operator=                                                               {{{2
 *
 * List copy operator (e.g. Assuming Dlist a, b... do "a = b"). Like the copy
 * constructor, this is also a "deep copy", not a "shallow copy".
 */

Dlist& Dlist::operator= (const Dlist &d) {
	
	Clear();
	Dnode *temp = new Dnode;
	for(temp = d.Begin(); temp != d.End(); temp = temp->flink){
		Push_Back(temp->s);
	}
	return *this;
}

/*
 * ~Dlist                                                                  {{{2
 *
 * Destructor
 */

Dlist::~Dlist() {

	Clear();
	delete sentinel;

}

// ----------------------------------------------------------------------------
// Getters                                                                 {{{1
// ----------------------------------------------------------------------------


/*
 * Empty                                                                   {{{2
 *
 * Returns TRUE if the list is empty. FALSE otherwise.
 */

bool Dlist::Empty() const {
	if(size == 0){
		return true;
	}else{
		return false;
	}
}

/*
 * Size                                                                    {{{2
 *
 * Returns the number of nodes in the list (excluding the sentinel).
 */

size_t Dlist::Size() const {
	return size;
}

// ----------------------------------------------------------------------------
// Non-Front/Back Insertion Functions                                      {{{1
// ----------------------------------------------------------------------------

/*
 * Insert_Before                                                           {{{1
 *
 * Inserts a new node with string "s" before node "n" in the list.
 *
 * e.g. Assume list L as [S] -> [a] -> [b] -> [c].
 *      Assume n points to "b".
 *      Insert_Before("d", n)...
 *                       [S] -> [a] -> [d] -> [b] -> [c].
 */

void Dlist::Insert_Before(const string &s, Dnode *n) {

	Dnode *newNode = new Dnode;

	newNode->s = s;
	newNode->flink = n;
	newNode->blink = n->blink;
	newNode->blink->flink = newNode;
	n->blink = newNode;
	size++;

}

/*
 * Insert_After                                                            {{{1
 *
 * Inserts a new node with string "s" after node "n" in the list.
 *
 * e.g. Assume list L as [S] -> [a] -> [b] -> [c].
 *      Assume n points to "b".
 *      Insert_After("d", n)...
 *                       [S] -> [a] -> [b] -> [d] -> [c].
 */

void Dlist::Insert_After(const string &s, Dnode *n) {

	 Dnode *newNode = new Dnode;

	 newNode->s = s;
	 newNode->flink = n->flink;
	 newNode->blink = n;
	 newNode->flink->blink = newNode;
	 n->flink = newNode;
	 size++;
}

// ----------------------------------------------------------------------------
// Deletion Handlers                                                       {{{1
// ----------------------------------------------------------------------------

/*
 * Erase                                                                   {{{2
 *
 * Deletes node "n" from the list.
 */

void Dlist::Erase(Dnode *n) {

	n->blink->flink = n->flink;
	n->flink->blink = n->blink;
	size--;
	delete n;
}

/*
 * Clear                                                                   {{{2
 *
 * Clears the list of all elements except the sentinel node.
 */

void Dlist::Clear() {

	while(!Empty()){
		Erase(sentinel->flink);
	}
}

// ----------------------------------------------------------------------------
// Push Functions                                                          {{{1
// ----------------------------------------------------------------------------

/* Put new strings on the front or back of the list */

/*
 * Push_Front                                                              {{{2
 *
 * Puts a new string on the front of the list.
 *
 * e.g. Assume list L as [S] -> [a] -> [b] -> [c].
 *      Push_Front(d)... [S] -> [d] -> [a] -> [b] -> [c].
 */

void Dlist::Push_Front(const std::string &s) {

	Insert_After(s, Rend());
	
}

/*
 * Push_Back                                                               {{{2
 *
 * Puts a new string at the end of the list.
 *
 * e.g. Assume list L as [S] -> [a] -> [b] -> [c].
 *      Push_Back(d)...  [S] -> [a] -> [b] -> [c] -> [d].
 */

void Dlist::Push_Back(const std::string &s) {

	Insert_Before(s, End());
	
}

// ----------------------------------------------------------------------------
// Pop Functions                                                           {{{1
// ----------------------------------------------------------------------------

/* Remove and return the first or last element of the list */

/*
 * Pop_Front                                                               {{{2
 *
 * Removes the first node from the list and returns its value.
 *
 * e.g. Assume list L as [S] -> [a] -> [b] -> [c].
 *      Pop_Front()...   [S] -> [b] -> [c].
 */

string Dlist::Pop_Front() {

	string value = Begin()->s;
	Erase(Begin());
	return value;

}

/*
 * Pop_Back                                                                {{{2
 *
 * Removes the last node from the list and returns its value.
 *
 * e.g. Assume list L as [S] -> [a] -> [b] -> [c].
 *      Pop_Back()...    [S] -> [a] -> [b].
 */

string Dlist::Pop_Back() {

	string value = Rbegin()->s;
	Erase(Rbegin());
	return value;

}

// ----------------------------------------------------------------------------
// Node Accessor Functions                                                 {{{1
// ----------------------------------------------------------------------------

/*
 * Begin                                                                   {{{2
 *
 * Returns a pointer to the first node on the list.
 */

Dnode *Dlist::Begin() const {
	return sentinel->flink;
}

/*
 * End                                                                     {{{2
 *
 * Returns a pointer to the node past the last node on the list.
 */

Dnode *Dlist::End() const {
	return sentinel;
}

/*
 * Rbegin                                                                  {{{2
 *
 * Returns a pointer to the last node on the list.
 */

Dnode *Dlist::Rbegin() const {
	return sentinel->blink;
}

/*
 * Rend                                                                    {{{2
 *
 * Returns a pointer to the node before the first node on the list.
 */

Dnode *Dlist::Rend() const {
	return sentinel;
}
