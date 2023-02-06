#include "gol.h"

int  row,col,step,ascii,weight,file;
char *filename;

int main(int argc, char *argv[]){
    srand(time(NULL));

    row = 30;
    col = 90;
    step = 75000;
    weight = 5;

    filename = malloc(sizeof(char) * MAXLINE);
    parseopts(argc,argv);
    MAP map = newmap();

    if (file) {
        mapfile(map,filename);
    } else {
        genmap(map);
    }

    while (1){
        updatemap(map);
        printdisplay(map);
        usleep(step);
    }

    freemap(map);
    free(filename);
    return 0;
}


void printdisplay(MAP map){
    printf("%s",CLEAR);
    for (int r = 0; r < row; r++){
        for (int c = 0; c < col; c++){
            if (ascii){
                printf("%c",map.line[r].cell[c].cellchar);
            } else {
                printf("%4s",map.line[r].cell[c].pixel);
            }
        }
        putc('\n',stdout);
    }

    return;
}
