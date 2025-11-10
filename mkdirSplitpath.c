#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

// helper to create a new node
struct NODE* createNode(char *name, char fileType, struct NODE *parent) {
    struct NODE *node = (struct NODE *)malloc(sizeof(struct NODE));
    if(!node) exit(1);

    strcpy(node->name, name);
    node->fileType = fileType;
    node->childPtr = NULL;
    node->siblingPtr = NULL;
    node->parentPtr = parent;
    return node;
}

// splitPath function (basic, enough to test mkdir)
struct NODE* splitPath(char* pathName, char* baseName, char* dirName) {
    if(strcmp(pathName, "/") == 0) {
        strcpy(dirName, "/");
        baseName[0] = '\0';
        return root;
    }

    char temp[64];
    strcpy(temp, pathName);

    char *lastSlash = strrchr(temp, '/');
    if(lastSlash) {
        *lastSlash = '\0';
        strcpy(baseName, lastSlash + 1);
        strcpy(dirName, temp[0] ? temp : "/");
    } else {
        strcpy(baseName, temp);
        dirName[0] = '\0';
    }

    // traverse from root or cwd
    struct NODE *parent = (temp[0] == '/' ? root : cwd);
    if(strcmp(dirName, "/") != 0 && dirName[0] != '\0') {
        char *token = strtok(dirName, "/");
        while(token) {
            struct NODE *child = parent->childPtr;
            while(child && strcmp(child->name, token) != 0)
                child = child->siblingPtr;
            if(!child) {
                printf("ERROR: directory %s does not exist\n", token);
                return NULL;
            }
            parent = child;
            token = strtok(NULL, "/");
        }
    }

    return parent;
}

// mkdir function
void mkdir(char pathName[]) {
    if(strcmp(pathName, "/") == 0 || strlen(pathName) == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    char dirName[64], baseName[64];
    struct NODE *parent = splitPath(pathName, baseName, dirName);
    if(!parent) return;

    // check if it already exists
    struct NODE *child = parent->childPtr;
    while(child) {
        if(strcmp(child->name, baseName) == 0 && child->fileType == 'D') {
            printf("MKDIR ERROR: directory %s already exists\n", baseName);
            return;
        }
        child = child->siblingPtr;
    }

    // create and attach new directory
    struct NODE *newNode = createNode(baseName, 'D', parent);
    if(!parent->childPtr)
        parent->childPtr = newNode;
    else {
        child = parent->childPtr;
        while(child->siblingPtr)
            child = child->siblingPtr;
        child->siblingPtr = newNode;
    }

    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);
}
