#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <error.h>
#include <time.h>
#include <wchar.h>

#define MAXLINE 10000
#define VERSION "0.2.2"

#define CLEAR "\e[1;1H\e[2J"

enum {
    DIE = -1, 
    DEAD = 0, 
    ALIVE = 1, 
    SPAWN = 1 << 1
};

/* error codes */
enum {
    NO_ERR = 0, 
    SYS_ERR = 1,
    FLAG_ERR = 1 << 1,
    STATE_ERR = 1 << 2, 
    COLOR_ERR = 1 << 3, 
    FILE_ERR = 1 << 4
};

typedef int ERROR;

typedef struct map MAP;
struct map { struct line *line; };
struct line { struct cell *cell; };
struct cell {
    char    pixel[4];
    char    cellchar;
    int     state;
};


/* main.c */
void errorcheck(ERROR);
void parseopts(int,char **);
void help(void);
void printdisplay(MAP);

/* map.c */
void genmap(MAP);
void mapfile(MAP, char *);
void updatemap(MAP);
int numneighbor(MAP,int,int);
int checkstate(MAP,int,int);
char statechar(int);
char *statecell(int);

/* struct.c */
struct map newmap(void);
void freemap(MAP);
