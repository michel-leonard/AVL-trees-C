#include "bst.h"

// https://github.com/key-value-pairs-C/Iterative-AVL

struct bst_entry *bst_at(struct bst_manager *const manager, const char *const key) {
	// #1 Abdul Bari jDM6_TnYIqE
	static struct bst_node *nodes[4];
	static char op[3];
	static int i, j;
	if (manager->root) {
		nodes[2] = manager->root;
		for (;;) {
			i = strcmp(key, nodes[2]->entry.key);
			if (i > 0) {
				if (nodes[2]->rel[1]) {
					nodes[2] = nodes[2]->rel[1];
				} else {
					if (manager->search_only)
						return 0;
					i = 1;
					break;
				}
			} else if (i) {
				if (nodes[2]->rel[0]) {
					nodes[2] = nodes[2]->rel[0];
				} else {
					if (manager->search_only)
						return 0;
					i = 0;
					break;
				}
			} else {
				manager->affected = 0;
				return &nodes[2]->entry;
			}
		}
	} else if (manager->search_only)
		return 0;
	else {
		nodes[2] = 0;
		i = -1;
	}
	const size_t bytes = 1 + strlen(key);
	// This may replace root.
	nodes[3] = *(i >= 0 ? &nodes[2]->rel[i] : &manager->root) = malloc(sizeof(struct bst_node) + bytes);
	// You can handle --> memory allocation <-- error if nodes[3] == 0
	memset(nodes[3], 0, sizeof(struct bst_node));
	nodes[3]->entry.key = (char *) (1 + nodes[3]);
	memcpy(nodes[3]->entry.key, key, bytes);
	nodes[3]->height = 1;
	nodes[3]->rel[2] = nodes[2];
	// Maybe nodes[2] is now too tall.
	for (; nodes[2] ; nodes[2] = nodes[2]->rel[2]) {
		i = nodes[2]->height;
		BST_HEIGHT(nodes[2]);
		if (i == nodes[2]->height) // Height not change ?
			break; // Retracing can stop.
		*op = BST_HAS_OP(nodes[2]);
		if (*op) {
			nodes[1] = nodes[2]->rel[*op == 'R'];
			op[1] = BST_OP_KIND(nodes[1]);
			i = op[1] == 'R', j = op[0] == op[1];
			nodes[0] = nodes[1]->rel[i];
			BST_SWAP(nodes[2], nodes[j]);
			BST_ROTATE(nodes[j], rel[!i], nodes[1 + j], rel[i]);
			nodes[2]->height = nodes[0]->height; // LL or RR done.
			if (j == 0) {
				BST_ROTATE(nodes[0], rel[i], nodes[2], rel[!i]); // The 3 height will change.
				nodes[1]->height = nodes[0]->height++; // LR or RL done.
			}
			// Rebalanced, nobody above would notice.
			break;
		}
	}
	++manager->count;
	manager->affected = 1;
	return &nodes[3]->entry;
}

