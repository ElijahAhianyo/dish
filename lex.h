#ifndef dish_lex_h
#define dish_lex_h

#include "common.h"

typedef enum {
    TOKEN_EOF, 
    TOKEN_PIPE, TOKEN_MINUS,
    TOKEN_STAR, TOKEN_SLASH, TOKEN_LT, TOKEN_LT_LT, TOKEN_GT, TOKEN_GT_GT,
    TOKEN_GT_AMP, TOKEN_AMP, TOKEN_2_GT,

    // commands
    TOKEN_COMMAND_LS,TOKEN_COMMAND_EXIT,TOKEN_COMMAND_HELP, 
    TOKEN_COMMAND_WC,TOKEN_COMMAND_CAT,TOKEN_COMMAND_HEAD,
    TOKEN_COMMAND_TAIL,TOKEN_COMMAND_SORT,

    // TOKEN_NEWLINE,
    TOKEN_ERROR,
    TOKEN_WORD, TOKEN_NUMBER
} token_type_t;


typedef struct {
    token_type_t type;
    const char *start;
    size_t len;
} token_t;

void token_free(token_t *token);

typedef struct {
    char *buf;
    const char *start;
    const char *current;
} lexer_t;

typedef struct {
    token_t *data;
    size_t len;
    size_t cap;
} token_array_t;

void lexer_init(lexer_t *lexer, const char *src);
void lexer_free(lexer_t *lexer);

void token_array_init(token_array_t *token_array);
void token_array_push(token_array_t *token_array, token_t token);
void token_array_free(token_array_t *token_array);

token_t scan_token(lexer_t *lexer);
void lex_all(lexer_t *lexer, token_array_t *token_array);

char *tokentostr(token_t *t);

#endif
