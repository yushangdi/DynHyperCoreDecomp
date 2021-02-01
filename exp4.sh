#!/usr/bin/env bash

DATASET="$1"
echo "$DATASET"
eps=0.4
lam=3
BATCH=1000000

[ -d "timing" ] || mkdir "timing"
[ -d "timing/outputs" ] || mkdir "timing/outputs"
[ -d "timing/dynamic_outputs" ] || mkdir "timing/dynamic_outputs"
[ -d "timing/outputs/${DATASET}_eps_lam" ] || mkdir "timing/outputs/${DATASET}_eps_lam"
[ -d "timing/dynamic_outputs/${DATASET}_eps_lam" ] || mkdir "timing/dynamic_outputs/${DATASET}_eps_lam"

echo "./FullyDynamic ${eps} ${lam} ~/datasets/edge_orientation_exps/${DATASET}_edges timing/outputs/${DATASET}_eps_lam/${DATASET} 0 ${BATCH}"
./FullyDynamic ${eps} ${lam} ~/datasets/edge_orientation_exps/${DATASET}_edges timing/outputs/${DATASET}_eps_lam/${DATASET} 0 ${BATCH} > timing/dynamic_outputs/${DATASET}_eps_lam/${DATASET}_${eps}_${lam}_${BATCH}.txt 

echo "python exp2.py ${DATASET} > timing/python_outputs_${DATASET}.txt" 
python exp2.py ${DATASET} > timing/exp2_python_outputs_${DATASET}.txt