#include "20161622.h"
//#include "Dir.h"
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>
typedef struct HistoryNode{
	char data[COMMANDSIZE];
	struct HistoryNode *next;
}his_node;



int Input();

void Help();
int Dir();
int History();

//struct 

int i;

//History();
his_node* head = NULL;
his_node* rear = NULL;

char command[ COMMANDSIZE ];
char com[COMMANDSIZE];



void Init(){
	for( i=0;i<MAX_PARAMETER ; i++){
		par[i][0] = '\0';

	}
}

void AddHistory(){
	his_node *new = (his_node*)malloc(sizeof(his_node));
	strcpy(new->data, command);
	new->next = NULL;

	if(head == NULL){
		head = new;
		rear = new;
	}
	else{
		rear->next = new;
		rear = new;
	}
}

int main(){
	int mode;
	while(1){
		Init();
		printf("sicsim> ");
		mode = Input();
		printf("mode: %d\n",mode);
		//printf("%d",Input());
		if(mode!=-1)AddHistory();	
		switch(mode){
			case -1: 
				continue;
				break;
			case H:
				Help();
				break;
			case D:
				Dir();
				break;
			case Q:
				return 0;
				break;
			case HI:
				History();
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
			if(par[0][0]=='\0')		return DU;
			else if(!IsHex(par[0])) return -1;
			else{
				if( (IsHex(par[1]) && par[2][0]=='\0') || 
					par[1][0]=='\0')	return DU;
				else				return -1;
			}
		}//processing command "dump"

		else if(!strcmp(com,str_e[0]) || !strcmp(com,str_e[1])){
			if(!IsHex(par[0]) || !IsHex(par[1]) || par[2][0]!='\0')
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
	if(strlen(ckstr) ==2){
		for(i=0; i<2; i++){
			if(ckstr[i]<'0' || (ckstr[i] > '9' && ckstr[i] <'A') || ckstr[i]>'Z')
				return 0;
		}
		return 1;
	}
	else return 0;
}

int Dir(){
	struct dirent *dir_ent;		//directory entry pointer
	struct stat dir_stat;		//directory 
	DIR* dir_p = opendir("."); 	//directory pointer
	
	if(dir_p== NULL) return -1;
	
	while(dir_ent = readdir(dir_p)){
		stat(dir_ent->d_name, &dir_stat);

		if(S_ISDIR(dir_stat.st_mode))
			printf("%s/\n",dir_ent->d_name);
		else if(dir_stat.st_mode & S_IXUSR)
			printf("%s*\n",dir_ent->d_name);
		else
			printf("%s\n",dir_ent->d_name);

	}
}

int History(){
	his_node* hp;
	int cnt=0;
	if(head==NULL) return 0;
	
	for(hp = head ; hp!= NULL ;hp = hp->next){
		printf("%-3d %s\n",++cnt,hp->data);
	}
	
	
	
}
