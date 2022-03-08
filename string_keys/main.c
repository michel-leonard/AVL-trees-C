#include <sys/time.h>
#include <stdio.h>
#include <assert.h>
#include "bst.c"

double microtime();
const char *rand_str();
void verifier(const struct bst_node *node);
void bst_avl_test_1();
void bst_avl_test_2();
void bst_avl_test_3();
void bst_avl_test_4();
void bst_avl_test_5();
void bst_avl_test_6();
void bst_avl_test_7();

// C AvlTree - Basic iterative implementation of an AVL tree.
int main() {
	fprintf(stderr, "-------------- Iterative AVL C Implementation - Binary Search Tree ---------------\n");
	fprintf(stderr, "--- using STRING and integer keys, with and without key -> value association ---\n\n");
	srand((int) microtime());
	bst_avl_test_1();
	bst_avl_test_2();
	bst_avl_test_3();
	bst_avl_test_4();
	bst_avl_test_5();
	bst_avl_test_6();
	bst_avl_test_7();
	// Mon, 07 Mar 2022 - Student at Université de Franche-Comté.
}

// I offer you two choices: righter or faster (I know you win).
double microtime() {
	struct timeval time;
	gettimeofday(&time, 0);
	return (double) time.tv_sec + (double) time.tv_usec / 1e6;
}

const char *rand_str() {
	static char buffer[] = "15 chars buffer";
	char *str = buffer;
	do *str++ = (char) (97 + rand() % 26);
	while (*str);
	return buffer;
}

static int verifier_count = 0;

void verifier(const struct bst_node *node) {
	if (node) {
		++verifier_count;
		const int left = node->rel[0] ? node->rel[0]->height : 0;
		const int right = node->rel[1] ? node->rel[1]->height : 0;
		if (left > right) {
			assert(node->height == 1 + left);
			assert(left - right <= 1);
		} else {
			assert(node->height == 1 + right);
			assert(right - left <= 1);
		}
		verifier(node->rel[0]);
		verifier(node->rel[1]);
	}
}

void bst_avl_test_1() {
	verifier_count = 0;
	fprintf(stderr, "Insert %d random keys and verify the tree for each one.\n", 8281);
	struct bst_manager m = {0};
	for (int i = 0; i < 8281; ++i) {
		bst_at(&m, rand_str());
		verifier(m.root);
	}
	fprintf(stderr, "Delete the root until the tree is empty and verify each time.\n");
	fflush(stderr);
	while (m.root) {
		bst_rm(&m, m.root->entry.key);
		verifier(m.root);
	}
	fprintf(stderr, "Verifier is OK, its recursive counter is %d.\n\n", verifier_count);
	fflush(stderr);
	bst_destroy(&m);
}

void bst_avl_test_2() {
	verifier_count = 0;
	char str[3] = "--";
	fprintf(stderr, "Insert %d sorted keys and verify the tree for each one.\n", 26 * 26);
	struct bst_manager m = {0};
	for (str[0] = 'a'; str[0] <= 'z'; ++str[0])
		for (str[1] = 'a'; str[1] <= 'z'; ++str[1]) {
			bst_at(&m, str);
			verifier(m.root);
		}
	fprintf(stderr, "Delete the keys and verify each time.\n");
	fflush(stderr);
	for (str[0] = 'a'; str[0] <= 'z'; ++str[0])
		for (str[1] = 'a'; str[1] <= 'z'; ++str[1]) {
			bst_rm(&m, str);
			verifier(m.root);
		}
	fprintf(stderr, "Verifier is OK, its recursive counter is %d.\n\n", verifier_count);
	fflush(stderr);
	bst_destroy(&m);
}

void bst_avl_test_3() {
	verifier_count = 0;
	char str[3] = "--";
	fprintf(stderr, "Insert %d sorted keys and verify the tree for each one, also store a value.\n", 26 * 26);
	struct bst_manager m = {0};
	for (str[0] = 'a'; str[0] <= 'z'; ++str[0])
		for (str[1] = 'a'; str[1] <= 'z'; ++str[1]) {
			bst_at(&m, str)->value.my_val = str[0] + str[1];
			assert(m.affected == 1);
			bst_at(&m, str);
			assert(m.affected == 0);
			verifier(m.root);
		}
	fprintf(stderr, "Delete the keys (reverse) and verify each time, also check the value.\n");
	fflush(stderr);

	m.search_only = 1;
	for (str[0] = 'z'; str[0] >= 'a'; --str[0])
		for (str[1] = 'z'; str[1] >= 'a'; --str[1]) {
			assert(bst_at(&m, str)->value.my_val == str[0] + str[1]);
			bst_rm(&m, str);
			assert(m.affected == 1);
			assert(bst_at(&m, str) == 0);
			bst_rm(&m, str);
			assert(m.affected == 0);
			verifier(m.root);
		}
	fprintf(stderr, "Verifier is OK, its recursive counter is %d.\n\n", verifier_count);
	fflush(stderr);
	bst_destroy(&m);
}

