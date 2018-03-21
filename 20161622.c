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
void Dump();
void Edit();
void Fill();
void Reset();
//struct 

//int i;

//History();
his_node* head = NULL;
his_node* rear = NULL;

char command[ COMMANDSIZE ];
char com[COMMANDSIZE];



void Init(){
	int i;
	for( i=0;i<MAX_PARAMETER ; i++){
		par[i][0] = '\0';
	}
}

void MemInit(){
	int i=0,j,k;
	char tmpstr1[] ="000000";
	char tmpstr2[]="00";
	int mod,div;

//	printf("hi");

	for(i =0; i<65536 ; i++){
		DecToHex(tmpstr1, i*16);
		strcpy(mem_addr[i],tmpstr1);
	}
	for(i=0; i<MAX_MEMORY ; i++)
		strcpy(mem[i],tmpstr2);

		
}

void DecToHex(char*hex,int dec){
	char init[] ="000000";
	char src;
	int i=5, mod;

	while(1){
	//	printf("wwwwww");
		if(dec==0) break;
		mod = dec%16;
		dec/=16;
		src = (mod>9) ? mod-10+'A' : mod+'0';
		init[i--] = src;
	}
	strcpy(hex,init);
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
	MemInit();
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
			case DU:
				Dump();
				break;
			case E:
				Edit();
			case F:
				Fill();
			case RESET:
				Reset();
		}
	}
}


int Input(){
	char tmp_ch; 	//input char
	int tmp_i=0;	//index
	int i;	

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
	
	else if(!strcmp(command,str_du[0]) || !strcmp(command,str_du[1]) ) return DU;
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
				return E;
			}
		}

		else if(!strcmp(com,str_f[0]) || !strcmp(com,str_f[1]) ){
			printf("||%s %s %s||\n",par[0],par[1],par[2]);
			if(!IsHex(par[0]) || !IsHex(par[1])
					|| !IsHex(par[2]) || par[3][0]!='\0')
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
	int i;
	//printf("%s:%d\n",ckstr,(int)strlen(ckstr));
	for(i=0; i<(int)strlen(ckstr); i++){
		if(ckstr[i]<'0' || (ckstr[i] > '9' && ckstr[i] <'A') || (ckstr[i]>'F'&&ckstr[i]<'a') || ckstr[i]>'f')
			return 0;
	}
	return 1;
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

void Dump(){
	int i,j;
	int s=0, e=0;
	char start[10];
	char end[10];
	strcpy(start, par[0]);
	strcpy(end, par[1]);
	
	/****processing start, end******/
	if(start[0] == '\0'){
		s = last_addr+1;
		e = last_addr+160;
	}
	else{
		s = HexToDec(start);
		if(end[0]=='\0') e = s+159; 
		else	e = HexToDec(end);
	}
	printf("s: %d e : %d\n", s,e);
	
	/******print memory******/
	for(i=s ;i<=e; i++){
		if(i==s){
			printf("%s ", mem_addr[i/16]);
			if(i%16!=0){
				for(j=0 ;j<i%16 ; j++)
					printf("   ");
			}
			printf("%s ",mem[i]);
			if(i%16==15) PrintASCII(s,e,i-i%16);
			continue;
		}
		if(i%16==0)
			printf("%s ", mem_addr[i/16]);
		printf("%s ",mem[i]);
		if(i%16==15) PrintASCII(s,e,i-i%16);
	}	//print from s to e
	if(e%16!=15){ 
		
		for(; i<e-e%16+16; i++)
			printf("   ");
		PrintASCII(s,e,e-e%16);
	}	//print rest
	last_addr = e;
}


void Edit(){
	char val[8], addr[8];
	strcpy(addr, par[0]);
	strcpy(val, par[1]);
	
	strcpy(mem[HexToDec(addr)],val);
}

void PrintASCII(int s, int e, int addr ){
	int i;
	int dec;
	printf("; ");
	for( i=addr; i < addr+16 ; i++){
		dec = HexToDec(mem[i]);
		if( (32<=dec && dec<=126) && (s<=i&&i<=e) )	// 20~7E 
			printf("%c",dec);
		else	 printf(".");
	}
	puts("");

}

void Fill(){
	int i,s,e;
	char start[8],end[8],value[8];
	strcpy(start, par[0]);
	strcpy(end, par[1]);
	strcpy(value, par[2]);
	
	printf("%s",value);
	s = HexToDec(start);
	e = HexToDec(end);
	for(i=s; i<=e ; i++){
		strcpy(mem[i],value);
	}
}

int HexToDec(char* hex){
	int i;
	int tmp, dec=0;
	for(i= 0 ; i< (int)strlen(hex); i++){
		if(hex[i]=='\0') break;
		tmp=hex[i];
		if('a'<=tmp && tmp<='f') tmp = tmp - 'a' + 10;
		else if('A'<=tmp && tmp<='F') tmp = tmp - 'A' +10;
		else if('0'<=tmp && tmp<= '9') tmp -= '0';
		dec *= 16;
		dec += tmp;
	}
	return dec;
}

void Reset(){
	int i=0;
	char tmpstr[] = "00";
	for(i=0 ; i<MAX_MEMORY ; i++){
		strcpy(mem[i],tmpstr);
	}
}
