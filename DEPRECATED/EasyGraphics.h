#ifndef EasyGraphics
#define EasyGraphics

#include <stdio.h>
#include <string.h>

#include "EasyString.h"
#include "EasyToken.h"


typedef struct /*Wall*/{
	char *Name;
	
	char *Horizontal;//Horizontal Line
	char *Vertical;//Vertical Line
	
	char *T_Left;//Top Left Corner
	char *T_Right;//Top Right Corner
	char *B_Left;//Bottom Left Corner
	char *B_Right;//Bottom Right Corner
	
	char *L_W;//Left Horizontal Line on a Vertical Line
	char *R_W;//Right Horizontal Line on a Vertical Line
	char *T_W;//Top Vertical Line on a Horizontal Line
	char *B_W;//Bottom Vertical Line on a Horizontal Line
	
	char *Cross;//Cross Symbol
}Wall;

Wall* newWall(char *Name,char *H,char *V,char *TL,char *TR,char *BL,char *BR,char *LW,char *RW,char *TW,char *BW,char *C){
	Wall *Output = (Wall*)malloc(sizeof(Wall));
	
	Output->Name = newString(Name);
	
	Output->Horizontal = newString(H);
	Output->Vertical = newString(V);
	
	Output->T_Left = newString(TL);
	Output->T_Right = newString(TR);
	Output->B_Left = newString(BL);
	Output->B_Right = newString(BR);
	
	Output->L_W = newString(LW);
	Output->R_W = newString(RW);
	Output->T_W = newString(TW);
	Output->B_W = newString(BW);
	
	Output->Cross = newString(C);
	
	return Output;
}

void freeWall(Wall *WallAdress){
	freeString(WallAdress->Name);
	
	freeString(WallAdress->Horizontal);
	freeString(WallAdress->Vertical);
	
	freeString(WallAdress->T_Left);
	freeString(WallAdress->T_Right);
	freeString(WallAdress->B_Left);
	freeString(WallAdress->B_Right);
	
	freeString(WallAdress->L_W);
	freeString(WallAdress->R_W);
	freeString(WallAdress->T_W);
	freeString(WallAdress->B_W);
	
	freeString(WallAdress->Cross);
	
	free(WallAdress);
}

Wall* basicThinWall(){
	return newWall("Thin Wall","-","|","/","\\","\\","/","+","+","+","+","+");
}

Wall* basicThickWall(){
	return newWall("Thick Wall","-","|","/","\\","\\","/","+","+","+","+","+");
}

void FixTableRatio(int *Size,int *Rows,int *Columns){
	if((*Rows < 1 && *Columns < 1) || *Rows * *Columns> *Size){
		*Rows = *Size;
		*Columns = 1;
	}
	else if(*Rows == 0){
		for(int i = 0;i<*Size;i+=*Columns){
			*Rows += 1;
		}
		*Columns = 0;
		for(int i = 0;i<*Size;i+=*Rows){
			*Columns += 1;
		}
	}
	else if(*Columns == 0){
		for(int i = 0;i<*Size;i+=*Rows){
			*Columns += 1;
		}
		*Rows = 0;
		for(int i = 0;i<*Size;i+=*Columns){
			*Rows += 1;
		}
	}
	else if(*Rows * *Columns < *Size){
		*Columns = 0;
		FixTableRatio(Size,Rows,Columns);
	}
}

void printMultiple(int Size,char *Char){
	for(int i = 0;i<Size;i++){
		printf(Char);
	}
}

void printConnectedSymbols(char *Left_Symbol,char *Connection_Symbol,char *Right_Symbol,int Connection_Size){
	printf(Left_Symbol);
	printMultiple(Connection_Size,Connection_Symbol);
	printf(Right_Symbol);
}

void printSpecialConnectedSymbols(char *Left_Symbol,char *Connection_Symbol,char *Intersection_Symbol,char *Right_Symbol,int* Connection_Sizes,int IntersectionsAmount){
	printf(Left_Symbol);
	for(int i = 0,Length = 0;i<IntersectionsAmount;i++){
		Length = *(Connection_Sizes + i);
		for(int l = 0;l<Length;l++){
			printf(Connection_Symbol);
		}
		if(i+1 != IntersectionsAmount){
			printf(Intersection_Symbol);
		}
	}
	printf(Right_Symbol);
}

