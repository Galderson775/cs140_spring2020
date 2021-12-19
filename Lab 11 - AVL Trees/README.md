# cs140 (Spring 2020)

# AVLTree.cpp
Your job is to implement AVL trees in avltree.cpp. You should implement it according to the rules outlined in the lecture notes on AVL Trees.
The header file for AVL trees is in avltree.h:

```
#include <iostream>
#include <vector>
using namespace std;

class AVLTNode {
  public:
    AVLTNode *left;
    AVLTNode *right;
    AVLTNode *parent;
    int height;
    string key;
    void *val;
};

class AVLTree {
  public:
    AVLTree();
    ~AVLTree();
    int Insert(string key, void *val);
    void *Find(string key);
    int Delete(string key);
    void Print();
    int Size();
    int Empty();
    vector <void *> Sorted_Vector();
  protected:
    AVLTNode *sentinel;
    int size;
    vector <void *> array;

    void Check_Balance(AVLTNode *n);
    AVLTNode *Rebalance(AVLTNode *n);
    void Rotate(AVLTNode *n);
   
    void recursive_inorder_print(int level, AVLTNode *n);
    void recursive_make_vector(AVLTNode *n);
    void recursive_destroy(AVLTNode *n);

};
```

The public methods are exactly the same as the Binary Search Tree data structure from the lecture notes. There are just four differences:

1. Each node has a height field.
2. There is a protected method called Rotate(n). It should perform a rotation about node n, updating all pointers and heights of all relevant nodes.
3. There is a protected method called Rebalance(n). It should check to see if a node is balanced. If it is, it should just return n. If it is not balanced, then it should rebalance it by properly identifying and performing the type of rebalance (Zig-Zig or Zig-Zag), and then returning the root of the new subtree.
4. There is a protected method called Check_Balance(n). Its job is to set the height of node n, and then to set n equal to Rebalance(n). If the new height of n is different from its old height, then it should continue checking n->parent. If not, it can return.

Now, when you perform insertion to create a node n, call Check_Balance(n->parent). That will identify whether rebalancing is necessary on any node up to the root, and it will perform the rebalancing.
When you perform deletion on n, use Check_Balance() to fix the balancing when you're done. If you order your operations correctly, you can simply make a recursive call to Delete() when n has two children.

The only traversal that you should modify from the lecture nodes is recursive_inorder_print(). It should print the height of each node before it prints its keys.

The other traversals will be unchanged from the lecture notes on binary search trees. Test your code with avltree_test:
