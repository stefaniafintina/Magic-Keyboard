#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"
#include "mk_functions.h"

#define DATA_SIZE 40
int main(void)
{
	char spatiu, word[50], command[20];
	char file[20];
	int k;
	trie_t *trie;
	trie = trie_create(DATA_SIZE, ALPHABET_SIZE, ALPHABET, free);

	while (1) {
		scanf("%s", command);
		if (strcmp(command, "LOAD") == 0) {
			scanf("%c", &spatiu);
			scanf("%s", file);
			FILE *in = fopen(file, "rt");
			load(trie, in);
			fclose(in);
		} else if (strcmp(command, "INSERT") == 0) {
			scanf("%c", &spatiu);
			scanf("%s", word);
			trie_insert(trie, word, word);
		} else if (strcmp(command, "AUTOCORRECT") == 0) {
			scanf("%c", &spatiu);
			scanf("%s", word);
			scanf("%d", &k);
			trie_node_t *curr = trie->root;
			int cnt = trie_autocorrect(trie, curr, word, k, 0);
			if (cnt == 0)
				printf("No words found\n");
		} else if (strcmp(command, "AUTOCOMPLETE") == 0) {
			scanf("%c", &spatiu);
			scanf("%s", word);
			scanf("%d", &k);
			trie_node_t *curr = trie->root;
			autocomplete(trie, curr, word, k);
		} else if (strcmp(command, "EXIT") == 0) {
			trie_free(trie);
			return 0;
		} else if (strcmp(command, "REMOVE") == 0) {
			scanf("%c", &spatiu);
			scanf("%s", word);
			trie_remove(trie, word);
		}
	}
	return 0;
}
