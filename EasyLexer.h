#ifndef EasyLexerHeader

#include <stdlib.h>
#include <string.h>

#include "EasyString.h"
#include "EasyToken.h"

int charIsNumber(char *Input,char *Numbers){
	for(int i = 0;i<strlen(Numbers);i++){
		if(*(Numbers+i) == *Input){
			return 1;
		}
	}
	return 0;
}

void filterTokensByChar(TokenArray *Filtered,char *Character,int Position){
	Token *Current_Token;
	for(int i = 0;i<Filtered->size;i++){
		if(*(Filtered->used+i)){//if the current slot is being used,do:
			Current_Token = *(Filtered->PointerArray+i);
			if(!(strlen(Current_Token->value) < Position)){//if the position is not beyond the char size,do:
				if(!(*(Current_Token->value+Position) == *Character)){//if the current char on the current position is not the char,do:
					*(Filtered->used+i) = 0;//Forget this Token
				}
			}
			else{
				*(Filtered->used+i) = 0;//Forget this Token
			}
		}
	}
}

Token* getTokenByStream(TokenArray *Tokens,char *Stream,int Size){
	Token *Current_Token;
	for(int i = 0;i<Tokens->size;i++){
		if(*(Tokens->used+i)){//if the current slot is being used,do:
			Current_Token = *(Tokens->PointerArray+i);
			if((strlen(Current_Token->value) == Size)){//if the value has the same size,do:
				int Found = 1;
				for(int l = 0;l<Size;l++){
					if(*(Current_Token->value+l) != *(Stream+l)){
						
						Found = 0;
						break;
					}
				}
				if(Found){
					return Current_Token;
				}
			}
		}
	}
	return Current_Token;
}

Token* makeTokenWithStream(char *Tag,char *Stream,int Size){
	
	char *ValueOutput = newLimitedCopy(Stream,Size);
	
	Token *Output = newToken(Tag,ValueOutput,0);
	
	return Output;
}

TokenArray* LexicalAnalisys(char *Input,TokenArray *Tokens,char *NumberTag,char *Numbers,char *UnknownTag){
	TokenArray *Output = newTokenArray(1);
	
	int Length = strlen(Input);
	
	char Unknown_Stream[Length];
	char Current_Stream[Length];
	char Previous_Stream[Length];
	
	for(int i = 0;i<Length;i++){
		Unknown_Stream[i] = '#';
		Current_Stream[i] = '#';
		Previous_Stream[i] = '#';
	}
	
	int Unknown_Size = 0;
	int Current_Size = 0;
	int Previous_Size = 0;
	
	int isNumber = 0;
	int isKnown = 0;
	
	int prevNumber = 0;
	int prevKnown = 0;
	
	TokenArray *FilteredKnown = newTokenArray(1);
	int foundKnown = 0;
	
	char SpaceChar = ' ';
	
	char *Current_Char;
	int Current_CharIsNumber = 0;
	
	for(int i = 0;i<strlen(Input);i++){
		Current_Char = (Input+i);
		
		//Checks if the current char is a Number
		Current_CharIsNumber = charIsNumber(Current_Char,Numbers);
		
		//Increments The Current Stream
		Current_Stream[Current_Size] = *Current_Char;
		Current_Size++;
		
		if(Current_Size == 1){//if this is a new stream,Do:
			//Reset the FilteredKnown Tokens to the base value
			copyTokenArray(FilteredKnown,Tokens);
			
			if(Current_CharIsNumber){//If the current Char is a number,then this stream is also a number
				isNumber = 1;
			}
			else{
				filterTokensByChar(FilteredKnown,Current_Char,Current_Size-1);
				foundKnown = getUsedSizeInTokenArray(FilteredKnown);
				if(foundKnown > 0){
					isKnown = 1;
				}
				else{
					if(*Current_Char != SpaceChar){
						Unknown_Stream[Unknown_Size] = *Current_Char;
						Unknown_Size++;
					}
					if(!(1+i == Length)){
						Current_Size = 0;
						continue;
					}
				}
			}
			if(Unknown_Size > 0){//if there is something in the Unknown Stream, Do:
				pushTokenInArray(Output,makeTokenWithStream(UnknownTag,Unknown_Stream,Unknown_Size));
				Unknown_Size = 0;
			}
		}
		else{//if the current stream is filled with something,do:
			//Detail: To Reach this point, it must be something other than Unknown
			if(!isNumber || !Current_CharIsNumber){//if it is not a number or the current char is not a number
				isNumber = 0;
				if(isKnown){
					filterTokensByChar(FilteredKnown,Current_Char,Current_Size-1);
					foundKnown = getUsedSizeInTokenArray(FilteredKnown);
				
					if(foundKnown == 0){
						isKnown = 0;
					}
				}
				else{
				}
			}
		}
		
		if(!isKnown && !isNumber && (prevKnown || prevNumber)){//if it is nothing now, but used to be,do:
			if(prevNumber){
				pushTokenInArray(Output,makeTokenWithStream(NumberTag,Previous_Stream,Previous_Size));
			}
			else if(prevKnown){
				pushTokenInArray(Output,getTokenByStream(Tokens,Previous_Stream,Previous_Size));
			}
			
			Current_Size = 0;
			i = i-1;
		}
		
		if(i+1 == Length && Current_Size > 0){
			if(isNumber){
				pushTokenInArray(Output,makeTokenWithStream(NumberTag,Current_Stream,Current_Size));
			}
			else if(prevKnown){
				pushTokenInArray(Output,getTokenByStream(Tokens,Current_Stream,Current_Size));
			}
			
		}
		
		prevNumber = isNumber;
		prevKnown = isKnown;
		
			
		for(int l = 0;l<Current_Size;l++){
			Previous_Stream[l] = Current_Stream[l];
		}
		
		Previous_Size = Current_Size;
	}
	
	freeTokenArray(FilteredKnown,0);
	
	return Output;
}

#define EasyLexerHeader
#endif
