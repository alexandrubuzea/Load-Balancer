/*  Copyright 2021 Buzea Alexandru-Mihai-Iulian 311CAb
    and Data Structures Team (skel)  */

#ifndef LOAD_BALANCER_H_
#define LOAD_BALANCER_H_

#include "server.h"
#include <stdio.h>

#define NUM_SERVERS 100000
#define NUM_COPY 3

struct load_balancer;
typedef struct load_balancer load_balancer;

/*  A function that allocs memory and initialises a load balancer.
 */

load_balancer* init_load_balancer();

/*  A function that frees the memory allocated for a load_balancer.
 *  #nomemoryleaks
 */

void free_load_balancer(load_balancer* main);

/*  A function that stores the given pair on one of the servers and returns
 *  the server on which the pair was stored (through parameter <server_id>).
 */

void loader_store(load_balancer* main, char* key, char* value, int* server_id);

/*  A function that returns the associated value of a key by finding
 *  on which server was the pair stored (and NULL if the key does not exist).
 *  In addition, the function also returns the server id on which the
 *  pair was stored via the parameter <server_id>
 */

char* loader_retrieve(load_balancer* main, char* key, int* server_id);

/*  A function that adds a server and realizes the balance between the
 *  newly-added server and the old server (by remapping some of the existing
 *  keys)
 */

void loader_add_server(load_balancer* main, int server_id);

/*  A function that moves all the keys from the server with given id in
 *  order to remove (free) the server
 */

void loader_remove_server(load_balancer* main, int server_id);

/*  A function that adds a copy of the server and realizes the balance of
 *  keys.
 */
void loader_add_copy(load_balancer *main, int server_id);

/*  A function that moves all the pairs from a copy of the server and
 *  then removes (frees) that copy #nomemoryleaks
 */
void loader_remove_copy(load_balancer *main, int server_id);

#endif  /* LOAD_BALANCER_H_ */
