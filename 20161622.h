#include <stdio.h>
#include <string.h>

#define COMMANDSIZE 55
#define MAX_TOKEN 50
#define MAX_PARAMETER 5
#define MAX_MEMORY 1048576 


char mem[ MAX_MEMORY ][3];

enum COMMANDTYPE {
	H = 1,
	D,
	Q,
	HI,
	DU,
	F,
	E,
	RESET,
	OPCODEMNEMONIC,
	OPCODELIST
};





/*command list*/
char *str_h[] = {"h","help"};
char *str_d[] = {"d","dir"};
char *str_q[] = {"q", "quit"};
char *str_hi[] = {"hi","history"};

char *str_du[] = {"du","dump"};
char *str_e[] = {"e","edit"};
char *str_f[] = {"f", "fill"};

char *str_reset = "reset";
char *str_opcodemnemonic = "opcode mnemonic";
char *str_opcodelist = "opcodelist";

int last_addr=-1;

char par[5][COMMANDSIZE];
char *tk[MAX_TOKEN]; //string token



int IsHex(char *ckstr );
int HexToDec(char *hex);
void DecToHex(char *hex, int dec);
void PrintASCII(int s, int e, int addr);
