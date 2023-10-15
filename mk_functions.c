#include "mk_functions.h"

void load(trie_t *trie, FILE *in)
{
	char line[1000];
	char *p;
	while (fgets(line, 1000, in)) {
		p = strtok(line, " \t\n");
		while (p) {
			trie_insert(trie, p, p);
			p = strtok(NULL, " \t\n");
		}
	}
}

int diferite(char *word1, char *word2)
{
	int k = 0;
	for (int i = 0; i < (int)strlen(word1); i++) {
		if (word1[i] != word2[i])
			k++;
	}
	return k;
}

int trie_autocorrect(trie_t *trie, trie_node_t *curr, char *word,
					 int k, int len)
{
	if (!curr)
		return 0;
	if (len == (int)strlen(word)) {
		if (curr->end_of_word == 1 &&
			diferite((char *)curr->value, word) <= k) {
			printf("%s\n", (char *)curr->value);
			return 1;
		}
		return 0;
	}
	int count = 0;
	for (int i = 0; i < 26; i++) {
		if (curr->children[i]) {
			count += trie_autocorrect(trie, curr->children[i], word,
									  k, len + 1);
		}
	}

	return count;
}

void autocomplete_case1(trie_t *trie, trie_node_t *curr,
						char *word, char *correct_word)
{
	if (correct_word[0] != 0)
		return;

	if (curr->end_of_word == 1) {
		strcpy(correct_word, curr->value);
		return;
	}

	for (int i = 0; i < 26; i++) {
		trie_node_t *child;
		child  = curr->children[i];
		if (child) {
			curr = child;
			autocomplete_case1(trie, curr, word, correct_word);
		}
	}
}

int min_len(trie_t *trie, trie_node_t *curr,
			int len)
{
	if (!curr)
		return 100;

	int min = 100;

	if (curr->end_of_word == 1) {
		min = len;
		return min;
	}

	for (int i = 0; i < 26; i++) {
		trie_node_t *child = curr->children[i];
		if (child) {
			int min_child = min_len(trie, child, len + 1);
			if (min_child < min)
				min = min_child;
		}
	}

	return min;
}

void autocomplete_case2(trie_t *trie, trie_node_t *curr, int len,
						int *min)
{
	if (!curr || *min == -1)
		return;

	if (curr->end_of_word == 1 && len == *min) {
		printf("%s\n", (char *)curr->value);
		*min = -1;
		return;
	}

	for (int i = 0; i < 26; i++) {
		trie_node_t *child = curr->children[i];
		if (child)
			autocomplete_case2(trie, child, len + 1, min);
	}
}

int max_freq(trie_t *trie, trie_node_t *curr,
			 int len)
{
	if (!curr)
		return -1;

	int freq_max = -1;

	if (curr->end_of_word == 1) {
		if (curr->freq > freq_max)
			freq_max = curr->freq;
	}

	for (int i = 0; i < 26; i++) {
		trie_node_t *child = curr->children[i];
		if (child) {
			int max_freq_child = max_freq(trie, child, len + 1);
			// printf("aici %d\n", max_freq_child);
			if (max_freq_child > freq_max)
				freq_max = max_freq_child;
		}
	}

	return freq_max;
}

void autocomplete_case3(trie_t *trie, trie_node_t *curr,
						int len, int *max)
{
	if (!curr || *max == -1)
		return;

	if (curr->end_of_word == 1 && curr->freq == *max) {
		printf("%s\n", (char *)curr->value);
		*max = -1;
		return;
	}

	for (int i = 0; i < 26; i++) {
		trie_node_t *child = curr->children[i];
		if (child)
			autocomplete_case3(trie, child, len + 1, max);
	}
}

void autocomplete(trie_t *trie, trie_node_t *curr,
				  char *word, int caz)
{
	for (int i = 0; i < (int)strlen(word); i++) {
		trie_node_t *child = curr->children[word[i] - 'a'];
		if (child) {
			curr = child;
		} else {
			if (caz > 0) {
				printf("No words found\n");
			} else {
				printf("No words found\n");
				printf("No words found\n");
				printf("No words found\n");
			}
			return;
		}
	}

	if (caz == 1) {
		char *correct_word = malloc(26);
		correct_word[0] = 0;
		autocomplete_case1(trie, curr, word, correct_word);
		printf("%s\n", correct_word);
		free(correct_word);
	} else if (caz == 2) {
		int min = min_len(trie, curr, 0);
		if (min != 100)
			autocomplete_case2(trie, curr, 0, &min);
		else
			printf("No words found\n");
	} else if (caz == 3) {
		int max = max_freq(trie, curr, 0);
		if (max != -1)
			autocomplete_case3(trie, curr, 0, &max);
		else
			printf("No words found\n");
	} else if (caz == 0) {
		char *correct_word = malloc(26);
		correct_word[0] = 0;
		autocomplete_case1(trie, curr, word, correct_word);
		printf("%s\n", correct_word);
		free(correct_word);

		int min = min_len(trie, curr, 0);
		if (min != 100)
			autocomplete_case2(trie, curr, 0, &min);
		else
			printf("No words found\n");

		int max = max_freq(trie, curr, 0);
		// printf("maxu %d\n", max);
		if (max != -1)
			autocomplete_case3(trie, curr, 0, &max);
		else
			printf("No words found\n");
	}
}
