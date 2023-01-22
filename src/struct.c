#include "gol.h"

extern int row,col;

struct map newmap(void){
    struct map tmp;
    tmp.line = malloc(row*col*sizeof(struct cell));
    for (int r = 0; r < row; r++){
        tmp.line[r].cell = malloc(sizeof(struct cell)*col);
    }
    return tmp;
}


void freemap(struct map map){
    for (int r = 0; r < row; r++){
        for (int c = 0; c < col; c++){
            free(map.line[r].cell);
        }
    }
    free(map.line);
    return;
}
