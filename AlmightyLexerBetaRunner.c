#include <stdio.h>

#include "EasyString.h"
#include "EasyToken.h"
#include "EasyLexer.h"

#include "EasyInput.h"
#include "EasyGraphics.h"

void Run(char *Input,int Maximum,int *LeftSpace,int *Vertical_Padding,int *Horizontal_Padding,Wall *WallAdress);

void Introduction(Wall *WallAdress);

void Command(char *Input,int Maximum,int *Continue,int *LeftSpace,int *Vertical_Padding,int *Horizontal_Padding,Wall *WallAdress);

void Help(int *LeftSpace,int *Vertical_Padding,int *Horizontal_Padding,Wall *WallAdress);

void Configure(char *Input,int Maximum,int *LeftSpace,int *Vertical_Padding,int *Horizontal_Padding,Wall *WallAdress);

void ConfigureGraphics(char *Input,int Maximum,int *LeftSpace,int *Vertical_Padding,int *Horizontal_Padding,Wall *WallAdress);

int parseInt(char *Input);

int main(){
	Wall *MyWall = basicThinWall();
	char *Input = newEmptyString(100);
	int Continue = 1;
	
	int LeftSpace = 5;
	int Vertical_Padding = 0;
	int Horizontal_Padding = 0;
	
	Introduction(MyWall);
	
	while(Continue){
		Command(Input,100,&Continue,&LeftSpace,&Vertical_Padding,&Horizontal_Padding,MyWall);
	}
	
	freeString(Input);
	freeWall(MyWall);
	return 0;
}

void Run(char *Input,int Maximum,int *LeftSpace,int *Vertical_Padding,int *Horizontal_Padding,Wall *WallAdress){
	TokenArray *Koroi = newTokenArray(1);
	
	setTokenInArray(Koroi,newToken(newString("PLUS"),newString("+"),1),0);
	pushTokenInArray(Koroi,newToken(newString("MINUS"),newString("-"),1));
	pushTokenInArray(Koroi,newToken(newString("MULTIPLY"),newString("*"),1));
	pushTokenInArray(Koroi,newToken(newString("DIVIDE"),newString("/"),1));
	pushTokenInArray(Koroi,newToken(newString("MODULUS"),newString("%"),1));
	pushTokenInArray(Koroi,newToken(newString("POWER"),newString("**"),1));
	pushTokenInArray(Koroi,newToken(newString("INCREMENT"),newString("++"),1));
	pushTokenInArray(Koroi,newToken(newString("DECREMENT"),newString("--"),1));
	
	printBoxedString("Existing Tokens:",*LeftSpace,*Vertical_Padding,*Horizontal_Padding,WallAdress);
	printf("\n");
	printBoxedTokenArray(Koroi,*LeftSpace,*Vertical_Padding,*Horizontal_Padding,WallAdress);
	
	printf("\n\n");
	
	printBoxedString("Insert The String To Be Analized:",*LeftSpace,*Vertical_Padding,*Horizontal_Padding,WallAdress);
	printf("\n\n");
	
	readEnhanced(Input,Maximum);
	
	char *UnknownTag = newString("UNKNOWN");
	char *NumberTag = newString("NUMBER");
	char *Numbers = ".0123456789";
	
	TokenArray *Lexed = LexicalAnalisys(Input,Koroi,NumberTag,Numbers,UnknownTag,1);
	
	printBoxedString("Result:",*LeftSpace,*Vertical_Padding,*Horizontal_Padding,WallAdress);
	printf("\n\n");
	printBoxedTokenArray(Lexed,*LeftSpace,*Vertical_Padding,*Horizontal_Padding,WallAdress);
	printf("\n\n");
	
	freeTokenArray(Lexed,0);
	freeTokenArray(Koroi,1);
	
	freeString(UnknownTag);
	freeString(NumberTag);
}

