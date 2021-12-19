// Robert Schenck
// Lab B
// 4/25/2020
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "avltree.hpp"
using namespace std;
using CS140::AVLTree;
using CS140::AVLNode;


AVLTree& AVLTree::operator= (const AVLTree &t)        
{
  Clear();

  sentinel->right = recursive_postorder_copy(t.sentinel->right);

  size = t.Size();

  return *this;
}

// Helper function for assigning nodes or whatever
inline void node_quickset(AVLNode *n, AVLNode *l, AVLNode *r, AVLNode *p){

	if(l){
		n->left = l;
	}
	if(r){
		n->right = r;
	}
	if(p){
		n->parent = p;
	}
}

void rotate(AVLNode *n){

	AVLNode *p, *g, *l, *r, *m;
	if(n->height == 0 || n->parent->height == 0){
		return;
	}
	p = n->parent;
	g = p->parent;
	if(p->left == n){
		m = p->right;
	}else if(p->right == n){
		m = p->left;
	}
	if(n->left != NULL){
		l = n->left;
	}
	if(n->right != NULL){
		r = n->right;
	}

	// Right rotation
	if(p->left == n){
		// Update grandparent node to have n as child
		if(g->left == p){
			g->left = n;
		}
		if(g->right == p){
			g->right = n;
		}
		// update nodes for right rotation
		node_quickset(n, NULL, p, g);
		node_quickset(p, r, m, n);
		node_quickset(r, NULL, NULL, p);
	}

	// Left rotation
	if(p->right == n){
		// Update grandparent node to have n as child
		if(g->left == p){
			g->left = n;
		}
		if(g->right == p){
			g->right = n;
		}
		// update nodes for left rotation 
		node_quickset(n, p, NULL, g);
		node_quickset(p, m, l, n);
		node_quickset(l, NULL, NULL, p);
	}
}

// Returns true if the AVL tree is unbalanced
bool imbalance(const AVLNode *n){

	return abs((int)n->left->height - (int)n->right->height) > 1;
}

// Height is largest of two children
void fix_height(AVLNode *n){

	n->height = max(n->left->height, n->right->height) + 1;
}

void fix_imbalance(AVLNode *n){

	AVLNode *p, *c;

	// Chooses greater height for p node
	if(n->left->height > n->right->height){
		p = n->left;
	}else{
		p = n->right;
	}
	// Choose greater height for c node, if tie than same direction as p node from n
	if(p->left->height > p->right->height){
		c = n->left;
	}else if(p->left->height < p->right->height){
		c = n->right;
	}else if(p->left->height == p->right->height){
		if(p == n->left){
			c = p->left;
		}else if(p == n->right){
			c = p->right;
		}
	}
	// zig-zig
	if(c == n->left->left || c == n->right->right){
		rotate(p);
	}
	// zig-zag
	if(c == n->left->right || c == n->right->left){
		rotate(c);
		rotate(c);
	}
}

/* I simply took Insert and Delete from their binary search tree
   implementations.  They aren't correct for AVL trees, but they are
   good starting points.  */

bool AVLTree::Insert(const string &key, void *val)
{
  AVLNode *parent;
  AVLNode *n;
  unsigned int h;

  parent = sentinel;
  n = sentinel->right;

  /* Find where the key should go.  If you find the key, return false. */

  while (n != sentinel) {
    if (n->key == key) return false;
    parent = n;
    n = (key < n->key) ? n->left : n->right;
  }

  /* At this point, parent is the node that will be the parent of the new node.
     Create the new node, and hook it in. */

  n = new AVLNode;
  n->key = key;
  n->val = val;
  n->parent = parent;
  n->height = 1;
  n->left = sentinel;
  n->right = sentinel;

  /* Use the correct pointer in the parent to point to the new node. */

  if (parent == sentinel) {
    sentinel->right = n;
  } else if (key < parent->key) {
    parent->left = n;
  } else {
    parent->right = n;
  }

  // From inserted node n until n equals sentinel
  while(n != sentinel){

	  // store height in a variable
	  h = n->height;
	  // fix height
	  fix_height(n);
	  // if heights stays the same break
	  if(h == n->height){
		  break;
	  }
	  // if imbalance fix imbalance and break
	  if(imbalance(n)){
		  fix_imbalance(n);
		  break;
	  }
	  // go up a node
	  n = n->parent;
  }

  // Increment the size
  size++;

  return true;
}
    
