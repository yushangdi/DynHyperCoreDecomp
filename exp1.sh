#!/usr/bin/env bash

DATASET="$1"
echo "$DATASET"
BATCH=0
if [[ "$DATASET" == "dblp" ]]; then
    BATCH=100000
elif [[ "$DATASET" == "livejournal" ]]; then
    BATCH=1000000
else
    echo "wrong dataset"
    exit
fi

[ -d "outputs" ] || mkdir "outputs"
[ -d "dynamic_outputs" ] || mkdir "dynamic_outputs"
[ -d "python_outputs" ] || mkdir "python_outputs"

[ -d "outputs/${DATASET}_eps_alpha" ] || mkdir "outputs/${DATASET}_eps_alpha"
[ -d "dynamic_outputs/${DATASET}_eps_alpha" ] || mkdir "dynamic_outputs/${DATASET}_eps_alpha"
[ -d "python_outputs/${DATASET}_eps_alpha" ] || mkdir "python_outputs/${DATASET}_eps_alpha"


### consider both eps and alpha. alpha = 0 means using theoretically efficient
epsilons=(0.02 0.04 0.06 0.08 0.1)
alphas=(0 1.1)


run_all_lam(){
    eps=$1
for lam in "${alphas[@]}"; do
    echo $eps $lam
    ./FullyDynamic ${eps} ${lam} ~/datasets/edge_orientation_exps/${DATASET}_edges outputs/${DATASET}_eps_alpha/${DATASET} 1 ${BATCH} > dynamic_outputs/${DATASET}_eps_alpha/${DATASET}_${eps}_${lam}.txt 
    python3 error.py ${eps} ${lam} ${DATASET} > python_outputs/${DATASET}_eps_alpha/${DATASET}_${eps}_${lam}.txt 
done
}
for eps in "${epsilons[@]}"; do
    run_all_lam $eps &
done

## check python_outputs for last line

### consider only lambdas
# eps=0.2 #does not matter

# run_lam(){
#     lam=$1
#     echo $lam 
#     echo "./FullyDynamic ${eps} ${lam} ~/datasets/edge_orientation_exps/${DATASET}_edges outputs/${DATASET}_eps_alpha/${DATASET} 1 ${BATCH} > dynamic_outputs/${DATASET}_eps_alpha/${DATASET}_${eps}_${lam}.txt "
#     ./FullyDynamic ${eps} ${lam} ~/datasets/edge_orientation_exps/${DATASET}_edges outputs/${DATASET}_eps_alpha/${DATASET} 1 ${BATCH} > dynamic_outputs/${DATASET}_eps_alpha/${DATASET}_${eps}_${lam}.txt 
#     python error.py ${eps} ${lam} ${DATASET} > python_outputs/${DATASET}_eps_alpha/${DATASET}_${eps}_${lam}.txt 
# }
# for lam in "${lambdas[@]}"; do
#     run_lam $lam &
# done