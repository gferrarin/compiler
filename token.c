/*
 * token.c
 */

#define MAX_TOKEN_LENGTH 250

#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int read_token (token *theToken, FILE *theFile) {
	char currentToken[MAX_TOKEN_LENGTH];
	if (fscanf(theFile, "%s", currentToken) == 1) {
		//printf("\n%s\n", currentToken); 
		int num;

		if (sscanf(currentToken, "%d", &num) == 1) {
			if(sscanf(currentToken, "0x%x", &num) ==1) {
				theToken->type = LITERAL;
				theToken->literal_value =  num;
			}
			else {
				int i;
				int invalid;
				invalid = 0;
				for (i = 0; i < strlen(currentToken); i++) {
					if (!isdigit(currentToken[i])) {
						if(currentToken[0] != '-') {
							invalid = 1;
						}
					}
				}
				if (invalid != 1) {
					theToken->type = LITERAL;
					//printf("%d\n", num);
					theToken->literal_value =  num;
				}
			}
		} 
	
		else {
			//printf("\n%s\n", currentToken); 
			strncpy(theToken->str, currentToken, 250);

			if (strncmp(theToken->str, "defun", MAX_TOKEN_LENGTH) == 0) {
				theToken->type = DEFUN;
			}
			else if (strncmp(theToken->str, "return", MAX_TOKEN_LENGTH) == 0) {
				theToken->type = RETURN;
			}
			else if (strncmp(theToken->str, "+", MAX_TOKEN_LENGTH) == 0) {
				theToken->type = PLUS;
			}
			else if (strncmp(theToken->str, "-", MAX_TOKEN_LENGTH) == 0) {
				theToken->type = MINUS;
			}
			else if (strncmp(theToken->str, "*", MAX_TOKEN_LENGTH) == 0) {
				theToken->type = MUL;
			}
			else if (strncmp(theToken->str, "/", MAX_TOKEN_LENGTH) == 0) {
				theToken->type = DIV;
			}
			else if (strncmp(theToken->str, "%", MAX_TOKEN_LENGTH) == 0) {
				theToken->type = MOD;
			}
			else if (strncmp(theToken->str, "and", MAX_TOKEN_LENGTH) == 0) {
				theToken->type = AND;
			}
			else if (strncmp(theToken->str, "or", MAX_TOKEN_LENGTH) == 0) {
				theToken->type = OR;
			}
			else if (strncmp(theToken->str, "not", MAX_TOKEN_LENGTH) == 0) {
				theToken->type = NOT;
			}
			else if (strncmp(theToken->str, "lt", MAX_TOKEN_LENGTH) == 0) {
				theToken->type = LT;
			}
			else if (strncmp(theToken->str, "le", MAX_TOKEN_LENGTH) == 0) {
				theToken->type = LE;
			}
			else if (strncmp(theToken->str, "eq", MAX_TOKEN_LENGTH) == 0) {
				theToken->type = EQ;
			}
			else if (strncmp(theToken->str, "ge", MAX_TOKEN_LENGTH) == 0) {
				theToken->type = GE;
			}
			else if (strncmp(theToken->str, "gt", MAX_TOKEN_LENGTH) == 0) {
				theToken->type = GT;
			}
			else if (strncmp(theToken->str, "if", MAX_TOKEN_LENGTH) == 0) {
				theToken->type = IF;
			}
			else if (strncmp(theToken->str, "else", MAX_TOKEN_LENGTH) == 0) {
				theToken->type = ELSE;
			}
			else if (strncmp(theToken->str, "endif", MAX_TOKEN_LENGTH) == 0) {
				theToken->type = ENDIF;
			}
			else if (strncmp(theToken->str, "drop", MAX_TOKEN_LENGTH) == 0) {
				theToken->type = DROP;
			}
			else if (strncmp(theToken->str, "dup", MAX_TOKEN_LENGTH) == 0) {
				theToken->type = DUP;
			}
			else if (strncmp(theToken->str, "swap", MAX_TOKEN_LENGTH) == 0) {
				theToken->type = SWAP;
			}
			else if (strncmp(theToken->str, "rot", MAX_TOKEN_LENGTH) == 0) {
				theToken->type = ROT;
			}
			else if (strncmp(theToken->str, ";", MAX_TOKEN_LENGTH) == 0) {
				theToken->type = BROKEN_TOKEN;
				char comment[1000];
				fgets(comment, 100, theFile);
			}
			else if (strncmp(theToken->str, ";;", MAX_TOKEN_LENGTH) == 0) {
				theToken->type = BROKEN_TOKEN;
				char comment[1000];
				fgets(comment, 100, theFile);
			}
			else if (strncmp(theToken->str, "arg", 3) == 0) {
				if (strlen(theToken->str) == 4) {
					int argNo;
					char arg[1];
					strcpy(arg, &currentToken[3]);
					argNo = (int) arg[0] - 48;
					theToken->type = ARG;
					theToken->arg_no = argNo;
				}
				if (strlen(theToken->str) == 5) {
					int argNo;
					char arg[2];
					strcpy(arg, &currentToken[3]);
					argNo = (int) arg[1];
					argNo = argNo + ((int) arg[0] * 10);
					
					theToken->type = ARG;
					theToken->arg_no = argNo;
				}
			}
			else {
				theToken->type = IDENT;
			}
		} 
	} 
	else {
		theToken->type = BROKEN_TOKEN;
	}
	//print_token(theToken);
	return 1;
}

const char *token_type_to_string (int type) {
	const char *types[] =  {
		"DEFUN", "IDENT", "RETURN", 
         "PLUS", "MINUS", "MUL", "DIV", "MOD", 
         "AND", "OR", "NOT", 
         "LT", "LE", "EQ", "GE", "GT",
         "IF", "ELSE", "ENDIF", 
         "DROP", "DUP", "SWAP", "ROT",
	 "ARG",
         "LITERAL",
        "BROKEN_TOKEN"
	};

	return types[type];
}

void print_token (token *theToken) {
	printf("%s\n", token_type_to_string(theToken->type));
}