#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define IM 0b10000000
#define SOP 0b11100000
#define WS 4
char mem[0x1000];
char *uout= &mem[0xFFFF];//heh

typedef struct {
	int * sp;//this must be 32 bits wide TODO:use uint_size
	int idm;
	char * ip;
	int irup;
	int eq;

}alu ;
alu  mainalu;
void loadrom() {
	mem[0]=0b11111111;
	mem[1]=0b11111111;
	mem[2]=0b11111111;
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
int popint() {
	int r=*(mainalu.sp);
	*(mainalu.sp)+=1;
	return r;
}
void pushint(int a) {
	*(mainalu.sp)=a;
	mainalu.sp+=1;
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
void ins_storesp(){//if you're like me you're probably wondering what on earth this does,
	//I belive it is for local variables stored on the stack
	int v=(*mainalu.sp);
	mainalu.sp+=1;
	*(mainalu.sp+(((*mainalu.ip)&SOP)*4))=v;
}//TODO: test these, we lack the instructions to right now
void ins_loadsp(){//see storesp
	int v= *(mainalu.sp+(((*mainalu.ip)&SOP)*4));
	mainalu.sp-=1;
	*(mainalu.sp)=v;
}
void ins_load() {
	//*mainalu.sp=mem[(*mainalu.sp&0xFFFFFFFD)*4];
	pushint(*(&mem+((popint()&0xFFFFFFFD)*4)));
}
void ins_store() {
	*mainalu.sp=mem[(*mainalu.sp&0xFFFFFFFD)*4];
}
void ins_addsp() {
}
void ins_poppc() {
	mainalu.ip=(popint());
}
void ins_add() {
	pushint(popint()+popint());
}
void ins_or() {
	pushint(popint()|popint());
}
void ins_not() {
	pushint(~popint());
}
void ins_flip() {
}
void opless() {
	switch(*mainalu.ip) {
	case 0b00001000:ins_load();break;
	case 0b00001100:ins_store();break;
	case 0b00000100:ins_poppc();break;
	case 0b00000101:ins_add();break;
	case 0b00000111:ins_or();break;
	case 0b00001001:ins_not();break;
	case 0b00001010:ins_flip();break;
		//default: illigal();

	}
}
void ins_em() {
	illigal();
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
		}else {
			mainalu.idm=0;
			switch(ins& SOP){
				case 0b01000000:ins_storesp();break;
				case 0b01100000:ins_loadsp();break;
				case 0b00100000:ins_em();break;
				case 0b00010000:ins_addsp();break;
				case 0:opless();break;
				//default: illigal();
			}
		}
	}	
	if(!dec)
		illigal();
	//mainalu.ip=mainalu.ip+1;
	if(*uout)
		putchar(*uout);
	uout=0;
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
