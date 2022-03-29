#include <stdio.h>
#include <math.h>

#define SIZE 40

void read_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		scanf("%lf", x++);
	}
}

void print_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		printf("%.4f ", x[i]);
	}
	printf("\n");
}

void read_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			scanf("%lf", &A[i][j]);
		}
	}
}

void print_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			printf("%.4f ", A[i][j]);
		}
		printf("\n");
	}
}

void print_mat_ind(double A[][SIZE], int m, int n, const int indices[]);

// 5.1
// Calculate matrix product, AB = A X B
// A[m][p] X B[p][n] = AB[m][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n) {
	for (int i=0; i<m; ++i) {
		for (int j=0; j<n; ++j) {
			AB[i][j] = 0;
			for (int k=0; k<p; ++k) {
				AB[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

// Calculate matrix - vector product
void mat_vec_product(double A[][SIZE], const double b[], double Ab[], int m, int n);

void backward_substit(double A[][SIZE], double x[], int n);

void backward_substitution_index(double A[][SIZE], const int indices[], double x[], int n);

// 5.2
// Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n) {
	for (int i=0; i<n; ++i) {
		for (int j=i+1; j<n; ++j) {
			if (A[i][i] == 0)	return NAN;
			double mult_by = A[j][i] / A[i][i];
			for (int k=0; k<n; ++k) {
				A[j][k] -= mult_by * A[i][k];
			}
		}
	}
	double ans = A[0][0];
	for (int i=1; i<n; ++i) {
		ans *= A[i][i];
	}
	return ans;
}

// 5.3
// Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.

double gauss(double A[][SIZE], const double b[], double x[], const int n, const double eps) {
	int row_indices[SIZE];
	int det_sign = 1;	// swapping rows changes sign of determinant
	double b_[SIZE];
	if (b != NULL) {
		for (int i=0; i<n; ++i)		b_[i] = b[i];
	}
	for (int i=0; i<n; ++i)		row_indices[i] = i;
	for (int i=0; i<n; ++i) {
		for (int j=i+1; j<n; ++j) {
			if (fabs(A[row_indices[i]][i]) < fabs(A[row_indices[j]][i])) {
				int t = row_indices[i];
				row_indices[i] = row_indices[j];
				row_indices[j] = t;
				det_sign = -det_sign;
			}
		}
		if (fabs(A[row_indices[i]][i]) < eps)		return 0;
		for (int j=i+1; j<n; ++j) {
			double mult_by = A[row_indices[j]][i] / A[row_indices[i]][i];
			for (int k=0; k<n; ++k) {
				A[row_indices[j]][k] -= mult_by * A[row_indices[i]][k];
			}
			if (b != NULL) {
				b_[row_indices[j]] -= mult_by * b_[row_indices[i]];
			}
		}
	}
	// Calculate the determinant
	double det = det_sign * A[row_indices[0]][0];
	for (int i=1; i<n; ++i) {
		det *= A[row_indices[i]][i];
	}
	// for (int i=0; i<n; ++i) {
	// 	printf("|");
	// 	for (int j=0; j<n; ++j) {
	// 		printf("%.1lf ", A[row_indices[i]][j]);
	// 	}
	// 	printf("|x%d|%.1lf\n", row_indices[i]+1, b_[row_indices[i]]);
	// }
	// Calculate elements of vector x
	if (det != 0 && b != NULL && x != NULL) {
		for (int i=n-1; i>=0; --i) {
			double t = b_[row_indices[i]];
			for (int j=i+1; j<n; ++j) {
				t -= A[row_indices[i]][j] * x[row_indices[j]];
			}
			x[row_indices[i]] = t / A[row_indices[i]][i];
		}
		// Fix vector x
		double x_[SIZE];
		for (int i=0; i<n; ++i) 	x_[i] = x[row_indices[i]];
		for (int i=0; i<n; ++i)		x[i] = x_[i];
	}
	return det;
}

// 5.4
// Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps) {
	// Setup B
	for (int i=0; i<n; ++i) {
		for (int j=0; j<n; ++j)		B[i][j] = (i==j) ? 1 : 0;
	}
	int row_indices[SIZE];
	int det_sign = 1;	// swapping rows changes sign of determinant
	double det = 1;
	for (int i=0; i<n; ++i)		row_indices[i] = i;
	for (int i=0; i<n; ++i) {
		for (int j=i+1; j<n; ++j) {
			if (fabs(A[row_indices[i]][i]) < fabs(A[row_indices[j]][i])) {
				int t = row_indices[i];
				row_indices[i] = row_indices[j];
				row_indices[j] = t;
				det_sign = -det_sign;
			}
		}
		if (fabs(A[row_indices[i]][i]) < eps)		return 0;
		for (int j=i+1; j<n; ++j) {
			double mult_by = A[row_indices[j]][i] / A[row_indices[i]][i];
			for (int k=0; k<n; ++k) {
				A[row_indices[j]][k] -= mult_by * A[row_indices[i]][k];
				B[row_indices[j]][k] -= mult_by * B[row_indices[i]][k];
			}
		}
		double div_by = A[row_indices[i]][i];
		for (int k=0; k<n; ++k) {
			A[row_indices[i]][k] /= div_by;
			B[row_indices[i]][k] /= div_by;
		}
		det *= div_by;
	}
	det *= det_sign;
	for (int i=n-1; i>0; --i) {
		for (int j=i-1; j>=0; --j) {
			double mult_by = A[row_indices[j]][i] / A[row_indices[i]][i];
			for (int k=0; k<n; ++k) {
				A[row_indices[j]][k] -= mult_by * A[row_indices[i]][k];
				B[row_indices[j]][k] -= mult_by * B[row_indices[i]][k];
			}
		}
	}
	// Fix B
	double B_[SIZE][SIZE];
	for (int i=0; i<n; ++i) {
		for (int j=0; j<n; ++j) 	B_[i][j] = B[row_indices[i]][j];
	}
	for (int i=0; i<n; ++i) {
		for (int j=0; j<n; ++j)		B[i][j] = B_[i][j];
	}
	return det;
}

int main(void) {

	double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
	double b[SIZE], x[SIZE], det, eps = 1.e-13;

	int to_do;
	int m, n, p;

	scanf ("%d", &to_do);

	switch (to_do) {
		case 1:
			scanf("%d %d %d", &m, &p, &n);
			read_mat(A, m, p);
			read_mat(B, p, n);
			mat_product(A, B, C, m, p, n);
			print_mat(C, m, n);
			break;
		case 2:
			scanf("%d", &n);
			read_mat(A, n, n);
			printf("%.4f\n", gauss_simplified(A, n));
			break;
		case 3:
			scanf("%d", &n);
			read_mat(A,n, n);
			read_vector(b, n);
			det = gauss(A, b, x, n, eps);
			printf("%.4f\n", det);
			if(det) print_vector(x, n);
			break;
		case 4:
			scanf("%d", &n);
			read_mat(A,n,n);
			printf("%.4f\n",matrix_inv(A, B, n, eps));
			print_mat(B, n, n);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

