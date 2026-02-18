#include <stdio.h>
#include "lex.h"
#include "compiler.h"
#include "executer.h"


static void repl(void) {
    char line[1024];

    for (;;) {
        command_t command;
        command_init(&command);

        printf("> ");
        if(!fgets(line, sizeof(line) , stdin)){
            printf("\n");
            break;
        }
        if(!compile(line, &command)){
            command_free(&command);
            continue;
        }
        execute(&command);
        // printf("we are done!\n");
        command_free(&command);
    }
}

int main(void){
    // for now go into the repl directly.

    repl();

    return 0;
}
