
#include"scaner.c"
#include<malloc.h>
typedef struct Data {
	char Name[11];
	TokenType type;
	int offset;
	struct Data *next;
	int countParam;
	int baseAddr;
	void *tb;
	int indexCmd;
} Data;

typedef struct TableSymbol {
	struct TableSymbol *parent;
	Data *head;// pointer head of data array
	Data *tail;//pointer tail of data array
	int baseAddr;
} TableSymbol;

// make tablesymbol
TableSymbol * mkTable(TableSymbol * parent) {
	TableSymbol *tb = (TableSymbol*)malloc(sizeof(TableSymbol));
	tb->parent = parent;
	tb->head = NULL;
	tb->tail = NULL;
	if (parent != NULL) {
		tb->baseAddr = getOffset(parent);
	}
	else {
		tb->baseAddr = 0;
	}
	
	return tb;
};

//check variable is declared?. return type of variable or error NOTDECLARE
TokenType checkDeclar(char Name[11], TableSymbol *tb) {
	Data *tmp; 
	TableSymbol *tb1;
	tb1 = tb; 
	while (tb1 != NULL) {
		tmp = tb1->head;
		while (tmp != NULL) {
			if (strcmp(tmp->Name, Name) == 0) return tmp->type;
			tmp = tmp->next;
		}
		tb1 = tb1->parent;
	}
	return NOTDECLARE;
};
//get ELEMENT
Data * getElement(char Name[11], TableSymbol *tb) {
	Data *tmp;
	TableSymbol *tb1;
	tb1 = tb;
	while (tb1 != NULL) {
		tmp = tb1->head;
		while (tmp != NULL) {
			if (strcmp(tmp->Name, Name) == 0) return tmp;
			tmp = tmp->next;
		}
		tb1 = tb1->parent;
	}
	return NULL;
}
//get address 
int getAddr(char Name[11], TableSymbol *tb) {
	Data *tmp;
	TableSymbol *tb1;
	tb1 = tb;
	int add;
	while (tb1 != NULL) {
		add = 0;
		tmp = tb1->head;
		while (tmp != NULL) {
			if (strcmp(tmp->Name, Name) == 0) break;
			add = add + tmp->offset;
			tmp = tmp->next;
		};
		if(tmp != NULL){
			if (strcmp(tmp->Name, Name) == 0) break;
		};
	
		tb1 = tb1->parent;
	};
	return add;
}

//check declare in procedure
TokenType checkDeclarProc(char Name[11], TableSymbol *tb) {
	Data *tmp;
	tmp = tb->head;
	while (tmp != NULL) {
		if (strcmp(tmp->Name, Name) == 0) return tmp->type;
		tmp = tmp->next;
	}


	return NOTDECLARE;
}

// add emlement into Tablesymbol when declare. return SUCCESS if success else return DECLARED
void enterElement(TableSymbol *tb, char Name[11], TokenType Type, int offset,int countParam, int baseAddr,TableSymbol *tb1) {
	if (checkDeclarProc(Name, tb) != NOTDECLARE) {
		printf("loi bien da duoc khai bao. tai dong %d cot %d", LineIndex, RowIndex);
		exit(0);
	}
	else {
		
		Data *data = (Data*)malloc(sizeof(Data));
		strcpy(data->Name , Name);
		data->offset = offset;
		data->type = Type;
		data->next = NULL;
		data->countParam = countParam;
		data->baseAddr = baseAddr;
		data->tb = (void*)tb1;
		if (tb->head == NULL) {
			tb->head = data;
			tb->tail = data;
		}
		else {
			tb->tail->next = data;
			tb->tail = data;
		}
		
		
	}
	

};



//delete table symbol
void deleteTab(TableSymbol *tb) {
	Data *tmp;
	Data *tmp1;
	tmp = tb->head;
	while (tmp != NULL) {
		tmp1 = tmp;
		tmp = tmp->next;
		free(tmp1);
	};
	free(tb);
	tb = NULL;
	
};

//get offset of table symbol of procedure 
int getOffset(TableSymbol *tb) {
	int offset=0;
	Data *tmp; 
	tmp = tb->head;
	while (tmp != NULL) {
		offset = offset + tmp->offset;
		tmp = tmp->next;
	};
	return offset;

}


//get countParam of proicedure 
int getCountParam(TableSymbol *tb, char Name[11]) {
	Data *tmp;
	TableSymbol *tb1;
	tb1 = tb;
	while (tb1 != NULL) {
		tmp = tb1->head;
		while (tmp != NULL) {
			if (strcmp(tmp->Name, Name) == 0) return tmp->countParam;
			tmp = tmp->next;
		}
		tb1 = tb1->parent;
	}
	return 0;
}

//get ofset of variable 
int getOfsetByName(char Name[11], TableSymbol *tb){
	Data *tmp;
	tmp = tb->head;
	while (tmp != NULL) {
		if (strcmp(tmp->Name, Name) == 0) {
			return tmp->offset;
		}
	}
	return 0;
}


