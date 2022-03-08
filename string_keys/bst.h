#ifndef BST_BST_H
#define BST_BST_H

#include <string.h>
#include <stdlib.h>

//  Fast Iterative C AVL Binary Search Tree Implementation : https://j.mp/C-AVL
// "Trees are the earth's endless effort to speak to the listening heaven."

// AVL tree is a self-balancing binary search tree, AVL stands for Adelson-Velskii and Landis :
// - Georgy Maximovich Adelson-Velsky (1922 –2014) was a Soviet and Israeli mathematician and computer scientist
// - Yevgeny Mikhaylovich Landis (1921 – 1997) was a Soviet mathematician

enum bst_direction {
	BST_LOW_TO_HIGH,
	BST_HIGH_TO_LOW,
};

struct bst_entry {
	char * key;
	union {
		double my_val ;
		int another_val ;
		// add in the union any type you want to handle as a key-value pair. <-- like in a MAP
		// you can also remove the value field if not needed. <-- like in a SET
	} value;
};

struct bst_node {
	struct bst_entry entry;
	struct bst_node * rel [3]; // [0, 1, 2] = [left child, right child, parent]
	int height ;
};

struct bst_manager {
	struct bst_node *root;
	size_t count;
	int affected;
	int search_only;
};

#define BST_ASSIGN(n_1, r_1, n_2, r_2) ((((n_2)->r_2 = (n_1)->r_1)) ? ((n_1)->r_1->rel[2] = (n_2)) : ((n_2)->r_2 = ((n_1)->r_1)))
#define BST_ROTATE(n_1, r_1, n_2, r_2) BST_ASSIGN(n_1, r_1, n_2, r_2); (n_2)->rel[2] = (n_1); (n_1)->r_1 = (n_2)
#define BST_SWAP(n_old, n_new) *((n_old)->rel[2] ? &(n_old)->rel[2]->rel[(n_old)->rel[2]->rel[1] == (n_old)] : &manager->root) = (n_new); (n_new)->rel[2] = (n_old)->rel[2]
#define BST_HEIGHT(a) ((a) -> height = (a)->rel[0] ? 1 + ((a)->rel[1] ? (a)->rel[(a)->rel[1]->height > (a)->rel[0]->height]->height : (a)->rel[0]->height) : (a)->rel[1] ? 1 + (a)->rel[1]->height : 1)
#define BST_HAS_OP(a) ((char) ((a)->height > 2 ? (a)->rel[0] ? (a)->rel[1] ? (a)->rel[0]->height > (a)->rel[1]->height ? (a)->rel[0]->height - (a)->rel[1]->height == 2 ? 'L' : 0 : (a)->rel[1]->height - (a)->rel[0]->height == 2 ? 'R' : 0 : (a)->rel[0]->height == 2 ? 'L' : 0 : (a)->rel[1] ? (a)->rel[1]->height == 2 ? 'R' : 0 : 0 : 0))
#define BST_OP_KIND(a) ((char) ((a)->rel[0] ? (a)->rel[1] ? nodes[1]->rel[0]->height > (a)->rel[1]->height ? 'L' : 'R' : 'L' : 'R'))

// Tested using string keys with compiler options -g -O2 -std=c99.
// Insertion of [ 1M, 10M, 100M ] sorted keys in [ 0.4s, 4s, 40s ].
// Rotations counter was [ 919k, 9M, 91M ]. <!-- YzSJBowPECY -->
struct bst_entry *bst_at(struct bst_manager *manager, const char *key);

// Tested using string keys with compiler options -g -O2 -std=c99.
// Deletion of [ 1M, 10M, 100M ] sorted keys in [ 0.4s, 4s, 41s ].
// Rotations counter was [ 527k, 5M, 52M ]. <!-- YzSJBowPECY -->
void bst_rm(struct bst_manager *manager, const char *key);

void bst_each(const struct bst_manager *manager, void(*fn)(struct bst_entry *, const void *args), const void *args, enum bst_direction i);

void bst_destroy(struct bst_manager *manager);

#endif
