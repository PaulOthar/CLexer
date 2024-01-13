#include "lexic_analyser.h"

#include <stdio.h>

//Token related

int lexic_token_build(lexic_token* token,int value_length,int details_length,int* details_value_length){
    int i = 0;

    if(token == NULL){
        return ERR_NULL_POINTER;
    }
    
    token->value = (char*)malloc(sizeof(char)*(value_length));
    token->id = 0;

    if(token->value == NULL){
        return ERR_NO_MEMORY;
    }

    if(details_length > 0){
        token->details = (char**)malloc(sizeof(char*)*details_length);
        if(token->details == NULL){
            return ERR_NO_MEMORY;
        }

        for(i = 0;i<details_length;i++){
            token->details[i] = (char*)malloc(sizeof(char)*details_value_length[i]);
            if(token->details[i]){
                return ERR_NO_MEMORY;
            }
        }
    }
    return 0;
}

int lexic_token_free_content(lexic_token* token,int details_length){
    if(!token){
        return ERR_NULL_POINTER;
    }
    if(details_length < 0){
        return ERR_INVALID_PARAM;
    }

    if(token->value){
        free(token->value);
    }

    if(token->details){
        int i = 0;
        for(i = 0;i<details_length;i++){
            free(token->details[i]);
        }
        free(token->details);
    }
}

//Branch related

int lexic_branch_init(lexic_branch* branch,char content,lexic_token* endpoint,char flags){
    if(!branch){
        return ERR_NULL_POINTER;
    }

    branch->content = content;
    branch->endpoint = endpoint;

    branch->children = NULL;
    branch->children_length = 0;

    branch->flags = flags;

    return 0;
}

int lexic_branch_free_content(lexic_branch* branch){
    if(!branch){
        return ERR_NULL_POINTER;
    }
    
    int i = 0;
    if(branch->children){
        for(i = 0;i<branch->children_length;i++){
            lexic_branch_free_content(branch->children[i]);
            free(branch->children[i]);
        }
        free(branch->children);
    }
    return 0;
}

int lexic_branch_append_child(lexic_branch* parent,lexic_branch* child){
    if(!parent || !child){
        return ERR_NULL_POINTER;
    }

    lexic_branch** children = (lexic_branch**)realloc(parent->children,sizeof(lexic_branch*)*(parent->children_length+1));

    if(!children){
        return ERR_NO_MEMORY;
    }

    children[parent->children_length] = child;

    parent->children = children;
    parent->children_length++;
    return 0;
}

int lexic_branch_append_new_child(lexic_branch* parent,char content,lexic_token* endpoint,char flags){
    if(!parent){
        return ERR_NULL_POINTER;
    }

    lexic_branch* child = (lexic_branch*)malloc(sizeof(lexic_branch));

    if(!child){
        return ERR_NO_MEMORY;
    }

    int res = lexic_branch_init(child,content,endpoint,flags);

    if(res != 0){
        lexic_branch_free_content(child);
        free(child);
        return res;
    }

    res = lexic_branch_append_child(parent,child);

    if(res != 0){
        lexic_branch_free_content(child);
        free(child);
        return res;
    }

    return 0;
}

lexic_branch* lexic_branch_find_child(lexic_branch* parent,char content){
    if(!parent){
        return NULL;
    }

    int i = 0;
    for(i = 0;i<parent->children_length;i++){
        if(parent->children[i]->content == content){
            return parent->children[i];
        }
    }

    return NULL;
}

//Codex related

int lexic_codex_init(lexic_codex* codex,int value_length,int details_length,int* details_value_length,char flags){
    if(!codex){
        return ERR_NULL_POINTER;
    }

    int i = 0;
    codex->token_value_length = value_length;
    codex->token_details_length = details_length;

    if(details_length && !details_value_length){
        return ERR_NULL_POINTER;
    }

    codex->token_details_value_length = (int*)malloc(sizeof(int)*details_length);
    if(!codex->token_details_value_length){
        return ERR_NO_MEMORY;
    }

    for(i = 0;i<details_length;i++){
        codex->token_details_value_length[i] = details_value_length[i];
    }

    codex->tokens = NULL;
    codex->tokens_length = 0;

    codex->flags = 0;

    codex->root = (lexic_branch*)malloc(sizeof(lexic_branch));
    
    if(!codex->root){
        return ERR_NO_MEMORY;
    }

    return lexic_branch_init(codex->root,'.',NULL,0);
}

int lexic_codex_append_token(lexic_codex* codex,lexic_token* token){
    if(!codex || !token){
        return ERR_NULL_POINTER;
    }

    lexic_token** tokens = (lexic_token**)realloc(codex->tokens,sizeof(lexic_token*)*(codex->tokens_length+1));

    if(!tokens){
        return ERR_NO_MEMORY;
    }

    codex->tokens = tokens;
    codex->tokens_length++;

    int i = 0;
    lexic_branch* curr_branch = codex->root;
    lexic_branch* prev_branch = curr_branch;

    int res = 0;
    for(i = 0;i<codex->token_value_length;i++){
        if(token->value[i] == '\0'){
            break;
        }
        curr_branch = lexic_branch_find_child(prev_branch,token->value[i]);
        
        if(!curr_branch){
            res = lexic_branch_append_new_child(prev_branch,token->value[i],NULL,0);
            if(res != 0){
                return res;
            }
            curr_branch = prev_branch->children[prev_branch->children_length-1];
        }
        
        prev_branch = curr_branch;
    }
    prev_branch->endpoint = token;
    return 0;
}

