# cs140 (Spring 2020)

# Cryptographic Hash Functions
There are several well-known algorithms that compute hash functions of arbitrarily-sized data. One example is the MD5 function that produces a 128-bit hash function from any sized data. Another example is SHA-1, which produces a 160-bit hash. Both functions attempt to generate uniformly distributed hashes from arbitrary data that are secure, in that you can't get any information about the data from the hash. I won't go into a discussion beyond that, except that SHA-1 does a better job, but is more computationally expensive. For our purposes, we will assume that the functions produce hashes from data that look like random streams of bits.
The openssl command generates hashes from files using either technique. For example:

```
UNIX> cat input-1.txt
Rosalita, jump a little lighter.
Senorita come sit by my fire.
UNIX> cat input-2.txt
Sosalita, jump a little lighter.
Senorita come sit by my fire.
UNIX> openssl md5 input-1.txt input-2.txt
MD5(input-1.txt)= b9937df3fefbe66d8fcdda363730bf14
MD5(input-2.txt)= 3a752ef1b9bfd1db6ba6a701b6772065
UNIX> openssl sha1 input-1.txt input-2.txt
SHA1(input-1.txt)= 9a2c3d93445fa844094eb213a17fc5996895c925
SHA1(input-2.txt)= 8886b6ef4671093b15c2dba387c3eb169e9db5d2
UNIX> 
```
The representation of the hashes is a long stream of hexadecimal. You can read each hex digit as four bits. Thus the first eight bits of b9937df3fefbe66d8fcdda363730bf14 are 10111001 (b9). The hexadecimal string is 32 characters long -- hence the hash is 128 bits.
The ASCII value of 'R' is 82 (binary 1010010) and the value of 'S' is 83 (binary 1010011). That means that input-1.txt and input-2.txt differ by exactly one bit. However, look how different the hashes of both values are. That's a great thing about both functions. (Look up avalanche effect if you want to learn more about that property).

Hashes like MD5 and SHA-1 are often used to represent large files. For example, suppose I have an MP3 file in my library that is 4 MB long, and has an MD5 hash of 6282f7860277242c80b69d3a20e0b40e. Suppose I have a CD that should have the song on it and I want to make sure that the song is not corrupted. I can calculate the hash of the song on the CD, and if it doesn't equal 6282f7860277242c80b69d3a20e0b40e, then I know it's corrupted. If it does equal 6282f7860277242c80b69d3a20e0b40e, I can assume with probability 1/2128 that it is not corrupted.


**hash140.h**
Take a look at the header file hash140.h:

```
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class HashTable {
  public:
    HashTable(int table_size, string function, string collision);
    void Add_Hash(string &key, string &val);
    string Find(string &key);
    void Print();
    int Total_Probes();
  protected:
    vector <string> keys;
    vector <string> vals;
    int nkeys;
    int Fxn;
    int Coll;
    int tmp;
};
```

This defines a HashTable class. It will be used to store keys and values. Both are strings. The keys are strings of hexadecimal digits, just like the MD5 and SHA-1 hashes. They may be of arbitrary size. The values are strings as well. The intent of the hash table is to store hashes as keys, and filenames as values. Then you can use the hashes to identify the contents of files, as described above.

The constructor creates a hash table of the given size. We will support two hash functions, "Last7" and "XOR". I'll define them later. The hash table will use open addressing with one of two collision resolution strategies: "Linear" for linear probing and "Double" for double hashing. More on that later too.

When you represent the hash table, you'll have two arrays -- one of keys and one of values. They'll both be the same size, and the element in vals[i] will correspond to the key in keys[i]. You represent an empty slot in keys and vals with an empty string.

The four protected ints named nkeys, Fxn, Coll and tmp are there for you to use however you wish. I use them as follows: I use nkeys store the number of keys that are in the hash table. I used Fxn to record which hash function I'm using, and Coll to record which collision resolution strategy I'm using. I used tmp to help write Total_Probes(). Basically, whenever I call Find(), I have it increment tmp with the number of probes that it took. That allows me to write Total_Probes easily.

Here are the methods. The constructor should be obvious.

Add_Hash() adds the given key/val pair to the hash table. You may assume that the key is not already in the hash table. You will insert them with the proper hash function and collision resolution strategy. You need to flag two possible errors:

