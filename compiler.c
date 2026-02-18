#include "compiler.h"
#include "lex.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>


char *help_msg = "Help Dish" \
    "./dish command [options]";

void parser_set_error(parser_t *parser, size_t token_index, const char *fmt, ...){
    va_list ap;
    va_start(ap, fmt);

    char buf[256];
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);

    parser->err.msg = strdup(buf);
    parser->err.has_error = true;
    parser->err.token_index = token_index;
}

static token_t parser_peek(parser_t *parser){
    return parser->tokens[parser->current];
}

static bool is_at_end(parser_t *parser){
    return (parser_peek(parser).type == TOKEN_EOF);
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

static bool consume(parser_t *parser, token_type_t tt, token_t *out_token){
    if(parser_peek(parser).type == tt){
        *out_token = parser_advance(parser);
        return true;
    }
    return false;
}


bool parser_match(parser_t *parser, token_type_t *tts, size_t len, bool consume_on_match){
    token_t curr = parser_peek(parser);
    for(int i=0; i < (int)len; i++){
        if (tts[i] == curr.type){
            if (consume_on_match) parser_advance(parser);
            return true;
        } 
    }
    return false;
}

void parser_init(parser_t *parser, token_t *tokens, size_t token_len){
    parser->current = 0;
    parser->start = 0;
    parser->tokens = tokens;
    parser->token_len = token_len;
}

void parser_free(parser_t *parser){
    if(!parser) return;

    for(int i = 0; i < (int)parser->token_len; i++){
        token_free(&parser->tokens[0]);
    }
    free(parser->tokens);
    parser_init(parser, NULL, 0);
}

/*
* Parse the command name of command and return true. Return false 
* if otherwise. When an error occurs, return false and set parser->err.has_error
* to true. 
*/
bool parser_command_name(parser_t *parser, char **name){
    token_type_t tts[] = {TOKEN_WORD};
    
    if (parser_match(parser, tts, 1, false)){
        token_t tok;
        if(!consume(parser, TOKEN_WORD, &tok)){
            // token_t curr = parser_peek(parser);
            goto set_error;
        }

        *name = tokentostr(&tok);
        return true;
    }

    return false;

    set_error:
        token_t curr = parser_peek(parser);
        printf("error at parser_command_name: %s \n", tokentostr(&curr));
        parser_set_error(parser, 0, help_msg);
        return false;
}

/*
* Parse the redirect in file and return true if found else false if 
* otherwise. When an error occurs it returns false and also sets 
* parser->err.has_error to true. 
*/
bool parser_in_file(parser_t *parser, char **infile){
    token_type_t tts[] = {TOKEN_LT};

    if (parser_match(parser, tts, 1, true)){
        token_t tok;
        if(!consume(parser, TOKEN_WORD, &tok)){
            goto set_error;
        }
        *infile = tokentostr(&tok);
        return true;
    }

    return false;

    set_error:
        token_t curr = parser_peek(parser);
        printf("error at parser_in_file: %s \n", tokentostr(&curr));
        parser_set_error(parser, 0, help_msg);
        return false;
}

/*
* Parse the redirect out file and return true if found else false if 
* otherwise. When an error occurs it returns false and also sets 
* parser->err.has_error to true. 
*/
bool parser_outfile(parser_t *parser, char **outfile) {
    token_type_t tts[] = {TOKEN_GT};

    if (parser_match(parser, tts, 1, true)){
        token_t tok;
        if(!consume(parser, TOKEN_WORD, &tok)){
            goto set_error;
        }
        *outfile = tokentostr(&tok);
        return true;
    }
    return false;
    
    set_error:
        token_t curr = parser_peek(parser);
        printf("error at parser_out_file: %s \n", tokentostr(&curr));
        parser_set_error(parser, 0, help_msg);
        return false;
}


/*
* Parse the redirect error file and return true if found else false if 
* otherwise. When an error occurs it returns false and also sets 
* parser->err.has_error to true. 
*/
bool parser_err_file(parser_t *parser, char **err_file){
    token_type_t tts[] = {TOKEN_2_GT};

    if (parser_match(parser, tts, 1, true)){
        token_t tok;
        if(!consume(parser, TOKEN_WORD, &tok)){
            goto set_error;
        }
        *err_file = tokentostr(&tok);
        return true;
    }
    return false;
    
    set_error:
        token_t curr = parser_peek(parser);
        printf("error at parser_out_file: %s\n", tokentostr(&curr));
        parser_set_error(parser, 0, help_msg);
        return false;
}

bool parser_args(parser_t *parser, simple_command_t *sc){
    token_type_t tts[] = {TOKEN_PIPE, TOKEN_AMP, TOKEN_EOF};
    while(!parser_match(parser, tts, 1, true)){
        token_t tok;
        if(!consume(parser, TOKEN_WORD, &tok)){
            goto set_error;
        }
        insert_argument(sc, tokentostr(&tok));
        return true;
    }

    return false;

    set_error:
        token_t curr = parser_peek(parser);
        printf("error at parser_args: %s\n", tokentostr(&curr));
        parser_set_error(parser, 0, help_msg);
        return false;
}
/*
* Extract individual commands (before the pipe) along with their redir
* files if present. Return true if successful and false if any error occured.
*/
bool parser_pipeline(parser_t *parser, simple_command_t *sc) {

    char *cmd_name = NULL;
    if (!parser_command_name(parser, &cmd_name)) return false;
    insert_argument(sc, cmd_name);

    if(!parser_in_file(parser, &sc->redir.in_file) && parser->err.has_error) return false;
    if(!parser_outfile(parser, &sc->redir.out_file) && parser->err.has_error) return false;
    if(!parser_err_file(parser, &sc->redir.err_file) && parser->err.has_error) return false;
    
    if(!parser_args(parser, sc) && parser->err.has_error) return false;

    return true;
}


bool parser_program(parser_t *parser, command_t *command){
    token_type_t tts [] = {TOKEN_EOF, TOKEN_AMP};

    while(!parser_match(parser, tts, 2, true)){
        simple_command_t sc;
        simple_command_init(&sc);

        if (!parser_pipeline(parser, &sc)) return false;
        insert_simple_command(command, &sc);
    }

    // try to set the background flag if set.
    token_type_t bg_tts [] = {TOKEN_AMP};
    if (parser_match(parser,bg_tts, 1, true)){
        command->background = true;
        parser_advance(parser);
    }
    return true;

}

void print_command(command_t *command){
    for(int i=0; i < (int)command->pipeline_len; i++){
        simple_command_t sc = command->pipeline[i];
        
        for(int j=0; j< (int)sc.argc; j++){
            char *arg = sc.argv[j];
            printf("%s ", arg);
        }
        printf(" | ");
    }
    printf("\n");
}

/*
* run lexer and parse tokens into commands. command will be 
* freed if program fails to compile(i.e if parser_program returns false).
*/
bool compile(const char *src, command_t *command){
    lexer_t lexer;
    parser_t parser;
    token_array_t token_array;

    token_array_init(&token_array);
    lexer_init(&lexer, src);

    lex_all(&lexer, &token_array);
    parser_init(&parser, token_array.data, token_array.len);
    
    if(!parser_program(&parser, command)){
        printf("%s\n", parser.err.msg);
        goto cleanup;
    }

    print_command(command);

    token_array_free(&token_array);
    lexer_free(&lexer);
    parser_free(&parser);
    return true;

    cleanup:
        command_free(command);
        parser_free(&parser);
        token_array_free(&token_array);
        lexer_free(&lexer);
        return false;
}

// cat < in.txt | echo "hey there"