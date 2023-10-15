#ifndef MK_FUNCTIONS_H_
#define MK_FUNCTIONS_H_
#include "trie.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void load(trie_t *trie, FILE * in);

int trie_autocorrect(trie_t *trie, trie_node_t *curr, char *word,
					 int k, int len);

void autocomplete_case1(trie_t *trie, trie_node_t *curr,
						char *word, char *correct_word);

int min_len(trie_t *trie, trie_node_t *curr,
			int len);

void autocomplete_case2(trie_t *trie, trie_node_t *curr,
						int len, int *min);

int max_freq(trie_t *trie, trie_node_t *curr,
			 int len);

void autocomplete_case3(trie_t *trie, trie_node_t *curr,
						int len, int *max);

void autocomplete(trie_t *trie, trie_node_t *curr,
				  char *word, int caz);

#endif
