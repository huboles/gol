#include "gol.h"

extern int row,col,weight;

void genmap(MAP map){
    int rnd = 0;
    for (int r = 0; r < row; r++){
        for (int c = 0; c < col; c++){
            rnd = rand();
            if (rnd % weight == 0){
                map.line[r].cell[c].state = ALIVE;
            } else {
                map.line[r].cell[c].state = DEAD;
            }
        }
    }
    return;
}

void mapfile(MAP map, char *file){
    FILE *fp = fopen(file, "r");
    if (!fp) errorcheck(FILE_ERR);
    
    int r = 0;
    int c = 0;
    char x = ' ';

    while ((x = getc(fp)) != EOF){
        /* exit if more rows than in map */
        if (r >= row) { break; }

        /* spaces are dead, anything else alive */
        if (x == ' ') { 
            map.line[r].cell[c].state = DEAD;
        } else if (c < col) {
            map.line[r].cell[c].state = ALIVE;
        }

        /* move down a row and reset columns at newlines and column limits */
        if (x == '\n' || c >= col ) { 
            map.line[r].cell[c].state = DEAD;
            r++,c=0; 
        }

        c++;
    }

    fclose(fp);
    return;
}

void updatemap(MAP map){
    MAP tmp = newmap();

    /* non edges */
    for (int r = 0; r < row-1; r++){
        for(int c = 0; c < col-1; c++){
            int state = checkstate(map,r,c);
            tmp.line[r].cell[c].state = state;
            tmp.line[r].cell[c].cellchar = statechar(state);
            strncpy(tmp.line[r].cell[c].pixel,statecell(state),4);
        }
    }

    /* left/right side */
    for (int r = 0; r < row - 1; r++){
        int state = checkstate(map,r,0);
        tmp.line[r].cell[col-1].state = state;
        tmp.line[r].cell[col-1].cellchar = statechar(state);
        strncpy(tmp.line[r].cell[col-1].pixel,statecell(state),4);
    }

    /* top/bottom row */
    for (int c = 0; c < col - 1; c++){
        int state = checkstate(map,0,c);
        tmp.line[row-1].cell[c].state = state;
        tmp.line[row-1].cell[c].cellchar = statechar(state);
        strncpy(tmp.line[row-1].cell[c].pixel,statecell(state),4);
    }

    /* copy to map */
    for(int r = 0; r < row; r++){
        for (int c = 0; c < col; c++){
            map.line[r].cell[c] = tmp.line[r].cell[c];
        }
    }

    return;
}

int checkstate(MAP map,int r, int c){
    int state = 0;
    int n = numneighbor(map, r, c);

    if (map.line[r].cell[c].state < ALIVE) {
        if (n == 3) {
            state = SPAWN;
        } else {
            state = DEAD;
        }
    } else {
        if (n > 1 && n < 4) {
            state = ALIVE;
        } else {
            state = DIE;
        }
    }

    return state;
}

char statechar(int state){
    switch (state) {
        case (DIE): return ' ';
        case (SPAWN): return '#';
        case (ALIVE): return '#';
        case (DEAD): return ' ';
        default:
            errorcheck(STATE_ERR);
            return 'E';
    }
}

char *statecell(int state){
    switch (state) {
        case (DIE): return "\u25CC";
        case (SPAWN): return "\u25CB";
        case (ALIVE): return "\u25EF";
        case (DEAD): return "\u2002";
        default:
            errorcheck(STATE_ERR);
            return "X";
    }
}


int numneighbor(MAP map, int r, int c){
    int n = 0;
    /* non-edges */
    if (r > 0 && r < row - 1 && c > 0 && c < col - 1){
        if (map.line[r+1].cell[c+1].state > DEAD) n++;
        if (map.line[r].cell[c+1].state > DEAD) n++;
        if (map.line[r-1].cell[c+1].state > DEAD) n++;
        if (map.line[r+1].cell[c].state > DEAD) n++;
        if (map.line[r-1].cell[c].state > DEAD) n++;
        if (map.line[r+1].cell[c-1].state > DEAD) n++;
        if (map.line[r].cell[c-1].state > DEAD) n++;
        if (map.line[r-1].cell[c-1].state > DEAD) n++;
    /* top/bottom row */
    } else if (r == 0 && c > 0 && c < col){
        if (map.line[r+1].cell[c+1].state > DEAD) n++;
        if (map.line[r].cell[c+1].state > DEAD) n++;
        if (map.line[row-2].cell[c+1].state > DEAD) n++;
        if (map.line[r+1].cell[c].state > DEAD) n++;
        if (map.line[row-2].cell[c].state > DEAD) n++;
        if (map.line[r+1].cell[c-1].state > DEAD) n++;
        if (map.line[r].cell[c-1].state > DEAD) n++;
        if (map.line[row-2].cell[c-1].state > DEAD) n++;
    /* left/right side */
    } else if (r > 0 && r < row && c == 0){
        if (map.line[r+1].cell[c+1].state > DEAD) n++;
        if (map.line[r].cell[c+1].state > DEAD) n++;
        if (map.line[r-1].cell[c+1].state > DEAD) n++;
        if (map.line[r+1].cell[c].state > DEAD) n++;
        if (map.line[r-1].cell[c].state > DEAD) n++;
        if (map.line[r+1].cell[col-2].state > DEAD) n++;
        if (map.line[r].cell[col-2].state > DEAD) n++;
        if (map.line[r-1].cell[col-2].state > DEAD) n++;
    }
    return n;
}