void Introduction(Wall *WallAdress){
	printBoxedString("Welcome To The Almighty Lexer!",5,0,0,WallAdress);
	printf("\n");
	printBoxedString("Made By Paul (PaulOthar)",5,0,0,WallAdress);
	printf("\n");
	printBoxedString("Special Thanks To Nempk1(Ademar Saphado) For Teaching Me Some C Syntax",5,0,0,WallAdress);
	printf("\n");
	printBoxedString("Type 'Help' To See The Avaliable Commands",5,0,0,WallAdress);
	printf("\n");
}

void Command(char *Input,int Maximum,int *Continue,int *LeftSpace,int *Vertical_Padding,int *Horizontal_Padding,Wall *WallAdress){
	readEnhanced(Input,Maximum);
	printf("\n\n");
	//There is no known way to compare strings in C (unknown to me, at least)
	//So forgive me for the many ifs
	
	if(compareStrings(Input,"Help")){
		Help(LeftSpace,Vertical_Padding,Horizontal_Padding,WallAdress);
	}
	else if(compareStrings(Input,"Quit")){
		printBoxedString("Thank You For Using The Almighty Lexer :D",*LeftSpace,*Vertical_Padding,*Horizontal_Padding,WallAdress);
		*Continue = 0;
	}
	else if(compareStrings(Input,"Configure")){
		Configure(Input,Maximum,LeftSpace,Vertical_Padding,Horizontal_Padding,WallAdress);
	}
	else if(compareStrings(Input,"Run Lexer")){
		Run(Input,Maximum,LeftSpace,Vertical_Padding,Horizontal_Padding,WallAdress);
	}
	else if(compareStrings(Input,"Paul")){
		printBoxedString("Did You Mean 'The Best Programmer In The World'?",*LeftSpace,*Vertical_Padding,*Horizontal_Padding,WallAdress);
		printf("\n\n");
	}
	else{
		printBoxedString("Unknown Command, Please, Try Again :(",*LeftSpace,*Vertical_Padding,*Horizontal_Padding,WallAdress);
		printf("\n\n");
	}
}

void Help(int *LeftSpace,int *Vertical_Padding,int *Horizontal_Padding,Wall *WallAdress){
	int S = 10;
	StringArray *Helping = newStringArray(S);
	int i = 0;
	*(Helping->PointerArray+i++) = newString("[Command]");
	*(Helping->PointerArray+i++) = newString("[Command Description]");
	*(Helping->PointerArray+i++) = newString("Help");
	*(Helping->PointerArray+i++) = newString("Shows All The Commands Avaliable");
	*(Helping->PointerArray+i++) = newString("Quit");
	*(Helping->PointerArray+i++) = newString("Closes The Almighty Lexer");
	*(Helping->PointerArray+i++) = newString("Configure");
	*(Helping->PointerArray+i++) = newString("Opens The Aesthetics Configuration Page");
	*(Helping->PointerArray+i++) = newString("Run Lexer");
	*(Helping->PointerArray+i++) = newString("Runs The Lexer (Duh)");
	
	
	printBoxedArray(Helping->PointerArray,S,*LeftSpace,*Vertical_Padding,*Horizontal_Padding,0,2,WallAdress);
	
	freeStringArray(Helping);
}

