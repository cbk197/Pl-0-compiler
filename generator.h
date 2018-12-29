#include<string.h>
typedef enum{
	LA=0, LV, LC, LI, INT, DCT, J, FJ, HLT, ST, CAL, EP, EF, RC, RI, WRC, WRI, WLN, ADD, SUB, MUL, DIV, NEG, CV, EQ, NE, GT, LT, GE, LE
}OpCode;

char TabOp[][10] = {
	"LA", "LV", "LC", "LI", "INT", "DCT", "J", "FJ", "HLT", "ST",
	 "CALL", "EP", "EF", "RC", "RI", "WRC", "WRI", "WLN", "ADD", "SUB", "MUL", "DIV", "NEG", "CV", "EQ", "NE", "GT", "LT", "GE", "LE"
};

typedef struct Instruction{
	OpCode op;
	int p;
	int q;
}Instruction;

Instruction cmdList[1000];
int countCmd = 0;
int stack[1500];

int PC = 0,B = 0, T = 1000;


//function enter instruction
void enterInstruction(OpCode op, int p, int q){
	if (countCmd < 1000) {
		cmdList[countCmd].op = op;
		cmdList[countCmd].p = p;
		cmdList[countCmd].q = q;
	};
	countCmd++;
	
	switch(op){
		case LA: {
			T = T + 1; 
			stack[T] = p + q;
			break;
		};
		case LV : {
			T = T + 1;
			stack[T] = stack[p + q];
			break;
		};
		case LC : {
			T = T+1;
			stack[T] = p;
			break;
		}
		case LI : {
			stack[T] = stack[stack[T]];
			break;
		}
		case INT : {
			T = T + p;
			break;
		}
		case DCT : {
			T = T- p;
			break;
		}
		case J : {
			PC = p;
			break;
		}
		case FJ : {
			if(stack[T] == 0){
				PC = p;
			};
			T = T - 1;
			break;
		}
		case HLT : {
			
			break;
		}
		case ST : {
			stack[stack[T-1]] = stack[T];
			T = T -2;
			break;
		}
		case CAL : { // p luon la 0 va q la dia chi ham con
			stack[T+2] = B;
			stack[T+3] = PC;
			stack[T+4] = p;
			B = T + 1;
			T = T + 5;
			PC = q;
			break;
		}
		case EP : {
			T = B-1;
			PC = stack[T+2];
			B = stack[B+1];
			break;
		}
		case EF : {
			T = B;
			PC = stack[B + 2];
			B = stack[B+1];
			break;
		}
		case RC : {
			T = T -1;
			break;
		}
		case RI : {
			T = T -1;
			break;
		}
		case WRC : {
			T = T -1;
			break;
		}
		case WRI : {
			T = T -1 ;
			break;
		}
		case WLN : {
			break;
		}
		case ADD : {
			T = T-1;
			stack[T] = stack[T] + stack[T+1];
			break;
		}
		case SUB : {
			T = T- 1;
			stack[T] = stack[T] - stack[T+1];
			break;
		}
		case MUL : {
			T = T- 1;
			stack[T] = stack[T] * stack[T+1];
			break;
		}
		case DIV : {
			T = T- 1;
			stack[T] = stack[T] / stack[T+1];
			break;
		}
		case NEG : {
			stack[T] = - stack[T];
			break;
		}
		case CV : {
			stack[T+1] = stack[T];
			T = T+ 1;
			break;
		}
		case EQ : {
			T = T -1;
			if(stack[T]== stack[T+1]){
				stack[T]=1;
			}else{
				stack[T]=0;
			}
			break;
		}
		case NE : {
			T = T -1;
			if(stack[T]== stack[T+1]){
				stack[T]=0;
			}else{
				stack[T]=1;
			}
			break;
		}
		case GT : {
			T = T -1;
			if(stack[T] > stack[T+1]){
				stack[T]=1;
			}else{
				stack[T]=0;
			}
			break;
		}
		case LT : {
			T = T -1;
			if(stack[T] < stack[T+1]){
				stack[T]=1;
			}else{
				stack[T]=0;
			}
			break;
		}
		case GE : {
			T = T -1;
			if(stack[T] > stack[T+1] || stack[T] == stack[T+1]){
				stack[T]=1;
			}else{
				stack[T]=0;
			}
			break;
		}
		case LE : {
			T = T -1;
			if(stack[T] < stack[T+1] || stack[T] == stack[T+1]){
				stack[T]=1;
			}else{
				stack[T]=0;
			}
			break;
		}
	}
}

