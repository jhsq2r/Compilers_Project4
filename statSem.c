#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"parser.h"

typedef struct {
    char* str;
    char* num;
} Element;

typedef struct {
    Element items[100];
    int top;
} Stack;

void initializeStack(Stack* stack) {
    stack->top = -1;
}

void push(Stack* stack, char* str, char* num) {
    
    Element newItem;
    newItem.str = strdup(str);
    newItem.num = strdup(num);

    stack->items[++stack->top] = newItem;
}


int find(Stack* stack, char* c) {
    for (int i = stack->top; i >= 0; i--) {
        if (strcmp(stack->items[i].str, c) == 0) {
            //printf("Element containing '%s' found: %s, %d\n", c, stack->items[i].str, stack->items[i].num);
            return 0; // Found
        }
    }

    //printf("Element containing '%s' not found in the stack.\n", c);
    return 1; // Not found
}

void popN(Stack* stack, int n) {
    if (n <= 0) {
        //printf("Invalid value of N\n");
        return;
    }

    while (n > 0 && stack->top >= 0) {
        free(stack->items[stack->top].str);
        stack->top--;
        n--;
    }
}

void popAllAndFree(Stack* stack) {
    while (stack->top >= 0) {
        
        free(stack->items[stack->top].str);
        stack->top--; 
    }
}
int varcounter = 0;
int uniqueStatements = 0;
void semantics(struct Node* root, Stack* stack){
        if(root != NULL){
                if(strcmp(root->nodeName,"program") == 0){

                    semantics(root->left, stack);
                    semantics(root->leftmiddle, stack);
                    semantics(root->rightmiddle, stack);
                    semantics(root->right, stack);

                }else if(strcmp(root->nodeName,"programExtended") == 0){
                    printf("Number of Global Variables: %d\n", varcounter);
                    varcounter = 0;
                    semantics(root->left, stack);
                    semantics(root->leftmiddle, stack);
                    semantics(root->rightmiddle, stack);
                    semantics(root->right, stack);

                }else if(strcmp(root->nodeName, "vars") == 0 && strcmp(root->tk1->tokenInstance, "create") == 0){
                    if(find(stack, root->tk2->tokenInstance) == 1){
                        push(stack,root->tk2->tokenInstance,"0");//mark with no initial value
                        varcounter = varcounter + 1;

                        semantics(root->left, stack);
                        semantics(root->leftmiddle, stack);
                        semantics(root->rightmiddle, stack);
                        semantics(root->right, stack);

                    }else{//if yes call error
                        printf("Error: %s already declared in scope\n", root->tk2->tokenInstance);
                        return;
                    }
                    //increment block var counter
                    
                }else if(strcmp(root->nodeName, "varsExtended") == 0 && strcmp(root->tk1->tokenInstance, ":=") == 0){
                    //printf("In varsExtended\n");
                    //edit top of stack to show that the variable was initialized with a value
                    stack->items[stack->top].num = root->tk2->tokenInstance;
                    semantics(root->left, stack);
                    semantics(root->leftmiddle, stack);
                    semantics(root->rightmiddle, stack);
                    semantics(root->right, stack);
                }else if(strcmp(root->nodeName, "block") == 0){
                    //printf("In block\n");
                    //pop stack varcounter times to clean scope
                    //printf("Popping %d times\n", varcounter);
                    popN(stack, varcounter);
                    //reset varcounter
                    varcounter = 0;
                    semantics(root->left, stack);
                    semantics(root->leftmiddle, stack);
                    semantics(root->rightmiddle, stack);
                    semantics(root->right, stack);
                }else if(strcmp(root->nodeName, "stat--assign") == 0){
                    if(find(stack, root->tk1->tokenInstance) == 1){
                        printf("Error: %s Not in scope\n", root->tk1->tokenInstance);
                        return;
                    }else{
                        printf("MULT 0\nADD ");                            
                        semantics(root->left, stack);
                        printf("STORE %s\n", root->tk1->tokenInstance);
                        semantics(root->leftmiddle, stack);
                        semantics(root->rightmiddle, stack);
                        semantics(root->right, stack);
                    }        
                }else if(strcmp(root->nodeName, "stat--assignSet") == 0){
                    if(find(stack, root->tk2->tokenInstance) == 1){
                        printf("Error: %s Not in scope\n", root->tk2->tokenInstance);
                        return;
                    }else{
                        printf("MULT 0\nADD ");  
                        semantics(root->left, stack);
                        printf("STORE %s\n", root->tk2->tokenInstance);
                        semantics(root->leftmiddle, stack);
                        semantics(root->rightmiddle, stack);
                        semantics(root->right, stack);
                    }
                }else if(strcmp(root->nodeName, "r") == 0 && strcmp(root->tk1->idTk,"IDENT_TK") == 0){
                //printf("In r\n");
                 //check if variable is in scope
                    if(find(stack, root->tk1->tokenInstance) == 1){
                        printf("Error: %s Not in scope\n", root->tk1->tokenInstance);
                        return;
                    }else{
                        printf("%s\n", root->tk1->tokenInstance);
                        semantics(root->left, stack);
                        semantics(root->leftmiddle, stack);
                        semantics(root->rightmiddle, stack);
                        semantics(root->right, stack);
                    }
                }else if(strcmp(root->nodeName, "r") == 0 && strcmp(root->tk1->idTk,"INT_TK") == 0){

                    printf("%s\n", root->tk1->tokenInstance);
                    semantics(root->left, stack);
                    semantics(root->leftmiddle, stack);
                    semantics(root->rightmiddle, stack);
                    semantics(root->right, stack);

                }else if(strcmp(root->nodeName, "stat--in") == 0){
                    //printf("In in\n");
                    //check if tk2 variable is in scope
                    if(find(stack, root->tk2->tokenInstance) == 1){
                        printf("Error: %s Not in scope\n", root->tk2->tokenInstance);
                        return;
                    }else{
                        printf("READ %s\n", root->tk2->tokenInstance);
                        semantics(root->left, stack);
                        semantics(root->leftmiddle, stack);
                        semantics(root->rightmiddle, stack);
                        semantics(root->right, stack);
                    }
                }else if(strcmp(root->nodeName, "stat--out") == 0){
                    printf("WRITE ");
                    semantics(root->left, stack);
                    semantics(root->leftmiddle, stack);
                    semantics(root->rightmiddle, stack);
                    semantics(root->right, stack);
                }else if(strcmp(root->nodeName, "stats") == 0){
                    semantics(root->left, stack);
                    semantics(root->leftmiddle, stack);
                    semantics(root->rightmiddle, stack);
                    semantics(root->right, stack);
                }else if(strcmp(root->nodeName, "mStat") == 0){
                    semantics(root->left, stack);
                    semantics(root->leftmiddle, stack);
                    semantics(root->rightmiddle, stack);
                    semantics(root->right, stack);
                }else if(strcmp(root->nodeName, "stat--label") == 0){
                    semantics(root->left, stack);
                    semantics(root->leftmiddle, stack);
                    semantics(root->rightmiddle, stack);
                    semantics(root->right, stack);
                }else if(strcmp(root->nodeName, "stat--if") == 0){
                    printf("MULT 0\nADD ");
                    semantics(root->left, stack);//expr1
                    printf("STORE iftemp\n");
                    printf("MULT 0\nADD ");
                    semantics(root->rightmiddle, stack);//expr2
                    printf("SUB iftemp\n");
                    uniqueStatements++;
                    semantics(root->leftmiddle, stack);//ro
                    printf("doifstat%d: ", uniqueStatements);
                    semantics(root->right, stack);//stat
                    printf("outif%d: ", uniqueStatements);
                }else if(strcmp(root->nodeName, "stat--pick") == 0){
                    semantics(root->left, stack);
                    semantics(root->leftmiddle, stack);
                    semantics(root->rightmiddle, stack);
                    semantics(root->right, stack);
                }else if(strcmp(root->nodeName, "stat--whileloop") == 0){
                    semantics(root->left, stack);
                    semantics(root->leftmiddle, stack);
                    semantics(root->rightmiddle, stack);
                    semantics(root->right, stack);
                }else if(strcmp(root->nodeName, "stat--repeatloop") == 0){
                    semantics(root->left, stack);
                    semantics(root->leftmiddle, stack);
                    semantics(root->rightmiddle, stack);
                    semantics(root->right, stack);
                }else if(strcmp(root->nodeName, "stat--goto") == 0){
                    semantics(root->left, stack);
                    semantics(root->leftmiddle, stack);
                    semantics(root->rightmiddle, stack);
                    semantics(root->right, stack);
                }else if(strcmp(root->nodeName, "stat--block") == 0){
                    semantics(root->left, stack);
                    semantics(root->leftmiddle, stack);
                    semantics(root->rightmiddle, stack);
                    semantics(root->right, stack);
                }else if(strcmp(root->nodeName, "pickbody") == 0){
                    semantics(root->left, stack);
                    semantics(root->leftmiddle, stack);
                    semantics(root->rightmiddle, stack);
                    semantics(root->right, stack);
                }else if(strcmp(root->nodeName, "ro") == 0){
                    if(strcmp(root->tk1->tokenInstance, "<") == 0){
                        //expr1 < expr2 with expr2 - expr1
                        printf("BRPOS doifstat%d\n", uniqueStatements);
                        printf("BR outif%d\n", uniqueStatements);
                    }else if(strcmp(root->tk1->tokenInstance, ">") == 0){
                        //expr1 > expr2
                        printf("BRNEG doifstat%d\n", uniqueStatements);
                        printf("BR outif%d\n", uniqueStatements);
                    }else if(strcmp(root->tk1->tokenInstance, ".") == 0){
                        //expr1 ... expr2 (true if both are even or odd)
                        printf("PLACEHOLDER\n");
                    }else if(strcmp(root->tk1->tokenInstance, "==")){
                        //expr1 == expr2
                        printf("BRZERO doifstat%d\n", uniqueStatements);
                        printf("BR outif%d\n", uniqueStatements);
                    }else if(strcmp(root->tk1->tokenInstance, "=!=")){
                        //expr1 =!= expr2 (true if not equal)
                        printf("BRNEG doifstat%d\n", uniqueStatements);
                        printf("BRPOS doifstat%d\n", uniqueStatements);
                        printf("BR outif%d\n", uniqueStatements);
                    }
                }else if(strcmp(root->nodeName, "expr") == 0){
                    //printf("MULT 0\n");
                    semantics(root->left, stack);
                    if(root->leftmiddle != NULL){
                        printf("SUB ");
                    }
                    semantics(root->leftmiddle, stack);
                }else if(strcmp(root->nodeName, "n") == 0){
                    semantics(root->left, stack);
                    semantics(root->leftmiddle, stack);
                    semantics(root->rightmiddle, stack);
                    semantics(root->right, stack);
                }else if(strcmp(root->nodeName, "nExtended") == 0){
                    if(root->tk1 != NULL){
                        if(strcmp(root->tk1->tokenInstance, "+") == 0){
                            if(root->left != NULL){
                                printf("ADD ");
                            }
                            semantics(root->left, stack);
                            if(root->leftmiddle != NULL){
                                printf("ADD ");
                            }
                            semantics(root->leftmiddle, stack);
                        }else if(strcmp(root->tk1->tokenInstance, "/") == 0){
                            if(root->leftmiddle != NULL){
                               printf("DIV ");
                            }
                            semantics(root->left, stack);
                            if(root->leftmiddle != NULL){
                               printf("DIV ");
                            }
                            semantics(root->leftmiddle, stack);
                        }
                    }
                }else if(strcmp(root->nodeName, "a") == 0){
                    semantics(root->left, stack);
                    if(root->tk1 != NULL){
                        printf("MULT ");
                    }
                    semantics(root->leftmiddle, stack);
                    semantics(root->rightmiddle, stack);
                    semantics(root->right, stack);
                }else if(strcmp(root->nodeName, "m") == 0){
                    if(root->tk1 != NULL){
                        //printf("ADD ");
                        printf("STORE temp\n");
                        printf("LOAD ");
                        semantics(root->left, stack);
                        printf("MULT -1\n");
                        printf("ADD temp\n");
                    }else{
                        semantics(root->left, stack);
                    }
                }else{
                    printf("Something weird is going on...\n");
                    printf("%s\n", root->nodeName);
                }
                
                
        }
}


void staticSemantics(struct Node* root){
    Stack stack;
    initializeStack(&stack);

    printf("STATIC SEMANTICS ERRORS:\n");
    semantics(root, &stack);

    printf("STOP\n");
    for (int i = stack.top; i >= 0; i--) {
        printf("%s %s\n", stack.items[i].str,stack.items[i].num);
    }

    popAllAndFree(&stack);
}