#include "lex.h"
#include "string.h"
#include "command.h"
#include "memory.h"

bool is_at_end(lexer_t *lexer){
    return *lexer->current == '\0';
}

/*
* create a token from the lexer details. The lexer->start is
* zero-copied (we only store a pointer) so it must not be freed
* since we rely on the parser to strdup it
*/
token_t make_token(lexer_t *lexer, token_type_t token_type){
    token_t token;
    size_t len = (size_t)(lexer->current - lexer->start);
    token.start = lexer->start;
    token.type = token_type;
    token.len = len;
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

void lexer_init(lexer_t *lexer, const char *src){
    lexer->buf = src;
    lexer->start = lexer->buf;
    lexer->current = lexer->buf;
}


void lexer_free(lexer_t *lexer){
    if (!lexer) return;
    free(lexer->buf);
    lexer_init(lexer, NULL);
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
    return TOKEN_WORD;
}

static token_type_t command_type(lexer_t *lexer){
    switch(lexer->start[0]) {
        case 'w': return check_command(lexer, 1, 1, "c", TOKEN_COMMAND_WC);
    };
    return TOKEN_WORD;
}

static token_t command(lexer_t *lexer){
    while (is_alpha(*lexer->current))
    {
        advance(lexer);
    }

    return make_token(lexer, command_type(lexer));
    
}

void lex_all(lexer_t *lexer, token_array_t *token_array){
    while(!is_at_end(lexer)){
        token_t tok = scan_token(lexer);
        token_array_push(token_array, tok);
    }
    token_t eof = make_token(lexer, TOKEN_EOF);
    token_array_push(token_array, eof);
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
    return maketok(TOKEN_WORD);
    #undef maketok
}


void token_array_init(token_array_t *token_array){
    token_array->cap=0;
    token_array->len=0;
    token_array->data=NULL;
}

void token_array_push(token_array_t *token_array, token_t token){
    if(token_array->cap < token_array->len + 1){
        size_t old_cap = token_array->cap;
        token_array->cap = GROW_CAPACITY(old_cap);
        token_array->data = GROW_ARRAY(token_t, token_array->data, old_cap, token_array->cap);
    }
    token_array->data[token_array->len] = token;
    token_array->len++;
}

void token_array_free(token_array_t *token_array){
    for(int i = 0; i < (int)token_array->len; i++){
        token_free(&token_array->data[i]);
    }
    free(token_array->data);
    token_array_init(token_array);
}

char *tokentostr(token_t *t){
    char *dup = strndup(t->start, t->len);
    return dup;
}

void token_free(token_t *token){
    token->start = NULL;
    token->len = 0;
    token->type = TOKEN_ERROR;
}
