#ifndef EasyTokenHeader

#include <stdlib.h>
#include <stdio.h>

#include "EasyString.h"

//Token Unit
typedef struct /*Token*/{
	char *name;			//Pointer To String
	char *value;		//Pointer To String
	int isConstant;		//Integer(Behaving as Boolean)
}Token;

Token* newToken(char *Name,char *Value,int isConstant){
	Token *Output = (Token*)malloc(sizeof(Token));
	
	Output->name = Name;//The Name is Always a Reference
	Output->isConstant = isConstant;
	
	if(isConstant){
		//if this token is a constant, then the value is a Reference
		Output->value = Value;
	}
	else{
		//if this token is not a constant, then the value is a random string that needs to be allocated
		Output->value = newString(Value);
	}
	
	return Output;
}

void freeToken(Token *TokenAdress){
	freeString(TokenAdress->value);
	
	if(TokenAdress->isConstant){
		//if the token is a constant, then it can deallocate the name, since there is only one reference
		freeString(TokenAdress->name);
	}
	
	free(TokenAdress);
}

int costToken(Token *TokenAdress){
	int PointerSize = sizeof(TokenAdress);
	int IntegerSize = sizeof(int);
	int ValueSize = costString(TokenAdress->value);
	int NameSize = 0;
	
	if(TokenAdress->isConstant){
		//if it is a constant, then the name is only allocated for this token
		//therefore, counting as an adition allocation size
		NameSize = costString(TokenAdress->name);
	}
	else{
		//if it is not a constant, then the name is recycled, costing only the pointer
		NameSize = sizeof(TokenAdress->name);
	}
	
	return PointerSize + IntegerSize + ValueSize + NameSize;
}

void printToken(Token *TokenAdress){
	printf("[%s : %s][%d]",TokenAdress->name,TokenAdress->value,TokenAdress->isConstant);
}

//Token Array
typedef struct /*TokenArray*/{
	Token **PointerArray;	//Array of Token Pointers
	int size;				//Integer
	int *used;				//Array of Integers
}TokenArray;

TokenArray* newTokenArray(int Size){
	TokenArray *Output = (TokenArray*)malloc(sizeof(TokenArray));
	
	Output->size = Size;
	Output->PointerArray = (Token**)malloc(Size * sizeof(Token*));
	Output->used = (int*)malloc(Size * sizeof(int));
	
	for(int i = 0;i<Size;i++){
		*(Output->used+i) = 0;
	}
	
	return Output;
}

void freeTokenArray(TokenArray *TokenArrayAdress,int FreeConstants){
	Token *Current_Token;
	int Current_Is_Used;
	
	int FreeConstantAllowed = 0;
	int CurrentIsNotAConstant = 0;
	
	for(int i = 0;i<TokenArrayAdress->size;i++){
		Current_Is_Used = *(TokenArrayAdress->used+i);
		if(Current_Is_Used){
			//if the index is being used, do:
			
			Current_Token = *(TokenArrayAdress->PointerArray+i);
			CurrentIsNotAConstant = !Current_Token->isConstant;
			FreeConstantAllowed = FreeConstants && Current_Token->isConstant;
			
			if(FreeConstantAllowed || CurrentIsNotAConstant){
				//if the current token is not a constant, Or
				//the current token is a constant, but it is allowed to free, do:
				
				freeToken(Current_Token);
			}
		}
	}
	
	free(TokenArrayAdress->PointerArray);
	free(TokenArrayAdress->used);
	free(TokenArrayAdress);
}

