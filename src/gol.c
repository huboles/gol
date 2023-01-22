#include "gol.h"

int  row,col,step,ascii,weight;

int main(int argc, char *argv[]){
    srand(time(NULL));

    int stop = 1;
    row = 30;
    col = 90;
    step = 75000;
    ascii = 0;
    weight = 5;

    parseopts(argc,argv);

    struct map map = newmap();
    genmap(map);

    while (stop > 0){
        stop = updatemap(map);
        printdisplay(map);
        usleep(step);
    }

    freemap(map);

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
