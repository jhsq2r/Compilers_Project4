#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"parser.h"  
#include<windows.h>


struct Node* getNode(char* name){
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
                if (newNode != NULL){
                        newNode->nodeName = strdup(name);
                        newNode->tk1 = NULL;
                        newNode->tk2 = NULL;
                        newNode->tk3 = NULL;
                        newNode->left = NULL;
                        newNode->leftmiddle = NULL;
                        newNode->rightmiddle = NULL;
                        newNode->right = NULL;
                        return newNode;
                }
}

void freeTree(struct Node* root){
        if(root != NULL){
                freeTree(root->left);
                freeTree(root->leftmiddle);
                freeTree(root->rightmiddle);
                freeTree(root->right);
                free(root->nodeName);
                free(root->tk1);
                free(root->tk2);
                free(root->tk3);
                free(root);
        }
}

void printPreOrderNow(struct Node* root, int level){
        if(root != NULL){
                if(root->tk1 != NULL){
                        printf("Level: %d || %s ||", level, root->nodeName);
                        if(root->tk1 != NULL){
                                printf(" %s ||", root->tk1->tokenInstance);
                        }
                        if(root->tk2 != NULL){
                                printf(" %s ||", root->tk2->tokenInstance);
                        }
                        if(root->tk3 != NULL){
                                printf(" %s ||", root->tk3->tokenInstance);
                        }
                        printf("\n");
                }
                printPreOrderNow(root->left,level+1);
                printPreOrderNow(root->leftmiddle, level+1);
                printPreOrderNow(root->rightmiddle, level+1);
                printPreOrderNow(root->right, level+1);
        }
}

int tokenNum = 0;

struct Node* vars(struct Token* list);
struct Node* varsExtended(struct Token* list);
struct Node* block(struct Token* list);
struct Node* stats(struct Token* list);
struct Node* mStat(struct Token* list);
struct Node* stat(struct Token* list);
struct Node* program(struct Token* list);
struct Node* programExtended(struct Token* list);
struct Node* pickbody(struct Token* list);
struct Node* ro(struct Token* list);
struct Node* roExtended(struct Token* list);
struct Node* expr(struct Token* list);
struct Node* n(struct Token* list);
struct Node* nExtended(struct Token* list);
struct Node* a(struct Token* list);
struct Node* m(struct Token* list);
struct Node* r(struct Token* list);

struct Token* currentToken(int x, struct Token* list){

        if(x == 0){
                return list;
        }else{
                int y = 1;
                for(y; y < 100; y++){
                        list = list->next;
                        if(y == x){
                                return list;
                        }
                }
 
        }
}




struct Node* parser(struct Token* list){
        //printf("In parser Function...\n");
        struct Node* root;
        root = program(list);
        if(strcmp(currentToken(tokenNum, list)->idTk,"EOF_TK") == 0){
                printf("Success!!!\n");
        }else{
                printf("Error in parser function\n");
        }

        printPreOrderNow(root,0);
        //freeTree(root);

        return root;
}

struct Node* program(struct Token* list){
        //printf("In program Function...Token: %s\n", list->tokenInstance);
        struct Node* temp = getNode("program");
        temp->left = vars(list);
        if(strcmp(currentToken(tokenNum, list)->idTk,"KW_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"tape") == 0){
                temp->tk1 = currentToken(tokenNum, list);
                tokenNum++;
                temp->leftmiddle = programExtended(list);
                return temp;
        }else{
                //error expected tape
                printf("Error: Expected 'tape' got %s\n", currentToken(tokenNum, list)->tokenInstance);


        }
}

struct Node* vars(struct Token* list){
        //printf("In vars Function...Token: %s\n", list->tokenInstance);
        struct Node* temp = getNode("vars");
        if(strcmp(currentToken(tokenNum, list)->idTk,"KW_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"create") == 0){
                temp->tk1 = currentToken(tokenNum, list);
                tokenNum++;
                if(strcmp(currentToken(tokenNum, list)->idTk,"IDENT_TK") == 0){
                        temp->tk2 = currentToken(tokenNum, list);
                        tokenNum++;
                        temp->left = varsExtended(list);
                }
        }else{
                return NULL;
        }
}

struct Node* varsExtended(struct Token* list){
        //printf("In varsExtended Function...Token: %s\n", list->tokenInstance);
        struct Node* temp = getNode("varsExtended");
        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,";") == 0){
                temp->tk1 = currentToken(tokenNum, list);
                tokenNum++;
                return temp;
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,":=") == 0){
                temp->tk1 = currentToken(tokenNum, list);
                tokenNum++;
                if(strcmp(currentToken(tokenNum, list)->idTk,"INT_TK") == 0){
                        temp->tk2 = currentToken(tokenNum, list);
                        tokenNum++;
                        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,";") == 0){
                                tokenNum++;
                                temp->left = vars(list);
                                return temp;
                        }else{
                                //error expected ;
                                printf("Error: Expected ';' got %s\n", currentToken(tokenNum, list)->tokenInstance);


                        }
                }else{
                        //error expected INT
                        printf("Error: Expected 'INT' got %s\n", currentToken(tokenNum, list)->tokenInstance);

                }
        }else{
                //error expected ; or := INT ;
                printf("Error: Expected ';' or ':= INT' got %s\n", currentToken(tokenNum, list)->tokenInstance);


        }
}