int costTokenArray(TokenArray *TokenArrayAdress,int IncludeConstants){
	int PointerSize = sizeof(TokenArrayAdress);
	int IntegerSize = sizeof(int);
	int IntegerPointerSize = sizeof(TokenArrayAdress->used);
	int IntegerPointedSize = TokenArrayAdress->size * sizeof(int);
	int ActualTokenArrayPointerSize = sizeof(TokenArrayAdress->PointerArray);
	int TokensSize = 0;
	
	Token *Current_Token;
	int Current_Is_Used;
	
	int IncludeConstantAllowed = 0;
	int CurrentIsNotAConstant = 0;
	
	for(int i = 0;i<TokenArrayAdress->size;i++){
		Current_Is_Used = *(TokenArrayAdress->used+i);
		if(Current_Is_Used){
			//if the index is being used, do:
			
			Current_Token = *(TokenArrayAdress->PointerArray+i);
			CurrentIsNotAConstant = !Current_Token->isConstant;
			IncludeConstantAllowed = IncludeConstants && Current_Token->isConstant;
			
			if(IncludeConstantAllowed || CurrentIsNotAConstant){
				//if the current token is not a constant, Or
				//the current token is a constant, but it is allowed to Include, do:
				
				TokensSize += costToken(Current_Token);
			}
		}
	}
	
	return PointerSize + IntegerSize + IntegerPointerSize + IntegerPointedSize + ActualTokenArrayPointerSize + TokensSize;
}

void printTokenArray(TokenArray *TokenArrayAdress){
	Token *Current_Token;
	int Current_Is_Used;
	for(int i = 0;i<TokenArrayAdress->size;i++){
		if(i>0){
			printf("\n");
		}
		Current_Is_Used = *(TokenArrayAdress->used+i);
		printf("[%d] -> ",i);
		if(Current_Is_Used){
			//if the index is being used, do:
			
			Current_Token = *(TokenArrayAdress->PointerArray+i);
			printToken(Current_Token);
		}
		else{
			printf("Empty");
		}
	}
}

void setTokenInArray(TokenArray *TokenArrayAdress,Token *TokenAdress,int Index){
	if(Index < TokenArrayAdress->size && Index >= 0){
		//if the current index is an actual valid index, do:
		
		*(TokenArrayAdress->PointerArray+Index) = TokenAdress;
		*(TokenArrayAdress->used+Index) = 1;
	}
}

Token* getTokenInArray(TokenArray *TokenArrayAdress,int Index){
	if(Index < TokenArrayAdress->size && Index >= 0 && *(TokenArrayAdress->used+Index) == 1){
		//if the current index is an actual valid index, and the index is being used, do:
		return *(TokenArrayAdress->PointerArray+Index);
	}
	return NULL;
}

void forgetTokenInArray(TokenArray *TokenArrayAdress,int Index){
	if(Index < TokenArrayAdress->size && Index >= 0 && *(TokenArrayAdress->used+Index) == 1){
		//if the current index is an actual valid index, and the index is being used, do:
		*(TokenArrayAdress->used+Index) = 0;
	}
}

void organizeTokensInArray(TokenArray *TokenArrayAdress){
	int Found = 0;
	for(int i = 0;i<TokenArrayAdress->size;i++){
		Found = 0;
		if(!*(TokenArrayAdress->used+i)){
			//if the current space is not being used,Do:
			for(int l = i;l<TokenArrayAdress->size;l++){
				if(*(TokenArrayAdress->used+l)){
					//if the current space is being used,Do:
					*(TokenArrayAdress->used+l) = 0;
					*(TokenArrayAdress->used+i) = 1;
					*(TokenArrayAdress->PointerArray+i) = *(TokenArrayAdress->PointerArray+l);
					*(TokenArrayAdress->PointerArray+l) = NULL;
					Found = 1;
					break;
				}
			}
		}
		else{
			//if the current space is being used,Do:
			Found = 1;
		}
		
		if(!Found){
			//if no token was found this iteration, then quit, the job is done
			break; 
		}
	}
}

void modifyTokenArraySize(TokenArray *TokenArrayAdress,int NewSize){
	TokenArrayAdress->PointerArray = (Token**)realloc(TokenArrayAdress->PointerArray,(NewSize) * sizeof(Token*));
	TokenArrayAdress->used = (int*)realloc(TokenArrayAdress->used,(NewSize) * sizeof(int));
	
	if(TokenArrayAdress->size < NewSize){
		for(int i = TokenArrayAdress->size;i<NewSize;i++){
			*(TokenArrayAdress->used+i) = 0;
		}
	}
	
	TokenArrayAdress->size = NewSize;
}

