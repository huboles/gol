#include "gol.h"


extern int row,col,step,ascii,color,weight;

void parseopts(int n, char **args){
    char c;
    while ((c = getopt(n,args,"hVacy:x:t:w:")) != -1){
        switch (c) {
            case 'h':
                help();
                break;
            case 'V':
                printf("v%s\n",VERSION);
                exit(0);
                break;
            case 'x':
                col = strtol(optarg,NULL,10);
                break;
            case 'y':
                row = strtol(optarg,NULL,10);
                break;
            case 't':
                step = strtol(optarg,NULL,10);
                break;
            case 'w':
                weight = strtol(optarg,NULL,10);
                break;
            case 'a':
                ascii = 1;
                break;
            case '?':
                fprintf(stderr, "\t%s\n",optarg);
                errorcheck(FLAG_ERR);
                break;
        }
    }

    return;
}

void errorcheck(int err){
    if (errno != 0) {
        fprintf(stderr,"\nERROR: %s\n",strerror(errno));
        exit(EXIT_FAILURE);
    } 

    if (err == NO_ERR) { return; }

    if ((err & FLAG_ERR) != 0) {
        fprintf(stderr,"ERROR: Unknown Flag\n");
        printf("Use -h for help");
    }

    if ((err & STATE_ERR) != 0) { fprintf(stderr,"ERROR: Could not determine cell state\n"); }

    exit(err);
}

void help(){
    printf("Usage:\n");
    printf("\tgol [OPTIONS]");
    printf("Option flags:\n");
    printf("\t-x [INT] number of columns, default: 90\n");
    printf("\t-y [INT] number of rows, default: 30\n");
    printf("\t-a\tascii only mode\n");
    printf("\t-t [ms]\tamount of time to wait in ms between generations, default: 75000\n");
    printf("\t-w [INT] initial distribution weight\n");
    printf("\t-V\tversion information\n");
    printf("\t-h\tshow this help\n");

    exit(0);
}
