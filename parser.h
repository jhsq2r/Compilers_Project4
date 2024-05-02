#ifndef PARSER_H
#define PARSER_H

#include "token.h"

struct Node {
        char* nodeName;
        struct Token* tk1;
        struct Token* tk2;
        struct Token* tk3;
        struct Node *left;
        struct Node *leftmiddle;
        struct Node *rightmiddle;
        struct Node *right;
};

struct Node* parser(struct Token* list);
void freeTree(struct Node* root);

#endif
