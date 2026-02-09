#include "lex.h"
#include "string.h"
#include "command.h"

bool is_at_end(lexer_t *lexer){
    return *lexer->current == '\n';
}


token_t make_token(lexer_t *lexer, token_type_t token_type){
    token_t token;
    token.start = lexer->start;
    token.type = token_type;
    token.len = (int)(lexer->current - lexer->start);
    return token;
}

static char advance(lexer_t *lexer) {
    lexer->current++;
    return lexer->current[-1];
}

static char peek(lexer_t *lexer){
    return *lexer->current;
}

static char peek_next(lexer_t *lexer){
    if (is_at_end(lexer)) return '\0';
    
    return lexer->current[1];
}

void init_lexer(lexer_t *lexer, char *src){
    lexer->start = src;
    lexer->current = src;
}

static bool is_alpha(char c){
    return (
        (c >= 'a' && c <= 'z' )||
        (c >= 'A' && c <= 'Z')
    );
}

static token_type_t check_command(lexer_t *lexer, int start, int len, char *c, token_type_t token_type){
    // first check if they are of the same len(short-circuit) then compare byte by byte
    if (lexer->current - lexer->start == start + len &&
        memcmp(lexer->current, c, len) == 0
    ){
            return token_type;
    }
    return TOKEN_UNKNOWN;
}

static token_type_t command_type(lexer_t *lexer){
    switch(lexer->start[0]) {
        case 'w': return check_command(lexer, 1, 1, "c", TOKEN_COMMAND_WC);
    };
    return TOKEN_UNKNOWN;
}

static token_t command(lexer_t *lexer){
    while (is_alpha(*lexer->current))
    {
        advance(lexer);
    }

    return make_token(lexer, command_type(lexer));
    
}

token_t scan_token(lexer_t *lexer){
    #define maketok(type) (make_token(lexer, type));

    if (is_at_end(lexer)) return maketok(TOKEN_EOF);

    char c = advance(lexer);

    if(is_alpha(c)) return command(lexer); 

    switch (c){
        case '|': return maketok(TOKEN_PIPE);
        case '-': return maketok(TOKEN_MINUS);
        // case ''
    };
    return maketok(TOKEN_UNKNOWN);
    #undef maketok
}
