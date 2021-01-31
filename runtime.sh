#!/usr/bin/env bash

# BATCH=1000000
# DATASET="livejournal"
DATASET="$1"
echo "$DATASET"
eps=0.4
lam=3

# if [[ "$DATASET" == "dblp" ]]; then
#     BATCH=100000
# elif [[ "$DATASET" == "livejournal" ]]; then
#     BATCH=1000000
# else
#     echo "wrong dataset"
#     exit
# fi

[ -d "timing/outputs/${DATASET}_eps_lam" ] || mkdir "timing/outputs/${DATASET}_eps_lam"
[ -d "timing/dynamic_outputs/${DATASET}_eps_lam" ] || mkdir "timing/dynamic_outputs/${DATASET}_eps_lam"

batches=(2 3 4 5 6 7)

for b in "${batches[@]}"; do
    BATCH=$((10**$b))
    echo $BATCH 
    echo "./FullyDynamic ${eps} ${lam} ~/datasets/edge_orientation_exps/${DATASET}_edges timing/outputs/${DATASET}_eps_lam/${DATASET} 0 ${BATCH}"
    ./FullyDynamic ${eps} ${lam} ~/datasets/edge_orientation_exps/${DATASET}_edges timing/outputs/${DATASET}_eps_lam/${DATASET} 0 ${BATCH} > timing/dynamic_outputs/${DATASET}_eps_lam/${DATASET}_${eps}_${lam}.txt 
done