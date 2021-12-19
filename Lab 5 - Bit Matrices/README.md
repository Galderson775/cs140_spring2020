# cs140 (Spring 2020)

# Bit-matrices
This lab is all about bit-matrices. These are matrices whose values can only be zero or one, and arithmetic is modulo 2. You would be surprised how powerful these are. They are used in fault-tolerant storage applications, among other things, which is why I am fond of them.
Let's take some simple examples. Here are four bit-matrices:


![C003](https://user-images.githubusercontent.com/89229416/146668447-f57ecf39-3717-482f-95f2-60c3d43b7ee7.jpg)
![C244](https://user-images.githubusercontent.com/89229416/146668449-84b90789-21ee-45b5-81cc-1f892441feca.jpg)
![RV](https://user-images.githubusercontent.com/89229416/146668451-5d11cb53-3f8a-46a9-839e-8f5ac73ffb70.jpg)
![CV](https://user-images.githubusercontent.com/89229416/146668453-1dd2dd86-a5ab-4458-8002-173a5f762063.jpg)

When you add bit-matrices, you simply add elements in corresponding rows and columns, modulo two. So, for example:

![C003](https://user-images.githubusercontent.com/89229416/146668454-aabda694-c78d-42ec-87d6-c17c991d167b.jpg)
![C244](https://user-images.githubusercontent.com/89229416/146668455-dbe29bac-f0be-4bd5-a0f9-ad927b584c00.jpg)
![3-plus-244](https://user-images.githubusercontent.com/89229416/146668458-142f5e6c-1e99-40fe-b86b-bcc0aa13ad6f.jpg)  
and  
![RV](https://user-images.githubusercontent.com/89229416/146668459-d507f78a-e9fe-4992-82ac-477fbe5bdefb.jpg) + ![RV](https://user-images.githubusercontent.com/89229416/146668460-f485a55d-272b-45a2-a489-6f306f361602.jpg)
= ![ZV](https://user-images.githubusercontent.com/89229416/146668462-6792c62b-94f4-4d36-8743-023a2b2da071.jpg)

Bit-matrix multiplication is just like standard matrix multiplication, except that addition and multiplication of the individual elements is done modulo two. For example, multiplying files/RV.txt and files/CV.txt will yield a 1 X 1 matrix whose value is:
```
(1*1) + (0*0) + (1*1) + (0*1) + (0*1) + (0*0) + (0*0) + (1*1)
```
# Bitmatrix.hpp
You have a big job -- to implement the Bitmatrix class, plus the BM_Hash class and four additional procedures that work with bitmatrices. Pretty much all explanation is in the header file, include/bitmatrix.hpp:

```
#pragma once

#include <string>
#include <vector>

class Bitmatrix {
  public:

    /* Bitmatrix creation constructors / methods. */

    Bitmatrix(int rows, int cols);           /* Create an empty bitmatrix with the given size. 
                                                Throw the string "Bad rows" if (rows <= 0).
                                                Throw the string "Bad cols" if (cols <= 0). */

    Bitmatrix(const std::string &fn);        /* Read the bitmatrix from a file.  
                                               Throw "Can't open file" if you can't open the file.
                                               Throw "Bad file format" if you can't read the file. */

    Bitmatrix *Copy() const;                 /* Create a new bitmatrix using new, which is a copy of the 
                                                caller's bitmatrix, and return a pointer to it. */

    /* Bitmatrix storage methods. */

    bool Write(const std::string &fn) const; /* Write to a file.  You will print one line per row of the 
                                                bitmatrix, and each line is only composed of 0's and 1's.
                                                Return true if successful and false if not. */

    void Print(size_t w) const;               /* Print on standard output.  You will print one line per row
                                                of the bitmatrix.  Each line is composed of 0's and 1's, and
                                                there will be a space after every w characters (don't put a
                                                space at the end if the number of columns is a multiple of w).
                                                Also put a blank line after every w rows (except the last,
                                                if the number of rows is a multiple of w). */
                           
    bool PGM(const std::string &fn,          /* Create a PGM file. Each entry is a p by p square, */
             int p,                          /* which is white for zero and 100 for gray.  If border is */ 
             int border) const;              /* greater than zero, then there should be a black border
                                                of that many pixels separating each square and around 
                                                the whole matrix.  Return true if successful and false 
                                                if not. */

    /* Bitmatrix access methods. */

    int Rows() const;                            /* Return the number of rows */
    int Cols() const;                            /* Return the number of columns */
    char Val(int row, int col) const;            /* Return the specified element ('0' or '1').
                                                    Return 'x' if row or col is bad. */

    /* Bitmatrix modification methods. */

    bool Set(int row, int col, char val);        /* Set the specified element to val.  
                                                    Val must be 0, 1, '0' or '1'.
                                                    If val is 0 or 1, store '0'/'1' in the matrix.
                                                    Return true if successful and false if not. */
    bool Swap_Rows(int r1, int r2);              /* Swap these rows.  Return true 
                                                    if successful and false if not. */
    bool R1_Plus_Equals_R2(int r1, int r2);      /* Set the row r1 to the sum of row r1 and r2. 
                                                    Return true if successful and false if not. */

  protected:
    std::vector <std::string> M;                 /* The matrix.  Elements are '0' or '1'. */
};

/* These four procedures will allocate and create a new bitmatrix from other bitmatrices.
   They must be written using the methods of the Bitmatrix class above. They should return NULL
   if they are unsuccessful. */

Bitmatrix *Sum(const Bitmatrix *a1, const Bitmatrix *a2);
Bitmatrix *Product(const Bitmatrix *a1, const Bitmatrix *a2);
Bitmatrix *Sub_Matrix(const Bitmatrix *a1, const std::vector <int> &rows);
Bitmatrix *Inverse(const Bitmatrix *m);

/* We are also going to support storage and retrieval of bitmatrices through a hash table. */

class HTE {                              /* This is a "hash table entry".  Our hash table stores */
  public:                                /* vectors of pointers to these, using separate chaining. */
    std::string key;
    Bitmatrix *bm;
};

class BM_Hash {                                           /* This is our bitmatrix hash table. */
  public:
    BM_Hash(int size);                                    /* You specify the table size in the constructor. 
                                                             Throw the string "Bad size" if (size <= 0). */
    bool Store(const std::string &key, Bitmatrix *bm);    /* Store a bitmatrix with the given key. 
                                                             Return true if successful and false if not. 
                                                             Return false if the key is already there. */
    Bitmatrix *Recall(const std::string &key) const;      /* Retrieve a bitmatrix with the given key. 
                                                             Return NULL if unsuccessful. */
    std::vector <HTE> All() const;                        /* Return all of the hash table entries. */
  protected:
    std::vector < std::vector <HTE> > Table;              /* This is the hash table. */
};
```
Here is some additional detail:

* Bitmatrix(string fn) reads bitmatrix files in a specific format:
  * Blank lines are ignored.
  * Non-blank lines must be composed solely of zeros, ones or whitespace.
  * Each non-blank line represents a row of the matrix (with whitespace ignored).
  * Each row has to have the same number of columns.
* void Print(int w): Print the matrix on standard output. If w is less than or equal to zero, print each row on its own line with no spaces. Otherwise, print a space after every w columns and a blank line after every w rows.
* void PGM(string fn, int pixels, int border): Create a PGM file from the bit-matrix. This is how I made the pictures above. The zero entries are white (255) and the one entries are gray (100). Each entry is a pixels by pixels square. If border is greater than zero, then there should be a black (0) border of that many pixels separating each square and around the whole matrix. The above matrices were created with PGM(xxx, 10, 2).

The only piece of data in a bitmatrix is a vector of strings named M. If M is storing a r by c matrix, then it will contain r strings, each of which has c characters. The characters are either '0' for zero or '1' for one.

So, you have to implement 12 methods, and only one of them (PGM()) is difficult. Were I you, I'd wait until the end to do PGM().

You have to implement four procedures which operate on pointers. Since these are not part of the data structure, you have to use Rows() Cols(), Set(), and Val() to implement them. You should not modify the input matrices in any way (the "const" keywords won't let you):

* Bitmatrix *Sum(const Bitmatrix *a1, const Bitmatrix *a2): This creates a new bit-matrix which is the sum of a1 and a2. If a1 and a2 are not the same size, return NULL.
* Bitmatrix *Product(const Bitmatrix *a1, const Bitmatrix *a2): This creates a new bit-matrix which is the product of a1 and a2. This product will have a1->Rows() rows and a2->Cols() columns. If a1->Cols() and a2->Rows() do not match, then return NULL.
* Bitmatrix *Sub_Matrix(const Bitmatrix *a1, const vector &rows): This creates a new bit-matrix composed of the specified rows of the given bit-matrix. It is ok to repeat entries in rows. However, if rows is empty or contains bad indices, return NULL.
* Bitmatrix *Inverse(const Bitmatrix *a1): Create and return the inverse of a1. To do this, you should also use the Swap_Rows() and R1_Plus_Equals_R2() methods. I'll go into more detail on how to invert a bit-matrix below. If a1 is not square or not invertible, return NULL.

The first three of these are very easy. Inverse() will be tougher.
Finally, you are also going to implement a hash table to store bit-matrices with keys that are strings. You should use the djb_hash() function from class as the hash function and you should use separate chaining as the collision resolution mechanism. Your hash table is a vector of vectors of HTE's (hash table entries). Each hash table entry stores a key and a pointer to a bit-matrix.

The BM_Hash class has four methods, which are described in the header file. A little more detail on All(): Return a vector of all hash table entries in the table. The vector should be ordered just like the hash table. In other words, suppose "A" hashes to 5, "B" hashes to 1 and "C" hashes to 1. And suppose that "B" was added to the table before "C". Then All() should return the HTE's in the order "B", "C", "A". You should not call new or delete on bit-matrices when you implement any of the hash table methods.
