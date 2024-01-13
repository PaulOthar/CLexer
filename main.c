#include "lexic_analyser.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc,char** argv){
    lexic_codex codex;
    lexic_result result;

    lexic_codex_init(&codex,16,0,NULL,0);

    lexic_codex_append_new_token(&codex,"Abacate",NULL);
    lexic_codex_append_new_token(&codex,"Abobora",NULL);
    lexic_codex_append_new_token(&codex,"Abestada",NULL);
    lexic_codex_append_new_token(&codex,"Acerebrado",NULL);
    lexic_codex_append_new_token(&codex,"Acelora",NULL);
    lexic_codex_append_new_token(&codex,"Opora",NULL);
    
    lexical_analysis(&codex,&result,"Opora Acel Acelora karay Abestada AaA",38,MODE_TOKEN_ADDRESS);

    int i = 0;

    lexic_token** tok = result.tokens;
    
    for(i = 0;i<result.tokens_length;i++){
        printf("%s\n",tok[i]->value);
    }

    lexic_result_free_contents(&result);
    lexic_codex_free_content(&codex);
    return 0;
}