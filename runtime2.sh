#!/usr/bin/env bash

DATASET="$1"
echo "$DATASET"
eps=0.4
lam=3
BATCH=1000000

[ -d "timing/outputs/${DATASET}_eps_lam" ] || mkdir "timing/outputs/${DATASET}_eps_lam"
[ -d "timing/dynamic_outputs/${DATASET}_eps_lam" ] || mkdir "timing/dynamic_outputs/${DATASET}_eps_lam"

echo "./FullyDynamic ${eps} ${lam} ~/datasets/edge_orientation_exps/${DATASET}_edges timing/outputs/${DATASET}_eps_lam/${DATASET} 0 ${BATCH}"
./FullyDynamic ${eps} ${lam} ~/datasets/edge_orientation_exps/${DATASET}_edges timing/outputs/${DATASET}_eps_lam/${DATASET} 0 ${BATCH} > timing/dynamic_outputs/${DATASET}_eps_lam/${DATASET}_${eps}_${lam}_${BATCH}.txt 