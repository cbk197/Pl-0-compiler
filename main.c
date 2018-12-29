#include"parser.c"
#ifndef SCANER
#include"scaner.c"
#endif
#include<stdio.h>

int main(int argc,char * argv[]){
	
	if(argc != 2){
		printf("loi tham so dau vao!\n");
		return 0;
	};
	fflush(stdin);
	f = fopen(argv[1],"rt");
	if(f==NULL){
		perror("Error");
		return 0;
	};
	Ch =' ';
	Token = getToken();
	program();

	fclose(f);
	return 0;
}
