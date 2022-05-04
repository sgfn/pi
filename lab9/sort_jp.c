#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STR_LEN 64
#define MAX_PERSONS 1024

typedef struct Person {
	int age;
	char first_name[MAX_STR_LEN];
	char last_name[MAX_STR_LEN];
} Person;

// Sort according to age (decreasing)
// When ages equal compare first name and then last name
int cmp_person(const void *p1, const void *p2) {
	Person *ps1 = (Person*)p1, *ps2 = (Person*)p2;
	if (ps1->age == ps2->age) {
		if (strcmp(ps1->first_name, ps2->first_name) == 0) {
			return strcmp(ps1->last_name, ps2->last_name);
		}
		else {
			return strcmp(ps1->first_name, ps2->first_name);
		}
	}
	else {
		return ps1->age < ps2->age;
	}

}

// Read data to Person array (till EOF)
int read_person_array(Person *persons) {
	char c = getc(stdin);
	char buffer[MAX_STR_LEN];
	int counter = 0;
	while (c != EOF) {
		ungetc(c, stdin);
		Person p1;
		fgets(buffer, MAX_STR_LEN, stdin);
		if (strlen(buffer) <= 1) 	break;
		p1.age = atoi(strtok(buffer, " "));
		strcpy(p1.first_name, strtok(NULL, " "));
		strcpy(p1.last_name, strtok(NULL, " "));
		// printf("%d %s %s\n", p1.age, p1.first_name, p1.last_name);
		persons[counter++] = p1;
		c = getc(stdin);
	}
	return counter;
}

// Print Person array
void print_person_array(Person *persons, int n) {
	for (int i=0; i<n; ++i) {
		printf("%d %s %s", persons[i].age, persons[i].first_name, persons[i].last_name);
	}
}

// Sort women first (woman's first name ends with 'a');
// Then sort women by age and men by last name
// Line consists of: age, first_name, last_name
// (in that order)
int cmp_lines(const void *l1, const void *l2) {
	char *line1 = (char*)l1, *line2 = (char*)l2;
	char buffer1[MAX_STR_LEN], buffer2[MAX_STR_LEN];
	strcpy(buffer1, line1);
	strcpy(buffer2, line2);
	int age1 = atoi(strtok(buffer1, " "));
	char *fname1 = strtok(NULL, " ");
	char *lname1 = strtok(NULL, " ");
	int age2 = atoi(strtok(buffer2, " "));
	char *fname2 = strtok(NULL, " ");
	char *lname2 = strtok(NULL, " ");

	if (fname1[strlen(fname1)-1] == 'a') {
		if (fname2[strlen(fname2)-1] == 'a') {
			return age1 > age2;
		}
		else {
			return -1;
		}
	}
	else {
		if (fname2[strlen(fname2)-1] == 'a') {
			return 1;
		}
		else {
			return strcmp(lname1, lname2);
		}
	}
}

// Read lines with students' data (as text)
int read_lines(char lines[][MAX_STR_LEN]) {
	int current = 0;
	char c = getc(stdin);
	char buffer[MAX_STR_LEN];
	while (c != EOF) {
		ungetc(c, stdin);
		fgets(buffer, MAX_STR_LEN, stdin);
		if (strlen(buffer) <= 1)	break;
		strcpy(lines[current++], buffer);
		c = getc(stdin);
	}
	return current;
}

// Print sorted lines
void print_lines(char lines[][MAX_STR_LEN], int n) {
	for (int i=0; i<n; ++i) {
		printf("%s", lines[i]);
	}
}

// -------------------------------------------------

int read_int() {
	char buf[MAX_STR_LEN];
	int n;
	fgets(buf, MAX_STR_LEN, stdin);
	sscanf(buf, "%d", &n);
	return n;
}

int main(void) {
	int to_do = read_int();
	int n;
	Person persons[MAX_PERSONS];
	char lines[MAX_PERSONS][MAX_STR_LEN];
	switch (to_do) {
		case 1:
			n = read_person_array(persons);
			qsort(persons, (size_t)n, sizeof(Person), cmp_person);
			print_person_array(persons, n);
			break;
		case 2:
			n = read_lines(lines);
			qsort(lines, (size_t) n, MAX_STR_LEN, cmp_lines);
			print_lines(lines, n);
			break;
		default:
			printf("Nothing to do for %d\n", to_do);
			break;
	}
}

