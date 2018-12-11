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
	char * id1 = buf;
	char * id2 = lcount(id1, &sum);

	return 0;
}

