#ifndef LEXIC_ANALYSER
#define LEXIC_ANALYSER

#include <stdlib.h>

#define ERR_NULL_POINTER -1
#define ERR_NO_MEMORY -2
#define ERR_INVALID_PARAM -3

#define MODE_TOKEN_ADDRESS 0
#define MODE_CHAR_TOKEN_ID 1
#define MODE_SHORT_TOKEN_ID 2
#define MODE_INT_TOKEN_ID 3

typedef struct /*lexic_token*/{
    char* value;//lenght defined by codex
    char** details;//array lenght and word lenght defined by codex
    
    int id;
    char flags;
}lexic_token;

typedef struct _lexic_branch{
    char content;
    lexic_token* endpoint;

    struct _lexic_branch** children;
    int children_length;

    char flags;
}lexic_branch;

typedef struct /*lexic_codex*/{
    int token_value_length;
    int token_details_length;
    int* token_details_value_length;

    lexic_token** tokens;
    int tokens_length;

    lexic_branch* root;

    char flags;
}lexic_codex;

typedef struct /*lexic_result*/{
    void* tokens;//could be an array of indices of many sizes or token references
    int tokens_length;

    char flags;
}lexic_result;

/*Token related
    lexic_token_build
        Allocates the attributes for the specified token address

    lexic_token_free_content
        Frees allocated attributes from the specified token address
*/

int lexic_token_build(lexic_token* token,int value_length,int details_length,int* details_value_length);

int lexic_token_free_content(lexic_token* token,int details_length);

/*Branch related
*/

int lexic_branch_init(lexic_branch* branch,char content,lexic_token* endpoint,char flags);

int lexic_branch_free_content(lexic_branch* branch);

int lexic_branch_append_child(lexic_branch* parent,lexic_branch* child);

int lexic_branch_append_new_child(lexic_branch* parent,char content,lexic_token* endpoint,char flags);

lexic_branch* lexic_branch_find_child(lexic_branch* parent,char content);

/*Codex related
*/

int lexic_codex_init(lexic_codex* codex,int value_length,int details_length,int* details_value_length,char flags);

int lexic_codex_append_token(lexic_codex* codex,lexic_token* token);

int lexic_codex_append_new_token(lexic_codex* codex,char* value,char** details);

int lexic_codex_free_content(lexic_codex* codex);

/*Result related
*/

int lexic_result_init(lexic_result* result,char flags);

int lexic_result_append_token(lexic_result* result,lexic_token* token);

int lexic_result_free_contents(lexic_result* result);

//Lexic analisys related

int lexical_analysis(lexic_codex* codex,lexic_result* result,char* sample,int sample_length,char mode);

#endif