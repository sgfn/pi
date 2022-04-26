#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#define TAB_SIZE  1000
#define BUF_SIZE  1000

double get(int cols, int row, int col, const double *A) {
	return A[row*cols+col];
}

void set(int cols, int row, int col, double *A, double value) {
	A[row*cols+col] = value;
}

void prod_mat(int rowsA, int colsA, int colsB, double *A, double *B, double *AB) {
	for (int i=0; i<rowsA; ++i) {
		for (int j=0; j<colsB; ++j) {
			double val = 0;
			for (int k=0; k<colsA; ++k) {
				val += get(colsA, i, k, A) * get(colsB, k, j, B);
			}
			set(colsB, i, j, AB, val);
		}
	}
}

void read_mat(int rows, int cols, double *t) {
	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j) {
			double val;
			scanf("%lf", &val);
			set(cols, i, j, t, val);
		}
	}
}

void print_mat(int rows, int cols, double *t) {
	for (int i=0; i<rows; ++i) {
		for (int j=0; j<cols; ++j) {
			printf("%.2f ", get(cols, i, j, t));
		}
		printf("\n");
	}
}

int read_char_lines(char *tab[]) {
	int counter = 0;
	char * line;
	char s[BUF_SIZE];
	char c = getc(stdin);
	while (c != EOF) {
		ungetc(c, stdin);

		fgets(s, BUF_SIZE, stdin);
		line = (char*)malloc(strlen(s)+1);
		strcpy(line, s);
		tab[counter++] = line;

		c = getc(stdin);
	}

	return counter;
}

void write_char_line(char *tab[], int n) {
	printf("%s", tab[n]);
}

void delete_lines(char *tab[], int line_count) {
	for (int i=0; i<line_count; ++i) {
		free(tab[i]);
	}
}

int read_dbl_lines_v1(double *ptr_tab[]) {
	int counter = 1, current = 0;
	ptr_tab[1] = &ptr_tab[0][0];
	char c = getc(stdin);
	int row_not_empty = 0;
	while (c != EOF) {
		if (c == '\n' && row_not_empty) {
			ptr_tab[++counter] = &ptr_tab[0][current];
			row_not_empty = 0;
		}
		else {
			ungetc(c, stdin);
			double val;
			scanf("%lf", &val);
			ptr_tab[0][current++] = val;
			row_not_empty = 1;
		}
		c = getc(stdin);
	}
	return counter-1;
}

void write_dbl_line_v1(double *ptr_tab[], int n) {
	double * p = ptr_tab[n];
	double * last = ptr_tab[n+1];
	while (p != last) {
		printf("%.2f ", *(p++));
	}
}

int main(void) {
	int to_do;

	scanf ("%d", &to_do);

	double A[TAB_SIZE], B[TAB_SIZE], C[TAB_SIZE];
	int n, lines_counter, rowsA,colsA,rowsB,colsB;
	char *char_lines_table[TAB_SIZE];
	double series_table[TAB_SIZE];
	double *ptr_table[TAB_SIZE];

	switch (to_do) {
		case 1:
			scanf("%d %d",&rowsA,&colsA);
			read_mat(rowsA, colsA, A);
			scanf("%d %d",&rowsB,&colsB);
			read_mat(rowsB, colsB, B);
			prod_mat(rowsA,colsA,colsB,A,B,C);
			print_mat(rowsA,colsB,C);
			break;
		case 2:
			scanf("%d",&n);
			ptr_table[0] = series_table;
			lines_counter = read_dbl_lines_v1(ptr_table);
			write_dbl_line_v1(ptr_table,n);
			break;
		case 3:
			scanf("%d", &n);
			lines_counter = read_char_lines(char_lines_table);
			write_char_line(char_lines_table,n);
			delete_lines(char_lines_table,lines_counter);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
	}
	return 0;
}
