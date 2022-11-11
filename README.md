
# Fast Iterative C AVL Binary Search Tree
Basic C99 **100% iterative** implementation of AVL trees (self-balanced binary search trees).\
Time complexity for all operation (access, insertion, deletion) is `O(log(n))`.

This ~300 lines C software :
* uses a **height field** and a **parent pointer** for each tree node
* can be used to manage one or more SET (keys) or MAP (key-value pairs)
* organize quick lookup, insertion, or deletion of a key in such a way that your tree always remains balanced

Every binary search tree guarantee `O(height)` worst-case complexity for lookup, insertion, and deletion.\
Thanks to the rotations, the heights of your BSTs are always lesser than `1.44 * log2(number of keys)`.

# Status

Ready for deployment to a production environment.

### Real use of AVL

There is one github project that this [AVL](https://github.com/michel-leonard/C-Quadratic-Sieve#AVL-trees) logic, it's a Pure C factorizer using self-initialising **Quadratic Sieve**.

In 2022, AVL has never been involved in any issues on this project, which has been tested for weeks and days non-stop.

# Functions
The 4 provided functions are :
* [bst_at](#bst_at)
* [bst_rm](#bst_rm)
* [bst_each](#bst_each)
* [bst_destroy](#bst_destroy)

# Structures
The 3 provided structures :
* [bst_manager](#bst_manager)
* [bst_entry](#bst_entry)
* [bst_node](#bst_node) (internal use only)

# Synopsis

```c
#include <stdio.h>
#include <ctype.h>
#include "bst.c"

int main() {

// Let say you are reading some text before exiting.
char input[] = "The problem of a rapid search is well illustrated "
"in terms of looking up a word in an ordinary dictionary containing "
"n words. Finding a given word in such a dictionary requires "
"at most C log n operations. How to organize a quick lookup "
"in a dictionary whose content is constantly changing ?"
"This problem was solved by Adelson-Velsky and Landis."
"It turns out that words in a constantly growing dictionary should "
"not be ordered linearly, but rather should be organized as a "
"binary tree with a natural ordering: the right subtree goes up "
"and the left subtree goes down. The tree should be balanced,"
"that is, the heights of the left and right child subtrees "
"of any node should differ by at most one. Lookup of a word in "
"such a tree with n nodes also requires C log n operations."
"But what is most important is that both deletion and insertion "
"in such a tree require the same number of operations. However,"
"when a new word is inserted and takes its place in the naturally "
"ordered tree (which requires C log n operations), the tree may "
"become unbalanced: the height of the right subtree rooted at "
"some 'unbalanced' node may differ from the height of the left "
"subtree by more than one."

"The AVL algorithm in this case performs a rebalancing of the tree "
"in a neighbourhood of the 'unbalanced' node with a finite"
"(independent of n) number of operations in such a way that "
"the tree becomes balanced again.";

// You configured bst.h like that :

/*
	struct bst_entry {
		char * key;
		union {
			int occurences ;
		} value;
	};
*/

// You create a bst_manager.
struct bst_manager m = {0};

for (char *word = input, *curr = word ; *word ; word = curr, curr = word) {
	while (isalpha(*curr)) ++curr;
	*curr = 0;
	// You basically count the number of occurences of words.
	++bst_at(&m, word)->value.occurences;
	while (*++curr && !isalpha(*curr));
}

// And print some results.
printf("Found %d words.\n", (int)m.count);

const char *words[] = {"tree", "should", "be", "balanced", 0};
for (int i = 0 ; words[i] ; ++i) {
	const struct bst_entry *word = bst_at(&m, words[i]);
	printf("'%s' has %d occurences.\n", words[i], word->value.occurences);
}

// Found 123 words.
// 'tree'     has 8 occurences.
// 'should'   has 4 occurences.
// 'be'       has 3 occurences.
// 'balanced' has 2 occurences.

// Then destroy your bst_manager.
bst_destroy(&m);

return 0 ;

}

```

bst_at
------------

**parameter 1:** *struct [bst_manager](#bst_manager) \* manager*\
**parameter 2:** *char \* key* **OR** *int key* **OR** *custom key*\
**return value:** *struct [bst_entry](#bst_entry) \**

Perform a regular key lookup into the manager's binary search tree.\
The duplicates are not allowed, a key requested multiple times result to the same [bst_entry](#bst_entry) \*.

**The key is found :**
* *bst_at* return the `bst_entry *` corresponding to the key
* *bst_at* will have set the `manager->affected` field to to 0

otherwise,

**The `manager->search_only` field is false** (the default) :
* *bst_at* insert the key into the `bst_manager` tree (using malloc)
* *bst_at* works in such a way that your tree always remains balanced
* *bst_at* return the `bst_entry *` corresponding to the key
* *bst_at* will have set the `manager->affected` field to to 1
* *bst_at* will have incremented the `manager->count` counter of entries

**The `manager->search_only` field is true** (you configured it) :
* *bst_at* return 0
* *bst_at* will have set the `manager->affected` field to to 0

```c
bst_at(&m, str); // reading or inserting the key str to the manager's binary search tree.
```

bst_rm
------------

**parameter 1:** *struct [bst_manager](#bst_manager) \* manager*\
**parameter 2:** *char \* key* **OR** *int key* **OR** *custom key*\
**return value:** *void*

Perform a deletion of a key into the manager's binary search tree.

**The key is found :**
* *bst_rm* delete the key from the tree, and release the memory (using free)
* *bst_rm* works in such a way that your tree always remains balanced
* *bst_rm* will have set the `manager->affected` field to to 1
* *bst_at* will have decremented the `manager->count` counter of entries

**The key is not found :**
* *bst_rm* will have set the `manager->affected` field to to 0

```c
bst_rm(&m, str); // removing the key str from the manager's binary search tree.
```

bst_each
------------

**parameter 1:** *struct [bst_manager](#bst_manager) \* manager*\
**parameter 2:** *void(\*fn)(struct [bst_entry](#bst_entry) \*, void \*args)\
**parameter 3:** *void \*args\
**parameter 4:** *enum [bst_direction](#bst_direction) direction\
**return value:** *void*

Perform a Morris tree traversal to execute your callback over each entry.\
The space complexity of this operation is O(1).

**Your callback will receive :**
* a struct `bst_entry *` as first argument
* the `void *args` parameter 3 as second argument

The execution will be done from `BST_LOW_TO_HIGH` or from `BST_HIGH_TO_LOW`, accordingly to the last parameter.

```c
// assuming your_function have to print all keys from low to high.
void your_function(struct bst_entry *entry, const void *separator) { 
  printf("%s%s", entry->key, (char *) separator);
}

// executing your function on each node with your given separator.
bst_each(&m, &your_function, &" - ", BST_LOW_TO_HIGH);
```

bst_destroy
------------

**parameter 1:** *struct [bst_manager](#bst_manager) \*manager*\
**return value:** *void*

Clear the manager's binary search tree, free all the memory using a stack.

```c
bst_destroy(&m); // terminate the work.
```

# bst_manager
The [bst_manager](../main/integer_keys/bst.h) is a simple C99 struct, which must be zeroed to start the job properly, it holds :
* The (bst_node \*) root of the tree (software set, user read)
* A (size_t) counter of entries (software set, user read)
* A (boolean) search_only field (user set, software read)
* A (boolean) affected field (software set, user read)

```c
struct bst_manager m = {0}; // zeroing a manager, it's now ready to start.
```

The counter of entries is updated upon insertion or deletion.

**The `manager->search_only` field :**
* you can set it to 1, so no insertion will be done if the key is absent.
* you can set it to 0, so the insertion will be done if the key is absent.

The `manager->afffected field` is set to 1 if the insertion or the deletion happened just now, 0 otherwise.\
You can use multiple bst_manager to handle multiple trees.

# bst_entry
This C struct holding a key and a value represent your data, every [bst_node](#bst_node) contains a bst_entry. 

The demo is showing some examples with string key and integer key.\
You can easily use a custom type of key, by executing minor changes in the source (update the signature of functions and the comparator).

```c
// Configured like a basic SET manager (keys only).
struct bst_entry {
		char * key;
	};
	
// Configured like a basic MAP manager (key-value pairs).
struct bst_entry {
		char * key;
		union {
			int occurences ;
			// add to the union all types you will use as value.
		} value;
	};
```
You can safely remove the value field if not needed, it's provided by default for convenience.

# bst_direction
The bst_direction is an enum that contains two values :
* BST_LOW_TO_HIGH
* BST_HIGH_TO_LOW

It can be used with [bst_each](#bst_each) to indicate which direction it should perform its traversal.

```c
// executing your function on each node with your given separator.
bst_each(&m, &your_callback, 0, BST_HIGH_TO_LOW);
```

# bst_node
The bst_node is a simple C99 struct always used internally and never shown holding :
* struct [bst_entry](#bst_entry) entry (the data)
* struct bst_node * rel[3] (the 3 relations, left child, right child and parent)
* int height (always equal to `1 + Max(height left child, height right child)`)


# Get started
The provided tests are using both [string](../main/string_keys/main.c) and [integer](../main/integer_keys/main.c) keys, you can copy some code.\
The provided tests are testing all functions, excepted bst_destroy (ask valgrind --leak-check=full for it).\
If you want to associate a value with a key, the  struct holds a value field.

```c
struct bst_manager m = {0}; // zeroing a manager.
bst_at(&m, "key")->value.my_val = 25; // insert a new entry and set its value on the fly.
bst_at(&m, "key")->value.my_val == 25; // read the value from the manager's tree.
bst_rm(&m, "key"); // remove the entry holding that key.
bst_destroy(&m); // clear the manager.
```

# The compilation
The following parameters i used.
```
cmake_minimum_required(VERSION 3.17)
project(bst C)

set(CMAKE_C_STANDARD 99)
set (CMAKE_C_FLAGS "-Wall -Wextra -pedantic -g -O2 -std=c99")

#add_executable(bst integer_keys/main.c)
add_executable(bst strings_keys/main.c)
```

# Other / Technical
If you use this software into a multithreaded program, you should remove the 6 **static** keywords in the source code.\
Every node in the tree know its parent, only the root node has 0 for parent, the [bst_manager](#bst_manager) hold this particular node.

To explain rotations : "when imbalance is encountered, you send the middle value (it involves 3 nodes) to the root."\
This software know when imbalance is encountered because it keep updated a height field for each node.\
There is a verifier in the tests that asserts the formula of height for each node.\
This C implementation assumes that the height of 0 (no node) is 0.

- When **inserting** an element, `at most one rotation` is done
- When **deleting** an element, `zero, one or more than one rotation` may be done

When [bst_at](#bst_at) and [bst_rm](#bst_rm) functions are performing standard rotation 3 nodes are involved :
- nodes[2] is on the top (parent)
- nodes[1] is on the path (child of 2)
- nodes[0] is on the bottom (child of 1)

During the rotation the **op** variable holds the rotation name (RR, RL, LL or LR).\
This software will break the retracing loop as soon as possible, i think it will not unnecessarily verify that 1 = 1.

# My tests
```
I always use C++ and 10k managers per test, then insert between 0 and 5k entries.
You can see a lite version of my verifier in the tests, the original :
- verifies the whole tree after each operation
- expects using a recursive function to find the right heights
- expects (by opposing an unordered set) to retrieve all values previously inserted
- expects (by opposing a shuffled vector) to retrieve all values previously inserted
- expects (by opposing a sorted vector) to retrieve all values previously inserted
- all of it and more when inserting and/or deleting

Using string keys with compiler options -g -O2 -std=c99.
Insertion of [ 1M, 10M, 100M ] sorted keys in [ 0.4s, 4s, 40s ].
Rotations counter was [ 919k, 9M, 91M ]. <!-- YzSJBowPECY -->
```

# AVL
AVL tree is a self-balancing binary search tree, AVL stands for Adelson-Velskii and Landis :
* Georgy Maximovich [Adelson-Velsky](https://www.math.toronto.edu/askold/2014-UMN-4-e-Adelson-.pdf "Adelson-Velsk") (1922-2014) was a Soviet and Israeli mathematician and computer scientist
* Yevgeny Mikhaylovich Landis (1921-1997) was a Soviet mathematician

Shortest link to this page : [bit.ly/C-AVL](http://bit.ly/C-AVL)
