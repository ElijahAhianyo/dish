#ifndef dish_compiler_h
#define dish_compiler_h

#include "common.h"
#include "command.h"
#include "lex.h"

typedef struct {
    bool has_error;
    char *msg;
    size_t token_index;
} parser_error_t;

typedef struct {
    size_t start;
    size_t current;
    token_t *tokens;
    parser_error_t err;
} parser_t;

void parser_init(parser_t *parser, token_t *tokens);
void parser_free(parser_t *parser);

void parser_set_error(parser_t *p, size_t token_index, const char *fmt, ...);

void compile(const char **src, command_t *command);

#endif