#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <error.h>
#include <time.h>
#include <wchar.h>

#define MAXLINE 10000
#define VERSION "0.2.1"

#define CLEAR "\e[1;1H\e[2J"

enum {DIE = -1, DEAD = 0, ALIVE = 1, SPAWN = 1 << 1};

/* error codes */
enum {NO_ERR = 0, FLAG_ERR = 1, STATE_ERR = 1 << 1, COLOR_ERR = 1 << 2};

struct cell {
    char    pixel[4];
    char    cellchar;
    int     state;
};

struct line {
     struct cell *cell;
};

struct map {
    struct line *line;
};

/* main.c */
void errorcheck(int);
void parseopts(int,char **);
void help(void);
void printdisplay(struct map);

/* map.c */
void genmap(struct map);
int updatemap(struct map);
int numneighbor(struct map,int,int);
int checkstate(struct map,int,int);
char statechar(int);
char *statecell(int);

/* struct.c */
struct map newmap(void);
void freemap(struct map);