int lexic_codex_append_new_token(lexic_codex* codex,char* value,char** details){
    if(!codex || !value){
        return ERR_NULL_POINTER;
    }

    lexic_token* token = (lexic_token*)malloc(sizeof(lexic_token));

    if(!token){
        return ERR_NO_MEMORY;
    }
    
    int res = lexic_token_build(token,codex->token_value_length,codex->token_details_length,codex->token_details_value_length);
    
    if(res != 0){
        lexic_token_free_content(token,codex->token_details_length);
        free(token);
        return res;
    }

    int i = 0;
    for(i = 0;i<codex->token_value_length;i++){
        token->value[i] = value[i];
    }

    int l = 0;
    for(i = 0;i<codex->token_details_length;i++){
        for(l = 0;l<codex->token_details_value_length[l];l++){
            token->details[i][l] = details[i][l];
        }
    }

    token->id = codex->tokens_length;
    res = lexic_codex_append_token(codex,token);

    if(res != 0){
        lexic_token_free_content(token,codex->token_details_length);
        free(token);
        return res;
    }

    return 0;
}

int lexic_codex_free_content(lexic_codex* codex){
    if(!codex){
        return ERR_NULL_POINTER;
    }

    if(codex->token_details_value_length){
        free(codex->token_details_value_length);
    }

    if(codex->tokens){
        int i = 0;
        for(i = 0;i<codex->tokens_length;i++){
            lexic_token_free_content(codex->tokens[i],codex->token_details_length);
            free(codex->tokens[i]);
        }
        free(codex->tokens);
    }

    if(codex->root){
        lexic_branch_free_content(codex->root);
        free(codex->root);
    }
}

//Result related

int lexic_result_init(lexic_result* result,char flags){
    if(!result){
        return ERR_NULL_POINTER;
    }
    result->tokens = NULL;
    result->tokens_length = 0;
    result->flags = flags;
}

int lexic_result_append_token(lexic_result* result,lexic_token* token){
    if(!result || !token){
        return ERR_NULL_POINTER;
    }

    void* reall = NULL;

    switch(result->flags){
        case 0:reall = (void*)realloc(result->tokens,sizeof(lexic_token*)*(result->tokens_length+1));
            break;
        case 1:reall = (void*)realloc(result->tokens,sizeof(char)*(result->tokens_length+1));
            break;
        case 2:reall = (void*)realloc(result->tokens,sizeof(short)*(result->tokens_length+1));
            break;
        case 3:reall = (void*)realloc(result->tokens,sizeof(int)*(result->tokens_length+1));
            break;
    }

    if(!reall){
        return ERR_NO_MEMORY;
    }

    switch(result->flags){
        case 0:((lexic_token**)reall)[result->tokens_length] = token;
            break;
        case 1:((char*)reall)[result->tokens_length] = token->id;
            break;
        case 2:((short*)reall)[result->tokens_length] = token->id;
            break;
        case 3:((int*)reall)[result->tokens_length] = token->id;
            break;
    }
    result->tokens = reall;
    result->tokens_length++;
    return 0;
}

int lexic_result_free_contents(lexic_result* result){
    if(!result){
        return ERR_NULL_POINTER;
    }

    if(result->tokens){
        free(result->tokens);
    }
}

//Lexical analisys related

int lexical_analysis(lexic_codex* codex,lexic_result* result,char* sample,int sample_length,char mode){
    if(!codex || !result || !sample){
        return ERR_NULL_POINTER;
    }
    if(sample_length <= 0){
        return ERR_INVALID_PARAM;
    }

    lexic_result_free_contents(result);

    lexic_result_init(result,mode);
    
    lexic_branch* root = codex->root;//convenience
    char c = root->content;
    lexic_branch* curr_branch = root;
    lexic_branch* prev_branch = root;

    lexic_token* known_token = NULL;

    int i = 0;
    for(i = 0;i<sample_length;i++){
        c = sample[i];

        curr_branch = lexic_branch_find_child(prev_branch,c);
        
        if(!curr_branch){//Invalid token
            if(prev_branch == root){//Unknown single char token
                curr_branch = root;
                continue;
            }
            //Used to be a known token
            if(prev_branch->endpoint){//Used to be a endpoint
                known_token = prev_branch->endpoint;
                lexic_result_append_token(result,known_token);
                known_token = NULL;
            }
            //Is either a known or not
            curr_branch = root;
            prev_branch = root;

            i--;//Step backwards, so this current invalid char becomes the head
        }
        
        prev_branch = curr_branch;
    }

    if(prev_branch->endpoint){//last possible token in the string
        known_token = prev_branch->endpoint;
        lexic_result_append_token(result,known_token);
    }

    return 0;
}

//Print related

void lexic_token_print(lexic_token* token){
    printf("[%d] = '%s'\n",token->id,token->value);
}

void lexic_branch_print(lexic_branch* branch){
    printf("'%c' |- %d E:%X\n",branch->content,branch->children_length,branch->endpoint);
}