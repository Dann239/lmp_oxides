#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#define min(a,b) (a < b ? a : b)

int main(int argc, char** argv) {
    unsigned int nproc = 1, pind = 1, nsteps = 1;
    char mask[256];
    char buff[2048];
    double start = 1, end = 1;

    switch(argc) {
    case 5:
        sscanf(argv[4], "%d", &nsteps);
    case 4:
        sscanf(argv[3], "%lf", &end);
        sscanf(argv[2], "%lf", &start);
    case 2:
        sscanf(argv[1], "%d", &nproc);
        break;
    default:
        return 0;
    }
    if(nproc > 1024)
        return 0;

    for(; pind < nproc && fork(); pind++);

    memset(mask, '0', 256);
    mask[1] = 'x';
    mask[(nproc - 1) / 4 + 2 - (pind - 1) / 4] = '0' + (1 << ((pind - 1) % 4));
    mask[(nproc - 1) / 4 + 3] = 0;

    for(int i = pind - 1; i < nsteps; i += nproc) {
        double scale = start + i * (end - start);

        sprintf(buff, "output/logs/log_%.6lf.lammps", scale);
        dup2(open(buff, O_WRONLY | O_CREAT, 0644), STDOUT_FILENO);
	
        sprintf(buff, "(echo \"variable scale string %.6lf\" && cat scripts/in.Fe3O4) > /tmp/in_%d_%d.Fe3O4 ; taskset %s lmp -i /tmp/in_%d_%d.Fe3O4\n", scale, pind, (i - pind + 1) / nproc + 1, mask, pind, (i - pind + 1) / nproc + 1);
        printf(buff);
        system(buff);
    }

    for(int i = 1; i < nproc && pind == nproc; i++)
        wait(NULL);
    return 0;
}