There is no more room in the hash table.
There is room, but the key can't go in the hash table. This can only happen with double hashing -- I'll give a concrete example below.
In both of these two cases, your program should exit after flagging the error.
Find() looks for the given key in the hash table and returns its associated value, or the empty string if the key is not in the hash table.

Print() should print all non-empty slots in the hash table, one per line. The index should be printed first, right justified and padded to five characters. Then the key and the value, each separated by a space.

Total_Probes() traverses the hash table, and for every key, calculates how many probes it takes to find the key. It returns this value. Again, I'll give examples below.

I've written a main() procedure in hash_test.cpp. It needs to be compiled with your implementation of HashTable, which should be in hash140.cpp. You run it as follows:

```
hash_test table-size function(Last7|XOR) resolution(Linear|Double) [dictionary]
```
The first three arguments are obvious. The last is optional, and is a "dictionary" file of key/value pairs. Hash_test first inserts all key/value pairs from the dictionary. Then it accepts commands on standard input:

  * ADD key val: Adds the given key/val pair. Both should be one word, and the key should be in hexadecimal. ADD first calls FIND to see if the key is in the table so be forewarned that if you just write ADD and not FIND, this command will not work properly. My suggestion is that if you write ADD and want to test it, then have FIND return the empty string, "", so that ADD will work properly.
  * FIND key. This prints the val assocated with key.
  * PRINT prints the hash table.
  * TPROBES prints the total number of probes for every key in the hash table.
I'll show examples of calling this later.

# Reading Hexadecimal
To read an integer i in hexadecimal from standard input, you do:
```
cin >> hex >> i;
```
Extending on this, read_as_hex.cpp reads strings from standard input and assumes that they are hexadecimal representations of integers. It prints out the decimal and hexidecimal values of the integers:
```
#include <string>
#include <iostream>
#include <sstream>
using namespace std;

main()
{
  string s;
  istringstream ss;
  int i;

  while (cin >> s) {
    ss.clear();
    ss.str(s);
    if (ss >> hex >> i) printf("%d 0x%x\n", i, i);
  }
}
```
Giving it "10" and "20" as input prints out the proper values:
```
UNIX> echo 10 20 | read_as_hex
16 0x10
32 0x20
UNIX> 
```
Why do I tell you this? Well.......

# Hash Functions
The keys that you store are hexadecimal strings that can be any length. You need to hash these strings into integers. We are going to do this in two ways.
The first is called Last7. It treats the last seven digits of the hash string as a number in hexadecimal. Thus, the hash of "b9937df3fefbe66d8fcdda363730bf14" will be 120635156, which is equal to 0x730bf14. If the string has fewer than 7 characters, then simply treat the whole string as a hexadecimal number. Thus, the hash of "11" will be 17, which is equal to 0x11.

The second hash function is called XOR. With this hash function, you break up the string into seven-character words, and then treat each word as a number in hexadecimal. The last word can be fewer than seven characters. You then calculate the bitwise exclusive-or of each of these numbers. That is your hash function. So, let's take an easy example: "a000000a0000101". First, we break this up into seven-digit words (except for the last one): "a000000", "a000010" and "1". Their bitwise exclusive-or is equal to 0x11, so the XOR hash of "a000000a0000101" is equal to 17.

I have written a program named hashcode that you can use to determine the hash code computed by either Last7 or XOR. For example:
```
UNIX> hashcode Last7 ead364abc12876
hash code = 197208182
UNIX> hashcode XOR ead364abc12876
hash code = 90971708
```
You can find hashcode in /home/bvanderz/cs140/Labs/Lab4.

# Tips For Computing The Hash Functions
Here are some helpful hints for computing the two hash functions:

1. You can use a string's size() function to find the number of characters in a string.
2. You can use a string's substr() function to extract substrings of length 7 from the string
  * Last7: For Last7 the starting position is 7 characters from the end of the string, and the end of the string can be located using the .size() function
  * XOR: For XOR the starting positions for your substrings are at multiples of 7-0, 7, 14, 21, etc. You can use a for loop that increments by 7, rather than by 1 to get to each of the starting positions.
