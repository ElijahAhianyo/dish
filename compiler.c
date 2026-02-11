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


void parser_init(parser_t *parser, token_t *tokens){
    parser->current = 0;
    parser->start = 0;
    parser->tokens = tokens;
}

void parser_free(parser_t *parser){

}

typedef struct {
    token_t *in_file;
    token_t *out_file;
    token_t *err_file
} std_args_t;

void std_args_init(std_args_t *std_args){
    std_args->err_file = NULL;
    std_args->in_file = NULL;
    std_args->out_file = NULL;
}

void marshall_simple_command(simple_command_t *sc, std_args_t *sa){
    
}

void parser_parse(parser_t *parser, command_t *command){
    while(!is_at_end(parser)){
        simple_command_t sc;
        simple_command_init(&sc);
        std_args_t std_args;
        std_args_init(&std_args);
        
        token_t curr = parser_advance(parser);
        switch (curr.type)
        {
        case TOKEN_LESS:
            {
                token_t t = consume(parser, TOKEN_WORD);
                std_args.in_file = tokentostr(&t);
            }
            break;
        case TOKEN_GREAT:
        {
            token_t t = consume(parser, TOKEN_WORD);
            std_args.out_file = tokentostr(&t);
        }
        break;

        case TOKEN_2_GREAT:
        {
            token_t t = consume(parser, TOKEN_WORD);
            std_args.err_file = tokentostr(&t);
        }
        break;

        case TOKEN_PIPE:
        {
            parser_advance(parser);
            marshall_simple_command(&sc, &std_args);
            break;
        }

        case TOKEN_WORD:
        {
            char *str_arg = tokentostr(&curr);
            insert_argument(&sc, str_arg);
            break;
        }
        
        default:
            break;
        }


        simple_command_free(&sc);
    }
}

void compile(const char **src, command_t *command){
    lexer_t lexer;
    parser_t parser;
    token_array_t token_array;
    command_t command;

    token_array_init(&token_array);
    lexer_init(&lexer, src);

    lex_all(&lexer, &token_array);
    parser_init(&parser, token_array.data);
    parser_parse(&parser, &command);


    clean:
        token_array_free(&token_array);
        parser_free(&parser);
        lexer_free(&lexer);
        
}