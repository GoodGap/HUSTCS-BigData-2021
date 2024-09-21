#ifndef _HASH_TABLE_H
#define _HASH_TABLE_H
#include "util/types.h"

#define HASH_TABLE_SIZE 128

struct hash_node {
  struct hash_node *next;
  void *key;
  void *value;
};

// this is a generic hash linked table for KERNEL SPACE
struct hash_table {
  struct hash_node head[HASH_TABLE_SIZE];
  int (*virtual_hash_equal)(void *key1, void *key2);
  size_t (*virtual_hash_func)(void *key);
  int (*virtual_hash_put)(struct hash_table *hash_table, void *key, void *value);
  void *(*virtual_hash_get)(struct hash_table *hash_table, void *key);
  int (*virtual_hash_erase)(struct hash_table *hash_table, void *key);
};

int hash_table_init(struct hash_table *list, int (*virtual_hash_equal)(void *key1, void *key2),
                   size_t (*virtual_hash_func)(void *key),
                   int (*virtual_hash_put)(struct hash_table *hash_table, void *key, void *value),
                   void *(*virtual_hash_get)(struct hash_table *hash_table, void *key),
                   int (*virtual_hash_erase)(struct hash_table *hash_table, void *key));

#endif