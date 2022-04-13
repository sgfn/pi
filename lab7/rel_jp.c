#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_REL_SIZE 1000
#define MAX_RANGE 1000

typedef struct {
	int first;
	int second;
} pair;

// Add pair to existing relation if not already there
int add_relation (pair* relation, int size, pair elems) {
	for (int i=0; i<size; ++i) {
		if (relation[i].first == elems.first && relation[i].second == elems.second) {
			return size;
		}
	}
	relation[size++] = elems;
	return size;
}

// Case 1:
// The relation R is reflexive if xRx for every x in X
int is_reflexive(pair* relation, int size) {
	for (int i=0; i<size; ++i) {
		int x = relation[i].first;
		int is_alright = 0;
		for (int j=0; j<size; ++j) {
			if (relation[j].first == x && relation[j].second == x) {
				is_alright = 1;
				break;
			}
		}
		if (is_alright == 0)	return 0;
	}
	return 1;
}
// The relation R on the set X is called irreflexive
// if xRx is false for every x in X
int is_irreflexive(pair* relation, int size) {
	for (int i=0; i<size; ++i) {
		int x = relation[i].first;
		for (int j=0; j<size; ++j) {
			if (relation[j].first == x && relation[j].second == x) {
				return 0;
			}
		}
	}
	return 1;
}
// A binary relation R over a set X is symmetric if:
// for all x, y in X xRy <=> yRx
int is_symmetric(pair* relation, int size) {
	for (int i=0; i<size; ++i) {
		int x = relation[i].first;
		int y = relation[i].second;
		int is_alright = 0;
		for (int j=0; j<size; ++j) {
			if (relation[j].first == y && relation[j].second == x) {
				is_alright = 1;
				break;
			}
		}
		if (is_alright == 0)	return 0;
	}
	return 1;
}
// A binary relation R over a set X is antisymmetric if:
// for all x,y in X if xRy and yRx then x=y
int is_antisymmetric(pair* relation, int size) {
	for (int i=0; i<size; ++i) {
		int x = relation[i].first;
		int y = relation[i].second;
		for (int j=0; j<size; ++j) {
			if (relation[j].first == y && relation[j].second == x) {
				if (x != y)		return 0;
			}
		}
	}
	return 1;
}
// A binary relation R over a set X is asymmetric if:
// for all x,y in X if at least one of xRy and yRx is false
int is_asymmetric(pair* relation, int size) {
	for (int i=0; i<size; ++i) {
		int x = relation[i].first;
		int y = relation[i].second;
		for (int j=0; j<size; ++j) {
			if (relation[j].first == y && relation[j].second == x) {
				return 0;
			}
		}
	}
	return 1;
}
// A homogeneous relation R on the set X is a transitive relation if:
// for all x, y, z in X, if xRy and yRz, then xRz
int is_transitive(pair* relation, int size) {
	for (int i=0; i<size; ++i) {
		int x = relation[i].first;
		int y = relation[i].second;
		for (int j=0; j<size; ++j) {
			if (relation[j].first != y)		continue;
			int z = relation[j].second;
			int is_alright = 0;
			for (int k=0; k<size; ++k) {
				if (relation[k].first == x && relation[k].second == z) {
					is_alright = 1;
					break;
				}
			}
			if (is_alright == 0)	return 0;
		}
	}
	return 1;
}

// Case 2:
// A partial order relation is a homogeneous relation that is
// reflexive, transitive, and antisymmetric
int is_partial_order(pair* relation, int size) {
	return (is_reflexive(relation, size) && is_antisymmetric(relation, size) && is_transitive(relation, size));
}
int is_connected(pair*, int);
// A total order relation is a partial order relation that is connected
int is_total_order(pair* relation, int size) {
	return (is_partial_order(relation, size) && is_connected(relation, size));
}
int get_domain(pair*, int, int*);
// Relation R is connected if for each x, y in X:
// xRy or yRx (or both)
int is_connected(pair* relation, int size) {
	int unique_vals[MAX_REL_SIZE*2];
	int u_size = get_domain(relation, size, unique_vals);

	for (int i=0; i<u_size; ++i) {
		int x = unique_vals[i];
		for (int j=0; j<u_size; ++j) {
			int y = unique_vals[j];
			if (x==y)	break;
			int is_alright = 0;
			for (int k=0; k<size; ++k) {
				if ((x==relation[k].first&&y==relation[k].second) || (x==relation[k].second&&y==relation[k].first)) {
					is_alright = 1;
					break;
				}
			}
			if (is_alright==0)	return 0;
		}
	}
	return 1;
}

