#include "common.h"

typedef struct {
    size_t capacity;
    size_t argc;
    char **argv;
    char *out_file;
    char *in_file;
    char *err_file;
} simple_command_t;

void insert_argument (simple_command_t *simple_command, char *arg);
void simple_command_init(simple_command_t *simple_command);
void simple_command_free(simple_command_t *simple_command);


typedef struct {
    size_t capacity;
    size_t pipeline_len;
    simple_command_t **pipeline;
    int background;
} command_t;


void command_init(command_t command);

void prompt(void);
void print(void);
void execute(void);
void clear(void);
void insert_simple_command(command_t *command, simple_command_t *simple_command);
