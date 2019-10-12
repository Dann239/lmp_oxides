#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
    if(argc < 4)
        return 0;
    double start, end, step;
    sscanf(argv[1], "%lf", &start);
    sscanf(argv[2], "%lf", &end);
    sscanf(argv[3], "%lf", &step);
    for(double i = start; i <= end; i+= step) {
        char cmd[2048];
        sprintf(cmd, "scripts/measure.sh %lf", i);
        system(cmd);
    }
    return 0;
}
