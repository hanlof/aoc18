#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char map[150][151];
char * shadow = &map[0][0];
char * end = &map[149][149];

// direction (4)
// track-type (4)
// inter-dir (3)

// inter-order: left, straight, right, left, straight, ...

#define RIGHT 0
#define LEFT 1
#define UP 2
#define DOWN 3
#define DN DOWN
#define STRAIGHT 2
#define DASH 0
#define PIPE 1
#define SLASH 2
#define BSLASH 3
#define PLUS 4
#define MK(movement,inter,track) (movement | inter << 2 | track << 4)
struct cart {
	int movement;
	int inters_dir;
	char * pos;
};

struct cart * carts;

// x:147 y:23 |
// x:49 y:41 |
// x:110 y:69 |
// x:93 y:73 -
// x:117 y:76 |
// x:59 y:80 -
// x:33 y:82 -
// x:114 y:83 -
// x:77 y:94 -
// x:12 y:99 |
// x:51 y:107 -
// x:114 y:113 -
// x:12 y:115 |
// x:39 y:117 |
// x:103 y:120 -
// x:114 y:126 -
// x:33 y:146 -

#define M(C, R) if (c->movement == C) { c->movement = R; }
void intersection(struct cart * c)
{
	int cart_n = c - carts;
	if (c->inters_dir == LEFT) {
		//printf("L(%d) ", cart_n);
		M(UP, LEFT)
		else M(LEFT, DOWN)
		else M(DOWN, RIGHT)
		else M(RIGHT, UP)
		else { printf("WTF\n"); }
		c->inters_dir = STRAIGHT;
	} else if (c->inters_dir == RIGHT) {
		//printf("R(%d) ", cart_n);
		M(UP, RIGHT)
		else M(RIGHT, DOWN)
		else M(DOWN, LEFT)
		else M(LEFT, UP)
		else { printf("WTF\n"); }
		c->inters_dir = LEFT;
	} else if (c->inters_dir == STRAIGHT) {
		//printf("S(%d) ", cart_n);
		// movement does not change!
		c->inters_dir = RIGHT;
	}
	if (c->movement == UP) {
		c->pos = c->pos - 151;
	} else if (c->movement == DOWN) {
		c->pos = c->pos + 151;
	} else if (c->movement == LEFT) {
		c->pos = c->pos - 1;
	} else if (c->movement == RIGHT) {
		c->pos = c->pos + 1;
	}
}

void update_pos(struct cart * c)
{
	switch (*(c->pos)) {
		case '/':
			if (c->movement == UP) {
				c->pos = c->pos + 1;
				c->movement = RIGHT;
			} else if (c->movement == DOWN) {
				c->pos = c->pos - 1;
				c->movement = LEFT;
			} else if (c->movement == LEFT) {
				c->pos = c->pos + 151;
				c->movement = DOWN;
			} else if (c->movement == RIGHT) {
				c->pos = c->pos - 151;
				c->movement = UP;
			}
			break;
		case '\\':
			if (c->movement == UP) {
				c->pos = c->pos - 1;
				c->movement = LEFT;
			} else if (c->movement == DOWN) {
				c->pos = c->pos + 1;
				c->movement = RIGHT;
			} else if (c->movement == LEFT) {
				c->pos = c->pos - 151;
				c->movement = UP;
			} else if (c->movement == RIGHT) {
				c->pos = c->pos + 151;
				c->movement = DOWN;
			}
			break;
		case '-':
			if (c->movement == LEFT) {
				c->pos = c->pos - 1;
			} else if (c->movement == RIGHT) {
				c->pos = c->pos + 1;
			}
			break;
		case '|':
			if (c->movement == DOWN) {
				c->pos = c->pos + 151;
			} else if (c->movement == UP) {
				c->pos = c->pos - 151;
			}
			break;
		case '+':
			intersection(c);
			break;
		default:
			printf("!!!!\n");
			break;
	}
}

int comp(struct cart * c1, struct cart * c2)
{
	if (c1->pos < c2-> pos) return -1;
	if (c1->pos > c2-> pos) return 1;
	return 0;
}

#define X(cart) (((cart)->pos - shadow) % 151)
#define Y(cart) (((cart)->pos - shadow) / 151)
int main(int argc, char * argv[])
{
	int fd = open("input-13", O_RDONLY);
	(void) read(fd, map, 150*151);
	//printf("%s\n", &map[149][60]);

	carts = calloc(sizeof(struct cart), 1);
	int last_cart = 0;

	// convert carts to other representation
	// direction, intersection-dir, track-type
	for (char * t = shadow; t <= end; ++t) {
		switch (*t) {
			case '^':
				carts[last_cart].movement = UP;
				*t = '|';
				goto common;
			case 'v':
				carts[last_cart].movement = DOWN;
				*t = '|';
				goto common;
			case '<':
				carts[last_cart].movement = LEFT;
				*t = '-';
				goto common;
			case '>':
				carts[last_cart].movement = RIGHT;
				*t = '-';
common:
				carts[last_cart].pos = t;
				carts[last_cart].inters_dir = LEFT;
				last_cart++;
				carts = realloc(carts, sizeof(struct cart) * (last_cart + 1));
		}
	}
	last_cart--;

	int ticks = 1;
	// do one step!
	while (ticks < 20000) {
		int crash = 0;
		for (struct cart * c = carts; c <= &carts[last_cart]; c++) {
			update_pos(c);
			// check collision
			for (struct cart * c2 = carts; c2 <= &carts[last_cart]; c2++) {
				if (c == c2) continue;
				if (c->pos == c2->pos) {
					printf("BOOM! ticks: %d %ld,%ld\n",
						ticks, X(c), Y(c));
					printf("BOOM! pos: %d\n", c->pos - shadow);
					c->pos = &shadow[151*150];
					c2->pos = &shadow[151*150];
					crash = 1;
					break;
				}
			}
		}
		ticks++;
		qsort(carts, last_cart + 1, sizeof(struct cart), comp);
		if (crash) {
			last_cart -= 2;
			printf("last_cart:%d\n", last_cart);
			if (last_cart < 2) {
				printf("x:%d y:%d\n", X(&carts[last_cart]), Y(&carts[last_cart]));
			}
		}
	}

}