bool AVLTree::Delete(const string &key)
{
  AVLNode *n, *parent, *mlc;
  string tmpkey;
  void *tmpval;

  /* Try to find the key -- if you can't return false. */

  n = sentinel->right;
  while (n != sentinel && key != n->key) {
    n = (key < n->key) ? n->left : n->right;
  }
  if (n == sentinel) return false;

  /* We go through the three cases for deletion, although it's a little
     different from the canonical explanation. */

  parent = n->parent;

  /* Case 1 - I have no left child.  Replace me with my right child.
     Note that this handles the case of having no children, too. */

  if (n->left == sentinel) {
    if (n == parent->left) {
      parent->left = n->right;
    } else {
      parent->right = n->right;
    }
    if (n->right != sentinel) n->right->parent = parent;
    delete n;
    size--;

  /* Case 2 - I have no right child.  Replace me with my left child. */

  } else if (n->right == sentinel) {
    if (n == parent->left) {
      parent->left = n->left;
    } else {
      parent->right = n->left;
    }
    n->left->parent = parent;
    delete n;
    size--;

  /* If I have two children, then find the node "before" me in the tree.
     That node will have no right child, so I can recursively delete it.
     When I'm done, I'll replace the key and val of n with the key and
     val of the deleted node.  You'll note that the recursive call 
     updates the size, so you don't have to do it here. */

  } else {
    for (mlc = n->left; mlc->right != sentinel; mlc = mlc->right) ;
    tmpkey = mlc->key;
    tmpval = mlc->val;
    Delete(tmpkey);
    n->key = tmpkey;
    n->val = tmpval;
    return true;
  }

  // from n = parent go up until sentinel
  n = n->parent;
  while(n != sentinel){

	  // fix height
	  fix_height(n);
	  // if imbalance fix
	  if(imbalance(n)){
		  fix_imbalance(n);
	  }
	  // go up node
	  n = n->parent;

  }

  return true;
}
               
/* You need to write these two.  You can lift them verbatim from your
   binary search tree lab. */

vector <string> AVLTree::Ordered_Keys() const
{
  vector <string> rv;
  AVLNode *node;
  node = sentinel->right;
  make_key_vector(node, rv);
  return rv;

}
    
void AVLTree::make_key_vector(const AVLNode *n, vector<string> &v) const
{
	if(n == sentinel){
		return;
	}
	make_key_vector(n->left, v);
	v.push_back(n->key);
	make_key_vector(n->right, v);
}
     
size_t AVLTree::Height() const
{
  return sentinel->right->height;
}
		
/* You need to write this to help you with the assignment overload.
   It makes a copy of the subtree rooted by n.  That subtree is part
   of a different tree -- the copy will be part of the tree that
   is calling the method. */

AVLNode *AVLTree::recursive_postorder_copy(const AVLNode *n) const
{
	AVLNode *nl, *nr;

	if(n->left != sentinel){
		nl = recursive_postorder_copy(n->left);
	}
	if(n->right != sentinel){
		nr = recursive_postorder_copy(n->right);
	}

	AVLNode *i = new AVLNode();
	i->left = nl;
	i->right = nr;
	i->parent = sentinel;
	i->key = n->key;
	i->val = n->val;
	i->height = n->height;

	if(nl != sentinel){
		nl->parent = i;
	}
	if(nr != sentinel){
		nr->parent = i;
	}

	return i;
}
