#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "parser.h"
#include "statSem.h"


//Author: Jarod Stagner
//Date: 04/10/2024

void help(){
        printf("Incorrect Usage\nHere are the valid ways to run this program:\n./Project1 [filename]\n");
        printf("The program will then tokenize the code in the file and output it to the screen\n");
        printf("After, it will put it through our grammar and report its validity\n");
        printf("Lastly, it will display the tree and any errors having to do with static semantics\n");
}

int main(int argc, char** argv) {

        FILE *file;
        char buffer[100];

        if(argc > 2){//if invalid

                help();
                return EXIT_FAILURE;

        }else if(argc == 2){//if file input

                file = fopen(argv[1],"r");

        }else{//if stdin

                file = fopen("tempfile.txt", "w+");

                while (fgets(buffer, sizeof(buffer),stdin) != NULL){
                        fputs(buffer, file);
                }

                fseek(file, 0, SEEK_SET);

        }

        if(file == NULL){
                printf("Problem with given file... exiting\n");
                return EXIT_FAILURE;
        }
        fseek(file, 0, SEEK_END);
        if(ftell(file) == 0){
                fclose(file);
                printf("Given file is empty... exiting\n");
                return EXIT_FAILURE;
        }else{
                fseek(file, 0, SEEK_SET);
        }

        FILE *filterfile;
        filterfile = fopen("filteredFile.txt", "w+");
        filter(file,filterfile);
        fseek(filterfile, 0, SEEK_SET);

        struct Token* tokenList = scanner(filterfile);
        displayTokenList(tokenList);

        struct Node* tree;
        tree = parser(tokenList);

        staticSemantics(tree);

        freeTree(tree);

        // while(tokenList != NULL){
        //         struct Token* temp = tokenList;
        //         tokenList = tokenList->next;
        //         free(temp);
        // }

        fclose(file);
        fclose(filterfile);

        return 0;
}
