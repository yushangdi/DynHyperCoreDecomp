#!/usr/bin/env bash

BATCH=1000000
# DATASET="livejournal"
DATASET="$1"
echo "$DATASET"

if [[ "$DATASET" == "dblp" ]]; then
    BATCH=100000
elif [[ "$DATASET" == "livejournal" ]]; then
    BATCH=1000000
else
    echo "wrong dataset"
    exit
fi

[ -d "outputs/${DATASET}_eps_lam" ] || mkdir "outputs/${DATASET}_eps_lam"
[ -d "dynamic_outputs/${DATASET}_eps_lam" ] || mkdir "dynamic_outputs/${DATASET}_eps_lam"
[ -d "python_outputs/${DATASET}_eps_lam" ] || mkdir "python_outputs/${DATASET}_eps_lam"


# epsilons=(0.8 1.6 3.2 6.4)
epsilons=(0.2 0.4 0.8 1.6 3.2 6.4)
#
lambdas=(3 6 12 24 48 96)


run_all_lam(){
    eps=$1
    for lam in "${lambdas[@]}"; do
        echo $eps $lam 
        ./FullyDynamic ${eps} ${lam} ~/datasets/edge_orientation_exps/${DATASET}_edges outputs/${DATASET}_eps_lam/${DATASET} 1 ${BATCH} > dynamic_outputs/${DATASET}_eps_lam/${DATASET}_${eps}_${lam}.txt 
        python3 error.py ${eps} ${lam} ${DATASET} > python_outputs/${DATASET}_eps_lam/${DATASET}_${eps}_${lam}.txt 
    done
}
for eps in "${epsilons[@]}"; do
    run_all_lam $eps &
done