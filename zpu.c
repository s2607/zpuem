#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define IM 0b10000000
#define WS 4
char mem[0x1000];

typedef struct {
	int * sp;
	int idm;
	char * ip;
	int irup;
	int eq;

}alu ;
alu  mainalu;
void loadrom() {
	mem[0]=0b11111111;
	mem[1]=0b11111111;
}
void machinestate() {
	printf("SP:   %#08x\nIDM:  %d\nIP:   %#08x\nIRUP: %d\nEQ:   %d\nSTOP: %#08x\n"
		,(unsigned int)(mainalu.sp-(int *)&mem[0]),mainalu.idm,
		(unsigned int)(mainalu.ip-&mem[0]),mainalu.irup,
		mainalu.eq, *mainalu.sp);
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
	printf("ins_im\n");
		//Maybe this should be a macro?
	*(mainalu.sp)=((*mainalu.sp)<<7);
	if(!mainalu.idm)
		mainalu.idm=1;
	int a=(int )((*mainalu.ip)&(~IM));
	*mainalu.sp=a&(0x000000FF)|(*mainalu.sp);//This line may have issues
	mainalu.ip=mainalu.ip+1;
}
void decode() {
	/*switch(mem[mainalu.ip]) {
		case 0 : debug();break;
		default : illigal();
	}*/
	char ins = *mainalu.ip;
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
	mainalu.sp=(int *)&mem[0x0ffb];
	mainalu.ip=&mem[0x0000];
	mainalu.idm=0;
	loadrom();
	printf("starting simulation\n");
	start();
	return 0;
}
