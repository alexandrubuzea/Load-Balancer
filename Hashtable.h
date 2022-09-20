/*  Copyright 2021 Buzea Alexandru-Mihai-Iulian
	and Data Structures Team (for skel and hash functions  */

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

#include "LinkedList.h"

#define MAX_BUCKET_SIZE 30

/*  This structure will be used in order to store key-value pairs in
 *  our hashtable.
 */

struct info {
	void *key;
	void *value;
};

/*  This structure will be used to store our data and to organize it
 *  properly (such that adding a pair, removing a pair and finding
 *  a pair should happen in O(1)).
 */

typedef struct hashtable_t hashtable_t;
struct hashtable_t {
	linked_list_t **buckets;
	unsigned int size;
	unsigned int hmax;
	unsigned int (*hash_function)(void*);
	int (*compare_function)(void*, void*);
};

/*  This function creates (allocs) a hashtable using malloc()
 */

hashtable_t *
ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
		int (*compare_function)(void*, void*));

/*  This function is used to store (add) a pair. We use void * in
 *  order to create a general hashtable (a hashtable that can store)
 *  all kinds of datatypes.
 */

void
ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size);

/*  This function returns the value associated with the given key.
 *  If the key is not found, the function returns NULL.
 */

void *
ht_get(hashtable_t *ht, void *key);

/*  This function returns just logical values: 1 (if the given key
 *  does exist in the hashtable), or 0 (if the key does not exist).
 */

int
ht_has_key(hashtable_t *ht, void *key);

/*  This function removes a pair identified by the key (if the key does
 *  not exist, no removal or replacement occurs).
 */

void
ht_remove_entry(hashtable_t *ht, void *key);

/*  This function returns the number of non-empty buckets (lists that
 *  have at least one node with a valid key-value pair).
 */

unsigned int
ht_get_size(hashtable_t *ht);

/*  This function returns the total number of buckets of the hashtable
 *  (even if they are empty or not).
 */

unsigned int
ht_get_hmax(hashtable_t *ht);

/*  This function frees all the allocated memory of the hashtable.
 *  #nomemoryleaks
 */

void
ht_free(hashtable_t *ht);

/*  This function is used to compare strings (can be added in a
 *  struct hashtable if the keys are strings). Based on strcmp
 *  from string.h library.
 */

int
compare_function_strings(void *a, void *b);

/*  This function is used to get the hash of a key (only if keys are
 *  strings) based on a certain formula.
 */

unsigned int
hash_function_string(void *a);

#endif  // HASHTABLE_H_
