#include <stdio.h>
#include "lex.h"


static void replit(void) {
    char line[1024];
    lexer_t lexer;
    

    for (;;) {
        printf("> ");
        if(!fgets(line, sizeof(line) , stdin)){
            printf("\n");
            break;
        }
        init_lexer(&lexer, line);
        scan_token(&lexer);
        printf("we are done!\n");
    }
}

int main(void){
    // for now go into the replit directly.

    replit();

    return 0;
}