struct Node* programExtended(struct Token* list){
        struct Node* temp = getNode("programExtended");
        //printf("In programExtended Function...Token: %s\n", list->tokenInstance);
        if(strcmp(currentToken(tokenNum, list)->idTk,"KW_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"func") == 0){
                temp->tk1 = currentToken(tokenNum, list);
                tokenNum++;
                if(strcmp(currentToken(tokenNum, list)->idTk,"IDENT_TK") == 0){
                        temp->tk2 = currentToken(tokenNum, list);
                        tokenNum++;
                        temp->left = block(list);
                        temp->leftmiddle = block(list);
                        return temp;
                }else{
                        //error expected IDENT_TK
                        printf("Error: Expected 'IDENT_TK' got %s\n", currentToken(tokenNum, list)->tokenInstance);


                }
        }else{
                temp->left = block(list);
                return temp;
        }
}

struct Node* block(struct Token* list){
        //printf("In block Function...Token: %s\n", list->tokenInstance);
        struct Node* temp = getNode("block");
        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"{") == 0){
                tokenNum++;
                temp->left = vars(list);
                temp->leftmiddle = stats(list);
                if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"}") == 0){
                        tokenNum++;
                        return temp;
                }else{
                        //errpr expected }
                        printf("Error: Expected '}' got %s\n", currentToken(tokenNum, list)->tokenInstance);


                }
        }else{
                //error expected {
                printf("Error: Expected '{' got %s\n", currentToken(tokenNum, list)->tokenInstance);


        }
}

struct Node* stats(struct Token* list){
        //printf("In stats Function...Token: %s\n", list->tokenInstance);
        struct Node* temp = getNode("stats");
        temp->left = stat(list);
        temp->leftmiddle = mStat(list);
        return temp;
}

struct Node* mStat(struct Token* list){
        //printf("In mStat Function...Token: %s\n", list->tokenInstance);
        struct Node* temp = getNode("mStat");
        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance, "}") == 0){
                return NULL;
        }else{
                temp->left = stat(list);
                temp->leftmiddle = mStat(list);
                return temp;
        }
}

