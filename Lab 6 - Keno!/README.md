# cs140 (Spring 2020)

# The format of this lab
I have written a header file, candyCrush.hpp and a program candyCrushTest.cpp that uses the class/methods defined in that header file. Your job is to create the file candyCrush.cpp, which implements the methods defined in candyCrush.hpp.

You are allowed to add additional protected methods to candyCrush.hpp but you may not change the function prototypes of the existing methods nor may you delete any of them. You are not allowed to modify candyCrushTest.cpp. You will turn in candyCrush.hpp and candyCrush.cpp, and the TA's will compile and test them with the candyCrushTest.cpp that is in the lab directory.


# Candy Crush
Candy Crush is a match-based puzzle game that awards points for collapsing columns or rows of contiguous candies. Below is a sample Candy Crush screen shot:

![candy_crush](https://user-images.githubusercontent.com/89229416/146668544-1e5d64ff-a995-4481-be5d-ca29844173ea.jpg)

One of the possible matches would be the two yellow-colored toffee candies at the beginning of the first row of the diagram and another would be the two purple-colored candies at the end of the first row of the diagram. There are other matches you could make in the columns, but for this lab we are going to focus on row matches. Specifically you are going to use an STL list to implement a single row of a simplified Candy Crush like game.

It would be too complicated to create fancy graphics for this lab assignment, so instead you will be using a list of strings to represent flavors of candy. In Part A you will write code to randomly generate the first candy sequence, put it in the list, and print out the portion of the list that you filled. In Part B you will generate enough candy sequences to completely fill a 12 item row of candies. In Part B you will also write code to allow the user to select a sequence of candies and assign points based on the length of the sequence. Finally in Part C you will remove the candy sequence selected by the user from the row, slide the remaining candies to the left, and fill the right side of the row with new candies.

# Input
Your program will read from stdin. Here is some sample input and how to interpret it:
```
256  10     // Seed for the random number generator and length of a candy crush row
grape cherry orange lemon toffee    // The candy flavors
20 10  // The probability for each possible candy sequence length 
10 20
25 30
20 50
15 70
15 120
0  150
0  180
0  210
0 300
```
You will be using a random number generator for this lab so the first input is the seed you will use to the random number generator. It must be an integer.
The second input on the first line is the length of the candy crush row and must be a non-negative integer between 1 and 100.

The second line of the input will be a list of the candy flavors that will be used in the game.

The remaining lines of the input will be the probabilities of generating a flavor sequence of a particular length and the points they are worth. The sequence lengths are not explicitly given. Your program should assume they begin with 1. Hence in the above input there is a 20% chance of generating a sequence of length 1 and it is worth 10 points if a player selects a sequence of length 1. Similarly there is a 25% chance of generating a sequence of length 3 and it is worth 30 points when a player selects it. You must provide a probability and point value for every sequence length up to the row length, since it is possible to have a row that consists only of one flavor. Note that there is no chance of generating a sequence of length 7-10. However, suppose you generate a "toffee" sequence of length 4 and then generate a "toffee" sequence of length 5. You will end up with a contiguous "toffee" sequence of length 9.

The probabilities must add up to 100 and must be non-negative integers.

# candyCrush.hpp
Take a look at the header file candyCrush.hpp:
```
class candyCrush {
public:
  candyCrush(const string &inputFile);
  int getRowLength() const;
  int getScore() const;
  void printCandy() const;
  int play(int choice);		
protected:
  list<string> candy;
  vector<string> flavors;
  vector<int> points;
  vector<int> probabilities;
  int score;
  int rowLength;
};
``` 

You must minimally implement this public API. You are free to add additional protected methods and instance variables to help with your implementation. The method descriptions are as follows:

candyCrush(const string &inputFile): This constructor function sets up your initial game state. In this constructor you will generate a random sequence of candies and assign them to your candy list. You will do this as follows:

1. You will read the input and assign it to the appropriate instance variables. Here are a few additional instructions about what to do with input:
  * pass the seed to the srand function to initialize the random number generator. You do not need to save the seed once it's been passed to the srand function.
  * The probabilities should be stored as cumulative probabilities in the probabilities vector rather than individual probabilities. For example, suppose the maximum possible sequence was 4 and your probabilities were 40, 30, 20, and 10. Then your probabilities vector should be 40, 70, 90, 100, not 40, 30, 20, 10. The reason for storing cumulative probabilities is so that you can easily generate a sequence from a random number between 0 and 100. For example, if the random number is 30, you would generate a sequence of length 1 because 30 is between 0 and 40. If the random number is 90, then you will generate a sequence of length 4 because 95 is between 90 and 100. Note that these cumulative probabilities give each sequence the appropriate probability of being chosen because out of 100 random numbers, each has the appropriate proportion assigned to its sequence length.
2. You will then perform the following operations to generate the candy sequence and fill the list:
  * use rand() to generate a random number between 0 and n-1 to choose a candy where n is the number of candy flavors. Assign numbers to the candy flavors in the order they are presented to you in the input. For example given the sample input shown above, assignment of numbers to candies should be as follows:
```
  0: grape
1: cherry
2: orange
3: lemon
4: toffee
```
  * use rand() to generate a random number between 0 and 99 to choose the length of the candy sequence. Find the range in the probabilities vector which contains this number. Add 1 to the index associated with that range and you have the length of your sequence. The random number must be less than the cumulative probability associated with the index for that index to be chosen. For example, suppose your probabilities vector is:
```
0: 20 
1: 30
2: 55
3: 75
4: 90
5: 100
```
and your random number is 90. Then the index you will choose is 5, not 4, and your sequence length will be 6. If you want to make life easier on yourself, you can start storing your probabilities at index location 1 rather than 0, so that you don't have to remember to add 1 to the index to get your sequence length.
  * If the sequence length is k, add up to k strings of the flavor you generated in step 1 to the candy list. If adding the full k strings would cause you to overflow the row, then stop when you have filled the row.

int getRowLength() const: Returns the length of the candy crush row.

void printCandy() const: Prints the candy crush row as 8 flavors per row, left justified, in fields 10 characters wide.

int play(int choice): choice is an integer from 0 to rowLength-1 indicating which candy flavor the player selected. The test program error checks rowLength so you do not have to do so yourself. The return value is the number of points scored by the player on this turn. This method must:

Find the candy list element corresponding to the player's choice.
Compute the length of the flavor's sequence and add the points for that sequence to the user's score.
Delete the strings in that sequence from the candy list
If the sequence is of length k, then starting at the end of the candy list, add k flavors to the end of the list using the same strategy you used to generate the initial list (i.e., generate a flavor and a sequence length, add that many flavors to the end of the list, and repeat until the row is full). You probably want a separate method named something like fillCandyList to fill the list with candy.
play() does not print anything. There are commands you can use in the test program to print the candy crush row or the player's current score.

int getScore(): Returns the players current score.
