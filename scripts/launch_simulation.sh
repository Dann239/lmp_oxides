mkdir output output/logs output/dumps output/data output/restarts 2>/dev/null
echo Args: $(nproc) $@
gcc scripts/helper.c -o /tmp/helper_forker -std=c99 && time /tmp/helper_forker $(nproc) $@
