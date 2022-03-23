#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

int index_cmp(const void*, const void*);
int cmp(const void*, const void*);

char tab[MAX_IDS][MAX_ID_LEN];

char *keywords[] = {
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"
};

int find_idents() {
	int idents_top = 0;
	char curr_ident[MAX_ID_LEN] = "";
	int curr_ident_len = 0;

	int currchar, nextchar;
	int state = 0; // 0 - normal operation, 1 - line comment, 2 - block comment,
				   // 5 - string literal, 6 - char literal, 9 - identifier
	
    while ((currchar = getc(stdin)) != EOF) {
		switch (state) {
			case 0:
				if (currchar != '/') {
					if (currchar=='"') {
						state = 5;
						break;
					}
					if (currchar=='\'') {
						state = 6;
						break;
					}
					if (isalpha(currchar) || currchar=='_') {
						curr_ident[0] = currchar;
						curr_ident_len = 1;
						state = 9;
						break;
					}
					break;
				}
				nextchar = getc(stdin);
				if (nextchar == '/') {
					state = 1;
					break;
				}
				else if (nextchar == '*') {
					state = 2;
					break;
				}
				ungetc(nextchar, stdin);
				break;
			case 1:
				if (currchar == '\n')	state = 0;
				break;
			case 2:
				if (currchar != '*')	break;
				nextchar = getc(stdin);
				if (nextchar == '/')	state = 0;
				else					ungetc(nextchar, stdin);
				break;
			case 5: // might need to handle \" and \'
				if (currchar=='"') {
					state = 0; break;
				}
				if (currchar=='\\') {
					nextchar = getc(stdin); // if backslash, ignore next char
					break;
				}
				break;
			case 6:
				if (currchar=='\'') {
					state = 0; break;
				}
				if (currchar=='\\') {
					nextchar = getc(stdin);
					break;
				}
				break;
			case 9:
				if (isalnum(currchar) || currchar=='_')	{
					curr_ident[curr_ident_len++] = currchar;
					break;
				}
				else {
					// printf("FOUND IDENT '%s'", curr_ident);
					int is_new_ident = 1;
					for (int i=0; i<32; ++i) {
						if (strcmp(keywords[i], curr_ident) == 0) {
							is_new_ident = 0; break;
						}
					}
					if (is_new_ident==0) {
						// printf(" - IS A KEYWORD\n");
						memset(curr_ident, 0, curr_ident_len+1);
						state = 0; break;
					}

					for (int i=0; i<idents_top; ++i) {
						if (strcmp(tab[i], curr_ident) == 0) {
							is_new_ident = 0; break;
						}
					}
					if (is_new_ident==1) {
						// printf(" - IS A NEW IDENT\n");
						for (int i=0; i<=curr_ident_len; ++i) {
							char t = curr_ident[i];
							tab[idents_top][i] = t;
						}
						++idents_top;
					}
					// else printf(" - ALREADY PRESENT\n");

					memset(curr_ident, 0, curr_ident_len+1);
					state = 0;
					break;
				}
				break;
		}
    }
	return idents_top;
}

int cmp(const void* first_arg, const void* second_arg) {
	char *a = *(char**)first_arg;
	char *b = *(char**)second_arg;
	return strcmp(a, b);
}

int index_cmp(const void* first_arg, const void* second_arg) {
	int a = *(int*)first_arg;
	int b = *(int*)second_arg;
	return strcmp(tab[a], tab[b]);
}

int main(void) {
	printf("%d\n", find_idents());
	return 0;
}

