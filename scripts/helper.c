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

    char mask[256];
    memset(mask, '0', 256);
    mask[1] = 'x';
    mask[(nproc - 1) / 4 + 2 - (pind - 1) / 4] = '0' + (1 << ((pind - 1) % 4));
    mask[(nproc - 1) / 4 + 3] = 0;

    for(int i = pind - 1; i < nsteps; i += nproc) {
        double scale = start + i * (end - start) / nsteps;

        char log_name[256];
        sprintf(log_name, "output/logs/log_%.6lf.lammps", scale);
        dup2(open(log_name, O_WRONLY | O_CREAT, 0644), STDOUT_FILENO);

	char in_name[256];
        sprintf(in_name, "/tmp/in_%d_%d", pind, (i - pind + 1) / nproc + 1);

        char cmd[2048];
        sprintf(cmd, "(echo \"variable scale string %.6lf\" && cat scripts/in.Fe3O4) > %s ; taskset %s lmp -i %s ; rm %s\n", scale, in_name, mask, in_name, in_name);
        printf(cmd);
        system(cmd);
    }

    for(int i = 1; i < nproc && pind == nproc; i++)
        wait(NULL);
    return 0;
}
