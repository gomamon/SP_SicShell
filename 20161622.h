#include <stdio.h>
#include <string.h>

#define COMMANDSIZE 55
#define MAX_PARAMETER 8
#define MAX_MEMORY 1048576 
#define MAX_DATA 256
#define HASH_SIZE 20
#define HASH_MOD 20

char mem[ MAX_MEMORY ][3];


/**history **/
typedef struct HistoryNode{
	char data[COMMANDSIZE];
	struct HistoryNode *next;
}his_node;
his_node* his_head = NULL;
his_node* his_rear = NULL;



enum COMMANDTYPE {
	H = 0,
	D,
	Q,
	HI,
	RESET,
	OPCODELIST,
	
	DU,
	E,
	F,
	OPCODEMNEMONIC
};

typedef struct OpcodeList{
	struct OpcodeList* next;
	char opcode[5];
	char mnemonic[10];
	char form[5];
}opcode_list;


typedef struct{
	int size;
	opcode_list *head;
	opcode_list *rear;
}hash_table;
hash_table hash[20];

/*command list*/
char command_list[10][2][15]={
	{"h","help"},
	{"d","dir"},
	{"q", "quit"},
	{"hi","history"},
	{"reset","reset"},
	{"opcodelist","opcodelist"},
	{"du","dump"},
	{"e","edit"},
	{"f", "fill"},
	{"opcode","opcode"}
};
//free
void FreeHistory();
void FreeHash();


int last_addr=-1;

char par[MAX_PARAMETER][COMMANDSIZE];
char command[ COMMANDSIZE ];

int IsHex(char *ckstr );
int HexToDec(char *hex);
void DecToHex(char *hex, int dec);
void PrintASCII(int s, int e, int addr);

int ProcessCommand(char* cmd);
int MakeHashTable();
void MakeOpcodeList(char* opcode, char* mnemonic, char* mode);
int CheckParameter(int cmd_num);

void PrintData(int s, int e);
/*ERROR FUNCTION*/
int IsAddrLimitERROR(int addr);
int IsAddrERROR(int s, int e);
int IsDataLimitERROR(int data);
