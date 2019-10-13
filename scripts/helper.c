#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv) {
    if(argc == 3 || argc > 4)
        return 0;
    double args[3];
    for(int i = 1; i < argc; i++)
        sscanf(argv[i], "%lf", &args[i-1]);
    if(argc == 1)
        args[0] = 1;
    if(argc <= 2)
        args[2] = args[1] = args[0];
    if(args[2] <= 0)
        return 0;
    for(double i = args[0]; i < args[1] + 1e-6; i+= args[2]) {
        char cmd[512];
        sprintf(cmd, "\(echo \"variable scale string %.5lf\" && cat scripts/vars.Fe3O4 && cat scripts/in.Fe3O4) > /tmp/lmp_custom.Fe3O4 ; lmp -sf kk -k on t $(nproc) -i /tmp/lmp_custom.Fe3O4\n", i);
        system(cmd);
    }
    return 0;
}
