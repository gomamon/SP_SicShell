#include "20161622.h"
//#include "Dir.h"
#include <dirent.h>
#include <sys/stat.h>
#include <stdlib.h>

int Input();

void Help();
int Dir();
int History();
void Dump();
void Edit();
void Fill();
void Reset();

int OpcodeMnemonic();
void Opcodelist();
//struct 

//int i;




void Init(){
	int i;
	for( i=0;i<MAX_PARAMETER ; i++){
		par[i][0] = '\0';
	}

}

void MemInit(){
	int i=0,j,k;
	char tmp_str[]="00";
	int mod,div;

	for(i=0; i<MAX_MEMORY ; i++)
		strcpy(mem[i],tmp_str);
}

int MakeHashTable(){
	FILE *fp=fopen("opcode.txt","r");
	char in1[5],in2[10],in3[5];
	if(!fp)	return -1;

	for(int i=0; i<HASH_SIZE ; i++){
		hash[i].size = 0;
		hash[i].head = NULL;
		hash[i].rear = NULL;
	}

	while( fscanf(fp,"%s %s %s",in1,in2,in3)!=EOF ){
		MakeOpcodeList(in1,in2,in3);
	}
}

void MakeOpcodeList(char* opcode, char*mnemonic, char* mode){
	int i,idx=0;
	opcode_list* new = (opcode_list*)malloc(sizeof(opcode_list));

	for(i=0 ; i<(int)strlen(mnemonic) ; i++)
		idx += mnemonic[i];

	idx%=HASH_MOD;

	strcpy( new->opcode, opcode);
	strcpy( (new->mnemonic), mnemonic);
	strcpy( new->form , mode);
	new->next = NULL;

	hash[idx].size++;
	if(hash[idx].head==NULL){
		hash[idx].head = new;
		hash[idx].rear = new;
	}
	else{
		hash[idx].rear->next = new;
		hash[idx].rear = new;
	}
}


void AddHistory(){
	his_node *new = (his_node*)malloc(sizeof(his_node));
	strcpy(new->data, command);
	new->next = NULL;

	if(his_head == NULL){
		his_head = new;
		his_rear = new;
	}
	else{
		his_rear->next = new;
		his_rear = new;
	}
}

void FreeHistory(){
	his_node *tmp_p,*del_p;

	if(his_head==NULL) return;
	else{
		tmp_p = his_head;
		while(1){
			if(tmp_p==NULL) return;
			del_p = tmp_p;
			tmp_p = tmp_p->next;
			free(del_p);
		}
	}
}
void FreeHash(){
	opcode_list *tmp_p, *del_p;
	for(int i=0 ;i<HASH_SIZE ; i++){
		tmp_p = hash[i].head;
		while(1){
			if(tmp_p == NULL) return;
			del_p = tmp_p;
			tmp_p = tmp_p->next;
			free(del_p);
		}
	}
}

int main(){
	int mode;
	MemInit();
	MakeHashTable();
	while(1){
		Init();
		printf("sicsim> ");
		mode = Input();
		//printf("mode: %d\n",mode);
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
				FreeHistory();
				FreeHash();
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
				break;
			case F:
				Fill();
				break;
			case RESET:
				Reset();
				break;
			case OPCODELIST:
				Opcodelist();
				break;
			case OPCODEMNEMONIC:
				OpcodeMnemonic();
				break;


		}
	}
}
int ProcessCommand(char* cmd){
	int i, j;
	char *tk; //string token
	char tmp[COMMANDSIZE];

	/**Command TOKEN**/
	i=-1;
	strcpy(tmp, cmd);

	tk = strtok(tmp, " \t");
	if(!tk) return -1;
	do{
		if(i==-1) strcpy(cmd, tk);
		else strcpy(par[i],tk);
		i++;
		if(i>=MAX_PARAMETER){
			printf("UNKNOWN COMMAND!\n");
			return -1;
		}
	}while(tk = strtok(NULL, " \t"));	

	
	if(cmd[0] == '\0') return -1;		

	/**Check command {H = 0, D = 1, q = 2, ...  ,OPCODEMNEMONIC = 9 }**/
	for(i=0; i<10 ;i++)
		for(j=0 ; j<2 ; j++)
			if(!strcmp(cmd, command_list[i][j]))
				return CheckParameter(i);
	
	printf("UNKNOWN COMMAND!\n");
	return -1;
}


