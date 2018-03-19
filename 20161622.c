#include "20161622.h"
#include "Dir.h"
#define COMMANDSIZE 55
 
int Input();

void Help();
int Dir();

//int History();

//struct 


char command[ COMMANDSIZE ];
char *com;


char *start;
char *end;
char *left;

int IsError(int mode, char *ckstr );

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
				return 0;
				break;
			case 4:
//				History();
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
		//error message
	}
	command[tmp_i] = '\0';

	if(!strcmp(command,help[0])||!strcmp(command, help[1])) return 1;
	else if(!strcmp(command,dir[0]) || !strcmp(command, dir[1])) return 2;
	else if(!strcmp(command,quit[0])|| !strcmp(command, quit[1])) return 3;
	else if(!strcmp(command,history[0])||!strcmp(command,history[1])) return 4;
	else if(strrchr(command, ',')!=NULL){
		com=strtok(command," ");

		if(!strcmp(com,dump[0])|| !strcmp(com,dump[1])){
			printf("hhh");
			start = strtok(command, " ");
			if(IsError(1, start)) return -1;
			if(start != NULL){
				end = strtok(command, " ");
				if(end != NULL && IsError(1, end) ) return -1;

				left = strtok(command, " ");
				if(left != NULL ) return -1;
			}
			printf("s: %s e: %s l: %s\n",start,end,left);
		}

	}

	else return -1;
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

int IsError(int mode, char *ckstr ){
	/*
	 *mode 1: is dec?
	 *mode 2: is hex?
	 */
	switch(mode){
		case 1:
			if(ckstr[0]>='0'&&ckstr[0]<='9');
			break;
		case 2:

			break;

	}
}

int Dir(){
	

}

