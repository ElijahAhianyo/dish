#include "common.h"

typedef struct {
    int arg_len;
    char **args;
} simple_command_t;

void insert_argument (char *arg);
void init_simple_command(simple_command_t *simple_command);


typedef struct {
    int simple_commands_len;
    simple_command_t **simple_command;
    char *out_file;
    char *in_file;
    char *err_file;
    int background;
} command_t;


void init_command(command_t command);

void prompt(void);
void print(void);
void execute(void);
void clear(void);
void insert_simple_command(simple_command_t *simple_command);
