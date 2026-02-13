#include "compiler.h"
#include "lex.h"

static bool is_at_end(parser_t *parser){
    return (parser_peek(parser).type == TOKEN_EOF);
}

static token_t parser_peek(parser_t *parser){
    return parser->tokens[parser->current];
}

static token_t parser_peek_next(parser_t *parser){
    return parser->tokens[parser->current+1];
}

static token_t parser_advance(parser_t *parser){
    token_t token = parser_peek(parser);
    if (is_at_end(parser)) return token;
    parser->current++;
    return token;
} 

static token_t consume(parser_t *parser, token_type_t tt){
    if(parser_peek(parser).type == tt){
        parser_advance(parser);
        return parser_peek(parser);
    }
    //TODO: need to deal with the return type here
}


bool parser_match(parser_t *parser, token_type_t *tts, size_t len){
    token_t curr = parser_peek(parser);
    for(int i; i < len; i++){
        if (tts[i] == curr.type){
            return true;
        } 
    }
    return false;
}

void parser_init(parser_t *parser, token_t *tokens){
    parser->current = 0;
    parser->start = 0;
    parser->tokens = tokens;
}

void parser_free(parser_t *parser){

}


char *parser_command_name(parser_t *parser){
    token_type_t tts[] = {TOKEN_WORD};
    
    if (parser_match(parser, tts, 1)){
        //TODO: proper error handling here and other areas.
        token_t tok = consume(parser, TOKEN_WORD);

        return tokentostr(&tok);
    }
    return NULL;
};
char *parser_in_file(parser_t *parser){
    token_type_t tts[] = {TOKEN_LESS};

    if (parser_match(parser, tts, 1)){
        parser_advance(parser);
        token_t tok = consume(parser, TOKEN_WORD);
        return tokentostr(&tok);
    }
    return NULL;
};

char *parser_outfile(parser_t *parser) {
    token_type_t tts[] = {TOKEN_GREAT};

    if (parser_match(parser, tts, 1)){
        parser_advance(parser);
        token_t tok = consume(parser, TOKEN_WORD);
        return tokentostr(&tok);
    }
    return NULL;
};
char *parser_err_file(parser_t *parser){
    token_type_t tts[] = {TOKEN_2_GREAT};

    if (parser_match(parser, tts, 1)){
        parser_advance(parser);
        token_t tok = consume(parser, TOKEN_WORD);
        return tokentostr(&tok);
    }
    return NULL;
}

void *parser_args(parser_t *parser, simple_command_t *sc){
    token_type_t tts[] = {TOKEN_PIPE};
    while(!parser_match(parser, tts, 1)){
        token_t tok = consume(parser, TOKEN_WORD);
        insert_argument(sc, tokentostr(&tok));
        parser_advance(parser);
    }
}

simple_command_t parser_pipeline(parser_t *parser) {
    simple_command_t sc;
    simple_command_init(&sc);
    redir_t redir;
    redir_init(&redir);

    char *cmd_name = parser_command_name(parser);
    if (cmd_name == NULL){
        perror("invalid command name");
    }
    insert_argument(&sc, cmd_name);

    redir.in_file = parser_in_file(parser);
    redir.out_file = parser_outfile(parser);
    redir.err_file = parser_err_file(parser);
    
    parser_args(parser, &sc);
    sc.redir = redir;

    return sc;
}


void parser_program(parser_t *parser, command_t *command){
    token_t curr = parser_peek(parser);
    
    while(curr.type != TOKEN_EOF){
        simple_command_t sc = parser_pipeline(parser);
        insert_simple_command(&command, &sc);
    }
    return;

}

void compile(const char **src, command_t *command){
    lexer_t lexer;
    parser_t parser;
    token_array_t token_array;

    token_array_init(&token_array);
    lexer_init(&lexer, src);

    lex_all(&lexer, &token_array);
    parser_init(&parser, token_array.data);
    parser_program(&parser, command);


    clean:
        token_array_free(&token_array);
        parser_free(&parser);
        lexer_free(&lexer);
        
}