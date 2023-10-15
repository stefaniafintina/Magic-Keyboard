#include "trie.h"

trie_node_t *trie_create_node(trie_t *trie)
{
	trie_node_t *node = malloc(sizeof(trie_node_t));
	node->end_of_word = 0;
	node->n_children = 0;
	node->value = NULL;
	node->freq = 0;
	node->children = malloc(trie->alphabet_size * sizeof(trie_node_t *));
	for (int i = 0; i < trie->alphabet_size; i++)
		node->children[i] = NULL;

	return node;
}

trie_t *trie_create(int data_size, int alphabet_size, char *alphabet,
					void (*free_value_cb)(void *mem))
{
	trie_t *trie = malloc(sizeof(trie_t));
	trie->alphabet_size = alphabet_size;
	trie->root = trie_create_node(trie);
	trie->root->end_of_word = 1;
	trie->root->value = malloc(data_size);
	*((int *)trie->root->value) = -1;
	trie->size = 0;
	trie->data_size = data_size;
	trie->alphabet = malloc(trie->alphabet_size + 1);
	strcpy(trie->alphabet, alphabet);
	trie->free_value_cb = free_value_cb;
	trie->nr_nodes = 1;
	return trie;
}

void trie_insert(trie_t *trie, char *key, void *value)
{
	trie_node_t *node;
	node = trie->root;
	for (int i = 0; i < (int)strlen(key); i++) {
		trie_node_t *child = node->children[key[i] - 'a'];
		if (!child) {
			child = trie_create_node(trie);
			node->n_children++;
			trie->nr_nodes++;
			node->children[key[i] - 'a'] = child;
		}
		node = child;
	}

	if (!node->value)
		node->value = malloc(trie->data_size);
	memcpy(node->value, value, trie->data_size);
	node->end_of_word = 1;
	node->freq++;
	trie->size++;
}

void *trie_search(trie_t *trie, char *key)
{
	trie_node_t *node;
	node = trie->root;

	for (int i = 0; i < (int)strlen(key); i++) {
		trie_node_t *child = node->children[key[i] - 'a'];
		if (!child)
			return NULL;
		node = child;
	}
	return node->value;
}

int __remove(trie_t *trie, trie_node_t *node, char *key)
{
	if (!node)
		return 0;

	if (strlen(key) == 0) {
		node->end_of_word = 0;
		free(node->value);
		node->freq = 0;
		node->value = NULL;
		trie->size--;
		if (node->n_children == 0) {
			free(node->children);
			free(node);
			trie->nr_nodes--;
			return 1;
		}
		return 0;
	}

	trie_node_t *child = node->children[key[0] - 'a'];
	int res = __remove(trie, child, key + 1);

	if (res == 1) {
		node->n_children--;
		node->children[key[0] - 'a'] = NULL;
		if (node->n_children == 0 && node->end_of_word == 0) {
			trie->nr_nodes--;
			free(node->children);
			free(node);
			return 1;
		}

		return 0;
	}

	return 0;
}

void trie_remove(trie_t *trie, char *key)
{
	__remove(trie, trie->root, key);
}

void trie_free_node(trie_node_t *node)
{
	if (!node)
		return;

	if (node->children) {
		for (int i = 0; i < 26; i++)
			trie_free_node(node->children[i]);
		free(node->children);
	}

	if (node->value) {
		free(node->value);
		node->value = NULL;
	}

	free(node);
	node = NULL;
}

void trie_free(trie_t *trie)
{
	if (!trie)
		return;

	trie_free_node(trie->root);

	free(trie->alphabet);

	free(trie);
}