int CheckParameter(int cmd_num){
	switch(cmd_num){
		case H:
		case D:
		case Q:
		case HI:
		case RESET:
		case OPCODELIST:
			if(par[0][0]=='\0')
				return cmd_num;
				 
			printf("UNKNOWN COMMAND!\n");
			return -1;
			break;

		case DU:
			if(par[0][0] == '\0')
				return cmd_num;
			else if( IsHex(par[0]) ){
				if( IsAddrLimitERROR( HexToDec(par[0]) ) )	return -1;
				if(par[1][0]=='\0') 
					return cmd_num;
				else if( par[1][0] == ',' && IsHex(par[2]) && par[3][0]=='\0'){
					if( IsAddrLimitERROR( HexToDec(par[2]) ) ) return -1;
					if( IsAddrERROR( HexToDec(par[0]) ,HexToDec(par[2]) ) )	return -1;
					return cmd_num;
				}
			}
			printf("UNKNOWN COMMAND!\n");
			return -1;
			break;
		case E:
			if(IsHex(par[0]) && par[1][0] ==',' && IsHex(par[2]) && par[3][0] == '\0'){
				if(IsAddrLimitERROR( HexToDec(par[0]) ))return -1;
				if( IsDataLimitERROR( HexToDec(par[2]) ) )return -1;
				return E;
			}
			printf("UNKNOWN COMMAND!\n");
			return -1;
			break;
		case F:
			if(IsHex(par[0]) && par[1][0] ==',' && IsHex(par[2]) 
					&& par[3][0] ==',' && IsHex(par[4]) && par[5][0]=='\0'){

				if(IsAddrLimitERROR( HexToDec(par[0]) ))return -1;
				if( IsAddrLimitERROR( HexToDec(par[2]) ) )return -1;
				if( IsAddrERROR( HexToDec(par[0]), HexToDec(par[2]) ) )return -1;
				if( IsDataLimitERROR( HexToDec(par[4]) ) )	return -1;
				return F;
			}
			printf("UNKNOWN COMMAND!\n");
			return -1;
			break;
		case OPCODEMNEMONIC:
			if(par[0][0]=='\0'){
				printf("No input mnemonic\n");
				return -1;
			}
			else if(par[1][0]=='\0'){
				return OPCODEMNEMONIC; 
			}
			printf("UNKNOWN COMMAND!\n");
			return -1;
			break;
		default:
			printf("UNKNOWN COMMAND!\n");
			return -1;
			break;
	}
}
int IsDataLimitERROR(int data){
	//function to check data limit
	//if data exceed limit(0x00 ~ 0xff), then return 1
	//if not, return 0
	
	if(data<MAX_DATA)
		return 0;
	else{
		printf("Exceede limit!\n");
		return 1;
	}
}
int IsAddrERROR(int s, int e){
	//Function to check start address and end address
	//If s is bigger than e, then return 1
	//If not, return 0
	
	if( e < s){
		printf("End address shouldn't be less than Start address!\n");
		return 1;
	}
	return 0;
}
int IsAddrLimitERROR(int addr){
	//function to check adrress limit
	//if adddr exceed limit(0x00000 ~ 0xfffff), then return 1
	//if not, return 0
	
	if(addr < MAX_MEMORY)
		return 0;
	else{
		printf("Exceeded limit!\n");
		return 1; 
	}
}


int IsHex( char *ckstr ){
	int i;
	if(ckstr[0] == '\0') return 0;
	for(i=0; i<(int)strlen(ckstr); i++){
		if(ckstr[i]<'0' || (ckstr[i] > '9' && ckstr[i] <'A') || (ckstr[i]>'F'&&ckstr[i]<'a') || ckstr[i]>'f')
			return 0;
	}
	return 1;
}


int Input(){
	char ch_in; 	//input char
	int i=0,j=0;	//i:index for cmd, j:index for command	
	char cmd[COMMANDSIZE];	//String to process command efficiently

	/**get command**/
	while(1){
		scanf("%c",&ch_in);
		if( ch_in == '\n')	break;
		if( ch_in == ','){
			cmd[i++] = ' '; 
			cmd[i++] = ','; 
			cmd[i++] = ' ';
			command[j++] = ch_in;
		}
		else{
			cmd[i++] = ch_in;
			command[j++] = ch_in;
		}
		if(i>=COMMANDSIZE-1){
			puts("UNKNOWN COMMAND");	
			return -1;
		}
	}
	cmd[i] = '\0';
	command[j] = '\0';
	return ProcessCommand(cmd);

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
	if(his_head==NULL) return 0;

	for(hp = his_head ; hp!= NULL ;hp = hp->next){
		printf("%-3d %s\n",++cnt,hp->data);
	}		
}

