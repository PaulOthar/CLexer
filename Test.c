#include <stdio.h>

#include "EasyString.h"
#include "EasyToken.h"
#include "EasyLexer.h"

int main(){
	TokenArray *Koroi = newTokenArray(1);
	
	setTokenInArray(Koroi,newToken(newString("PLUS"),newString("+"),1),0);
	pushTokenInArray(Koroi,newToken(newString("MINUS"),newString("-"),1));
	pushTokenInArray(Koroi,newToken(newString("DRECREMENT"),newString("--"),1));
	pushTokenInArray(Koroi,newToken(newString("MULTIPLY"),newString("*"),1));
	pushTokenInArray(Koroi,newToken(newString("DIVIDE"),newString("/"),1));
	
	printTokenArray(Koroi);
	
	printf("\n\n");
	
	char *Input = "2+3=5";
	char *UnknownTag = newString("UNKNOWN");
	char *NumberTag = newString("NUMBER");
	char *Numbers = ".0123456789";
	
	TokenArray *Lexed = LexicalAnalisys(Input,Koroi,NumberTag,Numbers,UnknownTag);
	
	printTokenArray(Lexed);
	
	freeTokenArray(Lexed,0);
	freeTokenArray(Koroi,1);
	
	freeString(UnknownTag);
	freeString(NumberTag);
	
	return 0;
}