int compare_ints(const void* a, const void* b) {
	return *(int*)a > *(int*)b;
}

int find_max_elements(pair* relation, int size, int* max_elems_container) {
	int unique_vals[MAX_REL_SIZE*2];
	int u_size = get_domain(relation, size, unique_vals);
	int m_size = 0;

	for (int i=0; i<u_size; ++i) {
		int g = unique_vals[i];
		int is_alright = 1;
		for (int j=0; j<size; ++j) {
			if (relation[j].first == g && relation[j].second != g) {
				is_alright = 0;
				break;
			}
		}
		if (is_alright == 1) {
			max_elems_container[m_size++] = g;
		}
	}
	qsort(max_elems_container, m_size, sizeof(int), compare_ints);
	return m_size;
}
int find_min_elements(pair* relation, int size, int* min_elems_container) {
	int unique_vals[MAX_REL_SIZE*2];
	int u_size = get_domain(relation, size, unique_vals);
	int m_size = 0;

	for (int i=0; i<u_size; ++i) {
		int g = unique_vals[i];
		int is_alright = 1;
		for (int j=0; j<size; ++j) {
			if (relation[j].second == g && relation[j].first != g) {
				is_alright = 0;
				break;
			}
		}
		if (is_alright == 1) {
			min_elems_container[m_size++] = g;
		}
	}
	qsort(min_elems_container, m_size, sizeof(int), compare_ints);
	return m_size;
}
int get_domain(pair* relation, int size, int* domain_container) {
	int d_size = 0;
	for (int i=0; i<size; ++i) {
		for (int whichone=0; whichone<2; ++whichone) {
			int x = (whichone==0) ? relation[i].first : relation[i].second;
			int present = 0;
			for (int j=0; j<d_size; ++j) {
				if (domain_container[j] == x) {
					present = 1;
					break;
				}
			}
			if (present == 0) {
				domain_container[d_size++] = x;
			}
		}
	}
	qsort(domain_container, d_size, sizeof(int), compare_ints);
	return d_size;
}

// Case 3:
int composition (pair* relation_1, int size_1, pair* relation_2, int size_2, pair* relation_res) {
	int res = 0;
	for (int i=0; i<size_1; ++i) {
		int x = relation_1[i].first;
		int y = relation_1[i].second;
		for (int j=0; j<size_2; ++j) {
			if (relation_2[j].first == y) {
				pair a = {x, relation_2[j].second};
				res = add_relation(relation_res, res, a);
			}
		}
	}
	return res;
}

int cmp (pair p1, pair p2) {
	if (p1.first == p2.first) return p1.second - p2.second;
	return p1.first - p2.first;
}

// Read number of pairs, n, and then n pairs of ints
int read_relation(pair* relation) {
	int n, x, y;
	scanf("%d", &n);
	for (int i=0; i<n; ++i) {
		scanf("%d %d", &x, &y);
		relation[i].first = x;
		relation[i].second = y;
	}
	return n;
}

void print_int_array(int *array, int n) {
	printf("%d\n", n);
	for (int i = 0; i < n; ++i) {
		printf("%d ", array[i]);
	}
	printf("\n");
}

int main(void) {
	int to_do;
	pair relation[MAX_REL_SIZE];
	pair relation_2[MAX_REL_SIZE];
	pair comp_relation[MAX_REL_SIZE];
	int domain[MAX_REL_SIZE];

	scanf("%d",&to_do);
	int size = read_relation(relation);
	int ordered, size_2, n_domain;

	switch (to_do) {
		case 1:
			printf("%d ", is_reflexive(relation, size));
			printf("%d ", is_irreflexive(relation, size));
			printf("%d ", is_symmetric(relation, size));
			printf("%d ", is_antisymmetric(relation, size));
			printf("%d ", is_asymmetric(relation, size));
			printf("%d\n", is_transitive(relation, size));
			break;
		case 2:
			ordered = is_partial_order(relation, size);
			n_domain = get_domain(relation, size, domain);
			printf("%d %d\n", ordered, is_total_order(relation, size));
			print_int_array(domain, n_domain);
			if (!ordered) break;
			int max_elements[MAX_REL_SIZE];
			int min_elements[MAX_REL_SIZE];
			int no_max_elements = find_max_elements(relation, size, max_elements);
			int no_min_elements = find_min_elements(relation, size, min_elements);
			print_int_array(max_elements, no_max_elements);
			print_int_array(min_elements, no_min_elements);
			break;
		case 3:
			size_2 = read_relation(relation_2);
			printf("%d\n", composition(relation, size,
			   relation_2, size_2, comp_relation));
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