struct Node* stat(struct Token* list){
        //printf("In stat Function...Token: %s\n", list->tokenInstance);
        if(strcmp(currentToken(tokenNum, list)->idTk,"KW_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"label") == 0){
                struct Node* temp = getNode("stat--label");
                temp->tk1 = currentToken(tokenNum, list);
                tokenNum++;
                if(strcmp(currentToken(tokenNum, list)->idTk,"IDENT_TK") == 0){
                        temp->tk2 = currentToken(tokenNum, list);
                        tokenNum++;
                        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,";") == 0){
                                tokenNum++;
                                return temp;
                        }else{
                                //error expected ;
                                printf("Error: Expected ';' got %s\n", currentToken(tokenNum, list)->tokenInstance);

                        }
                }else{
                        //error expected IDENT_TK
                        printf("Error: Expected 'IDENT_TK' got %s\n", currentToken(tokenNum, list)->tokenInstance);

                }
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"KW_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"cin") == 0){
                struct Node* temp = getNode("stat--in");
                temp->tk1 = currentToken(tokenNum, list);
                tokenNum++;
                if(strcmp(currentToken(tokenNum, list)->idTk,"IDENT_TK") == 0){
                        temp->tk2 = currentToken(tokenNum, list);
                        tokenNum++;
                        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,";") == 0){
                                tokenNum++;
                                return temp;
                        }else{
                                //error expected ;
                                printf("Error: Expected ';' got %s\n", currentToken(tokenNum, list)->tokenInstance);

                        }
                }else{
                        //error expected IDENT_TK
                        printf("Error: Expected 'IDENT_TK' got %s\n", currentToken(tokenNum, list)->tokenInstance);

                }
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"KW_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"cout") == 0){
                struct Node* temp = getNode("stat--out");
                temp->tk1 = currentToken(tokenNum, list);
                tokenNum++;
                temp->left = expr(list);
                if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,";") == 0){
                        tokenNum++;
                        return temp;
                }else{
                        //error expected ;
                        printf("Error: Expected ';' got %s\n", currentToken(tokenNum, list)->tokenInstance);

                }
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"KW_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"if") == 0){
                struct Node* temp = getNode("stat--if");
                temp->tk1 = currentToken(tokenNum, list);
                tokenNum++;
                if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"[") == 0){
                        tokenNum++;
                        temp->left = expr(list);
                        temp->leftmiddle = ro(list);
                        temp->rightmiddle = expr(list);
                        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"]") == 0){
                                tokenNum++;
                                if(strcmp(currentToken(tokenNum, list)->idTk,"KW_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"then") == 0){
                                        temp->tk2 = currentToken(tokenNum, list);
                                        tokenNum++;
                                        temp->right = stat(list);
                                        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,";") == 0){
                                                tokenNum++;
                                                return temp;
                                        }else{
                                                //error expected ;
                                                printf("Error: Expected ';' got %s\n", currentToken(tokenNum, list)->tokenInstance);

                                        }
                                }else{
                                        //error expected then
                                        printf("Error: Expected 'then' got %s\n", currentToken(tokenNum, list)->tokenInstance);

                                }
                        }else{
                                //error expected ]
                                printf("Error: Expected ']' got %s\n", currentToken(tokenNum, list)->tokenInstance);

                        }
                }else{
                        //error expected [
                        printf("Error: Expected '[' got %s\n", currentToken(tokenNum, list)->tokenInstance);

                }
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"KW_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"pick") == 0){
                struct Node* temp = getNode("stat--pick");
                temp->tk1 = currentToken(tokenNum, list);
                tokenNum++;
                temp->left = expr(list);
                temp->leftmiddle = pickbody(list);
                if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,";") == 0){
                        tokenNum++;
                        return temp;
                }else{
                        //error expected ;
                        printf("Error: Expected ';' got %s\n", currentToken(tokenNum, list)->tokenInstance);

                }
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"KW_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"while") == 0){
                struct Node* temp = getNode("stat--whileloop");
                temp->tk1 = currentToken(tokenNum, list);
                tokenNum++;
                if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"[") == 0){
                        tokenNum++;
                        temp->left = expr(list);
                        temp->leftmiddle = ro(list);
                        temp->rightmiddle = expr(list);
                        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"]") == 0){
                                tokenNum++;
                                temp->right = stat(list);
                                if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,";") == 0){
                                        tokenNum++;
                                        return temp;
                                }else{
                                        //error expected ;
                                        printf("Error: Expected ';' got %s\n", currentToken(tokenNum, list)->tokenInstance);

                                }
                        }else{
                                //error expected ]
                                printf("Error: Expected ']' got %s\n", currentToken(tokenNum, list)->tokenInstance);

                        }
                }else{
                        //error expected [
                        printf("Error: Expected '[' got %s\n", currentToken(tokenNum, list)->tokenInstance);
                }
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"KW_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"repeat") == 0){
                struct Node* temp = getNode("stat--repeatloop");
                temp->tk1 = currentToken(tokenNum, list);
                tokenNum++;
                temp->left = stat(list);
                if(strcmp(currentToken(tokenNum, list)->idTk,"KW_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"until") == 0){
                        temp->tk2 = currentToken(tokenNum, list);
                        tokenNum++;
                        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"[") == 0){
                                tokenNum++;
                                temp->leftmiddle = expr(list);
                                temp->rightmiddle = ro(list);
                                temp->right = expr(list);
                                if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"]") == 0){
                                        tokenNum++;
                                        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,";") == 0){
                                                tokenNum++;
                                                return temp;
                                        }else{
                                                //error expected ;
                                                printf("Error: Expected ';' got %s\n", currentToken(tokenNum, list)->tokenInstance);

                                        }
                                }else{
                                        //error expected ]
                                        printf("Error: Expected ']' got %s\n", currentToken(tokenNum, list)->tokenInstance);

                                }
                        }else{
                                //error expected ]
                                printf("Error: Expected '[' got %s\n", currentToken(tokenNum, list)->tokenInstance);

                        }
                }else{
                        //error expected until
                        printf("Error: Expected 'until' got %s\n", currentToken(tokenNum, list)->tokenInstance);

                }
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"KW_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"set") == 0){
                struct Node* temp = getNode("stat--assignSet");
                temp->tk1 = currentToken(tokenNum, list);
                tokenNum++;
                if(strcmp(currentToken(tokenNum, list)->idTk,"IDENT_TK") == 0){
                        temp->tk2 = currentToken(tokenNum, list);
                        tokenNum++;
                        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"=") == 0){
                                temp->tk3 = currentToken(tokenNum, list);
                                tokenNum++;
                                temp->left = expr(list);
                                if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,";") == 0){
                                        tokenNum++;
                                        return temp;
                                }else{
                                        //error expected ;
                                        printf("Error: Expected ';' got %s\n", currentToken(tokenNum, list)->tokenInstance);

                                }
                        }else{
                                //error expected =
                                printf("Error: Expected '=' got %s\n", currentToken(tokenNum, list)->tokenInstance);

                        }
                }else{
                        //error expected IDENT_TK
                        printf("Error: Expected 'IDENT_TK' got %s\n", currentToken(tokenNum, list)->tokenInstance);

                }
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"IDENT_TK") == 0){
                struct Node* temp = getNode("stat--assign");
                temp->tk1 = currentToken(tokenNum, list);
                tokenNum++;
                if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"=") == 0){
                        temp->tk2 = currentToken(tokenNum, list);
                        tokenNum++;
                        temp->left = expr(list);
                        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,";") == 0){
                                tokenNum++;
                                return temp;
                        }else{
                                //error expected ;
                                printf("Error: Expected ';' got %s\n", currentToken(tokenNum, list)->tokenInstance);

                        }
                }else{
                        //error expected =
                        printf("Error: Expected '=' got %s\n", currentToken(tokenNum, list)->tokenInstance);

                }
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"KW_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"jump") == 0){
                struct Node* temp = getNode("stat--goto");
                temp->tk1 = currentToken(tokenNum, list);
                tokenNum++;
                if(strcmp(currentToken(tokenNum, list)->idTk,"IDENT_TK") == 0){
                        temp->tk2 = currentToken(tokenNum, list);
                        tokenNum++;
                        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,";") == 0){
                                tokenNum++;
                                return temp;
                        }else{
                                //error expected ;
                                printf("Error: Expected ';' got %s\n", currentToken(tokenNum, list)->tokenInstance);

                        }
                }else{
                        //error expected IDENT_TK
                        printf("Error: Expected 'IDENT_TK' got %s\n", currentToken(tokenNum, list)->tokenInstance);

                }
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"{") == 0){
                struct Node* temp = getNode("stat--block");
                temp->left = block(list);
                return temp;
        }else{
                //error in stat
                printf("Error: (Stat)Expected something but got %s\n", currentToken(tokenNum, list)->tokenInstance);
                Sleep(5000);

        }


}

