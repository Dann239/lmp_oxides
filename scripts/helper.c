#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#define min(a,b) (a < b ? a : b)

int main(int argc, char** argv) {
    int nproc, pind;
    char mask[256];
    char buff[2048];   
    double args[3];     

    for(int i = 2; i < min(argc, 5); i++)
        sscanf(argv[i], "%lf", &args[i - 2]);
    switch(argc) {
    case 2:
        args[0] = 1;
    case 3:
        args[2] = args[1] = args[0];
    case 5:
        sscanf(argv[1], "%d", &nproc);
        break;
    default:
        return 0;
    }

    if(args[2] <= 0 || nproc > 1024)
        return 0;

    for(pind = 1; pind < nproc && fork(); pind++);

    memset(mask, '0', 256);
    mask[1] = 'x';
    mask[(nproc - 1) / 4 + 2 - (pind - 1) / 4] = '0' + (1 << ((pind - 1) % 4));
    mask[(nproc - 1) / 4 + 3] = 0;

    for(int i = pind - 1; i < (args[1] - args[0] + 1e-6) / args[2]; i += nproc) {
        double scale = args[0] + i * args[2];
        
        sprintf(buff, "output/logs/log_%.5lf.lammps", scale);
        dup2(open(buff, O_WRONLY | O_CREAT, 0644), STDOUT_FILENO);
        
        sprintf(cmd, "(echo \"variable scale string %.5lf\" && cat scripts/in.Fe3O4) > /scratch/pavlov/tmp/in_%d.Fe3O4 ; taskset %s lmp -i /scratch/pavlov/tmp/in_%d.Fe3O4\n", scale, pind, mask, pind);
        system(buff);
    }

    for(int i = 1; i < nproc && pind == nproc; i++)
        wait(NULL);
    return 0;
}
