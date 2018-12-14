#include<stdlib.h>
#include<malloc.h>
#include"paser1.h"
TokenType TokenCondi;
TableSymbol *tabCurrent;
void factor();//
void term();//
void expression();//
void condition();//
void block();//
void statement();//
void program();//
void term() {
	factor();
	while (Token == TIMES || Token == SLASH ) {
		Token = getToken();
		factor();
	};

};
void expression() {
	if (Token == PLUS || Token == MINUS) {
		Token = getToken();
	};
	term();
	while (Token == MINUS || Token == PLUS) {
		Token = getToken();
		term();
	};

};

void condition() {
	if (Token == ODD) {
		Token = getToken();
		expression();
	}
	else {
		expression();
		if (Token == LEQ || Token == NEQ || Token == LSS || Token == GTR || Token == GEQ || Token == EQU) {
			Token = getToken();
			expression();
		}
		else {
			printf("cau lenh dieu kien khong hop le tai dong : %d cot : %d ", LineIndex, RowIndex);
			exit(0);
		}
	}
	
};
void statement() {
	if (Token == IDENT) {
		
		TokenCondi = checkDeclar(Id, tabCurrent);
		if ( TokenCondi == NOTDECLARE) {
			printf("bien chua duoc khai bao tai dong %d cot %d", LineIndex, RowIndex);
			exit(0);
		}
		else {
			if (TokenCondi == PROCEDURE || TokenCondi == CONST) {
				printf("bien %s duoc su dung khong dung cach tai dong %d cot %d", preId, LineIndex, RowIndex);
				exit(0);
			}
		};
		Token = getToken();
		if (Token == LBRACK) {
			if (TokenCondi != ARRAY) {
				printf("bien %s khong phai la kieu array tai dong %d cot %d ", preId, LineIndex, RowIndex);
				exit(0);
			}
			Token = getToken();
			expression();
			if (Token == RBRACK) {
				Token = getToken();
			}else{
				printf("thieu dau dong ngoac vuong tai dong %d  cot : %d ", LineIndex, RowIndex);
				exit(0);
			};
		}
		else {
			if (TokenCondi != VAR) {
				printf("thieu chi so mang cua bien %s tai dong 5d cot %d ", preId, LineIndex, RowIndex);
				exit(0);
			}
		};
		if (Token == ASSIGN) {
			Token = getToken();
			expression();
		}
		else {
			printf("thieu toan tu gan tai dong %d  cot : %d ", LineIndex, RowIndex);
			exit(0);
		}
	}
	else {
		if (Token == CALL) {
			Token = getToken();
			if (Token == IDENT) {
				TableSymbol *tmp;
				int condi=0;
				tmp = tabCurrent;
				do {
					if (checkDeclar(Id, tmp) == PROCEDURE) {
						break;
					};
					tmp = tmp->parent;
					if (tmp == NULL) {
						printf("procedure chua duoc khai bao hoac da ton tai voi chuc nang khong phai 1 procedure tai dong %d cot %d", LineIndex, RowIndex);
						exit(0);
					}
				} while (tmp != NULL);
				int countPar = 0,count=0;
				countPar = getCountParam(tabCurrent, Id);
				Token = getToken();
				if (Token == LPARENT) {
					Token = getToken();
					expression();
					count++;
					while (Token == COMMA) {
						Token = getToken();
						expression();
						count++;
					};
					if (Token == RPARENT) {
						if (count != countPar) {
							printf("thieu tham so tai dong %d, cot %d", LineIndex, RowIndex);
							exit(0);
						}
						Token = getToken();
					}else{
						printf("thieu dau dong ngoac tai dong %d  cot : %d ", LineIndex, RowIndex);
						exit(0);
					};
				}
			}
			else {
				printf("CALL sai cu phap. sau CALL phai la ten ham(IDENT). tai : %d cot : %d ", LineIndex, RowIndex);
				exit(0);
			}
		}
		else {
			if (Token == BEGIN) {
				Token = getToken();
				statement();
				while (Token == SEMICOLON) {
					Token = getToken();
					statement();
				};
				if (Token != END) {
					printf("thieu END tai dong : %d cot : %d ", LineIndex, RowIndex);
					exit(0);
				};
				Token = getToken();
			}
			else {
				if (Token == IF) {
					Token = getToken();
					condition();
					if (Token == THEN) {
						Token = getToken();
						statement();
						if (Token == ELSE) {
							Token = getToken();
							statement();
						};

					}
					else {
						printf(" thieu THEN trong cau lenh IF tai dong: %d cot : %d ", LineIndex, RowIndex);
						exit(0);
					}
				}
				else {
					if (Token == WHILE) {
						Token = getToken();
						condition();
						if (Token == DO) {
							Token = getToken();
							statement();
						}
						else {
							printf(" thieu DO trong vong lap WHILE tai dong : %d cot : %d ", LineIndex, RowIndex);
							exit(0);
						}
					}
					else {
						if (Token == FOR) {
							Token = getToken();
							if (Token == IDENT) {
								if (checkDeclar(Id, tabCurrent) != VAR) {
									if (checkDeclar(Id, tabCurrent) == NOTDECLARE) {
										printf("bien chua duoc khai bao tai vi tri dong %d cot %d", LineIndex, RowIndex);
										exit(0);
									}
									else {
										printf("%s khong phai bien tai dong %d cot %d", Id, LineIndex, RowIndex);
										exit(0);
									}
								};
								Token = getToken();
								if (Token == ASSIGN) {
									Token = getToken();
									expression();
									if (Token == TO) {
										Token = getToken();
										expression();
										if (Token == DO) {
											Token = getToken();
											statement();
										}
										else {
											printf("thieu DO trong vong lap FOR tai dong: %d cot : %d ", LineIndex, RowIndex);
											exit(0);
										}
									}
									else {
										printf("thieu TO trong vong lap FOR tai dong : %d cot : %d ", LineIndex, RowIndex);
										exit(0);

									}
								}
								else {
									printf(" thieu ASSIGN trong vong lap FOR tai dong : %d cot : %d ", LineIndex, RowIndex);
									exit(0);
								}
							}
							else {
								printf("thieu ten bien trong vong lap FOR tai dong : %d cot : %d ", LineIndex, RowIndex);
								exit(0);
							}
						}
						
					}

				}

			}

		}

	}

};
void factor() {
	if (Token == IDENT) {
		if (checkDeclar(Id, tabCurrent) == NOTDECLARE) {
			printf("bien %s chua duoc khai bao tai dong %d cot %d", Id, LineIndex, RowIndex);
			exit(0);
		}
		else {
			if (checkDeclar(Id, tabCurrent) == PROCEDURE) {
				printf("bien %s la procedure chi co the goi khong duoc su dung trong bieu thuc tai dong %d cot %d", Id, LineIndex, RowIndex);
				exit(0);
			}
		}
		
		Token = getToken();
		if(Token == LBRACK){
			if (checkDeclar(preId, tabCurrent) != ARRAY) {
				printf("bien %s khong phai kieu mang loi tai dong %d cot %d", preId, LineIndex, RowIndex);
				exit(0);
			};
			Token = getToken();
			expression();
			if (Token == RBRACK) {
				Token = getToken();
			}
			else {
				printf("thieu dau dong ngoac tai dong %d cot %d", LineIndex, RowIndex);
				exit(0);
			}
			
		}
	}
	else {
		if (Token == NUMBER) {
			if (Num > 999999) {
				printf("loi tran so tai dong %d cot %d", LineIndex, RowIndex);
				exit(0);
			};
			Token = getToken();
		}
		else {
			if (Token == LPARENT) {
				Token = getToken();
				expression();
				if (Token == RPARENT) {
					Token = getToken();
				}
				else {
					printf("thieu dau dong ngoac tai dong: %d cot : %d ", LineIndex, RowIndex);
					exit(0);
				}
			}
			else {
				printf("sai cu phap toan tu tai dong: %d cot : %d ", LineIndex, RowIndex);
				exit(0);
			}
		}
	}
};

