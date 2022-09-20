/*  Copyright 2021 Buzea Alexandru-Mihai-Iulian 311CAb
	and Data Structures Team (skel)  */

#ifndef SERVER_H_
#define SERVER_H_

#include "Hashtable.h"

/*  The server_memory struct that will store the identifier of the server,
 *  its hash (it is better to always have the calculated hash inside the
 *  structure) and the server itself (implemented using a hashtable)
 */

typedef struct server_memory server_memory;

struct server_memory {
	int id;
	unsigned int hash;
	hashtable_t *ht;
};

/*  This function will create a server_memory structure by allocating
 *  the desired amount of memory (for the structure and for the
 *  hashtable - the database itself) using malloc() 
 */

server_memory* init_server_memory();

/*  This function will free the allocated memory for a certain server
 *  given as a parameter (#nomemoryleaks).
 */

void free_server_memory(server_memory* server);

/*  This function will store a key-value pair (only C-strings) on the
 *  given as a parameter. If the key already exists, the old value is
 *  replaced (overwritten).
 */

void server_store(server_memory* server, char* key, char* value);

/*  This function will remove a key-value pair (only C-strings) from the
 *  server given as a parameter. If the key does not exists, nothing will
 *  be removed or replaced.
 */

void server_remove(server_memory* server, char* key);

/*  This function will return the associated value for the given key, on
 *  the given server. If the desired value is not found, a NULL pointer
 *  will be returned.
 */

char* server_retrieve(server_memory* server, char* key);

/*  This function will rebalance (move) key-value pairs according to the
 *  value of the <type> parameter:
 *  	1. type = 1 corresponds to the case where the first server has the
 *  smallest hash in the hashring (its address is stored in the first node
 *  of the list which we used in our implementation)
 *  	2. type = 2 corresponds to the case where the first server is the
 *  last server in our hashring (has the greatest hash), and the next server
 *  is stored in the first node of the list (notice the circular structure).
 *  	3. type = 3 corresponds to all other cases (servers have other servers
 *  behind them and after them).
 *  Our function takes a pointer to the function used to get the hash of a
 *  key, because we need to determine if a certain key must be moved on
 *  another server. The general rule is that we move the correct keys from
 *  the next_server to server.
 */

void server_rebalance(server_memory *server, server_memory *next_server,
                      int type, unsigned int (* hash_key)(void *));

/* This function will move all the keys from server to the next_server
 * (the <server> parameter is the server which will be deleted after 
 * the removal of all keys).
 */

void server_empty(server_memory *server, server_memory *next_server);

#endif  /* SERVER_H_ */