//in result generator code


void printLine(OpCode op, int p, int q, int i) {
	switch (op) {
	case LA: {
		printf("\n%d %s %d ,%d ",i, TabOp[op], p, q);
		break;
	};
	case LV: {
		printf("\n%d %s %d ,%d ",i, TabOp[op], p, q);
		break;
	};
	case LC: {
		printf("\n%d %s %d ",i, TabOp[op], p);
		break;
	}
	case LI: {
		printf("\n%d %s ",i, TabOp[op]);
		break;
	}
	case INT: {
		printf("\n%d %s %d ",i, TabOp[op], p);
		break;
	}
	case DCT: {
		printf("\n%d %s %d  ",i, TabOp[op], p);
		break;
	}
	case J: {
		printf("\n%d %s %d  ",i, TabOp[op], p);
		break;
	}
	case FJ: {
		printf("\n%d %s %d ",i, TabOp[op], p);
		break;
	}
	case HLT: {
		break;
	}
	case ST: {
		printf("\n%d %s",i, TabOp[op]);
		break;
	}
	case CAL: { // p luon la 0 va q la dia chi ham con
		printf("\n%d %s %d ,%d ",i, TabOp[op], p, q);
		break;
	}
	case EP: {
		printf("\n%d %s",i, TabOp[op]);
		break;
	}
	case EF: {
		printf("\n%d %s ",i, TabOp[op]);
		break;
	}
	case RC: {
		printf("\n%s", TabOp[op]);
		break;
	}
	case RI: {
		printf("\n%d %s ",i, TabOp[op]);
		break;
	}
	case WRC: {
		printf("\n%d %s ",i, TabOp[op]);
		break;
	}
	case WRI: {
		printf("\n%d %s  ",i, TabOp[op]);
		break;
	}
	case WLN: {
		printf("\n%d %s ",i, TabOp[op]);
		break;
	}
	case ADD: {
		printf("\n%d %s ",i, TabOp[op]);
		break;
	}
	case SUB: {
		printf("\n%d %s ",i, TabOp[op]);
		break;
	}
	case MUL: {
		printf("\n%d %s",i, TabOp[op]);
		break;
	}
	case DIV: {
		printf("\n%d %s  ",i, TabOp[op]);
		break;
	}
	case NEG: {
		printf("\n%d %s  ", i, TabOp[op]);
		break;
	}
	case CV: {
		printf("\n%d %s  ", i, TabOp[op]);
		break;
	}
	case EQ: {
		printf("\n%d %s  ", i, TabOp[op]);
		break;
	}
	case NE: {
		printf("\n%d %s  ", i, TabOp[op]);
		break;
	}
	case GT: {
		printf("\n%d %s  ", i, TabOp[op]);
		break;
	}
	case LT: {
		printf("\n%d %s  ", i, TabOp[op]);
		break;
	}
	case GE: {
		printf("\n%d %s  ", i, TabOp[op]);
		break;
	}
	case LE: {
		printf("\n%d %s  ", i, TabOp[op]);
		break;
	}
	}
};
void printCode() {
	int i;
	for (i = 0; i < countCmd; i++) {
		printLine(cmdList[i].op, cmdList[i].p, cmdList[i].q, i);

	}
};
