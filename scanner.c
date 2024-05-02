#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include"scanner.h"

enum TokenType {   
        INT_TK = 1001,
        IDENT_TK = 1002,
        OP_TK = 1003,
        ERROR_TK = 100,
        EOF_TK = 1004,
        KW_TK = 1005
};

void printToken(int tkNum, char* word, int lineNum, int startPos){

        struct Token* tempToken = (struct Token*)malloc(sizeof(struct Token));

        if(tkNum == INT_TK){
                tempToken->idTk = "INT_TK";
        }else if(tkNum == IDENT_TK){
                tempToken->idTk = "IDENT_TK";
        }else if(tkNum == OP_TK){
                tempToken->idTk = "OP_TK";
        }else if(tkNum == KW_TK){
                tempToken->idTk = "KW_TK";
        }else if(tkNum == EOF_TK){
                tempToken->idTk = "EOF_TK";
        }

        tempToken->tokenInstance = strdup(word);
        tempToken->lineNum = lineNum;
        tempToken->charNum = startPos;

        printf("Token ID: %s, Token: %s, at %d.%d\n", tempToken->idTk, tempToken->tokenInstance, tempToken->lineNum, tempToken->charNum);

        free(tempToken);
}

void displayTokenList(struct Token* head){
        printf("Token List:\n");
        while(head != NULL){
                printf("Token ID: %s   %s\n", head->idTk, head->tokenInstance);
                head = head->next;
        }
}

struct Token* createToken(int tkNum, char* word, int lineNum, int startPos){

        struct Token* newToken = (struct Token*)malloc(sizeof(struct Token));
        if(tkNum == INT_TK){
                newToken->idTk = "INT_TK";
        }else if(tkNum == IDENT_TK){
                newToken->idTk = "IDENT_TK";
        }else if(tkNum == OP_TK){
                newToken->idTk = "OP_TK";
        }else if(tkNum == KW_TK){
                newToken->idTk = "KW_TK";
        }else if(tkNum == EOF_TK){
                newToken->idTk = "EOF_TK";
        }
        newToken->tokenInstance = strdup(word);
        newToken->lineNum = lineNum;
        newToken->charNum = startPos;
        newToken->next = NULL;

        return newToken;
}

void insertToken(struct Token** headRef, int tkNum, char* word, int lineNum, int startPos){

        struct Token* newToken = createToken(tkNum, word, lineNum, startPos);
        if(*headRef == NULL){
                *headRef = newToken;
                return;
        }
        struct Token* current = *headRef;
        while (current->next != NULL){
                current = current->next;
        }
        current->next = newToken;
}

enum State {
        S1 = 0,
        S2 = 1,
        S3 = 2,
        S4 = 3,
        S5 = 4,
        S6 = 5,
        S7 = 6,
        S8 = 7
};

const char *const specialChar[] = {
        "=","<",">",":",";","+","-","*","/","^",
        ".","(",")",",","{","}","[","]","|","&","!"
};

int isSpecialChar(char ch){
        int x;
        for(x = 0; x < 21; x++){
                if(*specialChar[x] == ch){
                        return x;
                }
        }
        return -1;
}

const char* keyWords[] = {
        "start", "stop", "while", "repeat", "until", "label", "return",
        "cin", "cout", "tape", "jump", "if", "then", "pick", "create",
        "set", "func"
};

int isKeyWord(char* word) {
    int wordLength = strlen(word);

    for (int i = 0; i < wordLength; i++) {
        for (int j = 0; j < 17; j++) {
            int keywordLength = strlen(keyWords[j]);
            if (strncmp(word + i, keyWords[j], keywordLength) == 0) {
                //printf("i is %d\n", i);
                return i; // Return the index where the keyword starts in the word
            }
        }
    }

    return -1; // Keyword not found
}


const int FSATABLE[][26] = {
//       =        <        >        :        ;        +        -        *        /        ^        .        (        )        ,        {        }        [        ]        |        &        !        a-Z      0-9      _        /n       EOF
        {S4,      OP_TK,   OP_TK,   S6,      OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   S7,      S8,      ERROR_TK,S2,      S3,      ERROR_TK,S1,      EOF_TK  },//S1
        {IDENT_TK,IDENT_TK,IDENT_TK,IDENT_TK,IDENT_TK,IDENT_TK,IDENT_TK,IDENT_TK,IDENT_TK,IDENT_TK,IDENT_TK,IDENT_TK,IDENT_TK,IDENT_TK,IDENT_TK,IDENT_TK,IDENT_TK,IDENT_TK,IDENT_TK,IDENT_TK,IDENT_TK,S2,      S2,      S2,      IDENT_TK,IDENT_TK},//S2
        {INT_TK,  INT_TK,  INT_TK,  INT_TK,  INT_TK,  INT_TK,  INT_TK,  INT_TK,  INT_TK,  INT_TK,  INT_TK,  INT_TK,  INT_TK,  INT_TK,  INT_TK,  INT_TK,  INT_TK,  INT_TK,  INT_TK,  INT_TK,  INT_TK,  INT_TK,  S3,      INT_TK,  INT_TK,  INT_TK},  //S3
        {OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   S5,      OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK},   //S4
        {OP_TK,   ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK},//S5
        {OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK,   OP_TK},   //S6
        {ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,OP_TK,   ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK},//S7
        {ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,OP_TK,   ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK,ERROR_TK} //S8
};


void filter(FILE *dirtyfile, FILE *cleanfile){
        char buffer[1024];
        while(fgets(buffer, sizeof(buffer), dirtyfile) != NULL){
                char *commentPosition = strstr(buffer, "//");
                if(commentPosition != NULL){
                        *commentPosition = '\n';
                        *(commentPosition + 1) = '\0';
                }
                fputs(buffer, cleanfile);
        }
}

