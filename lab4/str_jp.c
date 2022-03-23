#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <limits.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_DIGRAMS (LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR)

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

int count[MAX_DIGRAMS] = { 0 };

// function to be used as comparator to qsort (compares counters and thes sorts
// alphabetically when equal)
int cmp (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	if (count[va] == count[vb]) return va - vb; // sort alphabetically if counts equal
	return count[vb] - count[va];
}

// function to be used as comparator to qsort for digrams (compares counters and
// thes sorts alphabetically when equal)
int cmp_di (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	// sort according to second char if counts and the first char equal
	if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
	// sort according to first char if counts equal
	if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
	return count[vb] - count[va];
}

// count number of lines (nl), number of words (nw) and number of characters
// (nc) in the text read from stdin
void wc(int *nl, int *nw, int *nc) {
	*nl = 0; *nw = 0; *nc = -1;
    int currchar; int count = 0; int clear = 0;
    int prevchar = '\0';
    do {
        if (clear==1) {
            count = 0; clear = 0;
        }
        currchar = getc(stdin);
        (*nc)++;
        if ((currchar==' ' || currchar=='\t' || currchar=='\n') && (prevchar!=' ' && prevchar!='\t' && prevchar!='\n'))  (*nw)++;
        if (currchar=='\n') {
            (*nl)++; clear = 1;
        }
        count++;
        prevchar = currchar;
    } while (currchar!=EOF);
}

// count how many times each character from [FIRST_CHAR, LAST_CHAR) occurs
// in the text read from stdin. Sort chars according to their respective
// cardinalities (decreasing order). Set n_char and cnt to the char_no - th char
// in the sorted list and its cardinality respectively
void char_count(int char_no, int *n_char, int *cnt) {
    int currchar; int clear = 0; int tmpcounter = 0;
    do {
        if (clear==1) {
            tmpcounter = 0; clear = 0;
        }
        currchar = getc(stdin);
		if (FIRST_CHAR <= currchar && currchar < LAST_CHAR) {
			count[currchar-FIRST_CHAR]++;
		}
        if (currchar=='\n')		clear = 1;
        tmpcounter++;
    } while (currchar!=EOF);

	int indices[MAX_CHARS]; int t;
	for (int i=0; i<MAX_CHARS; ++i)	indices[i] = i;
	for (int i=0; i<MAX_CHARS-1; ++i) {
		for (int j=i+1; j<MAX_CHARS; ++j) {
			if (cmp(&indices[i], &indices[j]) > 0) {
				t = indices[i];
				indices[i] = indices[j];
				indices[j] = t;
			}
		}
	}
	*n_char = indices[char_no-1]+FIRST_CHAR;
	*cnt = count[*n_char-FIRST_CHAR];
}

// count how many times each digram (a pair of characters, from [FIRST_CHAR,
// LAST_CHAR) each) occurs in the text read from stdin. Sort digrams according
// to their respective cardinalities (decreasing order). Set digram[0] and
// digram[1] to the first and the second char in the digram_no - th digram_char
// in the sorted list. Set digram[2] to its cardinality.
void digram_count(int digram_no, int digram[]) {
	int currchar, prevchar = getc(stdin);
    while ((currchar = getc(stdin)) != EOF) {
		if (currchar<FIRST_CHAR || currchar>=LAST_CHAR || prevchar<FIRST_CHAR || prevchar>=LAST_CHAR) {
			prevchar = currchar;
			continue;
		}
		int i = (prevchar-FIRST_CHAR)*MAX_CHARS + (currchar-FIRST_CHAR);
		++count[i];
		prevchar = currchar;
    }

	int indices[MAX_DIGRAMS]; int t;
	for (int i=0; i<MAX_DIGRAMS; ++i)	indices[i] = i;

	qsort(indices, MAX_DIGRAMS, sizeof(int), cmp_di);

	digram[0] = indices[digram_no-1] / MAX_CHARS + FIRST_CHAR;
	digram[1] = indices[digram_no-1] % MAX_CHARS + FIRST_CHAR;
	digram[2] = count[indices[digram_no-1]];
}

// Count block and line comments in the text read from stdin. Set
// line_comment_counter and block_comment_counter accordingly
void find_comments(int *line_comment_counter, int *block_comment_counter) {
	*line_comment_counter = 0; *block_comment_counter = 0;
	int currchar, nextchar;
	int state = 0; // 0 - normal operation, 1 - line comment, 2 - block comment
    while ((currchar = getc(stdin)) != EOF) {
		switch (state) {
			case 0:
				if (currchar != '/')	break;
				nextchar = getc(stdin);
				if (nextchar == '/') {
					state = 1;
					++*line_comment_counter;
					break;
				}
				else if (nextchar == '*') {
					state = 2;
					++*block_comment_counter;
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
		}
    }
}

#define MAX_LINE 128

int read_line() {
	char line[MAX_LINE];
	int n;

	fgets (line, MAX_LINE, stdin); // to get the whole line
	sscanf (line, "%d", &n);
	return n;
}

int main(void) {
	int to_do;
	int nl, nw, nc, char_no, n_char, cnt;
	int line_comment_counter, block_comment_counter;
	int digram[3];

	to_do = read_line();
	switch (to_do) {
		case 1: // wc()
			wc (&nl, &nw, &nc);
			printf("%d %d %d\n", nl, nw, nc);
			break;
		case 2: // char_count()
			char_no = read_line();
			char_count(char_no, &n_char, &cnt);
			printf("%c %d\n", n_char, cnt);
			break;
		case 3: // digram_count()
			char_no = read_line();
			digram_count(char_no, digram);
			printf("%c%c %d\n", digram[0], digram[1], digram[2]);
			break;
		case 4:
			find_comments(&line_comment_counter, &block_comment_counter);
			printf("%d %d\n", block_comment_counter, line_comment_counter);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

