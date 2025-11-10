#include "types.h"
#include <stdio.h>
#include stdlib.h>
#include <string.h>

void mkdirFS(struct NODE *root, const char *path);

void printTree(struct NODE *root, int depth) {
	if (!root) return;
	for (int i = 0; i < depth; i++) printf("  ");
	printf("%s\n", root->name);
	for (int i = 0; i < root->numChildren; i++) {
		printTree(root->children[i], depth + 1);
	}
}

int main() {
	printf("Testing mkdirSplitpath.c with tree\n");

	struct NODE *root = malloc(sizeof(struct NODE));
	root->name = strdup("/");
	root->children = NULL;
	root->numChildren = 0;

	const char *paths[] = {
		"/home/user/docs",
		"/tmp",
		"/a/b/c/d"
	};

	int numPaths = sizeof(paths) / sizeof(paths[0]);

	for (int i = 0; i < numPaths; i++) {
		printf("\nCreating path: %s\n", paths[i]);
		mkdirFS(root, paths[i]);
	}

	printf("\nFile system tree\n");
	printTree(root, 0);

	return 0;
}
