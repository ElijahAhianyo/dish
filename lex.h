#include "common.h"

typedef enum {
    TOKEN_EOF, 
    TOKEN_PIPE, TOKEN_REDIRECT_IN, TOKEN_REDIRECT_OUT, TOKEN_MINUS,
    TOKEN_STAR, TOKEN_SLASH, TOKEN_LESS, TOKEN_LESS_LESS, TOKEN_GREAT, TOKEN_GREAT_GREAT,
    TOKEN_GREATAMPERSAND, TOKEN_AMPERSAND, TOKEN_2_GREAT,

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
    int len;
} token_t;

void token_free(token_t *token);

typedef struct {
    const char *start;
    const char *current;
} lexer_t;

typedef struct {
    token_t *data;
    size_t len;
    size_t cap;
} token_array_t;

void lexer_init(lexer_t *lexer, char *src);
void lexer_free(lexer_t *lexer);

void token_array_init(token_array_t *token_array);
void token_array_push(token_array_t *token_array, token_t token);
void token_array_free(token_array_t *token_array);

token_t scan_token(lexer_t *lexer);
void lex_all(lexer_t *lexer, token_array_t *token_array);

char *tokentostr(token_t *t);

