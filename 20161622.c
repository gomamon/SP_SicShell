#include "20161622.h"
//#include "Dir.h"
 




int Input();
void Help();
int Dir();
int i;

//int History();

//struct 


char command[ COMMANDSIZE ];
char com[COMMANDSIZE];

void Init(){
	for( i=0;i<MAX_PARAMETER ; i++){
		par[i][0] = '\0';

	}
}


int main(){
	while(1){
		Init();
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
	int tmp_i=0;	//index
	

	while(1){
		scanf("%c",&tmp_ch);
		if(tmp_ch == '\n')	break;
		command[tmp_i++] = tmp_ch;

		if(tmp_i>=COMMANDSIZE-1)	printf("Your command is too long!\n");
		//error message
	}
	command[tmp_i] = '\0';

	if(!strcmp(command,str_h[0]) || !strcmp(command, str_h[1]))			return H;
	else if(!strcmp(command,str_d[0]) || !strcmp(command, str_d[1]))	return D;
	else if(!strcmp(command,str_q[0]) || !strcmp(command, str_q[1]))	return Q;
	else if(!strcmp(command,str_hi[0]) || !strcmp(command, str_hi[1]))	return HI;
	
	else if(!strcmp( command, str_reset))						return RESET;
	else if(!strcmp( command, str_opcodemnemonic))			return OPCODEMNEMONIC;
	else if(!strcmp( command, str_opcodelist))				return OPCODELIST;
	
	else if(strrchr(command, ' ')!=NULL){
		//string token
		i=-1;
		tk[++i] =strtok(command, " ,");
		while(tk[i] != NULL){
			if(i==0) strcpy(com,tk[i]);
			else if(i<4) strcpy(par[i-1],tk[i]);
			else if(i>=4) return -1;
			tk[++i] = strtok(NULL, " ,");
		}
		printf("%s : %s %s %s",com,par[0],par[1],par[2]);		////////////debug
		//command check

		if(!strcmp(com,str_du[0])||!strcmp(com,str_du[1])){
			if(par[0]==NULL)		return DU;
			else if(!IsHex(par[0])) return -1;
			else{
				if( (IsHex(par[1]) && par[2]==NULL) || 
					par[1]==NULL)	return DU;
				else				return -1;
			}
		}//processing command "dump"

		else if(!strcmp(com,str_e[0]) || !strcmp(com,str_e[1])){
			if(!IsHex(par[0]) || !IsHex(par[1]) || par[2]!=NULL)
				return -1;
			else {
				strcpy(addr,par[0]);
				strcpy(val,par[1]);
				return DU;
			}
		}

		else if(!strcmp(com,str_f[0]) || !strcmp(com,str_f[1]) ){
			if(!IsHex(par[0]) || !IsHex(par[1])
					|| !IsHex(par[2]) || par[3]!=NULL)
				return -1;
			else{
				return F;
			}
		}
	
	}//processing command "edit"

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

int IsHex( char *ckstr ){

}

int Dir(){
	

}