void pushTokenInArray(TokenArray *TokenArrayAdress,Token *TokenAdress){
	modifyTokenArraySize(TokenArrayAdress,TokenArrayAdress->size+1);
	setTokenInArray(TokenArrayAdress,TokenAdress,TokenArrayAdress->size-1);
}

void copyTokenArray(TokenArray *Reciver,TokenArray *Reference){
	if(Reciver->size != Reference->size){
		modifyTokenArraySize(Reciver,Reference->size);
	}
	for(int i = 0;i<Reference->size;i++){
		if(*(Reference->used+i)){//if the slot is being used,do:
			setTokenInArray(Reciver,getTokenInArray(Reference,i),i);
		}
		else{
			*(Reciver->used+i) = 0;//forget the stored pointer
		}
	}
}

int getUsedSizeInTokenArray(TokenArray *TokenArrayAdress){
	int Output = 0;
	
	for(int i = 0;i<TokenArrayAdress->size;i++){
		Output += *(TokenArrayAdress->used+i);
	}
	
	return Output;
}

#define EasyTokenHeader
#endif

/*
 * A Token is a Mere Representation of a Structured Idea, such as 
 * Addition, Subtraction, Comparison and etc...
 * 
 * A Single Token Can Be Outputed Multiple Times When a Lexer is 
 * Finished, so there is no need whatsoever to use anything but 
 * References by pointers
 * 
 * Token Structure:
 * Name (String Reference)
 * Value (String Reference)
 * isConstant(Boolean Value)
 * 
 * The Name Will Always Be a Reference To an Existing String
 * And Since there is a possibility of existance of multiple tokens with
 * the same name, we have the "isConstant" variable, to define if this
 * is a Token with a Recycled name or not.
 * 
 * This Logic exists so there is no need to allocate the same name
 * multiple times, and in case of making a reference, not freeing the 
 * same string multiple times.
 * (I dont know if it yields any risk, and im not willing to find out)
 * 
 * Example of Tokens With Same Name:
 * [NUMBER:300]
 * [NUMBER:900.5]
 * [NUMBER:150]
 * 
 * If There was an single allocation for every token, then every token
 * would have an additional 6 Bytes of Cost (1 Byte per Char).
 * 
 * And if these Tokens needed to be deallocated, with the Name being a reference
 * it would have at least two Unnecessary Dealocation Cycles
*/

/*
 * A Token Codex is a Collection of Token Arrays, that are used to Hold
 * the only kind of token you want to read from a lexer
 * 
 * For Example:
 * You have the following Arrays:
 * 
 * Basic Arithmetic:
 * [PLUS:+]
 * [MINUS:-]
 * [MULTIPLY:*]
 * [DIVIDE:/]
 * [MODULUS:%]
 * [POWER:**]
 * 
 * Trigonometry Arithmetic:
 * [SINE:sin]
 * [COSSINE:cos]
 * [TANGENT:tan]
 * [ARCSINE:arcsin]
 * [ARCCOSSINE:arccos]
 * [ARCTANGENT:arctan]
 * [PI:3.14]
 * 
 * Boolean Arithmetic:
 * [TRUE:1]
 * [FALSE:0]
 * [EQUAL:=]
 * [DIFFERENT:!=]
 * [LOWER:<]
 * [HIGHTER:>]
 * [EQUAL_LOWER:<=]
 * [EQUAL_HIGHTER:>=]
 * [AND:&&]
 * [OR:||]
 * [NOT:!]
 * 
 * But the Following input will only need Basic Arithimetic Then you 
 * would create a simple Codex with only a reference to the Basic 
 * Arithimetic Array, just to not confuse some variables with Trigonometry terms
 * Or to not identify other therms, to create your very own Tokens, based on the
 * unrecognized inputs
*/