void bst_rm(struct bst_manager *const manager, const char *const key) {
	static struct bst_node *nodes[3];
	static char op[3];
	static int i, j;
	if (manager->root) {
		nodes[0] = manager->root;
		for (;;) {
			i = strcmp(key, nodes[0]->entry.key);
			if (i) {
				nodes[0] = nodes[0]->rel[i > 0];
				if (nodes[0] == 0) {
					manager->affected = 0;
					return;
				}
			} else
				break;
		}
	} else {
		manager->affected = 0;
		return;
	}
	// Normal search succeed.
	if (nodes[0]->rel[1]) {
		nodes[1] = nodes[0]->rel[1];
		if (nodes[0]->rel[0]) {
			if (nodes[1]->rel[0]) {
				do
					nodes[1] = nodes[1]->rel[0];
				while (nodes[1]->rel[0]); // Inorder successor.
				nodes[2] = nodes[1]->rel[2];
				BST_ASSIGN(nodes[1], rel[1], nodes[1]->rel[2], rel[0]);
				nodes[1]->rel[1] = nodes[0]->rel[1];
				nodes[1]->rel[1]->rel[2] = nodes[1];
				nodes[1]->height = nodes[0]->height;
			} else
				(nodes[2] = nodes[1])->height = 1 + nodes[0]->rel[0]->height;
		} else
			(nodes[2] = nodes[1])->height = 1;
		BST_ASSIGN(nodes[0], rel[0], nodes[1], rel[0]);
		nodes[1]->rel[2] = nodes[0]->rel[2];
	} else if (nodes[0]->rel[0]) {
		nodes[2] = nodes[1] = nodes[0]->rel[0];
		nodes[1]->rel[2] = nodes[0]->rel[2];
	} else {
		nodes[1] = 0;
		nodes[2] = nodes[0]->rel[2];
	}
	// This may replace root.
	*(nodes[0]->rel[2] ? &nodes[0]->rel[2]->rel[nodes[0] == nodes[0]->rel[2]->rel[1]] : &manager->root) = nodes[1];

	// Regular Binary Search Tree deletion done.
	// Releasing the dynamically allocated Memory.
	free(nodes[0]);

	for (i = 0 ; nodes[2] ; nodes[2] = nodes[2]->rel[2]) {
		j = nodes[2]->height;
		BST_HEIGHT(nodes[2]);
		if (j == nodes[2]->height && ++i == 3)
			break; // Retracing can stop.
		*op = BST_HAS_OP(nodes[2]);
		if (*op) {
			nodes[1] = nodes[2]->rel[*op == 'R'];
			for (;;) {
				op[1] = BST_OP_KIND(nodes[1]);
				i = op[1] == 'R', j = op[0] == op[1];
				nodes[0] = nodes[1]->rel[i];
				BST_SWAP(nodes[2], nodes[j]);
				BST_ROTATE(nodes[j], rel[!i], nodes[1 + j], rel[i]); // LL or RR done.
				if (j) {
					i = nodes[2]->rel[1] && nodes[2]->rel[1]->height >= nodes[0]->height;
					nodes[2]->height = nodes[i]->height;
				} else {
					BST_ROTATE(nodes[0], rel[i], nodes[2], rel[!i]); // LR or RL done.
					if (nodes[1]->rel[0] == 0 || nodes[1]->rel[0]->height < nodes[0]->height)
						nodes[2]->height = nodes[1]->height = nodes[i = 0]->height;
					else {
						nodes[2]->height = nodes[0]->height;
						if (2 == (nodes[1]->rel[1] ? nodes[2]->height - nodes[1]->rel[1]->height : nodes[2]->height)) {
							*op = 'L';
							nodes[2] = nodes[1];
							nodes[1] = nodes[2]->rel[0];
							continue;
						}
					}
				}
				break;
			}
			i += i >= j; // Will break.
		}
	}
	// Rebalanced out.
	manager->affected = 1;
	--manager->count;
}

void bst_each(const struct bst_manager *manager, void(*fn)(struct bst_entry *, const void *args), const void *args, enum bst_direction i) {
	struct bst_node *curr, *prev;
	const int j = !i;
	if (manager->root) {
		curr = manager->root;
		while (curr)
			if (curr->rel[i]) {
				prev = curr->rel[i];
				while (prev->rel[j] && prev->rel[j] != curr)
					prev = prev->rel[j];
				if (prev->rel[j]) {
					prev->rel[j] = 0;
					fn(&curr->entry, args);
					curr = curr->rel[j];
				} else {
					prev->rel[j] = curr;
					curr = curr->rel[i];
				}
			} else {
				fn(&curr->entry, args);
				curr = curr->rel[j];
			}
	}
}

void bst_destroy(struct bst_manager *const manager) {
	if (manager->root) {
		size_t index = 1;
		struct bst_node **stack = malloc(manager->root->height << 1 * sizeof(struct bst_node *)), *curr;
		stack[0] = manager->root;
		do {
			curr = stack[--index];
			if (curr->rel[0])
				stack[index++] = curr->rel[0];
			if (curr->rel[1])
				stack[index++] = curr->rel[1];
			free(curr);
		} while (index);
		free(stack);
		manager->root = 0;
		manager->affected = 1;
	}
	else
		manager->affected = 0;
}
