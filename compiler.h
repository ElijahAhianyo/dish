#ifndef dish_compiler_h
#define dish_compiler_h

#include "common.h"
#include "command.h"
#include "lex.h"

typedef struct {
    size_t start;
    size_t current;
    token_t *tokens;
} parser_t;

void parser_init(parser_t *parser, token_t *tokens);
void parser_free(parser_t *parser);

void compile(const char **src, command_t *command);

#endif