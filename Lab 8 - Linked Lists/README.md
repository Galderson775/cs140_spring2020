# cs140 (Spring 2020)

# Part 1: dlist.cpp
This lab has two parts. The first is straightforward. Write dlist.cpp so that it properly implements all of the methods of the Dlist class, as described in the lecture notes on linked data structures.
Your program should meet the following requirements:

1. You are not allowed to have any STL data structures or header files in your implementation -- you may only include <iostream> <cstdlib> and "dlist.h".
2. The implementation should be as described in the lecture notes: as a circular list with a sentinel node.
3. Your destructor is not allowed to call Erase() or Pop_Back() or Pop_Front(). You should manually delete every node on the list.
4. If your list is empty, then the Begin() and Rbegin() methods should return a pointer to the sentinel node. Since the End() and Rend() methods also return a pointer to the sentinel node, this arrangement will ensure that if you attempt to iterate through an empty list using a for loop, your for loop will immediately terminate without ever executing the loop body.  
The gradescript will test all of the dlistrev-x.cpp programs from the lecture notes, plus the program list_editor that I describe below.
  
# Part 2: dl_hash.cpp
Behold dl_hash.h, which gives the API for a hash table for strings:
```
#include "dlist.h"
#include <vector>
using namespace std;

class DL_Hash {
  public:
    DL_Hash(int size);
    ~DL_Hash();
    void Insert(string &s);
    int Present(string &s);
    void Erase(string &s);
    void Strip_All_Substring(string &s);
    void Print();
  protected:
    vector <Dlist *> table;
};
```
Your job is to implement this class in dl_hash.cpp.

The hash table is going to use separate chaining, using the Dlist type to implement the chains. Please make note that each element of the hash table is a pointer to a list, so you'll have to call "new Dlist" for every element of the table in your constructor. You'll also have to call delete in the destructor.

Insert() inserts a string into the hash table. Use djb_hash() as the hash function. If the string is already there, it should do nothing. Present() returns whether the string is present in the hash table, and Erase() erases the string if it is there. If it's not there, Erase() should do nothing.

Strip_All_Substring() should delete every entry in the hash table that has the given string as a substring. You should not use the Erase() method of the DL_Hash class for this. You should use the Erase() method of the Dlist class.

Finally, Print() prints out the hash table. It does this in order of hash table entry, and for strings that have the same hash value, they are printed in the order in which the elements were inserted into the hash table. The format should be one string per line, where each line contains the hash index, padded to four characters and right justified, then a space and then the string.

To test this, I have written dl_hash_test.cpp which is a simple command-based program which supports INSERT, INSERT, PRESENT, ERASE, STRIP, PRINT and QUIT. Its command line arguments are the table size and a prompt. Using it is straightforward:
```
UNIX> dl_hash_test 50 'DL_Hash>'
DL_Hash> INSERT Thor
DL_Hash> INSERT Binky
DL_Hash> INSERT Brody Masonry
DL_Hash> PRESENT Thor
Yes
DL_Hash> PRESENT Frenchy
No
DL_Hash> PRINT
  42 Binky
  42 Brody Masonry
  46 Thor
DL_Hash> ERASE Thor
DL_Hash> PRINT
  42 Binky
  42 Brody Masonry
DL_Hash> ERASE Thor           -- This does nothing, since Thor is no longer in the table.
DL_Hash> PRINT
  42 Binky
  42 Brody Masonry
DL_Hash> INSERT Binky         -- This does nothing, since Binky is already in the table.
DL_Hash> PRINT
  42 Binky
  42 Brody Masonry
DL_Hash> INSERT Pinky
DL_Hash> INSERT Dinky
DL_Hash> INSERT Winky
DL_Hash> PRINT
  33 Winky
  34 Dinky
  36 Pinky
  42 Binky
  42 Brody Masonry
DL_Hash> STRIP ink
DL_Hash> PRINT
  42 Brody Masonry
DL_Hash> QUIT
UNIX> 
```
# The list editor
List_editor.cpp is yet another command-line program that lets me test your code. There is always a current list that is being edited. The list holds strings that are single words, and the words in a list must be unique.
The following commands are supported:

* CLEAR: This deletes the list and creates a new one with new.
* PRINT_FORWARD: This prints the list, all on one line in the forward direction.
* PRINT_REVERSE: This prints the list, all on one line in the reverse direction.
* PUSH_BACK s: This calls Push_Back on the string s.
* PUSH_FRONT s: This calls Push_Front on the string s.
* POP_BACK s: This calls Pop_Back on the string s.
* POP_FRONT s: This calls Pop_Front on the string s.
* ERASE s: This calls Erase on the pointer to the node that holds string s. If s is not on the list, this does nothing.
* INSERT_BEFORE s1 s2: This calls Insert_Before(s1, d), where d is the pointer to the node that holds string s2. If s2 is not on the list, this does nothing.
* INSERT_AFTER s1 s2: This calls Insert_After(s1, d), where d is the pointer to the node that holds string s2. If s2 is not on the list, this does nothing.
* EMPTY: This returns whether the list is empty.
* SIZE: This returns the list's size.
* QUIT: Exit.
 
So, for example:
```
UNIX> list_editor 'Editor>'
Editor> PUSH_BACK a
Editor> PUSH_BACK b
Editor> PUSH_BACK c
Editor> PUSH_BACK d
Editor> PRINT_FORWARD
a b c d
Editor> PUSH_FRONT z
Editor> PRINT_FORWARD
z a b c d
Editor> PRINT_REVERSE
d c b a z
Editor> POP_BACK
d
Editor> POP_FRONT
z
Editor> PRINT_FORWARD
a b c
Editor> SIZE
3
Editor> EMPTY 
No
Editor> INSERT_BEFORE xxx b
Editor> PRINT_FORWARD
a xxx b c
Editor> INSERT_AFTER yyy b
Editor> PRINT_FORWARD
a xxx b yyy c
Editor> ERASE b
Editor> PRINT_FORWARD
a xxx yyy c
Editor> SIZE
4
Editor> CLEAR
Editor> EMPTY
Yes
Editor> PUSH_BACK a
Editor> PRINT_FORWARD
a
Editor> QUIT
UNIX>
```
