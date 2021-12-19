# cs140 (Spring 2020)

**The program that you should write: How much gold -- gold.cpp**

This program reads a "map" on standard input. It's really a text file in the following format:

  * A period stands for dirt
  * A dash stands for a rock with nothing under it
  * An upper case letter stands for rock with gold hiden under it. The amount of gold represented by the letter: A stands for one ounce, B stands for two ounces, etc.
  * There are no other characters on the map.
  
For example, the following map in map1.txt contains a lot of dirt, but it also contains one rock with nothing under it, and three rocks with gold: one ounce (A), three ounces (C) and 26 ounces (Z).
```
...............                                                            
..-............
.........A.....
..Z.........C..
```                                               
                                                            
Your job is to write a program called **gold.cpp**, which reads a map on standard input and prints the total ounces of gold on the map.

There is an example executable in **gold**. Try it out:
```
UNIX> gold < map1.txt\n  
30  
UNIX> cat map2.txt  
ABCDE.  
.F----.  
--...........G  
UNIX> gold < map2.txt  
28  
UNIX> 
```