void printBoxedString(char *String,int Left_Spaces,int Vertical_Padding,int Horizontal_Padding,Wall *WallAdress){
	if(Left_Spaces < 0 || Vertical_Padding < 0 || Horizontal_Padding < 0){
		return;
	}
	int Length = strlen(String);
	//Horizontal _
	//Vertical |
	
	printMultiple(Left_Spaces," ");
	printConnectedSymbols(WallAdress->T_Left,WallAdress->Horizontal,WallAdress->T_Right,Length+(Horizontal_Padding*2));
	printf("\n");
	
	for(int i = 0;i<Vertical_Padding;i++){
		printMultiple(Left_Spaces," ");
		printConnectedSymbols(WallAdress->Vertical," ",WallAdress->Vertical,Length+(Horizontal_Padding*2));
		printf("\n");
	}
	
	printMultiple(Left_Spaces," ");
	printf(WallAdress->Vertical);
	printMultiple(Horizontal_Padding," ");
	printf(String);
	printMultiple(Horizontal_Padding," ");
	printf(WallAdress->Vertical);
	printf("\n");
	
	for(int i = 0;i<Vertical_Padding;i++){
		printMultiple(Left_Spaces," ");
		printConnectedSymbols(WallAdress->Vertical," ",WallAdress->Vertical,Length+(Horizontal_Padding*2));
		printf("\n");
	}
	
	printMultiple(Left_Spaces," ");
	printConnectedSymbols(WallAdress->B_Left,WallAdress->Horizontal,WallAdress->B_Right,Length+(Horizontal_Padding*2));
}

void printBoxedArray(char **Strings,int Size,int Left_Spaces,int Vertical_Padding,int Horizontal_Padding,int Rows,int Columns,Wall *WallAdress){
	if(Left_Spaces < 0 || Vertical_Padding < 0 || Horizontal_Padding < 0 || Rows < 0 || Columns < 0){
		return;
	}
	
	FixTableRatio(&Size,&Rows,&Columns);
	
	int ColumnSizes[Columns];
	
	//Initialization
	for(int i = 0;i<Columns;i++){
		ColumnSizes[i] = 0;
	}
	
	for(int i = 0,Counter = 0,ColumnSize = 0;i<Size;i++,Counter++){
		if(Counter == Columns){
			Counter = 0;
			ColumnSize = 0;
		}
		ColumnSize = strlen(*(Strings+i));
		if(ColumnSize > ColumnSizes[Counter]){
			ColumnSizes[Counter] = ColumnSize;
		}
	}
	
	for(int i = 0;i<Columns;i++){
		ColumnSizes[i] += 2*Horizontal_Padding;
	}
	
	printMultiple(Left_Spaces," ");
	printSpecialConnectedSymbols(WallAdress->T_Left,WallAdress->Horizontal,WallAdress->B_W,WallAdress->T_Right,ColumnSizes,Columns);
	printf("\n");
	
	char *Current_String;
	for(int i = 0,Index = 0;i<Rows;i++){
		
		for(int l = 0;l<Vertical_Padding;l++){
			printMultiple(Left_Spaces," ");
			printf(WallAdress->Vertical);
			
			for(int x = 0;x<Columns;x++){
				printMultiple(ColumnSizes[x]," ");
				if(x+1 != Columns){
					printf(WallAdress->Vertical);
				}
			}
			printf(WallAdress->Vertical);
			printf("\n");
		}
		
		printMultiple(Left_Spaces," ");
		printf(WallAdress->Vertical);
		
		for(int l = 0;l<Columns;l++){
			
			if(Index<Size){
				printMultiple(Horizontal_Padding," ");
				Current_String = *(Strings+Index);
				printf(Current_String);
				printMultiple(ColumnSizes[l]-(2*Horizontal_Padding)-strlen(Current_String)," ");
				printMultiple(Horizontal_Padding," ");
			}
			else{
				printMultiple(ColumnSizes[l]," ");
			}
			Index++;
			if(l+1 != Columns){
				printf(WallAdress->Vertical);
			}
		}
		
		printf(WallAdress->Vertical);
		printf("\n");
		
		for(int l = 0;l<Vertical_Padding;l++){
			printMultiple(Left_Spaces," ");
			printf(WallAdress->Vertical);
			
			for(int x = 0;x<Columns;x++){
				printMultiple(ColumnSizes[x]," ");
				if(x+1 != Columns){
					printf(WallAdress->Vertical);
				}
			}
			printf(WallAdress->Vertical);
			printf("\n");
		}
		
		if(i+1 != Rows){
			printMultiple(Left_Spaces," ");
			printSpecialConnectedSymbols(WallAdress->R_W,WallAdress->Horizontal,WallAdress->Cross,WallAdress->L_W,ColumnSizes,Columns);
			printf("\n");
		}
	}
	
	printMultiple(Left_Spaces," ");
	printSpecialConnectedSymbols(WallAdress->B_Left,WallAdress->Horizontal,WallAdress->T_W,WallAdress->B_Right,ColumnSizes,Columns);
	printf("\n");
	
}

//Token Related

