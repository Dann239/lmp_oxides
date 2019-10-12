(echo "variable scale equal $1" && \
cat scripts/vars.Fe3O4 && \
cat scripts/in.Fe3O4) | \
lmp -sf kk -k on t 8