void block() {
	if (Token == CONST) {
		Token = getToken();
		if (Token == IDENT) {
			enterElement(tabCurrent, Id, CONST, 2,0);
			Token = getToken();
			if (Token == EQU) {
				Token = getToken();
				if (Token == NUMBER) {
					if (Num > 999999) {
						printf("loi tran so tai dong %d cot %d ", LineIndex, RowIndex);
						exit(0);
					};
					Token = getToken();
					while (Token == COMMA) {
						Token = getToken();
						if (Token == IDENT) {
							enterElement(tabCurrent, Id, CONST, 2,0);
							Token = getToken();
							if (Token == EQU) {
								Token = getToken();
								if (Token == NUMBER) {
									if (Num > 999999) {
										printf("loi tran so tai dong %d cot %d ", LineIndex, RowIndex);
										exit(0);
									};
									Token = getToken();
								}
								else {
									printf("thieu gia tri chi dinh cho IDENT tai dong: %d cot : %d ", LineIndex, RowIndex);
									exit(0);
								}
							}
							else {
								printf(" thieu dau bang tai dong : %d cot : %d ", LineIndex, RowIndex);
								exit(0);
							}
						}
						else {
							printf(" thieu ten bien sau dau phay tai dong : %d cot : %d ", LineIndex, RowIndex);
							exit(0);
						}
					};
					if (Token == SEMICOLON) {
						Token = getToken();
					}
					else {
						printf("thieu dau cham phay o dong : %d cot : %d ", LineIndex, RowIndex);
						exit(0);
					}
				}
				else {
					printf("thieu dau bang tai dong: %d cot : %d ", LineIndex, RowIndex);
					exit(0);
				}
			}
			else {
				printf("thieu dau bang tai dong : %d cot : %d ", LineIndex, RowIndex);
				exit(0);
			}
		}
		else {
			printf(" thieu ten bien sau CONST tai dong : %d cot : %d ", LineIndex, RowIndex);
			exit(0);
		}
	};
	
	if (Token == VAR) {
		Token = getToken();
		if (Token == IDENT) {
			
			Token = getToken();
			if(Token == LBRACK ){
				Token = getToken(); 
				if(Token == NUMBER){
					if (Num > 999999) {
						printf("loi tran so tai dong %d cot %d ", LineIndex, RowIndex);
						exit(0);
					};
					Token = getToken();
					if(Token == RBRACK){

						enterElement(tabCurrent, preId, ARRAY, 2*Num, 0);
						Token = getToken();
					}else{
						printf("thieu dau dong ngoac tai dong : %d cot : %d ", LineIndex, RowIndex);
						exit(0);
					}
				}else{
					printf("thieu kich thuoc mang tai dong : %d cot : %d ", LineIndex, RowIndex);
					exit(0);
				}
			}
			else {
				
				enterElement(tabCurrent, preId, VAR, 2, 0);
			}
			
			while (Token == COMMA) {
				Token = getToken();
				if (Token == IDENT) {
					Token = getToken();
					if(Token == LBRACK){
						Token = getToken();
						if(Token == NUMBER){
							if (Num > 999999) {
								printf("loi tran so tai dong %d cot %d ", LineIndex, RowIndex);
								exit(0);
							};
							Token = getToken();
							if(Token == RBRACK){
								enterElement(tabCurrent, preId, ARRAY, 2 * Num, 0);
								Token = getToken();
							}else{
								printf("thieu dau dong ngoac tai dong: %d cot : %d ", LineIndex, RowIndex);
								exit(0);
							}
						}else{
							printf("thieu kich thuoc mang tai dong : %d cot : %d ", LineIndex, RowIndex);
							exit(0);
						}
					}
					else {
						enterElement(tabCurrent, preId, VAR, 2, 0);
					}
				}
				else {
					printf("thieu ten bien sau dau phay tai dong: %d cot : %d ", LineIndex, RowIndex);
					exit(0);
				}
			};
			if (Token == SEMICOLON) {
				Token = getToken();
			}
			else {
				printf(" thieu dau cham phay tai dong : %d cot : %d ", LineIndex, RowIndex);
				exit(0);
			}
		}
		else {
			printf("thieu ten bien sau VAR tai dong: %d cot : %d ", LineIndex, RowIndex);
			exit(0);
		}
	};

	while (Token == PROCEDURE) {
		Token = getToken();
		if (Token == IDENT) {
			enterElement(tabCurrent, Id, PROCEDURE, 0,0);
			TableSymbol *tmpTab;
			tmpTab = tabCurrent;
			tabCurrent = mkTable(tmpTab);
			Token = getToken();
			if(Token == LPARENT ){
				int countParam = 0;
				Token = getToken(); 
				if (Token == VAR) {
					Token = getToken();
				};
					if(Token == IDENT){
						enterElement(tabCurrent, Id, VAR, 2, 0);
						countParam++;
						Token = getToken();
						while(Token == SEMICOLON){
							Token = getToken();
							if (Token == VAR) {
								Token = getToken();
							};
									if(Token == IDENT){
										enterElement(tabCurrent, Id, VAR, 2, 0);
										countParam++;
										Token = getToken();
									}else{
										printf("thieu IDENT sau COMMA tai dong : %d cot : %d ", LineIndex, RowIndex);
										exit(0);
									}
							
						}
						tabCurrent->parent->tail->countParam = countParam;
						if (Token != RPARENT) {
							printf("thieu dau dong ngoac tai dong : %d cot : %d ", LineIndex, RowIndex);
							exit(0);
						}
						else {
							Token = getToken();
						}
					}else{
						printf("thieu IDENT  tai dong : %d cot : %d ", LineIndex, RowIndex);
						exit(0);
					}
				
			}
			if (Token == SEMICOLON) {
				Token = getToken();
				block();
				
				if (Token == SEMICOLON) {
					Token = getToken();
				}
				else {
					printf("thieu dau cham phay tai dong: %d cot : %d ", LineIndex, RowIndex);
					exit(0);
				};
			}
			else {
				printf("thieu dau cham phay tai dong: %d cot : %d ", LineIndex, RowIndex);
				exit(0);
			}
		}
		else {
			printf("thieu ten thu tuc tai dong: %d cot : %d ", LineIndex, RowIndex);
			exit(0);
		}
	};

	if (Token == BEGIN) {
		Token = getToken();
		statement();
		while (Token == SEMICOLON) {
			Token = getToken();
			statement();
		};
		if (Token == END) {
			TableSymbol *tmp;
			tmp = tabCurrent;
			int offset = getOffset(tmp);
			tabCurrent = tabCurrent->parent;
			if (tabCurrent != NULL) {
				tabCurrent->tail->offset = offset;
			};
			deleteTab(tmp);
			Token = getToken();

		}
		else {
			printf("thieu END tai dong : %d cot : %d ", LineIndex, RowIndex);
			exit(0);
		}
	}
	else {
		printf("thieu BEGIN tai dong : %d cot : %d ", LineIndex, RowIndex);
		exit(0);
	};
};

void program() {
	if (Token == PROGRAM) {
		Token = getToken();
		if (Token == IDENT) {
			Token = getToken();
			if(Token == SEMICOLON){
				
				tabCurrent = mkTable(NULL);
				Token = getToken();
				block();
				if (Token == PERIOD) {
					Token = getToken();
					if(Token != ENDOF){
						printf("chuong trinh ket thuc boi dau cham. thua du lieu sau dau cham.");
						exit(0);
					};
					printf("chuong trinh dung cu phap");
				}
				else {
					printf("thieu dau cham tai dong : %d cot : %d ", LineIndex, RowIndex);
					exit(0);
				}
			}else{
				printf("thieu dau cham phay o dong : %d cot : %d ", LineIndex, RowIndex);
				exit(0);
			}
			
		}
		else {
			printf("thieu ten truong trinh tai dong : %d cot : %d ", LineIndex, RowIndex);
			exit(0);
		}
	}
	else {
		printf("thieu program tai dong : %d cot : %d ", LineIndex, RowIndex);
		exit(0);
	};
};

