#include<stdlib.h>
#include<malloc.h>
#include"paser1.h"
#include"generator.h"
TokenType OP;
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
	while (Token == TIMES || Token == SLASH) {
		if (Token == TIMES) {
			OP = MUL;
		}
		else {
			OP = DIV;
		}
		Token = getToken();
		factor();
		enterInstruction(OP, 0, 0);
	};

};
void expression() {
	if (Token == PLUS || Token == MINUS) {
		if (Token == MINUS) {
			OP = NEG;
		}
		Token = getToken();
	};
	term();
	if (OP == NEG) {
		enterInstruction(OP, 0, 0);
	};
	while (Token == MINUS || Token == PLUS) {
		if (Token == MINUS) {
			OP = ADD;
		}
		else {
			OP = SUB;
		}
		Token = getToken();
		term();
		enterInstruction(OP, 0, 0);
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
			if (Token == LEQ) {
				OP = LE;
			}
			else {
				if (Token == NEQ) {
					OP = NE;
				}
				else {
					if (Token == LSS) {
						OP = LT;
					}
					else {
						if (Token == GTR) {
							OP = GT;
						}
						else {
							if (Token == GEQ) {
								OP = GE;
							}
							else {
									OP = EQ;
							}
						}
					}
				}
			}
			Token = getToken();
			expression();
			enterInstruction(OP, 0, 0);
		}
		else {
			printf("cau lenh dieu kien khong hop le tai dong : %d cot : %d ", LineIndex, RowIndex);
			exit(0);
		}
	}

};
void statement() {
	Data *tmp0;
	int add;
	int index;
	if (Token == IDENT) {
		
		TokenCondi = checkDeclar(Id, tabCurrent);
		if (TokenCondi == NOTDECLARE) {
			printf("bien chua duoc khai bao tai dong %d cot %d", LineIndex, RowIndex);
			exit(0);
		}
		else {
			if (TokenCondi == PROCEDURE || TokenCondi == CONST) {
				printf("bien %s duoc su dung khong dung cach tai dong %d cot %d", preId, LineIndex, RowIndex);
				exit(0);
			}
		};
		
		tmp0 = getElement(Id, tabCurrent);
		add = getAddr(Id, tabCurrent);
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
				index = stack[T] * 2;
				
				enterInstruction(LA, tmp0->baseAddr, add + index);
			}
			else {
				printf("thieu dau dong ngoac vuong tai dong %d  cot : %d ", LineIndex, RowIndex);
				exit(0);
			};
		}
		else {
			if (TokenCondi != VAR) {
				printf("thieu chi so mang cua bien %s tai dong 5d cot %d ", preId, LineIndex, RowIndex);
				exit(0);
			};
			enterInstruction(LA, tmp0->baseAddr, add);
		};
		if (Token == ASSIGN) {
			Token = getToken();
			expression();
			enterInstruction(ST, 0, 0);
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
				int condi = 0;
				tmp = tabCurrent;
				do {
					if (checkDeclar(Id, tmp) == PROCEDURE) {
						tmp0 = getElement(Id, tabCurrent);
						break;
					};
					tmp = tmp->parent;
					if (tmp == NULL) {
						printf("procedure chua duoc khai bao hoac da ton tai voi chuc nang khong phai 1 procedure tai dong %d cot %d", LineIndex, RowIndex);
						exit(0);
					}
				} while (tmp != NULL);
				int countPar = 0, count = 0;
				countPar = getCountParam(tabCurrent, Id);
				Token = getToken();
				if (Token == LPARENT) {
					Token = getToken();
					enterInstruction(INT, 4, 0);
					enterInstruction(LC, tmp0->countParam, 0);
					expression();
					count++;
					while (Token == COMMA) {
						Token = getToken();
						expression();
						count++;
					};
					if (Token == RPARENT) {
						if (count < countPar) {
							printf("thieu tham so tai dong %d, cot %d", LineIndex, RowIndex);
							exit(0);
						};
						if (count > countPar) {
							printf("thua tham so tai dong %d, cot %d", LineIndex, RowIndex);
							exit(0);
						};
						PC = countCmd + 1;
						enterInstruction(DCT, 5 + tmp0->countParam, 0);
						enterInstruction(CAL, tmp0->baseAddr, tmp0->indexCmd);
						Token = getToken();
					}
					else {
						printf("thieu dau dong ngoac tai dong %d  cot : %d ", LineIndex, RowIndex);
						exit(0);
					};
				}
			}
			else {
				if (Token == READ || Token == READLN || Token == WRITE || Token == WRITELN) {
					if (Token == READ || Token == READLN) {
						OP = RI;
					}
					else {
						OP = WRI;
					}
					Token = getToken();
					if (Token == LPARENT) {
						Token = getToken();
						if (Token == IDENT) {
							tmp0 = getElement(Id, tabCurrent);
							add = getAddr(Id, tabCurrent);
							if (checkDeclar(Id, tabCurrent) == VAR ) {
								Token = getToken();
								if (Token == RPARENT) {
									Token = getToken();
								}
								else {
									printf("thieu dau dong ngoac.tai dong %d cot %d", LineIndex, RowIndex);
									exit(0);
								}
								enterInstruction(LA, tmp0->baseAddr, add);
							}
							else {
								if (checkDeclar(Id, tabCurrent) == ARRAY) {
									Token = getToken();
									if (Token == LBRACK) {
										Token = getToken();
										expression();
										if (Token == RBRACK) {
											index = stack[T];
											T = T - 1;
											enterInstruction(LA, tmp0->baseAddr, add+index);
											Token = getToken();
											if (Token == RPARENT) {
												Token = getToken();
											}
											else {
												printf("thieu dau dong ngoac.tai dong %d cot %d", LineIndex, RowIndex);
												exit(0);
											}
										}
										else {
											printf("thieu dau dong ngoac vuong.tai dong %d cot %d", LineIndex, RowIndex);
											exit(0);
										}
									}
									else {
										printf("thieu chi so cua mang. tai dong %d cot %d", LineIndex, RowIndex);
										exit(0);
									}
								}
								else {
									printf("bien chua duoc khai bao hoac su dung sai muc dich.tai dong %d cot %d", LineIndex, RowIndex);
									exit(0);
								}
							}
							enterInstruction(OP, 0, 0);
							enterInstruction(WLN, 0, 0);
							
						}
						else {
							printf("thieu ten bien tai ham vao ra.tai dong %d cot %d", LineIndex, RowIndex);
							exit(0);
						}
					}
					else {
						printf("CALL ham vao ra sai cu phap. tai dong %d cot %d", LineIndex, RowIndex);
						exit(0);
					}
				}else{
					printf("CALL sai cu phap. sau CALL phai la ten ham(IDENT) hoac cac ham vao ra. tai : %d cot : %d ", LineIndex, RowIndex);
					exit(0);
				}
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
					int Jump = countCmd;
					enterInstruction(FJ, 0, 0);
					if (Token == THEN) {
						Token = getToken();
						statement();
						int Jump2 = countCmd;
						enterInstruction(J, 0, 0);
						if (Token == ELSE) {
							cmdList[Jump].p = countCmd;
							Token = getToken();
							statement();
							cmdList[Jump2].p = countCmd;
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
						
						int jump3;
						jump3 = countCmd;
						condition();
						int jump = countCmd;
						enterInstruction(FJ, 0, 0);
						if (Token == DO) {
							Token = getToken();
							statement();
							enterInstruction(J, jump3, 0);
							cmdList[jump].p = countCmd;
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
								tmp0 = getElement(Id, tabCurrent);
								add = getAddr(Id, tabCurrent);
								enterInstruction(LA, tmp0->baseAddr, add);
								enterInstruction(CV, 0, 0);

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
									enterInstruction(ST, 0, 0);
									index = countCmd;
									int jump, jump1;
									jump1 = countCmd;
									enterInstruction(CV, 0, 0);
									enterInstruction(LI, 0, 0);
									if (Token == TO) {
										Token = getToken();
										expression();
										enterInstruction(LE, 0, 0);
										jump = countCmd;
										enterInstruction(FJ, 0, 0);
										if (Token == DO) {
											Token = getToken();
											statement();
											enterInstruction(CV, 0, 0);
											enterInstruction(CV, 0, 0);
											enterInstruction(LI, 0, 0);
											enterInstruction(LC, 1, 0);
											enterInstruction(ADD, 0, 0);
											enterInstruction(ST, 0, 0);
											enterInstruction(J, jump1, 0);
											cmdList[jump].p = countCmd;
											enterInstruction(DCT, 1, 0);
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
		if (checkDeclar(Id, tabCurrent) == ARRAY) {
			Data *tmp0;
			tmp0 = getElement(Id, tabCurrent);
			int add = getAddr(Id, tabCurrent);
			Token = getToken();
			if (Token == LBRACK) {
				Token = getToken();
				expression();
				if (Token == RBRACK) {
					Token = getToken();
				}
				else {
					printf("thieu dau dong ngoac vuong tai dong %d cot %d", LineIndex, RowIndex);
					exit(0);
				};
				int p = stack[T]*2;
				T = T - 1;
				enterInstruction(LV, tmp0->baseAddr, add + p);
				
			}
			else {
				printf("thieu chi so mang tai dong %d cot %d", LineIndex, RowIndex);
				exit(0);
			}
		}
		else {
			Data *tmp0;
			tmp0 = getElement(Id, tabCurrent);
			int add = getAddr(Id, tabCurrent);
			enterInstruction(LV, tmp0->baseAddr, add);
			Token = getToken();
			if (Token == LBRACK) {
				printf("khong phai bien mang. khong the gan chi so. tai dong %d cot %d", LineIndex, RowIndex);
				exit(0);
			};

		}
		
	}
	else {
		if (Token == NUMBER) {
			if (Num > 999999) {
				printf("loi tran so tai dong %d cot %d", LineIndex, RowIndex);
				exit(0);
			};
			enterInstruction(LC, Num, 0);
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
	Data *tmp0;
	int add;
	int jump, jump1;
	if (Token == CONST) {
		Token = getToken();
		if (Token == IDENT) {
			enterElement(tabCurrent, Id, CONST, 2, 0,tabCurrent->baseAddr,NULL);
			tmp0 = getElement(Id, tabCurrent);
			add = getAddr(Id, tabCurrent);
			Token = getToken();
			if (Token == EQU) {
				Token = getToken();
				if (Token == NUMBER) {
					if (Num > 999999) {
						printf("loi tran so tai dong %d cot %d ", LineIndex, RowIndex);
						exit(0);
					};
					stack[tmp0->baseAddr + add] = Num;
					Token = getToken();
					while (Token == COMMA) {
						Token = getToken();
						if (Token == IDENT) {
							enterElement(tabCurrent, Id, CONST, 2, 0,tabCurrent->baseAddr,NULL);
							tmp0 = getElement(Id, tabCurrent);
							add = getAddr(Id, tabCurrent);
							Token = getToken();
							if (Token == EQU) {
								Token = getToken();
								if (Token == NUMBER) {
									if (Num > 999999) {
										printf("loi tran so tai dong %d cot %d ", LineIndex, RowIndex);
										exit(0);
									};
									stack[tmp0->baseAddr + add] = Num;
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
			if (Token == LBRACK) {
				Token = getToken();
				if (Token == NUMBER) {
					if (Num > 999999) {
						printf("loi tran so tai dong %d cot %d ", LineIndex, RowIndex);
						exit(0);
					};
					Token = getToken();
					if (Token == RBRACK) {

						enterElement(tabCurrent, preId, ARRAY, 2 * Num,0,tabCurrent->baseAddr,NULL);
						Token = getToken();
					}
					else {
						printf("thieu dau dong ngoac tai dong : %d cot : %d ", LineIndex, RowIndex);
						exit(0);
					}
				}
				else {
					printf("thieu kich thuoc mang tai dong : %d cot : %d ", LineIndex, RowIndex);
					exit(0);
				}
			}
			else {

				enterElement(tabCurrent, preId, VAR, 2, 0, tabCurrent->baseAddr, NULL);
			}

			while (Token == COMMA) {
				Token = getToken();
				if (Token == IDENT) {
					Token = getToken();
					if (Token == LBRACK) {
						Token = getToken();
						if (Token == NUMBER) {
							if (Num > 999999) {
								printf("loi tran so tai dong %d cot %d ", LineIndex, RowIndex);
								exit(0);
							};
							Token = getToken();
							if (Token == RBRACK) {
								enterElement(tabCurrent, preId, ARRAY, 2 * Num, 0, tabCurrent->baseAddr, NULL);
								Token = getToken();
							}
							else {
								printf("thieu dau dong ngoac tai dong: %d cot : %d ", LineIndex, RowIndex);
								exit(0);
							}
						}
						else {
							printf("thieu kich thuoc mang tai dong : %d cot : %d ", LineIndex, RowIndex);
							exit(0);
						}
					}
					else {
						enterElement(tabCurrent, preId, VAR, 2, 0, tabCurrent->baseAddr, NULL);
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
		jump = countCmd;
		enterInstruction(J, 0, 0);
		Token = getToken();
		if (Token == IDENT) {
			enterElement(tabCurrent, Id, PROCEDURE, 0, 0,tabCurrent->baseAddr,NULL);
			TableSymbol *tmpTab;
			tmpTab = tabCurrent;
			tabCurrent = mkTable(tmpTab);
			tmpTab->tail->tb =(void*) tabCurrent;
			tmpTab->tail->indexCmd = countCmd;
			enterInstruction(INT,stack[T],0);
			Token = getToken();
			if (Token == LPARENT) {
				int countParam = 0;
				Token = getToken();
				if (Token == VAR) {
					Token = getToken();

					if (Token == IDENT) {
						enterElement(tabCurrent, Id, VAR, 2, 0, tabCurrent->baseAddr, NULL);
						tmp0 = getElement(Id, tabCurrent);
						add = getAddr(Id, tabCurrent);
						enterInstruction(LA, tmp0->baseAddr, add);
						enterInstruction(LC, stack[B + 5 + countParam], 0);
						enterInstruction(ST, 0, 0);
						countParam++;
						Token = getToken();
						while (Token == SEMICOLON) {
							Token = getToken();
							if (Token == VAR) {
								Token = getToken();

								if (Token == IDENT) {
									enterElement(tabCurrent, Id, VAR, 2, 0, tabCurrent->baseAddr, NULL);
									tmp0 = getElement(Id, tabCurrent);
									add = getAddr(Id, tabCurrent);
									enterInstruction(LA, tmp0->baseAddr, add);
									enterInstruction(LC, stack[B + 5 + countParam], 0);
									enterInstruction(ST, 0, 0);
									countParam++;
									Token = getToken();
								}
								else {
									printf("thieu IDENT sau COMMA tai dong : %d cot : %d ", LineIndex, RowIndex);
									exit(0);
								}
							}
							else {
								if (Token == IDENT) {
									
										countParam++;
										enterElement(tabCurrent, Id, VAR, 2, 1, tabCurrent->baseAddr, NULL);
										tmp0 = getElement(Id, tabCurrent);
										add = getAddr(Id, tabCurrent);
										enterInstruction(LA, tmp0->baseAddr, add);
										enterInstruction(LC, stack[B + 5 + countParam], 0);
										enterInstruction(ST, 0, 0);
										Token = getToken();
									
								}
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
					}
					else {
						printf("thieu IDENT  tai dong : %d cot : %d ", LineIndex, RowIndex);
						exit(0);
					}
				}
				else {
					if (Token == IDENT) {
						
							enterElement(tabCurrent, Id, VAR, 2, 1, tabCurrent->baseAddr, NULL);
							tmp0 = getElement(Id, tabCurrent);
							add = getAddr(Id, tabCurrent);
							enterInstruction(LA, tmp0->baseAddr, add);
							enterInstruction(LC, stack[B + 5 + countParam], 0);
							enterInstruction(ST, 0, 0);
							countParam++;
							Token = getToken();
						
						while (Token == SEMICOLON) {
							Token = getToken();
							if (Token == VAR) {
								Token = getToken();

								if (Token == IDENT) {
									enterElement(tabCurrent, Id, VAR, 2, 0, tabCurrent->baseAddr, NULL);
									tmp0 = getElement(Id, tabCurrent);
									add = getAddr(Id, tabCurrent);
									enterInstruction(LA, tmp0->baseAddr, add);
									enterInstruction(LC, stack[B + 5 + countParam], 0);
									enterInstruction(ST, 0, 0);
									countParam++;
									Token = getToken();
								}
								else {
									printf("thieu IDENT sau COMMA tai dong : %d cot : %d ", LineIndex, RowIndex);
									exit(0);
								}
							}
							else {
								if (Token == IDENT) {
										countParam++;
										enterElement(tabCurrent, Id, VAR, 2, 1, tabCurrent->baseAddr, NULL);
										tmp0 = getElement(Id, tabCurrent);
										add = getAddr(Id, tabCurrent);
										enterInstruction(LA, tmp0->baseAddr, add);
										enterInstruction(LC, stack[B + 5 + countParam], 0);
										enterInstruction(ST, 0, 0);
										Token = getToken();
									
								}
							}
						};
						tabCurrent->parent->tail->countParam = countParam;
						if (Token != RPARENT) {
							printf("thieu dau dong ngoac tai dong : %d cot : %d ", LineIndex, RowIndex);
							exit(0);
						}
						else {
							Token = getToken();
						}
					}
				}
				enterInstruction(DCT, countParam + 1,0);
			}
			if (Token == SEMICOLON) {
				Token = getToken();
				block();
				cmdList[jump].p = countCmd;
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
				enterInstruction(EP, 0, 0);
			};
			deleteTab(tmp);
			Token = getToken();

		}
		else {
			printf("thieu END tai dong : %d cot : %d %s", LineIndex, RowIndex,TokenTab[Token]);
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
			if (Token == SEMICOLON) {

				tabCurrent = mkTable(NULL);
				Token = getToken();
				enterInstruction(J, 1, 0);
				block();
				if (Token == PERIOD) {
					Token = getToken();
					if (Token != ENDOF) {
						printf("chuong trinh ket thuc boi dau cham. thua du lieu sau dau cham.");
						exit(0);
					};
					printCode();
					
				}
				else {
					printf("thieu dau cham tai dong : %d cot : %d ", LineIndex, RowIndex);
					exit(0);
				}
			}
			else {
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