void bst_avl_test_4() {
	char str[4] = "----";
	fprintf(stderr, "Insert %d sorted keys.", 26 * 26 * 26 * 26);
	struct bst_manager m = {0};
	double start = microtime();
	for (str[0] = 'a'; str[0] <= 'z'; ++str[0])
		for (str[1] = 'a'; str[1] <= 'z'; ++str[1])
			for (str[2] = 'a'; str[2] <= 'z'; ++str[2])
				for (str[3] = 'a'; str[3] <= 'z'; ++str[3])
					bst_at(&m, str);
	double end = microtime();
	fprintf(stderr, " Took %gs.\n", end - start);
	fflush(stderr);
	verifier(m.root);
	start = microtime();
	fprintf(stderr, "Delete the %d sorted keys from the rightmost.", 26 * 26 * 26 * 26);
	for (str[0] = 'z'; str[0] >= 'a'; --str[0])
		for (str[1] = 'z'; str[1] >= 'a'; --str[1])
			for (str[2] = 'z'; str[2] >= 'a'; --str[2])
				for (str[3] = 'z'; str[3] >= 'a'; --str[3])
					bst_rm(&m, str);
	end = microtime();
	fprintf(stderr, " Took %gs.\n\n", end - start);
	fflush(stderr);
	bst_destroy(&m);
}

void bst_avl_test_5() {
	char str[3] = "--";
	fprintf(stderr, "Insert %d keys, set a value, and read each value %d times.", 26 * 26, 26 * 26 * 26);
	struct bst_manager m = {0};
	double start = microtime();
	for (str[0] = 'a'; str[0] <= 'z'; ++str[0])
		for (str[1] = 'a'; str[1] <= 'z'; ++str[1])
			bst_at(&m, str)->value.my_val = str[0] + str[1];

	m.search_only = 1;
	for (int i = 0; i < 30000; ++i)
		for (str[0] = 'z'; str[0] >= 'a'; --str[0])
			for (str[1] = 'z'; str[1] >= 'a'; --str[1])
				assert(bst_at(&m, str)->value.my_val == str[0] + str[1]);

	double end = microtime();
	fprintf(stderr, " Took %gs.\n\n", end - start);
	fflush(stderr);
	bst_destroy(&m);
}

void bst_avl_test_6() {
	verifier_count = 0;
	char str[2] = "-";
	fprintf(stderr, "Play 3s with alphabet but verify the tree at each step.\n");
	struct bst_manager m = {0};
	const double end = 3 + microtime();
	while (microtime() < end) {
		fprintf(stderr, "*");
		for (int i = 0; i < 1000000; ++i) {
			str[0] = rand() % 26 + 65;
			switch (rand() % 5) {
				case 0:
				case 1:
					bst_at(&m, str);
					break;
				default:
					bst_rm(&m, str);
			}
			verifier(m.root);
			if (m.root == 0)
				fprintf(stderr, ".");
		}
	}
	fprintf(stderr, "\nVerifier is OK, its recursive counter is %d.\n\n", verifier_count);
	fflush(stderr);
	bst_destroy(&m);
}

void each_tester(struct bst_entry *entry, const void *arg) {
	fprintf(stderr, "%s%s", entry->key, (char *) arg);
}

void bst_avl_test_7() {
	verifier_count = 0;
	char str[2] = "-";
	fprintf(stderr, "Insert letters randomly and use the <each> function to print the keys.\n");
	struct bst_manager m = {0};
	for (int i = 0; i < 1000; ++i) {
		str[0] = rand() % 26 + 97;
		bst_at(&m, str);
	}
	bst_each(&m, &each_tester, &" < ", BST_LOW_TO_HIGH);
	fprintf(stderr, "\n");
	bst_each(&m, &each_tester, &" > ", BST_HIGH_TO_LOW);
	bst_destroy(&m);
}
