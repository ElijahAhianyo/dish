#include "command.h"
#include <stdlib.h>
#include "memory.h"


void simple_command_init(simple_command_t *simple_command){
    simple_command->capacity = 0;
    simple_command->argc = 0;
    simple_command->argv = NULL;
    simple_command->in_file = NULL;
    simple_command->out_file = NULL;
    simple_command->err_file = NULL;
}


void command_init(command_t *command) {
    command->capacity = 0;
    command->background = 0;
    command->pipeline_len = 0;
    command->pipeline = NULL;
}


void insert_argument(simple_command_t *simple_command, char *arg){
    
    if(simple_command->capacity < simple_command->argc + 1 ) {
        // increase array size if we're at the boundary
        int old_capacity = simple_command->capacity;
        simple_command->capacity = GROW_CAPACITY(old_capacity);
        simple_command->argv = GROW_ARRAY(char, simple_command->argv, old_capacity, simple_command->capacity);
    }
    simple_command->argv[simple_command->argc] = arg;
    simple_command->argc++;
}



void insert_simple_command(command_t *command, simple_command_t *simple_command){

    if(command->capacity < command->pipeline_len + 1){
        int old_capacity = command->capacity;
        command->capacity = GROW_CAPACITY(old_capacity);
        command->pipeline = GROW_ARRAY(simple_command_t, command->pipeline, old_capacity, command->capacity);
    }

    command->pipeline[command->pipeline_len] = simple_command;
    simple_command->argc++;
}