void printBoxedToken(Token *TokenAdress,int Left_Spaces,int Vertical_Padding,int Horizontal_Padding,Wall *WallAdress){
	int Name_Size = strlen(TokenAdress->name);
	int Value_Size = strlen(TokenAdress->value);
	int Sizes[] = {Name_Size,Value_Size,1};
	
	for(int i = 0;i<3;i++){
		Sizes[i] += 2*Horizontal_Padding;
	}
	
	//First Line
	printMultiple(Left_Spaces," ");
	printSpecialConnectedSymbols(WallAdress->T_Left,WallAdress->Horizontal,WallAdress->B_W,WallAdress->T_Right,Sizes,3);
	printf("\n");
	
	//Middle Line
	printMultiple(Left_Spaces," ");
	printf(WallAdress->Vertical);
	for(int i = 0;i<3;i++){
		printMultiple(Horizontal_Padding," ");
		switch(i){
			case 0:
			printf(TokenAdress->name);
			break;
			case 1:
			printf(TokenAdress->value);
			break;
			case 2:
			printf("%d",TokenAdress->isConstant);
			break;
		}
		printMultiple(Horizontal_Padding," ");
		if(i+1 != 3){
			printf(WallAdress->Vertical);
		}
	}
	printf(WallAdress->Vertical);
	printf("\n");
	
	//Last Line
	printMultiple(Left_Spaces," ");
	printSpecialConnectedSymbols(WallAdress->B_Left,WallAdress->Horizontal,WallAdress->T_W,WallAdress->B_Right,Sizes,3);
}

void getTokenSizes(TokenArray *TokenArrayAdress,int *Name_Size,int *Value_Size){
	Token *Current;
	int nameLength = 0;
	int valueLength = 0;
	for(int i = 0;i<TokenArrayAdress->size;i++){
		if(*(TokenArrayAdress->used+i)){
			Current = *(TokenArrayAdress->PointerArray+i);
			nameLength = strlen(Current->name);
			valueLength = strlen(Current->value);
			
			if(*Name_Size < nameLength){
				*Name_Size = nameLength;
			}
			if(*Value_Size < valueLength){
				*Value_Size = valueLength;
			}
		}
	}
}

void printBoxedTokenArray(TokenArray *TokenArrayAdress,int Left_Spaces,int Vertical_Padding,int Horizontal_Padding,Wall *WallAdress){
	int Name_Size = 0;
	int Value_Size = 0;
	
	getTokenSizes(TokenArrayAdress,&Name_Size,&Value_Size);
	
	int Sizes[] = {Name_Size,Value_Size,1};
	
	for(int i = 0;i<3;i++){
		Sizes[i] += 2*Horizontal_Padding;
	}
	
	//First Line
	printMultiple(Left_Spaces," ");
	printSpecialConnectedSymbols(WallAdress->T_Left,WallAdress->Horizontal,WallAdress->B_W,WallAdress->T_Right,Sizes,3);
	printf("\n");
	
	int Count = 0;
	for(int i = 0;i<TokenArrayAdress->size;i++){
		if(*(TokenArrayAdress->used+i)){
			Count++;
		}
	}
	Token *Current;
	for(int i = 0;i<TokenArrayAdress->size;i++){
		if(*(TokenArrayAdress->used+i)){
			Current = *(TokenArrayAdress->PointerArray+i);
			
			for(int l = 0;l<Vertical_Padding;l++){
				printMultiple(Left_Spaces," ");
				printf(WallAdress->Vertical);
				printMultiple(Sizes[0]," ");
				printf(WallAdress->Vertical);
				printMultiple(Sizes[1]," ");
				printf(WallAdress->Vertical);
				printMultiple(Sizes[2]," ");
				printf(WallAdress->Vertical);
				printf("\n");
			}
			
			printMultiple(Left_Spaces," ");
			printf(WallAdress->Vertical);
			//Name
			printMultiple(Horizontal_Padding," ");
			printf(Current->name);
			printMultiple(Sizes[0]-Horizontal_Padding-strlen(Current->name)," ");
			printf(WallAdress->Vertical);
			//Value
			printMultiple(Horizontal_Padding," ");
			printf(Current->value);
			printMultiple(Sizes[1]-Horizontal_Padding-strlen(Current->value)," ");
			printf(WallAdress->Vertical);
			//Is Constant
			printMultiple(Horizontal_Padding," ");
			printf("%d",Current->isConstant);
			printMultiple(Horizontal_Padding," ");
			printf(WallAdress->Vertical);
			printf("\n");
			
			for(int l = 0;l<Vertical_Padding;l++){
				printMultiple(Left_Spaces," ");
				printf(WallAdress->Vertical);
				printMultiple(Sizes[0]," ");
				printf(WallAdress->Vertical);
				printMultiple(Sizes[1]," ");
				printf(WallAdress->Vertical);
				printMultiple(Sizes[2]," ");
				printf(WallAdress->Vertical);
				printf("\n");
			}
			
			if(--Count > 0){
				printMultiple(Left_Spaces," ");
				printSpecialConnectedSymbols(WallAdress->R_W,WallAdress->Horizontal,WallAdress->Cross,WallAdress->L_W,Sizes,3);
				printf("\n");
			}
		}
	}
	
	//Last Line
	printMultiple(Left_Spaces," ");
	printSpecialConnectedSymbols(WallAdress->B_Left,WallAdress->Horizontal,WallAdress->T_W,WallAdress->B_Right,Sizes,3);
}




#endif
