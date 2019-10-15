mkdir output output/logs output/dumps output/data output/restarts 2>/dev/null
echo Args: $(nproc) $@
gcc scripts/helper.c -o /tmp/lammps_helper -std=c99 && time /tmp/lammps_helper $(nproc) $@
rm /tmp/lammps_helper