struct Node* pickbody(struct Token* list){
        struct Node* temp = getNode("pickbody");
        temp->left = stat(list);
        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,":") == 0){
                temp->tk1 = currentToken(tokenNum, list);
                tokenNum++;
                temp->leftmiddle = stat(list);
                return temp;
        }else{
                //error expected :
                printf("Error: Expected ':' got %s\n", currentToken(tokenNum, list)->tokenInstance);

        }
}

struct Node* ro(struct Token* list){
        struct Node* temp = getNode("ro");
        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"<") == 0){
                temp->tk1 = currentToken(tokenNum, list);
                tokenNum++;
                return temp;
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,">") == 0){
                temp->tk1 = currentToken(tokenNum, list);
                tokenNum++;
                return temp;
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,".") == 0){
                temp->tk1 = currentToken(tokenNum, list);
                tokenNum++;
                if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,".") == 0){
                        temp->tk2 = currentToken(tokenNum, list);
                        tokenNum++;
                        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,".") == 0){
                                temp->tk3 = currentToken(tokenNum, list);
                                tokenNum++;
                                return temp;
                        }else{
                                //error expected .
                                printf("Error: Expected '.' got %s\n", currentToken(tokenNum, list)->tokenInstance);

                        }
                }else{
                        //error expected .
                        printf("Error: Expected '.' got %s\n", currentToken(tokenNum, list)->tokenInstance);

                }
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"==") == 0){
                temp->tk1 = currentToken(tokenNum, list);
                tokenNum++;
                //temp->left = roExtended(list);
                return temp;
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"=!=") == 0){
                temp->tk1 = currentToken(tokenNum, list);
                tokenNum++;
                //temp->left = roExtended(list);
                return temp;
        }else{
                //error in ro
                printf("Error: (ro)Expected operator but got %s\n", currentToken(tokenNum, list)->tokenInstance);
        }
}

