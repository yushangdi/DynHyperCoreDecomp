#!/usr/bin/env bash

DATASET="$1"
echo "$DATASET"
# eps=0.4
# lam=3
BATCH=100000

[ -d "timing/outputs/${DATASET}_eps_lam" ] || mkdir "timing/outputs/${DATASET}_eps_lam"
[ -d "timing/dynamic_outputs/${DATASET}_eps_lam" ] || mkdir "timing/dynamic_outputs/${DATASET}_eps_lam"

# epsilons=(0.2 0.4 0.8 1.6 3.2 6.4)
lambdas=(6 12 24 48 96)


# run_all_lam(){
#     eps=$1
#     for lam in "${lambdas[@]}"; do
#         echo $eps $lam 
#         echo "./FullyDynamic ${eps} ${lam} ~/datasets/edge_orientation_exps/${DATASET}_edges timing/outputs/${DATASET}_eps_lam/${DATASET} 0 ${BATCH}"
#         ./FullyDynamic ${eps} ${lam} ~/datasets/edge_orientation_exps/${DATASET}_edges timing/outputs/${DATASET}_eps_lam/${DATASET} 0 ${BATCH} > timing/dynamic_outputs/${DATASET}_eps_lam/${DATASET}_${eps}_${lam}_${BATCH}.txt 
#     done
# }
# for eps in "${epsilons[@]}"; do
#     run_all_lam $eps &
# done

eps=0.4 #does not matter

run_lam(){
    lam=$1
    echo $lam 
        echo "./FullyDynamic ${eps} ${lam} ~/datasets/edge_orientation_exps/${DATASET}_edges timing/outputs/${DATASET}_eps_lam/${DATASET} 0 ${BATCH}"
        ./FullyDynamic ${eps} ${lam} ~/datasets/edge_orientation_exps/${DATASET}_edges timing/outputs/${DATASET}_eps_lam/${DATASET} 0 ${BATCH} > timing/dynamic_outputs/${DATASET}_eps_lam/${DATASET}_${eps}_${lam}_${BATCH}.txt 
}
for lam in "${lambdas[@]}"; do
    run_lam $lam 
done