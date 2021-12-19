# cs140 (Spring 2020)

This lab builds on the Binary Search Tree data structure from the lecture notes on trees. In particular, I have copied bstree.h, bstree.cpp and bstree_test.cpp to laba.h, laba.cpp, laba_test.cpp respectively. I then added some public and protected methods to laba.h. Your job is to implement them in laba.cpp:
  * **void Postorder()** -- this does a postorder printing of the tree. As with the Print() method, each line should precede each name by 2l spaces, where l is the depth of the node. When you implement it, you should use the protected method recursive_postorder(). You should not call Depth() to determine the depth of a node. The TA's will look for that, and they will take off for it.
  * **void Preorder()** -- this does a preorder printing of the tree. Its format is like Postorder(). When you implement it, you should use the protected method recursive_preorder(). Again, you should not call Depth() to determine the depth of a node. The TA's will look for that, and they will take off for it.
  * **int Depth(string key)** -- this prints the depth of the node with the given key. You don't need recursion to implement this. If the key is not in the tree, return -1.
  * **int Height()** -- this prints the depth of the node with the maximum depth in the tree. You should call the recursive protected method recursive_height() to implement this. You should not call Depth(). If the tree is empty, its height is -1.
  * **int IsAVL()** -- this returns 1 if the tree is an AVL tree and 0 if it is not. You should call the recursive protected method recursive_height_and_avl_check() to implement this. When I implemented recursive_height_and_avl_check(n), I had it return the height of the tree rooted at n if it is a legal AVL tree, and -2 if it is not a legal AVL tree.
  * **int Rotate(string key)** -- this finds the node with the given key and performs a rotation about this node, as defined in the AVL Tree Lecture Notes. It should return 1 if it is successful. It should return 0 if either the key is not in the tree, or the key is the root of the tree, since you cannot rotate the root.
  
Your job is to implement these methods in laba.cpp. That is the file that you should submit. You are not allowed to modify laba.h or laba_test.cpp.

# The testing program
The program that we will use in the gradescript is laba_test.cpp. It is simply an augmentation of bstree_test from the binary search tree lecture notes. It has additional commands for each of the methods that you are implementing. So:
```
UNIX> laba_test LABA':'
LABA: INSERT Adam Resistant 833-118-4192 470-10-0161
LABA: INSERT Dylan Politician 451-532-5606 940-23-2116
LABA: INSERT Kaitlyn Fortescue 323-215-8830 975-92-4704
LABA: PRINT
    Kaitlyn Fortescue
  Dylan Politician
Adam Resistant
LABA: INSERT Brianna Quintillion 537-619-7786 208-57-2191
LABA: PRINT
    Kaitlyn Fortescue
  Dylan Politician
    Brianna Quintillion
Adam Resistant
LABA: POSTORDER
    Brianna Quintillion
    Kaitlyn Fortescue
  Dylan Politician
Adam Resistant
LABA: PREORDER
Adam Resistant
  Dylan Politician
    Brianna Quintillion
    Kaitlyn Fortescue
LABA: HEIGHT
2
LABA: DEPTH Adam Resistant
0
LABA: DEPTH Brianna Quintillion
2
LABA: DEPTH Dylan Politician
1
LABA: ISAVL
No
LABA: ROTATE Dylan Politician
Rotated.
LABA: PRINT
  Kaitlyn Fortescue
Dylan Politician
    Brianna Quintillion
  Adam Resistant
LABA: ISAVL
Yes
LABA: QUIT
UNIX> 
``` 