3. Once you have the substring you can stuff it into an input stringstream and extract the integer value as shown in the section on Reading Hexadecimal.
4. For Last7 you are done once you have the integer value. For XOR you need to use the bitwise exclusive or operator (^) to xor the integer value with the existing hash value. You should initialize the hash value to 0 so that the first xor correctly sets the hash value to the value of the first substring.
5. Make sure that you use unsigned ints for your computations or else you will get the wrong results. Also make sure that your two hash functions return unsigned ints and that you assign these returned unsigned ints to unsigned int variables that you are using to compute your bucket indices.

# Collision Resolution
Linear probing is straightforward. With double hashing, you will use the specified hash function as the first hash function, and the other one as the second hash function. If the second hash function ends up yielding an increment of zero, set it to one.

# Examples
Let's start with a simple example -- putting one value into a hash table.
```
UNIX> hash_test 10 Last7 Linear
ADD a Fred
PRINT
    0 a Fred
<CNTL-D>
UNIX> 
```
Since 0xa = 10, the hash value is 10, which means that the key/val go to index 0.
Let's try some of those example keys above:
```
UNIX> hash_test 10 Last7 Linear
ADD b9937df3fefbe66d8fcdda363730bf14 Fred
PRINT
    6 b9937df3fefbe66d8fcdda363730bf14 Fred
ADD 11 Luther
PRINT
    6 b9937df3fefbe66d8fcdda363730bf14 Fred
    7 11 Luther
FIND b9937df3fefbe66d8fcdda363730bf14
b9937df3fefbe66d8fcdda363730bf14 Fred
FIND Fred
Didn't find Fred
<CNTL-D>
UNIX> hash_test 10 XOR Linear
ADD a000000a0000101 Fred
PRINT
    7 a000000a0000101 Fred
<CNTL-D>
UNIX> 
```
This example shows linear probing in action because "001", "1" and "a000000b" all hash to values that equal 1 mod 10:
```
UNIX> hash_test 10 Last7 Linear
ADD 001 Fred
ADD 1 Binky
ADD a000000b Jim
PRINT
    1 001 Fred
    2 1 Binky
    3 a000000b Jim
TPROBES
3
<CNTL-D>
UNIX> 
```
The total number of probes is three, because finding "001", requires zero probes, "1" requires one and "a000000b" requires two. The total is therefore three.
Let's try a harder example with double hashing:
```
UNIX> hash_test 10 Last7 Double
ADD 2 Fred
ADD 00000100000002 Binky
ADD c Luther
PRINT
    0 00000100000002 Binky
    2 2 Fred
    4 c Luther
TPROBES
2
<CNTL-D>
UNIX> 
```
Each of these keys hashes to two. First, "2" goes into index two. Then "00000100000002" collides. Its hash value using XOR is 0x12 = 18. We take 18 mod 10 and that gives us an increment of 8. Thus, the first probe will try 2+8 = 10, which is 0 mod 10. Since that slot is empty, "00000100000002" goes into index 0. Finally, when "c" collides, its hash value using XOR is also 2. Therefore, it goes into index 2+2=4.
When we call TPROBES, it returns 2, because "2" has zero probes, and "00000100000002" and "c" have one each.

Two final examples -- the first shows an example where the second hash produces an increment of zero. In that case, you set it to one:
```
UNIX> hash_test 10 Last7 Double
ADD 0 Fred
ADD a Binky
PRINT
    0 0 Fred
    1 a Binky
TPROBES
1
<CNTL-D>
UNIX> 
```
When we insert "a", both hash functions return 10, which equals 0 mod 10. Thus, we set the second hash function to 1, and the value goes into index 1.
The last example shows three keys that hash to 5 with both functions:
```
UNIX> hash_test 10 Last7 Double
ADD 5 Fred
ADD f Binky
PRINT
    0 f Binky
    5 5 Fred
ADD 19 Luther
Couldn't put 19 into the table
UNIX> 
```
When we try to put the third key into the table, it can't go there, because it keeps trying indices five and zero. Your code needs to work in this case -- it needs to identify that the key cannot go into table.

# When you can't put something into the hash table:
If the hash table is full, print "Hash Table Full" on standard error and exit.
If the hash table is not full print "Couldn't put xxx into the table" on standard error, where xxx is the key, and exit.

If your program is using double hashing and it cannot insert a key into the table, it may have to first search the entire table to determine if the table is full. If the table is full, it should print the hash table full message, and the couldn't put key into the table message otherwise.
