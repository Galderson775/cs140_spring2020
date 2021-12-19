// Robert Schenck
// 4/17/2020

#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include "bstree.hpp"
using namespace std;
using CS140::BSTree;
using CS140::BSTNode;

/* ---------------------------------------------- */
/* You'll write all of these as part of your lab. */

// Finds and returns the depth of the tree
int BSTree::Depth(const string &key) const{

	int depth = 0;
	BSTNode *node;
	node = sentinel->right;
	while(true){

		if(node == sentinel){
			return -1;
		}
		if(key == node->key){
			return depth;
		}
		node = (key < node->key) ? node->left : node->right;
		depth++;
	}
	return depth;

}
 
// Finds and returns the height of the tree
int BSTree::Height() const{

	int height;
	BSTNode *node;
	node = sentinel->right;
	height = recursive_find_height(node);
	height += 1;
	return height;

}

// Creates a vector of the keys in order
vector <string> BSTree::Ordered_Keys() const{

  vector <string> rv;
  BSTNode *node;
  node = sentinel->right;
  make_key_vector(node, rv);
  return rv;

}
 
// Copy constructor
BSTree::BSTree(const BSTree &t){

  sentinel = new BSTNode;
  sentinel->left = NULL;
  sentinel->right = sentinel;
  sentinel->parent = NULL;
  sentinel->key = "---SENTINEL---";
  sentinel->val = NULL;
  size = 0;

  operator=(t);

}

// Assignment Overload
BSTree& BSTree::operator= (const BSTree &t){

  Clear();

  vector<string> keys = t.Ordered_Keys();
  vector<void *> vals = t.Ordered_Vals();
  return *this;

}

// recursive helper function for height
int BSTree::recursive_find_height(const BSTNode *n) const{

	int height, temp_height;
	if(n !=sentinel){

		height = Depth(n->key);
		temp_height = recursive_find_height(n->right);
		if(temp_height > height){
			height = temp_height;
		}
		temp_height = recursive_find_height(n->left);
		if(temp_height > height){
			height = temp_height;
		}
	}
	return height;

}

// recursive function that makes vector for the keys
void BSTree::make_key_vector(const BSTNode *n, vector<string> &v) const{

  if(n == sentinel){
	  return;
  }
  
  make_key_vector(n->left, v);
  v.push_back(n->key);
  make_key_vector(n->right, v);

}

// makes a balanced tree
BSTNode *BSTree::make_balanced_tree(const vector<string> &sorted_keys, 
                            const vector<void *> &vals,
                            size_t first_index,
                            size_t num_indices) const{

	if(first_index > num_indices){
		return sentinel;
	}

	if((num_indices / 2) == 0){
		return sentinel;
	}
	BSTNode *i = new BSTNode;
	i->key = sorted_keys[num_indices];
	i->val = vals[num_indices];
	if(num_indices > 1){
		i->left = make_balanced_tree(sorted_keys, vals, first_index, num_indices / 2);
		if(num_indices > 2){
			i->right = make_balanced_tree(sorted_keys, vals, first_index + 2, num_indices);
		}
	}
	if(num_indices == sorted_keys.size()){
		sentinel->right = i;
	}
	i->left->parent = i;
	i->right->parent = i;

	return i;


}
