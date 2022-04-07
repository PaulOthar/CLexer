#ifndef EasyStringHeader

#include <stdlib.h>
#include <string.h>

char* newEmptyString(int size){
	return (char*)malloc(size*sizeof(char));
}

char* newString(char *Reference){
	int size = strlen(Reference);
	char *Output = (char*)malloc((size+1)*sizeof(char));
	
	strcpy(Output,Reference);
	
	return Output;
}

void freeString(char *Reference){
	free(Reference);
}

char* newLimitedCopy(char *Reference,int Limit){
	char *Output = (char*)malloc((Limit+1) * sizeof(char));
	for(int i = 0;i<Limit;i++){
		*(Output+i) = Reference[i];
	}
	
	*(Output+Limit) = '\0';
	
	return Output;
}

int costString(char *Reference){
	int PointerSize = sizeof(Reference);
	int CharArraySize = strlen(Reference)*sizeof(char);
	
	return PointerSize + CharArraySize;
}

int compareStrings(char *String1,char *String2){
	if(strlen(String1) == strlen(String2)){
		for(int i = 0;i<strlen(String1);i++){
			if(String1[i] != String2[i]){
				return 0;
			}
		}
		return 1;
	}
	else{
		return 0;
	}
}

//String Array Stuff

typedef struct /*String Array*/{
	char **PointerArray;	//Array Of String Pointers
	int size;				//Integer
	int *used;				//Array of Integers
}StringArray;

StringArray* newStringArray(int Size){
	StringArray *Output = (StringArray*)malloc(sizeof(StringArray));
	
	Output->size = Size;
	Output->PointerArray = (char**)malloc(Size * sizeof(char*));
	Output->used = (int*)malloc(Size * sizeof(int));
	
	for(int i = 0;i<Size;i++){
		*(Output->used+i) = 0;
	}
	
	return Output;
}

void freeStringArray(StringArray *StringArrayAdress){
	char *Current_String;
	int Current_Is_Used;
	
	for(int i = 0;i<StringArrayAdress->size;i++){
		Current_Is_Used = *(StringArrayAdress->used+i);
		if(Current_Is_Used){
			//if the index is being used, do:
			
			Current_String = *(StringArrayAdress->PointerArray+i);
			freeString(Current_String);
		}
	}
	
	free(StringArrayAdress->PointerArray);
	free(StringArrayAdress->used);
	free(StringArrayAdress);
}


#define EasyStringHeader
#endif
