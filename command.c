#include "command.h"
#include <stdlib.h>
#include "memory.h"

void redir_init(redir_t *redir){
    redir->err_file = NULL;
    redir->in_file = NULL;
    redir->out_file = NULL;
}

void redir_free(redir_t *redir){
    free(redir->err_file);
    free(redir->in_file);
    free(redir->out_file);
}

void simple_command_init(simple_command_t *simple_command){
    simple_command->capacity = 0;
    simple_command->argc = 0;
    simple_command->argv = NULL;
    redir_init(&simple_command->redir);
}

void simple_command_free(simple_command_t *simple_command){
    
    if(!simple_command) return;
    for(int i = 0; i < (int)simple_command->argc; i++){
        free(simple_command->argv[i]);
    }
    free(simple_command->argv);
    redir_free(&simple_command->redir);
    simple_command_init(simple_command);
}


void command_init(command_t *command) {
    command->capacity = 0;
    command->background = 0;
    command->pipeline_len = 0;
    command->pipeline = NULL;
}

void command_free(command_t *command){
    if (!command) return;

    for(int i = 0; i < (int)command->pipeline_len; i++){
        simple_command_free(&command->pipeline[i]);
    }
    free(command->pipeline);
    command_init(command);
}

/*
* Add an argument to a simple command. The caller must malloc(heap-allocate) arg
*/
void insert_argument(simple_command_t *simple_command, char *arg){
    
    if(simple_command->capacity < simple_command->argc + 1 ) {
        // increase array size if we're at the boundary
        int old_capacity = simple_command->capacity;
        simple_command->capacity = GROW_CAPACITY(old_capacity);
        simple_command->argv = GROW_ARRAY(char*, simple_command->argv, old_capacity, simple_command->capacity);
    }
    simple_command->argv[simple_command->argc] = arg;
    simple_command->argc++;
}


/*
* Add a simple command/pipeline to the program. The caller must not free simple_command
*/
void insert_simple_command(command_t *command, simple_command_t *simple_command){

    if(command->capacity < command->pipeline_len + 1){
        int old_capacity = command->capacity;
        command->capacity = GROW_CAPACITY(old_capacity);
        command->pipeline = GROW_ARRAY(simple_command_t, command->pipeline, old_capacity, command->capacity);
    }

    command->pipeline[command->pipeline_len] = *simple_command;
    simple_command->argc++;
}
