/*
 * file.c
 */

#include "token.h"
#include "stack.h"

#include <stdio.h>
int defunCheck = 0;
stack ifStack;
int ifCount = 0;
int tokCount = 0;
char fn[1000];

void write_assembly(token *currToken, FILE *output) {
	//DEFUN
	if ((currToken->type) == 0) {
		defunCheck = 1;
	}

	//IDENT
	else if ((currToken->type) == 1) {
		if (defunCheck == 1) {
			defunCheck = 0;
			char str[1000];
			sprintf(str, ".CODE\n");
			fputs(str, output);	

			sprintf(str,".FALIGN\n");
			fputs(str, output);

			sprintf(str, "%s\n", currToken->str);
			fputs(str, output);

			fprintf(output, ";; prologue\n");
			fprintf(output, "STR R7, R6, #-2;\n");
			fprintf(output, "STR R5, R6, #-3;\n");
			fprintf(output, "ADD R6, R6, #-3;\n");
			fprintf(output, "ADD R5, R6, #0;\n");
		} else {
			fprintf(output, "JSR %s\n", currToken->str);
		}
	}

	//RETURN
	else if ((currToken->type) == 2) {
		fprintf(output, "ADD R5, R5, #2\n");
		fprintf(output, "LDR R1, R6, #0\n");
		fprintf(output, "STR R1, R5, #0\n");
		fprintf(output, "ADD R5, R5, #-2\n");

		fprintf(output, ";; epilogue\n");
		fprintf(output, "ADD R6, R5, #0\n");
		fprintf(output, "LDR R5, R6, #0\n");
		fprintf(output, "LDR R7, R6, #1\n");
		fprintf(output, "ADD R6, R6, #2\n");

		fprintf(output, "RET\n");
	}

	//PLUS
	else if ((currToken->type) == 3) {
		fprintf(output, "LDR R1, R6, #0;\n");
		fprintf(output, "LDR R2, R6, #1;\n");
		fprintf(output, "ADD R2, R1, R2;\n");
		fprintf(output, "ADD R6, R6, #1;\n");
		fprintf(output, "STR R2, R6, #0;\n");
	}

	//MINUS
	else if ((currToken->type) == 4) {
		fprintf(output, "LDR R1, R6, #0;\n");
		fprintf(output, "LDR R2, R6, #1;\n");
		fprintf(output, "SUB R2, R1, R2;\n");
		fprintf(output, "ADD R6, R6, #1;\n");
		fprintf(output, "STR R2, R6, #0;\n");
	}

	//MUL
	else if ((currToken->type) == 5) {
		fprintf(output, "LDR R1, R6, #0;\n");
		fprintf(output, "LDR R2, R6, #1;\n");
		fprintf(output, "MUL R2, R1, R2;\n");
		fprintf(output, "ADD R6, R6, #1;\n");
		fprintf(output, "STR R2, R6, #0;\n");
	}

	//DIV
	else if ((currToken->type) == 6) {
		fprintf(output, "LDR R1, R6, #0;\n");
		fprintf(output, "LDR R2, R6, #1;\n");
		fprintf(output, "DIV R2, R1, R2;\n");
		fprintf(output, "ADD R6, R6, #1;\n");
		fprintf(output, "STR R2, R6, #0;\n");
	}

	//MOD
	else if ((currToken->type) == 7) {
		fprintf(output, "LDR R1, R6, #0;\n");
		fprintf(output, "LDR R2, R6, #1;\n");
		fprintf(output, "MOD R2, R1, R2;\n");
		fprintf(output, "ADD R6, R6, #1;\n");
		fprintf(output, "STR R2, R6, #0;\n");
	}

	//AND
	else if ((currToken->type) == 8) {
		fprintf(output, "LDR R1, R6, #0;\n");
		fprintf(output, "LDR R2, R6, #1;\n");
		fprintf(output, "AND R2, R1, R2;\n");
		fprintf(output, "ADD R6, R6, #1;\n");
		fprintf(output, "STR R2, R6, #0;\n");
	}

	//OR
	else if ((currToken->type) == 9) {
		fprintf(output, "LDR R1, R6, #0;\n");
		fprintf(output, "LDR R2, R6, #1;\n");
		fprintf(output, "OR R2, R1, R2;\n");
		fprintf(output, "ADD R6, R6, #1;\n");
		fprintf(output, "STR R2, R6, #0;\n");
	}

	//NOT
	else if ((currToken->type) == 10) {
		fprintf(output, "LDR R1, R6, #0;\n");
		fprintf(output, "NOT R1, R1;\n");
		fprintf(output, "ADD R6, R6, #1;\n");
		fprintf(output, "STR R1, R6, #0;\n");
	}

	//LT
	else if ((currToken->type) == 11) {
		fprintf(output, "LDR R1, R6, #0;\n");
		fprintf(output, "LDR R2, R6, #1;\n");
		fprintf(output, "CMP R1, R2;\n");
		fprintf(output, "BRn %s_LT%d;\n", fn, tokCount);
		fprintf(output, "BRzp %s_notLT%d;\n", fn, tokCount);
		
		fprintf(output, "%s_LT%d;\n", fn, tokCount);
		fprintf(output, "CONST R2, #1;\n");
		fprintf(output, "ADD R6, R6, #1;\n");
		fprintf(output, "STR R2, R6, #0;\n");
		fprintf(output, "BRnzp %s_END%d;\n", fn, tokCount);

		fprintf(output, "%s_notLT%d;\n", fn, tokCount);
		fprintf(output, "CONST R2, #0;\n");
		fprintf(output, "ADD R6, R6, #1;\n");
		fprintf(output, "STR R2, R6, #0;\n");

		fprintf(output, "%s_END%d;\n", fn, tokCount);
	}

	//LE
	else if ((currToken->type) == 12) {
		fprintf(output, "LDR R1, R6, #0;\n");
		fprintf(output, "LDR R2, R6, #1;\n");
		fprintf(output, "CMP R1, R2;\n");
		fprintf(output, "BRnz LE%d;\n", tokCount);
		fprintf(output, "BRp notLE%d;\n", tokCount);
		
		fprintf(output, "LE%d;\n", tokCount);
		fprintf(output, "CONST R2, #1;\n");
		fprintf(output, "ADD R6, R6, #1;\n");
		fprintf(output, "STR R2, R6, #0;\n");
		fprintf(output, "BRnzp END%d;\n", tokCount);

		fprintf(output, "notLE%d;\n", tokCount);
		fprintf(output, "CONST R2, #0;\n");
		fprintf(output, "ADD R6, R6, #1;\n");
		fprintf(output, "STR R2, R6, #0;\n");

		fprintf(output, "END%d;\n", tokCount);
	}

	//EQ
	else if ((currToken->type) == 13) {
		fprintf(output, "LDR R1, R6, #0;\n");
		fprintf(output, "LDR R2, R6, #1;\n");
		fprintf(output, "CMP R1, R2;\n");
		fprintf(output, "BRz %s_EQ%d;\n", fn, tokCount);
		fprintf(output, "BRnp %s_notEQ%d;\n", fn, tokCount);
		
		fprintf(output, "%s_EQ%d;\n", fn, tokCount);
		fprintf(output, "CONST R2, #1;\n");
		fprintf(output, "ADD R6, R6, #1;\n");
		fprintf(output, "STR R2, R6, #0;\n");
		fprintf(output, "BRnzp %s_END%d;\n", fn, tokCount);

		fprintf(output, "%s_notEQ%d;\n", fn, tokCount);
		fprintf(output, "CONST R2, #0;\n");
		fprintf(output, "ADD R6, R6, #1;\n");
		fprintf(output, "STR R2, R6, #0;\n");
		
		fprintf(output, "%s_END%d;\n", fn, tokCount);
	}

	//GE
	else if ((currToken->type) == 14) {
		fprintf(output, "LDR R1, R6, #0;\n");
		fprintf(output, "LDR R2, R6, #1;\n");
		fprintf(output, "CMP R1, R2;\n");
		fprintf(output, "BRzp %s_GE%d;\n", fn, tokCount);
		fprintf(output, "BRn %s_notGE%d;\n", fn, tokCount);
		
		fprintf(output, "%s_GE%d;\n", fn, tokCount);
		fprintf(output, "CONST R2, #1;\n");
		fprintf(output, "ADD R6, R6, #1;\n");
		fprintf(output, "STR R2, R6, #0;\n");
		fprintf(output, "BRnzp %s_END%d;\n", fn, tokCount);

		fprintf(output, "%s_notGE%d;\n", fn, tokCount);
		fprintf(output, "CONST R2, #0;\n");
		fprintf(output, "ADD R6, R6, #1;\n");
		fprintf(output, "STR R2, R6, #0;\n");

		fprintf(output, "%s_END%d;\n", fn, tokCount);
	}

	//GT
	else if ((currToken->type) == 15) {
		fprintf(output, "LDR R1, R6, #0;\n");
		fprintf(output, "LDR R2, R6, #1;\n");
		fprintf(output, "CMP R1, R2;\n");
		fprintf(output, "BRp %s_GT%d;\n", fn, tokCount);
		fprintf(output, "BRnz %s_notGT%d;\n", fn, tokCount);

		fprintf(output, "%s_GT%d;\n", fn, tokCount);
		fprintf(output, "CONST R2, #1;\n");
		fprintf(output, "ADD R6, R6, #1;\n");
		fprintf(output, "STR R2, R6, #0;\n");
		fprintf(output, "BRnzp %s_END%d;\n", fn, tokCount);

		fprintf(output, "%s_notGT%d;\n", fn, tokCount);
		fprintf(output, "CONST R2, #0;\n");
		fprintf(output, "ADD R6, R6, #1;\n");
		fprintf(output, "STR R2, R6, #0;\n");

		fprintf(output, "%s_END%d;\n", fn, tokCount); 
	}

	//IF
	else if ((currToken->type) == 16) {
		ifCount = ifCount + 1;
		push(&ifStack, ifCount);

		stack_elt* a = peek_first(&ifStack);

		fprintf(output, "LDR R1, R6, #0\n");
		fprintf(output, "ADD R6, R6, #1\n");
		fprintf(output, "CMPI R1, #0\n");
		fprintf(output, "BRz %s_ELSE%d\n", fn, a->top);	
	}

	//ELSE
	else if ((currToken->type) == 17) {
		stack_elt* a = peek_first(&ifStack);
		fprintf(output, "JMP %s_ENDIF%d\n", fn, a->top);

		a->elseSeen = 1; 
		fprintf(output, "%s_ELSE%d\n", fn, a->top);
		
	}

	//ENDIF
	else if ((currToken->type) == 18) {
		stack_elt* first;
		first = peek_first(&ifStack);

		if (first->elseSeen != 1) {
			fprintf(output, "%s_ELSE%d;\n", fn, first->top);
		} else {
			fprintf(output, "%s_ENDIF%d\n", fn, first->top);
		}
		pop(&ifStack);
	}

	//DROP
	else if ((currToken->type) == 19) {
		fprintf(output, "ADD R6, R6, #1;\n");
	}

	//DUP
	else if ((currToken->type) == 20) {
		fprintf(output, "LDR R1, R6, #0;\n");
		fprintf(output, "ADD R6, R6, #-1;\n");
		fprintf(output, "STR R1, R6, #0;\n");
	}

	//SWAP
	else if ((currToken->type) == 21) {
		fprintf(output, "LDR R1, R6, #0;\n");
		fprintf(output, "LDR R2, R6, #1;\n");

		fprintf(output, "STR R2, R6, #0;\n");
		fprintf(output, "ADD R6, R6, #1;\n");
		fprintf(output, "STR R1, R6, #0;\n");
		
		fprintf(output, "ADD R6, R6, #-1;\n");
		
	}

	//ROT
	else if ((currToken->type) == 22) {
		fprintf(output, "LDR R1, R6, #0;\n");
		fprintf(output, "LDR R2, R6, #1;\n");
		fprintf(output, "LDR R3, R6, #2;\n");
		
		fprintf(output, "STR R3, R6, #0;\n");

		fprintf(output, "ADD R6, R6, #1;\n");
		fprintf(output, "STR R1, R6, #0;\n");

		fprintf(output, "ADD R6, R6, #1;\n");
		fprintf(output, "STR R2, R6, #0;\n");

		fprintf(output, "ADD R6, R6, #-2;\n");
	}

	//ARG
	else if ((currToken->type) == 23) {
		int argShift;
		argShift = 2 + (currToken->arg_no);
		fprintf(output, "LDR R1, R5, %d\n", argShift);
		fprintf(output, "ADD R6, R6, #-1\n");
		fprintf(output, "STR R1, R6, #0\n");
	}

	//LITERAL
	else if ((currToken->type) == 24) {
		fprintf(output, "ADD R6, R6, #-1;\n");

		short lowerByte;
		lowerByte = (currToken->literal_value) & 0xFF;
		fprintf(output, "CONST R1, %d\n", lowerByte);

		short upperByte;
		upperByte = (currToken->literal_value & 0xFF00) >> 8;
		fprintf(output, "HICONST R1, %d\n", upperByte);
		fprintf(output, "STR R1, R6, #0\n");
	}

	//BROKEN_TOKEN
	else if ((currToken->type) == 25) {
	}
	else {
	}
}

int main(int argc, char **argv) {
	FILE* obj = fopen(argv[1], "rb");
	int i;
	char thisFile[1000];
	char fileCopy[1000];
	strcpy(thisFile, argv[1]);

	for(i = 0; i < strlen(argv[1]); i++) {
		if (thisFile[i] != '.') {
			fileCopy[i] = thisFile[i];
		}
		else {
			fileCopy[i] = '\0';
			i = strlen(argv[1]);
		}
	}

	strcpy(fn, fileCopy);

	strcat(fileCopy, ".asm");	
	FILE *output = fopen(fileCopy, "w");

	init_stack(&ifStack);

	char str[1000];
	sprintf(str, ".CODE\n");
	fputs(str, output);	

	sprintf(str,".FALIGN\n");
	fputs(str, output);


	while(!feof(obj)) {
		token* newToken;
		newToken = malloc (sizeof(*newToken));
		read_token(newToken, obj);
		write_assembly(newToken, output);
		tokCount = tokCount + 1;
		free(newToken);
	}

	fclose(obj); 
	fclose(output);
	return 0;
}