struct Token* scanner(FILE *file){

        struct Token* head = NULL;

        char ch;
        int index = 0;
        char word[256];

        int lineNum = 1;
        int charNum = 0;
        int startPosition = 0;

        int state = 0;
        int previousState = 0;

        while(ch = fgetc(file)){
                //printf("Char: %c, State: %d \n", ch, state);
                charNum++;
                if(ch == ' ' || ch == '\t'){
                        //check if keyword in S2
                        if(state == S2){
                                //word[index] = '\0';
                                if(isKeyWord(word) == 0){
                                        word[index] = '\0';
                                        state = KW_TK;
                                        fseek(file, -1, SEEK_CUR);
                                        charNum--;
                                        printToken(state, word, lineNum, startPosition);
                                        insertToken(&head, state, word, lineNum, startPosition);
                                        //printf("State: %d ,Token: %s, at %d.%d\n", state, word, lineNum, startPosition);
                                        state = 0;
                                        index = 0;
                                        memset(word, '\0', sizeof(word));
                                }else if(isKeyWord(word) > 0){
                                        int wordlength = strlen(word);
                                        word[isKeyWord(word)] = '\0';
                                        state = IDENT_TK;
                                        fseek(file, -(wordlength-(isKeyWord(word)+1)), SEEK_CUR);
                                        int y = 0;
                                        for(y; y < (wordlength-(isKeyWord(word)+1)); y++){
                                                charNum--;
                                        }
                                        printToken(state, word, lineNum, startPosition);
                                        insertToken(&head, state, word, lineNum, startPosition);
                                        //printf("State: %d ,Token: %s, at %d.%d\n", state, word, lineNum, startPosition);
                                        state = 0;
                                        index = 0;
                                }
                        }
                }else{
                        previousState = -1;
                        if(ch == EOF){
                                state = FSATABLE[state][25];
                        }else if(ch == '\n'){
                                previousState = state;
                                state = FSATABLE[state][24];
                                lineNum++;
                                charNum = 0;
                        }else if(ch == '_'){
                                previousState = state;
                                state = FSATABLE[state][23];
                        }else if(isdigit(ch)){
                                previousState = state;
                                state = FSATABLE[state][22];
                        }else if(isalpha(ch)){
                                previousState = state;
                                state = FSATABLE[state][21];
                        }else if(isSpecialChar(ch) != -1){
                                previousState = state;
                                state = FSATABLE[state][isSpecialChar(ch)];
                                //printf("State: %d \n", state);
                                //printf("Special Character Column: %d \n", isSpecialChar(ch));
                        }else{
                                state = ERROR_TK;
                        }
                        if(state < 10){
                                if(state > 0){
                                        if(index == 0){
                                                startPosition = charNum;
                                        }
                                        word[index++] = ch;
                                }
                        }else if(state == ERROR_TK){
                                printf("ERROR_TK reached, %d.%d\n", lineNum, charNum);
                                if(previousState == S1){
                                        printf("No Identifier, Integer, or Operator starts with %c\n", ch);
                                }else if(previousState == S7){
                                        printf("Expected '|' got '%c'\n", ch);
                                }else if(previousState == S8){
                                        printf("Expected '&' got '%c'\n", ch);
                                }else if(previousState == S5){
                                        printf("Expected '=' got '%c'\n", ch);
                                }else{
                                        printf("Invalid Char '%c'\n", ch);
                                }
                                break;
                        }else if(state != EOF_TK){
                                if((previousState == S4 && ch == '=') || (previousState == S5 && ch == '=') || (previousState == S6 && ch == '=') || (previousState == S7 && ch == '|') || (previousState == S8 && ch == '&')){
                                        word[index] = ch;
                                }else if(previousState != S4 && state == OP_TK && ch != '\n' && ch != '=' && ch != ':' && ch != '|' && ch != '&' && !(isdigit(ch)) && !(isalpha(ch))){
                                        word[index] = ch;
                                        startPosition++;
                                }else if(state == IDENT_TK){
                                        word[index] = '\0';
                                        //printf("IdenWord: %s, func value: %d\n", word, isKeyWord(word));
                                        if(isKeyWord(word) == 0){
                                                state = KW_TK;
                                                fseek(file, -1, SEEK_CUR);
                                                if(ch == '\n'){
                                                        lineNum--;
                                                }
                                                charNum--;
                                        }else if(isKeyWord(word) > 0){
                                                int wordlength = strlen(word);
                                                word[isKeyWord(word)] = '\0';
                                                state = IDENT_TK;
                                                fseek(file, -(wordlength-(isKeyWord(word)+1)), SEEK_CUR);
                                                int y = 0;
                                                for(y; y < (wordlength-(isKeyWord(word)+1)); y++){
                                                        charNum--;
                                                }
                                        }else{
                                                if(ch == '\n'){
                                                        lineNum--;
                                                }
                                                fseek(file, -1, SEEK_CUR);
                                                charNum--;
                                        }
                                }else{
                                        fseek(file, -1, SEEK_CUR);
                                        if(ch == '\n'){
                                                lineNum--;
                                        }
                                        charNum--;
                                }
                                printToken(state, word, lineNum, startPosition);
                                insertToken(&head, state, word, lineNum, startPosition);
                                //printf("State: %d ,Token: %s, at %d.%d\n", state, word, lineNum, startPosition);
                                state = 0;
                                index = 0;
                                memset(word, '\0', sizeof(word));
                        }else{
                                lineNum--;
                                printToken(state, word, lineNum, startPosition);
                                insertToken(&head, state, word, lineNum, startPosition);
                                return head;
                                //printf("State: %d ,Token: %s, at %d.%d\n", state, word, lineNum, startPosition);
                                break;
                        }
                }
        }
}
