#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define IM 0b10000000
char mem[4000];

typedef struct {
	int sp;
	int idm;
	int ip;
	int irup;
	int eq;
}alu ;
alu  mainalu;
void loadrom() {
	mem[0]=0b11111111;
}
void machinestate() {
	printf("SP:   %#08x\nIDM:  %d\nIP:   %#08x\nIRUP: %d\nEQ:   %d\nSTOP: %#08x\n"
		,mainalu.sp,mainalu.idm,mainalu.ip,mainalu.irup,
		mainalu.eq,mem[mainalu.sp]);
}
void illigal() {
	puts("ILLIGAL INSTRUCTION");
	machinestate();
	exit(1);
}
void debug() {
	//illigal();
	puts("NO DEBUGGER");
	machinestate();
	exit(0);
}
void ins_im(){
	mem[mainalu.sp]=mem[mainalu.ip]&(~IM);
	mainalu.ip=mainalu.ip+1;
}
void decode() {
	/*switch(mem[mainalu.ip]) {
		case 0 : debug();break;
		default : illigal();
	}*/
	char ins = mem[mainalu.ip];
	int dec=0;
	if(ins==0){
		debug();
		dec=1;	
	}
	else{
		if(ins &IM){
			ins_im();
			dec=1;
		}
	}	
	if(!dec)
		illigal();
	//mainalu.ip=mainalu.ip+1;
}
void start() {
	while(1) {
		decode();
	}
}
int main()
{
	mainalu.sp=0;
	mainalu.idm=0;
	loadrom();
	start();
	return 0;
}
