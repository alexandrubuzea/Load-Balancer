/*  Copyright 2021 Buzea Alexandru-Mihai-Iulian 311CAb
	and Data Structures Team (skel)  */

#include <stdlib.h>
#include <string.h>

#include "server.h"

server_memory* init_server_memory() {
    /*  allocation of memory for the whole structure  */
	server_memory *server = (server_memory *)malloc(sizeof(server_memory));

    /* allocation of memory and initialization of the hashtable  */
	server->ht = ht_create(MAX_BUCKET_SIZE, hash_function_string,
				 compare_function_strings);

	return server;
}

void server_store(server_memory* server, char* key, char* value) {
	if (!server)
		return;

    /*  using the associated function from Hashtable.h  */
	ht_put(server->ht, key, strlen(key) + 1, value, strlen(value) + 1);
}

void server_remove(server_memory* server, char* key) {
	if (!server)
		return;

    /*  using the associated function from Hashtable.h  */
	ht_remove_entry(server->ht, key);
}

char* server_retrieve(server_memory* server, char* key) {
	if (!server)
		return NULL;

    /*  using the associated function from Hashtable.h + one char * cast  */
	return (char *)ht_get(server->ht, key);
}

void free_server_memory(server_memory* server) {
	if (!server)
		return;

	if (!server->ht) {
		free(server);
		return;
	}

    /*  freeing the hashtable using the associated function  */
	ht_free(server->ht);

    /*  freeing the allocated memory for server struct  */
	free(server);
}

void server_rebalance(server_memory *server, server_memory *next_server,
                      int type, unsigned int (* hash_key)(void *))
{
    /* the three possible situations that can occur based on the positions of
     * the servers in the hashring 
     */

    if (type == 1) {
        for (unsigned int i = 0; i < next_server->ht->hmax; ++i) {
            ll_node_t *it = next_server->ht->buckets[i]->head;
            while (it != NULL) {
                char *key = (char *)(((struct info *)(it->data))->key);
                char *value = (char *)(((struct info *)(it->data))->value);
                unsigned int hash = hash_key(key);
                if (hash <= server->hash || hash > next_server->hash) {
                    server_store(server, key, value);
                    it = it->next;
                    server_remove(next_server, key);
                    continue;
                }
                it = it->next;
            }
        }
        return;
    }

    if (type == 2) {
        for (unsigned int i = 0; i < next_server->ht->hmax; ++i) {
            ll_node_t *it = next_server->ht->buckets[i]->head;
            while (it != NULL) {
                char *key = (char *)(((struct info *)(it->data))->key);
                char *value = (char *)(((struct info *)(it->data))->value);
                unsigned int hash = hash_key(key);
                if (hash <= server->hash && hash > next_server->hash) {
                    server_store(server, key, value);
                    it = it->next;
                    server_remove(next_server, key);
                    continue;
                }
                it = it->next;
            }
        }
        return;
    }

    if (type == 3) {
        for (unsigned int i = 0; i < next_server->ht->hmax; ++i) {
            ll_node_t *it = next_server->ht->buckets[i]->head;
            while (it != NULL) {
                char *key = (char *)(((struct info *)(it->data))->key);
                char *value = (char *)(((struct info *)(it->data))->value);
                unsigned int hash = hash_key(key);
                if (hash <= server->hash) {
                    server_store(server, key, value);
                    it = it->next;
                    server_remove(next_server, key);
                    continue;
                }
                it = it->next;
            }
        }
    }
}

void server_empty(server_memory *server, server_memory *next_server)
{
    /*  Basically moving all existing keys from server to next_server
    */

    for (unsigned int i = 0; i < server->ht->hmax; ++i) {
        ll_node_t *it = server->ht->buckets[i]->head;
        while (it != NULL) {
            char *key = (char *)(((struct info *)(it->data))->key);
            char *value = (char *)(((struct info *)(it->data))->value);
            server_store(next_server, key, value);
            it = it->next;
            server_remove(server, key);
        }
    }
}
