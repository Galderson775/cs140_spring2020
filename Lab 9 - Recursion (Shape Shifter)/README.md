# cs140 (Spring 2020)

# Part 1: Enumerating Strings
Your first job is write a program called enum, which is called as follows
```
enum length ones
```
This program should enumerate all strings of length length that contain ones ones, and (length-ones) zeros, and print them on standard output in sorted order. For example:
```
UNIX> enum 2 2
11
UNIX> enum 2 1
01
10
UNIX> enum 2 0
00
UNIX> enum 3 1
001
010
100
UNIX> enum 3 2
011
101
110
UNIX> enum 5 3
00111
01011
01101
01110
10011
10101
10110
11001
11010
11100
UNIX> enum 100 100
1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111
UNIX> enum 100 0
0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000
UNIX> 
```

You may puzzle about how to do this, but it is easy with recursion. What I did was define a class called Enum, which has two public methods:
  * A constructor that takes the length of the string
  * A recursive function called do_enumeration that takes an index and a number-of-ones.
It also has two protected pieces of data:
  * A string, which is initialized to have the proper length.
  * The length.
My main() processes the command line arguments, and then allocated an instance of Enum with the given length. It then calls the do_enumeration() method with an index of zero and number-of-ones equal to the command line argument.
do_enumeration() is recursive. The base case is when index is equal to length. Then it prints out the string and returns. In other cases, it calculates whether the character at the given index can be a zero. If so, it sets it to zero and recursively calls do_enumeration() with an incremented index, and the same number-of-ones.

When that recursive call returns, it determines if the character at the given index can be a one. If so, it sets it to one and recursively calls do_enumeration() with an incremented index, and the with number-of-ones decremented by one.

That's all there is to it.

# Part 2: ShapeShifter!!

**The Game**

In neopets.com, there is a game called "ShapeShifter." I won't screen-dump their pictures, since that probably violates a few copyrights, so I'll explain the game using my own pictures. You are given a setup like below, consisting of grid of swords and shields, an "active shape", and some "remaining shapes":  
![Pic-1](https://user-images.githubusercontent.com/89229416/146668991-f8158593-1df0-4254-84ac-e89b15d00fae.jpg)

You apply the active shape to a part of the grid where it fits. For example, in the picture above, you can apply the active shape to row zero, row one or row two (life is zero-indexed, of course). When you apply the shape, all grid cells that the shape overlaps change swords to shields and vice versa. For example, suppose you apply the active shape to row zero. The state of the system then changes to:
![Pic-2](https://user-images.githubusercontent.com/89229416/146669000-4b3ca4e1-19ce-4df0-8981-07389d3661f0.jpg)

The next shape can be applied to row 0 or row 1. Suppose we apply it to row 1. Now the state of the system is:
![Pic-3](https://user-images.githubusercontent.com/89229416/146669008-9c003647-e600-44e0-87d2-d661fbe297a5.jpg)

There is one more shape, which can be applied to columns 0, 1 or 2. We apply it do column zero, and the grid becomes all swords:
![Pic-4](https://user-images.githubusercontent.com/89229416/146669019-b7fb5a58-c3b1-459d-9f53-be0ad17ed40f.jpg)

# The Interactive Game Player

Since we are computer scientists, we are going to represent sheilds with zeros, and swords with ones. We will represent any rectangular grid of zeros and ones that has r rows and c columns with a vector of r strings. Each string in the vector has exactly c characters, which are either '0' or '1'.
Thus, the grid in our first example is:
```
{ "100", "101", "000" }
```
and the grid in the second example is:
```
{ "100", "101", "001" }
```
We can also represent shapes as grids of zeros and ones. For example, the shapes in the first example are:
```
{ "111" }
{ "110", "011" }
{ "1", "1", "1" }
```
and the shapes in the second example are:
```
{ "1" }
{ "11", "10" }
{ "01", "11" }
```
I have written an interactive game player, called ss_player, which takes a prompt and then a grid as command line arguments. After the prompt, each word on the command line specifies a different row of the grid. These words must all be the same size, and they must be composed solely of zeros and ones. Suppose I want to play using the first example above. Then I can play with:
```
UNIX> ss_player ShapeShifter: 100 101 000

The Grid:

100
101
000

ShapeShifter: 
```
At the prompt, I enter a shape and where to apply it. Each row of the shape is a separate word, and the last two words must specify a row and a column. Below, I show how you would specify the solution to the first example:
```
UNIX> ss_player ShapeShifter: 100 101 000

The Grid:

100
101
000

ShapeShifter: 111 0 0

The Grid:

011
101
000

ShapeShifter: 110 011 1 0

The Grid:

011
011
011

ShapeShifter: 1 1 1 0 0

The Grid:

111
111
111

ShapeShifter: <CNTL-D>
UNIX> 
```
Go ahead and try to do the second example on your own.
If you give a prompt of "-", there will be no prompt, which is useful for when you use the player with a file or output of a program as input.

# Your job: ss_solver
Your job is to write the program ss_solver.cpp. It takes a grid on its command line, and then each line of standard input specifies a shape. After reading all of the shapes, your program should output how to apply each shape to solve the puzzle. That means outputting each shape and the row and column at which it was applied. It should output the shapes in the same order as the input file. If there is no solution, it should simply exit with no output.
For example, the file ex1.txt contains the shapes for the first example:
```
111
110 011
1 1 1
```
ex2.txt contains the shapes for the second example. Here's the program running the two examples:
```
UNIX> ss_solver 100 101 000 < ex1.txt
111 0 0
110 011 1 0
1 1 1 0 0
UNIX> ss_solver 100 101 001 < ex2.txt
1 1 1
11 10 0 1
01 11 1 0
UNIX> 
```
In the first example, shape "111" was applied at grid coordinates (0,0), then shape "110 011" was applied at grid coordinates (1,0), and finally the shape "1 1 1" was applied at grid coordinates (0,0). Note that shapes are printed out in the same format in which they were read from standard input and that the shapes are being printed in the same order as they were read from standard input.
