#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define CASE_MASK ((char)(~((char)0x20)))
int match_delete_condition(char c1, char c2)
{
	if ((c1 & (~0x20)) != (c2 & (~0x20))) {
		return 0;
	}
	return c1 != c2;
}

int reduce(char * buf, int remaining)
{

	char * sof = &buf[0];
	char * eof = &buf[remaining - 1];
	char * c1 = &buf[0];
	char * c2 = &buf[1];

	while (1) {
		// precond: c1 and c2 point at non-null char
		if (match_delete_condition(*c1, *c2)) {
			remaining -= 2;
			*c1 = 0;
			*c2 = 0;
			while (*c1 == 0 && c1 > sof) --c1;
			if (c1 == sof) {
				while (*sof == 0) ++sof;
				c1 = sof;
				c2 = sof + 1;
			}
			while (*c2 == 0 && c2 < eof) ++c2;
		} else {
			if (c2 == eof) {
				break;
			}
			++c1;
			while (*c1 == 0 && c1 < c2) ++c1;
			++c2;
			while (*c2 == 0 && c2 < eof) ++c2;
		}
		// printf("rem:%d c1:%p(%c) c2:%p(%c)\n", remaining, c1-buf, *c1, c2-buf, *c2);
	}

//	printf("rem:%d c1:%ld(%c) c2:%ld(%c)\n", remaining, c1-buf, *c1, c2-buf, *c2);
//	c1 = buf;
//	while (c1 <= c2) {
//		if (*c1) {
//			printf("%s ", c1);
//		}
//		++c1;
//		while (*c1 != 0 && c1 < c2) ++c1;
//		while (*c1 == 0 && c1 < c2) ++c1;
//	}
//	printf("rem:%d c1:%ld(%c) c2:%ld(%c)\n", remaining, c1-buf, *c1, c2-buf, *c2);
	// AabCBlLb
	// aaAxxxxxx
	// aa00aA
	return remaining;
}

int remove_char(char * buf, char r)
{
	int counter = 0;
	while (*buf != 0) {
		if ((*buf & ~0x20) == (r & ~0x20)) {
			*buf = 0;
			counter++;
		}
		buf++;
	}
	return counter;
}

int main(int argc, char * argv[])
{
	char * buf = malloc(100000);
	int fd = open("input-5", O_RDONLY);
	int ret = read(fd, buf, 100000);
	close(fd);
	buf[ret] = 0;

	printf("6a: %d\n", reduce(buf, ret));
	//	fd = open("aoc-6", O_RDONLY);
	//	ret = read(fd, buf, 100000);
	//	close(fd);
	//	buf[ret] = 0;
	//	remove_char(buf, 'm');
	//	printf("%c: %d\n", 'm', reduce(buf, ret));

	for (char c = 'a'; c <= 'z'; c++) {
		fd = open("input-5", O_RDONLY);
		ret = read(fd, buf, 100000);
		close(fd);
		buf[ret] = 0;
		int t = remove_char(buf, c);
		printf("%c: %d\n", c, reduce(buf, ret) - t);
	}


}
