#include "gol.h"

int  row,col,step,ascii,weight,file;
char *filename;

int main(int argc, char *argv[]){
    srand(time(NULL));

    int stop = 1;
    row = 30;
    col = 90;
    step = 75000;
    ascii = 0;
    weight = 5;
    file = 0;

    filename = malloc(sizeof(char) * MAXLINE);
    parseopts(argc,argv);
    struct map map = newmap();

    if (file) {
        mapfile(map,filename);
    } else {
        genmap(map);
    }

    while (stop > 0){
        stop = updatemap(map);
        printdisplay(map);
        usleep(step);
    }

    freemap(map);
    free(filename);
    return 0;
}


void printdisplay(struct map map){
    printf("%s",CLEAR);
    for (int r = 0; r < row; r++){
        for (int c = 0; c < col; c++){
            if (ascii == 0){
                printf("%4s",map.line[r].cell[c].pixel);
            } else {
                printf("%c",map.line[r].cell[c].cellchar);
            }
        }
        putc('\n',stdout);
    }

    return;
}
