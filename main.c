#include <stdio.h>
#include "lex.h"
#include "compiler.h"
#include "executer.h"


static void replit(void) {
    char line[1024];
    lexer_t lexer;
    

    for (;;) {
        command_t command;
        command_init(&command);

        printf("> ");
        if(!fgets(line, sizeof(line) , stdin)){
            printf("\n");
            break;
        }
        compile(line, &command);
        execute(&command);
        printf("we are done!\n");
        clean:
            free(line);
            command_free(&command);
    }
}

int main(void){
    // for now go into the replit directly.

    replit();

    return 0;
}
