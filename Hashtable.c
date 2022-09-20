/*  Copyright 2021 Buzea Alexandru-Mihai-Iulian 311CAb  
    and Data Structures Team (for hash functions)  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#include "Hashtable.h"

int
compare_function_strings(void *a, void *b)
{
	char *str_a = (char *)a;
	char *str_b = (char *)b;

	return strcmp(str_a, str_b);
}

unsigned int
hash_function_string(void *a)
{
	unsigned char *puchar_a = (unsigned char*) a;
	unsigned long hash = 5381;
	int c;

	while ((c = *puchar_a++))
		hash = ((hash << 5u) + hash) + c;

	return hash;
}

hashtable_t *
ht_create(unsigned int hmax, unsigned int (*hash_function)(void*),
		int (*compare_function)(void*, void*))
{
	/* allocation of memory for the hashtable */

	hashtable_t *table = (hashtable_t *)malloc(sizeof(hashtable_t));
	DIE(!table, "Malloc failed\n");

	/* initialization of the fields */

	table->hmax = hmax;
	table->size = 0;
	table->hash_function = hash_function;
	table->compare_function = compare_function;
	table->buckets = (linked_list_t **)malloc(hmax * sizeof(linked_list_t *));
	DIE(!table->buckets, "Malloc failed\n");

	/* initialization of the buckets */

	for (unsigned int i = 0; i < hmax; ++i)
		table->buckets[i] = ll_create(sizeof(struct info));

	return table;
}

void
ht_put(hashtable_t *ht, void *key, unsigned int key_size,
	void *value, unsigned int value_size)
{
	if (!ht)
		return;

	unsigned int index = ht->hash_function(key) % ht->hmax;
	ll_node_t *it = ht->buckets[index]->head;

	/* if the bucket is empty, we need to increase the size of the
	 * hashtable - aka numebr of the non-empty buckets */

	if (it == NULL) {
		ht->size++;
		struct info *new = (struct info *)malloc(sizeof(struct info));
		new->key = malloc(key_size * sizeof(char));
		new->value = malloc(value_size * sizeof(char));
		memcpy(new->key, key, key_size);
		memcpy(new->value, value, value_size);
		ll_add_nth_node(ht->buckets[index], 0, new);
		free(new);
		return;
	}

	/* if we already have the key, we just need to overwrite the value */

	while (it != NULL) {
		void *it_key = ((struct info *)(it->data))->key;
		if (!ht->compare_function(it_key, key)) {
			void *it_value = ((struct info *)(it->data))->value;
			memcpy(it_value, value, value_size);
			return;
		}
		it = it->next;
	}

	/* if we didn't find the key, we add a new entry */

	struct info *new = (struct info *)malloc(sizeof(struct info));
	new->key = malloc(key_size * sizeof(char));
	new->value = malloc(value_size * sizeof(char));
	memcpy(new->key, key, key_size);
	memcpy(new->value, value, value_size);
	ll_add_nth_node(ht->buckets[index], ht->buckets[index]->size, new);
	free(new);
}

void *
ht_get(hashtable_t *ht, void *key)
{
	/* just searching through the bucket */

	unsigned int index = ht->hash_function(key) % ht->hmax;
	ll_node_t *it = ht->buckets[index]->head;
	while (it != NULL) {
		void *it_key = ((struct info *)(it->data))->key;
		if (!ht->compare_function(it_key, key))
			return (((struct info *)(it->data))->value);
		it = it->next;
	}
	return NULL;
}

int
ht_has_key(hashtable_t *ht, void *key)
{
	void *value = ht_get(ht, key);
	if (value != NULL)
		return 1;
	return 0;
}

void
ht_remove_entry(hashtable_t *ht, void *key)
{
	/* just searching through the bucket */

	unsigned int index = ht->hash_function(key) % ht->hmax;
	ll_node_t *it = ht->buckets[index]->head;
	int count = 0;
	while (it != NULL) {
		void *it_key = ((struct info *)(it->data))->key;
		if (!ht->compare_function(it_key, key)) {
			ll_node_t *del = ll_remove_nth_node(ht->buckets[index], count);
			free(((struct info *)(del->data))->key);
			free(((struct info *)(del->data))->value);
			free(del->data);
			free(del);
			if (ht->buckets[index]->size == 0)
				ht->size--;
			return;
		}
		++count;
		it = it->next;
	}
}

void
ht_free(hashtable_t *ht)
{
	/* freeing all the buckets and the hashtable itself #nomemoryleaks */

	for (unsigned int i = 0; i < ht->hmax; ++i) {
		ll_node_t *it = ht->buckets[i]->head;
		while (it != NULL) {
			free(((struct info *)(it->data))->key);
			free(((struct info *)(it->data))->value);
			it = it->next;
		}
		ll_free(ht->buckets + i);
	}
	free(ht->buckets);
	free(ht);
}

unsigned int
ht_get_size(hashtable_t *ht)
{
	if (ht == NULL)
		return 0;

	return ht->size;
}

unsigned int
ht_get_hmax(hashtable_t *ht)
{
	if (ht == NULL)
		return 0;

	return ht->hmax;
}
