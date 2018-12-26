#define SCANER 1
#include<stdio.h>
#include<ctype.h>
#include<string.h>
#include "scaner.h"
TokenType Token;
int Num, LineIndex=1, Parent=0, Brack =0, RowIndex = 0, LenAdd =0;
FILE *f;
char Ch;
char Id[MAX_IDENT_LEN];
char preId[MAX_IDENT_LEN];
TokenType checkKeyword(char *id){
	int i ;
	for(i = 3 ; i < 22; i++){
		if(strcmp(id,TokenTab[i])==0){
			return i;
		}
	}
	memset(preId, 0, 11);
	strcpy(preId, id);
	return IDENT;
}

char getCh(){
	char c ; 
	c = fgetc(f);
	LenAdd ++;
	printf("%c", toupper(c));
	return toupper(c);
}
TokenType getToken(){
	RowIndex = RowIndex + LenAdd;
	LenAdd =0;
	memset(Id,0,11);
	while(Ch ==' '|| Ch =='\t'|| Ch == '\n') {
		if(Ch =='\n'){
			LineIndex++;	
			LenAdd =0;
			RowIndex = 0;
		};
		Ch = getCh();
		
	};
	if(feof(f)){
			return ENDOF;
	};
	if(isalpha(Ch)){
		int l = 0;
		do {
			if(l<10){
				Id[l++]=Ch;
				Ch = getCh();
			}else{
				Ch = getCh();
			};
			
		}while(isalpha(Ch)||isdigit(Ch)  );
		return checkKeyword(Id);
	}else{
		if(isdigit(Ch)){
			Num = 0 ;
			int lenNumber = 0 ;
			do{
				lenNumber++;
				if(lenNumber <9){
					Num = Num*10 + Ch -48;
				};
				Ch = getCh();
			}while(isdigit(Ch));
			return NUMBER;
		}else if(Ch ==':'){
			Ch = getCh();
			if(Ch  == '=')
			{
				Ch = getCh();
				return ASSIGN;
			}else return NONE;
		
		}else if(Ch == ';') {
			Ch = getCh();
			return SEMICOLON;
		}else if(Ch == '+'){
			Ch = getCh();
			return PLUS;
		}else if( Ch == ','){
			Ch = getCh();
			return COMMA;
		}else if(Ch == '-'){
			Ch= getCh();
			return MINUS;
		}else if(Ch == '*'){
			Ch = getCh();
			return TIMES;
		}else if(Ch == '/'){
			Ch = getCh();
			return SLASH;
		}else if(Ch == '='){
			Ch = getCh();
			return EQU;
		}else if(Ch == '='){
			Ch = getCh();
			return EQU;
		}else if(Ch =='<'){
			Ch=getCh();
			if(Ch == '>'){
				Ch = getCh();
				return NEQ;
			}else if(Ch == '='){
				Ch = getCh();
				return LEQ;
			}else return LSS;
		}else if(Ch == '>'){
			Ch = getCh();
			if (Ch == '='){
				Ch = getCh();
				return GEQ;
			}else return GTR;
		}else if ( Ch == '('){
			Ch = getCh();
			Parent ++;
			return LPARENT;
		}else if ( Ch == ')'){
			Ch = getCh();
			Parent --;
			return RPARENT;
		}else if ( Ch == '['){
			Ch = getCh();
			Brack ++;
			return LBRACK;
		}else if ( Ch == ']'){
			Ch = getCh();
			Brack --;
			return RBRACK;
		}else if ( Ch == '.'){
			Ch = getCh();
			return PERIOD;
		}else  return NONE;
			
		}
	}