struct Node* roExtended(struct Token* list){
        struct Node* temp = getNode("roExtended");
        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"=") == 0){
                temp->tk1 = currentToken(tokenNum, list);
                tokenNum++;
                return temp;
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"!") == 0){
                temp->tk1 = currentToken(tokenNum, list);
                tokenNum++;
                if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"=") == 0){
                        temp->tk2 = currentToken(tokenNum, list);
                        tokenNum++;
                        return temp;
                }else{
                        //error expected =
                        printf("Error: Expected '=' got %s\n", currentToken(tokenNum, list)->tokenInstance);

                }
        }else{
                //error expected = or !=
                printf("Error: Expected '=' or '!=' got %s\n", currentToken(tokenNum, list)->tokenInstance);

        }
}

struct Node* expr(struct Token* list){
        struct Node* temp = getNode("expr");
        temp->left = n(list);
        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"-") == 0){
                temp->tk1 = currentToken(tokenNum, list);
                tokenNum++;
                temp->leftmiddle = expr(list);
                return temp;
        }else{
                return temp;
        }

}

struct Node* n(struct Token* list){
        struct Node* temp = getNode("n");
        temp->left = a(list);
        temp->leftmiddle = nExtended(list);
        return temp;
}

struct Node* nExtended(struct Token* list){
        struct Node* temp = getNode("nExtended");
        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"/") == 0){
                temp->tk1 = currentToken(tokenNum, list);
                tokenNum++;
                temp->left = a(list);
                temp->leftmiddle = nExtended(list);
                return temp;
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"+") == 0){
                temp->tk1 = currentToken(tokenNum, list);
                tokenNum++;
                temp->left = a(list);
                temp->leftmiddle = nExtended(list);
                return temp;
        }else{
                return NULL;
        }
}

struct Node* a(struct Token* list){
        struct Node* temp = getNode("a");
        temp->left = m(list);
        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"*") == 0){
                temp->tk1 = currentToken(tokenNum, list);
                tokenNum++;
                temp->leftmiddle = a(list);
                return temp;
        }else{
                return temp;
        }
}

struct Node* m(struct Token* list){
        struct Node* temp = getNode("m");
        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"^") == 0){
                temp->tk1 = currentToken(tokenNum, list);
                tokenNum++;
                temp->left = m(list);
                return temp;
        }else{
                temp->left = r(list);
                return temp;
        }
}

struct Node* r(struct Token* list){
        struct Node* temp = getNode("r");
        if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,"(") == 0){
                tokenNum++;
                temp->left = expr(list);
                if(strcmp(currentToken(tokenNum, list)->idTk,"OP_TK") == 0 && strcmp(currentToken(tokenNum, list)->tokenInstance,")") == 0){
                        tokenNum++;
                        return temp;
                }else{
                        //error expected )
                        printf("Error: Expected ')' got %s\n", currentToken(tokenNum, list)->tokenInstance);

                }
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"IDENT_TK") == 0){
                temp->tk1 = currentToken(tokenNum, list);
                tokenNum++;
                return temp;
        }else if(strcmp(currentToken(tokenNum, list)->idTk,"INT_TK") == 0){
                temp->tk1 = currentToken(tokenNum, list);
                tokenNum++;
                return temp;
        }else{
                //error in r
                printf("Error: Expected 'IDENTIFIER', 'INTEGER', or '(' got %s\n", currentToken(tokenNum, list)->tokenInstance);

        }
}
