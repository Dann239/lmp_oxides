mkdir output output/logs output/dumps output/data 2>/dev/null
gcc scripts/helper.c -o /tmp/helper_forker && time /tmp/helper_forker $@
