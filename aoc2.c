#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

struct c {
	int twos;
	int threes;
};

char * lcount(char * i, struct c * o)
{
	o->twos = 0;
	o->threes = 0;
	int counts[26] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int x = 0;
	while (*i != '\n' && *i != 0) {
		counts[*i - (int)'a']++;
	//	printf("%c", *i);
		++i;
	}
	while (x < 26) {
		//printf("%d %d\n", x, counts[x]);
		if (counts[x] == 2) o->twos++;
		if (counts[x] == 3) o->threes++;
		x++;
	}
	//printf("%d %d\n", o->twos, o->threes);
	return ++i;
}

int countdiff(char * i1, char ** i2)
{
	int diffcount = 0;
	// XXX this condition assumes i2 will hit \0 before i1
	while (*i1 != '\n' && **i2 != 0) {
		if (*i1 != **i2) diffcount++;
		i1++;
		(*i2)++;
	}
	if (diffcount == 1) {
		printf("%.27s\n%.27s\n", i1-26, (*i2)-26);
	}
	return diffcount;
}

/*
 * wrziyfdmlumeqvaatbiosngkxc
 * wrziyfdmlumeqvaatbiosngkoc
 *
 * wrziyfdmlumeqvaatbiosngkc
 *
 */

int main(int argc, char * argv[])
{
	int fd = open("input-2", O_RDONLY);
	char buf[100000];
	int ret = read(fd, buf, 100000);
	buf[ret] = 0;
	struct c o;


	char * r = buf;
	struct c sum = { 0, 0 };
	while (*r != 0) {
		r = lcount(r, &o);
		if (o.twos) sum.twos++;
		if (o.threes) sum.threes++;
	//	printf("%d %d\n", sum.twos, sum.threes);
	}
	printf("2a: %d\n", sum.twos * sum.threes);

	// 2b: find substrings that differ by exactly one character
	char * t1 = buf;
	char * t2 = lcount(t1, &sum); // to get the start of the second line
	char ** id2 = &t2;

	// outer loop
	while (t1 < buf + ret - 30) {
		// inner loop
		while (**id2 != 0) {
			countdiff(t1, id2);
			(*id2)++;
		}
		t1 = lcount(t1, &sum);
		*id2 = lcount(t1, &sum);
	}

	return 0;
}

