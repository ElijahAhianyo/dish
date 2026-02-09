#include "common.h"

typedef enum {
    TOKEN_EOF, 
    TOKEN_PIPE, TOKEN_REDIRECT_IN, TOKEN_REDIRECT_OUT, TOKEN_MINUS,
    TOKEN_STAR, TOKEN_SLASH, TOKEN_LESS, TOKEN_LESS_LESS, TOKEN_GREAT, TOKEN_GREAT_GREAT,
    TOKEN_GREATAMPERSAND, TOKEN_AMPERSAND,

    // commands
    TOKEN_COMMAND_LS,TOKEN_COMMAND_EXIT,TOKEN_COMMAND_HELP, 
    TOKEN_COMMAND_WC,TOKEN_COMMAND_CAT,TOKEN_COMMAND_HEAD,
    TOKEN_COMMAND_TAIL,TOKEN_COMMAND_SORT,

    // TOKEN_NEWLINE,
    TOKEN_ERROR,
    TOKEN_UNKNOWN
} token_type_t;


typedef struct {
    token_type_t type;
    const char *start;
    int len;
} token_t;


typedef struct {
    const char *start;
    const char *current;
} lexer_t;

void init_lexer(lexer_t *lexer, char *src);
token_t scan_token(lexer_t *lexer);
