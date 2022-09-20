/*  Copyright 2021 Buzea Alexandru-Mihai-Iulian 311CAb  */

#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include "utils.h"

/*  Basic structure for a node in a linked list.
 */

typedef struct ll_node_t ll_node_t;
struct ll_node_t
{
    void* data;
    ll_node_t* next;
};

/*  A structure that stores a pointer to the first node in the linked list
 *  and other parameters of the list.
 */

typedef struct linked_list_t linked_list_t;
struct linked_list_t
{
    ll_node_t* head;
    unsigned int data_size;
    unsigned int size;
};

/*  A function that creates (allocs memory) a linked list and initialises
 *  its length and data_size (a general linked list, that can store all
 *  kinds of datatypes).
 */

linked_list_t*
ll_create(unsigned int data_size);

/*  A function that adds a node with the given data (it uses deep copy)
 *  on the nth position.
 */

void
ll_add_nth_node(linked_list_t* list, unsigned int n, const void* data);

/*  A function that removes (and returns !) the nth node from the linked
 * list. The one that uses this function must free its memory!
 */

ll_node_t*
ll_remove_nth_node(linked_list_t* list, unsigned int n);

/*  A function that returns the size of a linked list.
 */

unsigned int
ll_get_size(linked_list_t* list);

/*  A function that frees all the allocated memory of the linked list.
 *  #nomemoryleaks.
 */

void
ll_free(linked_list_t** pp_list);

#endif  // LINKEDLIST_H_
