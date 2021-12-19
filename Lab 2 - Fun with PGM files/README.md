# cs140 (Spring 2020)

# Lab Requirements
1.You must use a vector for the hflip and vflip programs. You may only use a single 1D vector for the hflip program. For the vflip program it will be simpler if you use a vector of vectors (i.e., a 2D vector), but you can also complete the program by reading the entire pgm file into a 1D vector. If you use a 1D vector for vflip, then if the pgm file has rows of size r and columns of size c, then the pixels of row 0 will occupy entries 0--r-1 of the vector, the pixels of row 1 will occupy entries r--2r-1 of the vector, the pixels of row 2 will occupy entries 2r--3r-1 of the vector, and so on. In general, the pixels of row i will occupy entries i*r -- (i+1)*r-1 of the vector.
2. Use printf(), not cout to print your output. If you need to write to stderr, then use fprintf. For example:
fprintf(stderr, "Bad PGM file -- first word is not P2\n");
3. Use cin, not scanf and not getline, for this lab.
4. You will need to use stringstreams to convert the command line arguments in BigWhite to numbers but you do not need to use stringstreams for the other programs in this lab. It is simpler to perform your type checking and string to integer conversions using cin.

# Executables
As always, there are executables for each part of the lab in the lab directory. Make sure you test them out and make sure that your answer matches them exactly. If you have simple questions about what your program should do, try checking the executable first rather than asking the TA. Often, the answer is there.

# PGM Files
This lab lets you create and manipulate PGM files. PGM stands for portable gray-map, and is file format for non-color digital pictures. PFG files are nice because they are ASCII files that you can view with an editor like vi or emacs, and that you can also view with certain software.
Unfortunately, most web browsers do not have support for PGM files. However, every environment has many ways of dealing with them. For example, Photoshop, Open Office and Gimp both support them as image types. I would guess that iphoto and whatever photo editor comes with Windows handles them as well.

Most Unix environments have a program called convert, which will convert pgm's to jpgs for easy viewing. For example, on our labs:

```
UNIX> cp /home/jplank/cs140/Labs/Lab2/Nigel.pgm .
UNIX> convert Nigel.pgm Nigel.jpg
```
You may now view Nigel.jpg with a web browser. Test it out. It should look like:

![Nigel](https://user-images.githubusercontent.com/89229416/146668071-e031a21b-36bc-4b82-9c0a-df05b5156fd0.jpg)

A picture is simply a two-dimensional array of pixels. In a PGM file, a pixel is a gray value between 0 and 255. Zero is black, and 255 is white. Everything in between is a different shade of gray.

PGM files have a specific format. (Actually, they are more robust than this -- I've removed comments to make them easier for you). A PGM file is composed of words. The first word is P2. Then the next two words define the number of pixels in the picture -- first is the number of columns in the two-dimensional array, and then the number of rows. The next word is the number 255. So, if you look at the beginning of Red.pgm, you'll see:
```
UNIX> head Red.pgm
P2
235 183
255
 67  74  76  80  76  77  70  67  75  82 102 119 129 133 130 127 128 
130 134 131 130 129 130 133 132 125 116 109 101 102 105 105 113 119 
128 133 138 137 137 137 141 143 143 144 140 143 146 144 140 138 128 
122 118 107 106 104 106 107 115 124 127 121 114 108 103  98  97 108 
122 138 140 140 139 129 123 125 127 141 145 147 147 146 147 147 147 
149 150 150 150 155 168 185 195 198 202 201 201 200 200 199 201 199 
200 201 207 205 206 203 204 205 206 206 205 206 207 207 207 207 207 
UNIX>
```
This means that Red.pgm is a 183 * 235 array of pixels. After the 255 come all the pixels. First, the pixels in the top row, then the pixels in the next row, etc. Note that the ASCII formatting of the file doesn't mean anything -- there could be one pixel per line and the file would still be a legal PGM file. In Red.pgm above, the first 235 pixels are those in the top row, then the next 235 are in the second row, and so on. There will be a total of 183*235 = 43005 pixels. After the last pixel, the file ends.
Before you go any further, create a PGM file of your own -- make it 10x10 and give the pixels any value you want. Take a look at it. Something like:

```
P2
10 10
255
0 10 20 30 40 50 60 70 80 90
10 20 30 40 50 60 70 80 90 100
20 30 40 50 60 70 80 90 100 110
30 40 50 60 70 80 90 100 110 120
40 50 60 70 80 90 100 110 120 130
50 60 70 80 90 100 110 120 130 140
60 70 80 90 100 110 120 130 140 150
70 80 90 100 110 120 130 140 150 160
80 90 100 110 120 130 140 150 160 170
90 100 110 120 130 140 150 160 170 180
```

This should look like:

![small](https://user-images.githubusercontent.com/89229416/146668095-47dd0bf6-859d-4dd2-a795-85213e9b5140.jpg)

Cool, no?

# Program #1: Pgminfo
Your first PGM program should take a PGM file on standard input and report the number of rows, the number of columns, the total number of pixels, and the average value of all the pixels, padded to three decimal places. Your program should work on all valid PGM files, and should print out an error (using cerr) on any invalid PGM file. Examples of invalid PGM files are:
  * Those that don't begin with P2.
  * Those that don't have non-negative integers after the P2.
  * Those that don't have the number 255 after the number of rows and columns.
  * Those that contain the wrong number of pixels after the P2. This includes having too many pixels.
  * Those that contain pixels whose values are not numbers between 0 and 255.
Here is an example of pgminfo running on some of the PGM files. Note, I have a few bad PGM files in Bad-1.pgm, etc. You need to make sure that your output matches mine exactly. And I mean exactly, meaning the same punctuation, same number of spaces, and same capitalization. Use printf(), not cout.
```
UNIX> ./pgminfo < Red.pgm
# Rows:         183
# Columns:      235
# Pixels:     43005
Avg Pixel:  120.142
UNIX> ./pgminfo < Pike.pgm
# Rows:         235
# Columns:      197
# Pixels:     46295
Avg Pixel:   99.932
UNIX> ./pgminfo < Bad-2.pgm
Bad PGM file -- No column specification
UNIX> ./pgminfo < Bad-5.pgm
Bad PGM file -- pixel 99 is not a number between 0 and 255
UNIX> ./pgminfo < Bad-6.pgm
Bad PGM file -- Extra stuff after the pixels
UNIX> 
```
When I print a pixel number, it is zero-indexed. So the first pixel is pixel zero.

# Program #2: Bigwhite

This program takes two numbers as its command line arguments -- the number of rows and the number of columns. It then writes a PGM file on standard output which contains that number of rows and columns, all of white pixels. Again, you should error check to make sure that the proper number of command line arguments are given, that they are integers and in the proper range. On an error, print the error statement to stderr. As an example, try:
```
UNIX> ./bigwhite 20 10 > a.pgm
```
This will create a PGM file a.pgm, which has 20 rows and 10 columns of white pixels.
Your output in this and the next three programs should match mine exactly when there is an error. Otherwise, the PGM files that it emits should be equivalent to mine (they should make the same picture), but do not have to have the same output exactly.

# Program #3: Neg
**Neg** takes a PGM file on standard input, and prints a PGM file on standard output that is the negative of the input file. If the PGM file is not valid (same parameters as pgminfo), print an error to standard error.
For example, here is the negative of ** Red.pgm**:

![RedNeg](https://user-images.githubusercontent.com/89229416/146668123-570f7188-a8bc-4646-b217-bc0733f3d9ce.jpg)

# Program #4: Hflip

**Hflip** reads a PGM file on standard input, and prints a PGM file on standard output. The output file should be the horizontal reflection of the input file -- in other words, left is right and right is left.
You'll have to use a vector for this program.

Here's the **hflip** of **Red.pgm**:

![RedFlip](https://user-images.githubusercontent.com/89229416/146668137-ac17d3bc-3958-4f51-a0b6-dd8de51452f0.jpg)

# Program #5: Vflip
**Vflip** is just like **hflip**, only it flips the file vertically:

![Red-VFlip](https://user-images.githubusercontent.com/89229416/146668149-7b05f89c-c447-45b7-afee-a807e3d4a212.jpg)
