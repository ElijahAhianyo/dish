#include "common.h"

typedef struct {
    char *out_file;
    char *in_file;
    char *err_file;
} redir_t;

void redir_init(redir_t *redir);
void redir_free(redir_t *redir);

typedef struct {
    size_t capacity;
    size_t argc;
    char **argv;
    redir_t redir;
} simple_command_t;

void insert_argument (simple_command_t *simple_command, char *arg);
void simple_command_init(simple_command_t *simple_command);
void simple_command_free(simple_command_t *simple_command);


typedef struct {
    size_t capacity;
    size_t pipeline_len;
    simple_command_t *pipeline;
    int background;
} command_t;


void command_init(command_t *command);
void command_free(command_t *command);

void insert_simple_command(command_t *command, simple_command_t simple_command); //TODO: Does simple_command really have to be a ref here?
