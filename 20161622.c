#include "20161622.h"
#define COMMANDSIZE 55


void Help();
int Input();
int Dir();
int Quit();


char command[ COMMANDSIZE ];
char com_init[COMMANDSIZE]={'\0', };

int main(){
	while(1){
		printf("sicsim> ");
		switch(Input()){
			case 0: 
				continue;
				break;
			case 1:
				Help();
				break;
			case 2:
				Dir();
				break;
			case 3:
				Quit();
				break;

		}
	}
}


int Input(){
	char tmp_ch; 	//input char
	int tmp_i=0;	//command index
	
	

	while(1){
		scanf("%c",&tmp_ch);
		if(tmp_ch == '\n')	break;
		command[tmp_i++] = tmp_ch;

		if(tmp_i>=COMMANDSIZE-1)	printf("Your command is too long!\n");
	}
	command[tmp_i] = '\0';


	if(!strcmp(command,h)||!strcmp(command, help)) return 1;
	else if(!strcmp(command,d)||!strcmp(command,dir)) return 2;
	else if(!strcmp(command,hi)||!strcmp(command,history)) return 3;
	else if(!strcmp(command,du)||!strcmp(command,dump)) return 4;
//	else if() return 5;
//	else if() return 6;
//	else if() return 7;
//	else if() return 8;
	else return 0;
}

void Help(){
	printf("h[elp]\n"
			"d[ir]\n"
			"q[uit]\n"
			"hi[story]\n"
			"du[mp] [start, end]\n"
			"e[dit] address, value\n"
			"f[ill] start, end, value\n"
			"reset\n"
			"opcode mnemonic\n"
			"opcodelist\n"
			);
}

int Dir(){

}

int Quit(){

}
