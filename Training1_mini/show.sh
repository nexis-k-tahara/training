#include<stdio.h>
#include<stdlib.h>


void main(){
	File *file;
	int i;
	char rdata[256]
	
	file = fopen("students.dat","rb");
	if(file == NULL){
		printf("ファイルオープンに失敗しました。\n")
		exit(1);
	}
	fread(rdata,sizeof(char),sizeof(rdata),file);
	fclose(file);
	for(i=0;i<sizeof(radata);i++){
		printf("%s",rdata[i]);
	}
	printf("\n");


}
