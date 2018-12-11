#include <stdio.h>
#include <stdlib.h>

struct patch {
	int id;
	int x;
	int y;
	int w;
	int h;
};

int main(int argc, char * argv[])
{
	struct patch tmp;

	FILE * f = fopen("input-3", "r");
	char * map = (char*)calloc(1024, 1024);
	int sum = 0;
	while (EOF != fscanf(f, "#%d @ %d,%d: %dx%d\n", &tmp.id, &tmp.x, &tmp.y, &tmp.w, &tmp.h)) {
		int y = tmp.y + tmp.h - 1;
		while (y >= tmp.y) {
			int x = tmp.x + tmp.w - 1;
			while (x >= tmp.x) {
				char * t = &map[1024 * y + x];
				//printf("%p %x\n", map, t-map);
				switch (*t) {
					case 0: *t = 1; break;
					case 1: *t = 2; sum++; break;
					default: *t = 3; break;
				}
				x--;
			}
			y--;
		}
	}
	printf("3a: %d\n", sum);

	// 3b
	fclose(f);
	f = fopen("input-3", "r");
	while (EOF != fscanf(f, "#%d @ %d,%d: %dx%d\n", &tmp.id, &tmp.x, &tmp.y, &tmp.w, &tmp.h)) {
		int y = tmp.y + tmp.h - 1;
		int thisone = 1;
		while (y >= tmp.y) {
			int x = tmp.x + tmp.w - 1;
			while (x >= tmp.x) {
				char * t = &map[1024 * y + x];
				//printf("%p %x\n", map, t-map);
				if (*t != 1) thisone = 0;
				x--;
			}
			y--;
		}
		if (thisone) {
			printf("3b: %d\n", tmp.id);
		}
	}

	//printf("%d\n", s);
	//printf("id:%d x:%d y:%d w:%d :h%d\n", tmp.id, tmp.x, tmp.y, tmp.w, tmp.h);
	return 0;
}
