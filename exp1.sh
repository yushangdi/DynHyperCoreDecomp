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

[ -d "outputs/${DATASET}_eps_lam" ] || mkdir "outputs/${DATASET}_eps_lam"
[ -d "dynamic_outputs/${DATASET}_eps_lam" ] || mkdir "dynamic_outputs/${DATASET}_eps_lam"
[ -d "python_outputs/${DATASET}_eps_lam" ] || mkdir "python_outputs/${DATASET}_eps_lam"


### consider both eps and lambdas
epsilons=(0.2 0.4 0.8 1.6 3.2 6.4)
lambdas=(3 6 12 24 48 96)


run_all_lam(){
    eps=$1
for lam in "${lambdas[@]}"; do
    # echo $eps $lam
    echo $lam 
    ./FullyDynamic ${eps} ${lam} ~/datasets/edge_orientation_exps/${DATASET}_edges outputs/${DATASET}_eps_lam/${DATASET} 1 ${BATCH} > dynamic_outputs/${DATASET}_eps_lam/${DATASET}_${eps}_${lam}.txt 
    python3 error.py ${eps} ${lam} ${DATASET} > python_outputs/${DATASET}_eps_lam/${DATASET}_${eps}_${lam}.txt 
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
#     echo "./FullyDynamic ${eps} ${lam} ~/datasets/edge_orientation_exps/${DATASET}_edges outputs/${DATASET}_eps_lam/${DATASET} 1 ${BATCH} > dynamic_outputs/${DATASET}_eps_lam/${DATASET}_${eps}_${lam}.txt "
#     ./FullyDynamic ${eps} ${lam} ~/datasets/edge_orientation_exps/${DATASET}_edges outputs/${DATASET}_eps_lam/${DATASET} 1 ${BATCH} > dynamic_outputs/${DATASET}_eps_lam/${DATASET}_${eps}_${lam}.txt 
#     python error.py ${eps} ${lam} ${DATASET} > python_outputs/${DATASET}_eps_lam/${DATASET}_${eps}_${lam}.txt 
# }
# for lam in "${lambdas[@]}"; do
#     run_lam $lam &
# done