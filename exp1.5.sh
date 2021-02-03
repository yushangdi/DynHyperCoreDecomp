#!/usr/bin/env bash

DATASET="$1"
echo "$DATASET"
BATCH=0
if [[ "$DATASET" == "dblp" ]]; then
    BATCH=100000
elif [[ "$DATASET" == "dblp_insertion" ]]; then
    BATCH=100000
elif [[ "$DATASET" == "livejournal_insertion" ]]; then
    BATCH=1000000
elif [[ "$DATASET" == "livejournal" ]]; then
    BATCH=1000000
else
    echo "wrong dataset"
    exit
fi

[ -d "timing" ] || mkdir "timing"
[ -d "timing/outputs" ] || mkdir "timing/outputs"
[ -d "timing/dynamic_outputs" ] || mkdir "timing/dynamic_outputs"
[ -d "timing/outputs/${DATASET}_eps_alpha" ] || mkdir "timing/outputs/${DATASET}_eps_alpha"
[ -d "timing/dynamic_outputs/${DATASET}_eps_alpha" ] || mkdir "timing/dynamic_outputs/${DATASET}_eps_alpha"

# epsilons=(0.1 0.2) #0.1 0.2 0.4 0.8 1.6 3.2 6.4
# alphas=(1.1) #0 


run_all_lam(){
    eps=$1
    for lam in "${alphas[@]}"; do
        echo $eps $lam 
        echo "./FullyDynamic ${eps} ${lam} ~/datasets/edge_orientation_exps/${DATASET}_edges timing/outputs/${DATASET}_eps_alpha/${DATASET} 0 ${BATCH}"
        ./FullyDynamic ${eps} ${lam} ~/datasets/edge_orientation_exps/${DATASET}_edges timing/outputs/${DATASET}_eps_alpha/${DATASET} 0 ${BATCH} > timing/dynamic_outputs/${DATASET}_eps_alpha/${DATASET}_${eps}_${lam}_${BATCH}.txt 
    done
}
for eps in "${epsilons[@]}"; do
    run_all_lam $eps 
done

echo "python exp1.5.py ${DATASET} > timing/exp1_5_python_outputs_${DATASET}.txt" 
python exp1.5.py ${DATASET} > timing/exp1_5_python_outputs_${DATASET}.txt

# eps=0.4 #does not matter

# run_lam(){
#     lam=$1
#     echo $lam 
#         echo "./FullyDynamic ${eps} ${lam} ~/datasets/edge_orientation_exps/${DATASET}_edges timing/outputs/${DATASET}_eps_alpha/${DATASET} 0 ${BATCH}"
#         ./FullyDynamic ${eps} ${lam} ~/datasets/edge_orientation_exps/${DATASET}_edges timing/outputs/${DATASET}_eps_alpha/${DATASET} 0 ${BATCH} > timing/dynamic_outputs/${DATASET}_eps_alpha/${DATASET}_${eps}_${lam}_${BATCH}.txt 
# }
# for lam in "${lambdas[@]}"; do
#     run_lam $lam 
# done