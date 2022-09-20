/*  Copyright 2021 Buzea Alexandru-Mihai-Iulian 311CAb  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LinkedList.h"

linked_list_t*
ll_create(unsigned int data_size)
{
    linked_list_t *list = (linked_list_t *)malloc(sizeof(linked_list_t));
    DIE(!list, "Malloc failed\n");
    list->head = NULL;
    list->size = 0;
    list->data_size = data_size;
    return list;
}

void
ll_add_nth_node(linked_list_t* list, unsigned int n, const void* new_data)
{
    ll_node_t *newnode = (ll_node_t *)malloc(sizeof(ll_node_t));
    DIE(!newnode, "Malloc failed\n");

    newnode->data = malloc(list->data_size * sizeof(char));
    DIE(!newnode->data, "Malloc failed\n");

    memcpy(newnode->data, new_data, list->data_size);

    /* if we need to add a node at the beginning of the list */

    if (n == 0 || list->size == 0) {
        newnode->next = list->head;
        list->head = newnode;
        list->size++;
        return;
    }

    /* adding a node in the general case */

    ll_node_t *current = list->head;
    if (n > list->size)
        n = list->size;

    for (unsigned int i = 0; i < n - 1; ++i) {
        current = current->next;
    }
    newnode->next = current->next;
    current->next = newnode;
    list->size++;
}

ll_node_t*
ll_remove_nth_node(linked_list_t* list, unsigned int n)
{
    ll_node_t *todelete;

    /* if we need to remove the first node */

    if (n == 0) {
        todelete = list->head;
        list->head = list->head->next;
        list->size--;
        return todelete;
    }

    if (n >= list->size)
        n = list->size - 1;

    /* removing the node in the general case */

    todelete = list->head;
    for (unsigned int i = 0; i < n - 1; ++i) {
        todelete = todelete->next;
    }

    ll_node_t *prevnode = todelete;
    todelete = todelete->next;
    prevnode->next = todelete->next;
    todelete->next = NULL;
    list->size--;

    return todelete;
}

unsigned int
ll_get_size(linked_list_t* list)
{
    if (!list)
        return 0;

    return list->size;
}

void
ll_free(linked_list_t** pp_list)
{
    if (!(*pp_list))
        return;

    /* freeing the list if list is empty */

    if (!(*pp_list)->head) {
        free(*pp_list);
        return;
    }

    /* freeing the list in general case #nomemoryleaks */

    ll_node_t *current = (*pp_list)->head;
    ll_node_t *nextnode = current->next;

    for (unsigned int i = 0; i < (*pp_list)->size - 1; ++i) {
        free(current->data);
        free(current);
        current = nextnode;
        nextnode = nextnode->next;
    }

    free(current->data);
    free(current);
    free(*pp_list);
    *pp_list = NULL;
}
