#ifndef MKDIRSPLITPATH_H
#define MKDIRSPLITPATH_H

#include "types.h"

struct NODE *splitPath(char *pathName, char *baseName, char *dirName);
void printTree(struct NODE *node, int depth);

#endif
