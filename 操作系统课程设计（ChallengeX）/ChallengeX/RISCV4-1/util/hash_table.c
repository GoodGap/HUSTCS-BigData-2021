#include "util/hash_table.h"
#include "util/types.h"
#include "kernel/pmm.h"

static int default_equal(void *key1, void *key2) { return key1 == key2; }

static int default_put(struct hash_table *hash_table, void *key, void *value) {
  struct hash_node *node = (struct hash_node *)alloc_page();
  if (hash_table->virtual_hash_get(hash_table, key) != NULL) return -1;
  node->key = key;
  node->value = value;

  size_t index = hash_table->virtual_hash_func(key);
  struct hash_node *head = hash_table->head + index;

  node->next = head->next;
  head->next = node;
  return 0;
}

static void *defalut_get(struct hash_table *hash_table, void *key) {
  size_t index = hash_table->virtual_hash_func(key);
  struct hash_node *head = hash_table->head + index;
  struct hash_node *node = head->next;
  while (node) {
    if (hash_table->virtual_hash_equal(node->key, key)) return node->value;
    node = node->next;
  }
  return NULL;
}

static int default_erase(struct hash_table *hash_table, void *key) {
  size_t index = hash_table->virtual_hash_func(key);
  struct hash_node *head = hash_table->head + index;
  while (head->next && !hash_table->virtual_hash_equal(head->next->key, key))
    head = head->next;
  if (head->next) {
    struct hash_node *node = head->next;
    head->next = node->next;
    free_page(node);
    return 0;
  } else
    return -1;
}

int hash_table_init(struct hash_table *list,
                   int (*equal)(void *key1, void *key2),
                   size_t (*func)(void *key),
                   int (*put)(struct hash_table *hash_table, void *key, void *value),
                   void *(*get)(struct hash_table *hash_table, void *key),
                   int (*erase)(struct hash_table *hash_table, void *key)) {
  for (int i = 0; i < HASH_TABLE_SIZE; i++) list->head[i].next = NULL;
  if (func == NULL) return -1;
  list->virtual_hash_func = func;
  list->virtual_hash_equal = equal ? equal : default_equal;
  list->virtual_hash_put = put ? put : default_put;
  list->virtual_hash_get = get ? get : defalut_get;
  list->virtual_hash_erase = erase ? erase : default_erase;
  return 0;
}