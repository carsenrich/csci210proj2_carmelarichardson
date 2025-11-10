#include "types.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern struct NODE* root;
extern struct NODE* cwd;

// helper function to create a new node
struct NODE* createNode(char *name, char type, struct NODE *parent) {
    struct NODE *newNode = (struct NODE*)malloc(sizeof(struct NODE));
    strcpy(newNode->name, name);
    newNode->fileType = type;
    newNode->childPtr = NULL;
    newNode->siblingPtr = NULL;
    newNode->parentPtr = parent;
    return newNode;
}

// mkdir implementation
void mkdir(char pathName[]) {
    if(strcmp(pathName, "/") == 0 || strlen(pathName) == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    char dirName[64], baseName[64];
    struct NODE *parent = splitPath(pathName, baseName, dirName);
    if(!parent) return;

    // check if directory already exists
    struct NODE *child = parent->childPtr;
    while(child) {
        if(strcmp(child->name, baseName) == 0 && child->fileType == 'D') {
            printf("MKDIR ERROR: directory %s already exists\n", pathName);
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

// splitPath implementation
struct NODE* splitPath(char* pathName, char* baseName, char* dirName) {
    if(strcmp(pathName, "/") == 0) {
        strcpy(dirName, "/");
        strcpy(baseName, "");
        return root;
    }

    char temp[64];
    strcpy(temp, pathName);

    char *lastSlash = strrchr(temp, '/');
    if(lastSlash) {
        // path has directories
        size_t dirLen = lastSlash - temp;
        if(dirLen == 0) strcpy(dirName, "/");  // e.g., "/file"
        else {
            strncpy(dirName, temp, dirLen);
            dirName[dirLen] = '\0';
        }
        strcpy(baseName, lastSlash + 1);
    } else {
        // no slashes, current directory
        strcpy(dirName, "");
        strcpy(baseName, pathName);
    }

    struct NODE *current;
    if(pathName[0] == '/') current = root;
    else current = cwd;

    // traverse the dirName path
    if(strcmp(dirName, "/") != 0 && strlen(dirName) != 0) {
        char copy[64];
        strcpy(copy, dirName);
        char *token = strtok(copy, "/");
        while(token) {
            struct NODE *child = current->childPtr;
            while(child && !(strcmp(child->name, token) == 0 && child->fileType == 'D'))
                child = child->siblingPtr;
            if(!child) {
                printf("ERROR: directory %s does not exist\n", token);
                return NULL;
            }
            current = child;
            token = strtok(NULL, "/");
        }
    }

    return current;
}