void Dump(){
	int i,j;
	int s=0, e=0;
	char start[10];
	char end[10];
	strcpy(start, par[0]);
	strcpy(end, par[2]);

	/****processing start, end******/
	if(start[0] == '\0'){
		if(last_addr == MAX_MEMORY-1) s = 0;
		else s = last_addr+1;
		e = s+159;
	}
	else{
		s = HexToDec(start);
		if(end[0]=='\0') e = s+159; 
		else	e = HexToDec(end);
	}
	
	if(e>= MAX_MEMORY){
		e = MAX_MEMORY-1;
	}

	PrintData(s,e);
	last_addr = e;
}

void PrintData(int s,int e){
	//Function to print according to format the data in memory(mem) at address(addr)
	int i,j;

	for(i=s ;i<=e; i++){
		if(i==s){
			printf("%05X ",i);
			if(i%16!=0){
				for(j=0 ;j<i%16 ; j++)
					printf("   ");
			}
			
			printf("%02X ",HexToDec(mem[i]));
			
			if(i%16==15) PrintASCII(s,e,i-i%16);
			continue;
		}

		if(i%16==0)
			printf("%05X ",i);

		printf("%02X ",HexToDec(mem[i]));
		
		if(i%16==15) PrintASCII(s,e,i-i%16);
	}	//print from s to e
	if(e%16!=15){ 

		for(; i<e-e%16+16; i++)
			printf("   ");
		PrintASCII(s,e,e-e%16);
	}	//print rest
}
void Edit(){
	// Fuction to modify data in the memory

	char val[8], addr[8];
	strcpy(addr, par[0]);
	strcpy(val, par[2]);

	strcpy(mem[HexToDec(addr)],val);
}

void PrintASCII(int s, int e, int addr ){
	// Function to output ASCII data of the line containing the address

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
	//Function to fill from s(start) ~ e(end)

	int i,s,e;
	char start[8],end[8],value[8];
	strcpy(start, par[0]);
	strcpy(end, par[2]);
	strcpy(value, par[4]);

	s = HexToDec(start);
	e = HexToDec(end);
	for(i=s; i<=e ; i++){
		strcpy(mem[i],value);
	}
}

int HexToDec(char* hex){
	//Function to convert Hexadecimal to Decimal

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
	//Function to fill all memory '0'

	int i=0;
	char tmpstr[] = "00";
	for(i=0 ; i<MAX_MEMORY ; i++){
		strcpy(mem[i],tmpstr);
	}
}

void Opcodelist(){
	//Function to print opcode list

	int i=0,flag=0; //i:index , flag: if flag is 1, print '->'
	opcode_list* tmp; //opcode_list pointer for searching

	for(i=0 ; i<HASH_MOD ; i++){
		printf("%d : ",i);
		flag = 0;
		if(hash[i].head == NULL) {
			printf("empty\n");
			continue;
		}
		for(tmp = hash[i].head; tmp!=NULL ; tmp=tmp->next){
			if(flag) printf(" -> ");
			else flag = 1;
			printf("[%s,%s]",tmp->mnemonic,tmp->opcode);	
		}
		puts("");
	}
}



int OpcodeMnemonic(){	
	//Function to find opcode.
	//if you find the opcode print and return 1.
	//if the opcode is not exist, print error message and return 0.

	opcode_list* tmp;	// opcode_list pointer for searching
	char key[10];		// mnemonic of the opcode you want to obtain
	int idx=0,i;		// idx: hash table index, i:index

	strcpy(key, par[0]);

	for( i=0; i<(int)strlen(key) ; i++)	
		idx += key[i];
	idx%=HASH_MOD;
	for(tmp = hash[idx].head; tmp!=NULL; tmp = tmp->next){
		if(!strcmp( key, tmp->mnemonic)) {
			printf("opcode is %s\n",tmp->opcode);
			return 1;
		}
	}	
	printf("OPCODE not found\n");
}