void Configure(char *Input,int Maximum,int *LeftSpace,int *Vertical_Padding,int *Horizontal_Padding,Wall *WallAdress){
	printBoxedString("What Do You Wish To Configure?",*LeftSpace,*Vertical_Padding,*Horizontal_Padding,WallAdress);
	printf("\n");
	int S = 4;
	StringArray *Helping = newStringArray(S);
	int i = 0;
	*(Helping->PointerArray+i++) = newString("Left Spaces");
	*(Helping->PointerArray+i++) = newString("Vertical Padding");
	*(Helping->PointerArray+i++) = newString("Horizontal Padding");
	*(Helping->PointerArray+i++) = newString("Graphics");
	
	printBoxedArray(Helping->PointerArray,S,*LeftSpace,*Vertical_Padding,*Horizontal_Padding,0,1,WallAdress);
	printf("\n\n");
	
	freeStringArray(Helping);
	
	readEnhanced(Input,Maximum);
	
	if(compareStrings(Input,"Left Spaces")){
		printBoxedString("Insert The Desired Value For The Left Spaces:",*LeftSpace,*Vertical_Padding,*Horizontal_Padding,WallAdress);
		printf("\n\n");
		readEnhanced(Input,Maximum);
		*LeftSpace = parseInt(Input);
		printf("\n\n");
	}
	else if(compareStrings(Input,"Vertical Padding")){
		printBoxedString("Insert The Desired Value For The Vertical Padding:",*LeftSpace,*Vertical_Padding,*Horizontal_Padding,WallAdress);
		printf("\n\n");
		readEnhanced(Input,Maximum);
		*Vertical_Padding = parseInt(Input);
		printf("\n\n");
	}
	else if(compareStrings(Input,"Horizontal Padding")){
		printBoxedString("Insert The Desired Value For The Horizontal Padding:",*LeftSpace,*Vertical_Padding,*Horizontal_Padding,WallAdress);
		printf("\n\n");
		readEnhanced(Input,Maximum);
		*Horizontal_Padding = parseInt(Input);
		printf("\n\n");
	}
	else if(compareStrings(Input,"Graphics")){
		ConfigureGraphics(Input,Maximum,LeftSpace,Vertical_Padding,Horizontal_Padding,WallAdress);
	}
}

void ConfigureGraphics(char *Input,int Maximum,int *LeftSpace,int *Vertical_Padding,int *Horizontal_Padding,Wall *WallAdress){
	int S = 22;
	StringArray *Helping = newStringArray(S);
	int i = 0;
	*(Helping->PointerArray+i++) = newString("Horizontal Wall");
	*(Helping->PointerArray+i++) = newString(WallAdress->Horizontal);
	*(Helping->PointerArray+i++) = newString("Vertical Wall");
	*(Helping->PointerArray+i++) = newString(WallAdress->Vertical);
	
	*(Helping->PointerArray+i++) = newString("Top Left Corner");
	*(Helping->PointerArray+i++) = newString(WallAdress->T_Left);
	*(Helping->PointerArray+i++) = newString("Top Right Corner");
	*(Helping->PointerArray+i++) = newString(WallAdress->T_Right);
	*(Helping->PointerArray+i++) = newString("Bottom Left Corner");
	*(Helping->PointerArray+i++) = newString(WallAdress->B_Left);
	*(Helping->PointerArray+i++) = newString("Bottom Right Corner");
	*(Helping->PointerArray+i++) = newString(WallAdress->B_Right);
	
	*(Helping->PointerArray+i++) = newString("Left Dash Wall");
	*(Helping->PointerArray+i++) = newString(WallAdress->L_W);
	*(Helping->PointerArray+i++) = newString("Right Dash Wall");
	*(Helping->PointerArray+i++) = newString(WallAdress->R_W);
	*(Helping->PointerArray+i++) = newString("Top Dash Wall");
	*(Helping->PointerArray+i++) = newString(WallAdress->T_W);
	*(Helping->PointerArray+i++) = newString("Bottom Dash Wall");
	*(Helping->PointerArray+i++) = newString(WallAdress->B_W);
	
	*(Helping->PointerArray+i++) = newString("Cross");
	*(Helping->PointerArray+i++) = newString(WallAdress->Cross);
	
	printBoxedArray(Helping->PointerArray,S,*LeftSpace,*Vertical_Padding,*Horizontal_Padding,0,2,WallAdress);
	printf("\n\n");
	
	freeStringArray(Helping);
}

int parseInt(char *Input){
	int size = strlen(Input);
	int Output = 0;
	
	for(int i = 0;i<size;i++){
		switch(*(Input+i)){
			case '1':
			Output += 1;
			break;
			case '2':
			Output += 2;
			break;
			case '3':
			Output += 3;
			break;
			case '4':
			Output += 4;
			break;
			case '5':
			Output += 5;
			break;
			case '6':
			Output += 6;
			break;
			case '7':
			Output += 7;
			break;
			case '8':
			Output += 8;
			break;
			case '9':
			Output += 9;
			break;
		}
		if(i+1 != size){
			Output *= 10;
		}
	}
	
	return Output;
}
