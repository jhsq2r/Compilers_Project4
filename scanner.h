#ifndef SCANNER_H
#define SCANNER_H

#include "token.h"

void filter(FILE *dirtyfile, FILE *cleanfile);
struct Token* scanner(FILE *file);
void displayTokenList(struct Token* head);

#endif
