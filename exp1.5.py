import numpy as np
import pandas as pd
import re

total_time_find = re.compile("# PBBS time: Finished!!!: .+?\n")
def get_median_batch_time(dataset, eps, lam, batchsize):
    f1 = open("./timing/dynamic_outputs/%s_eps_lam/%s_%s_%s_%s.txt" % (dataset, dataset, eps, lam, batchsize))  
    data1 = f1.read()
    times = []
    for line in total_time_find.findall(data1+"\n"):
        times.append(float(line[25:-1]))
    med_ind = np.argsort(times[1:])[1]
    med_val = np.median(times[med_ind+1])
    return med_ind, med_val

def get_timing2(i, dataset, r, eps, lam):
    batchsize = 10**i
    data_timing = pd.read_csv("timing/outputs/%s_eps_lam/%s_%s_%s_round_%s_1e+%s_timing.out" % (dataset,dataset, eps, lam, r, i), header = None, sep = " ").to_numpy()
    timings = []
    sizes = []
    time_acc = 0
    size_acc = 0
    for (batch, time) in data_timing:
        time_acc = time_acc + time
        size_acc = size_acc + 1
        if(batch % batchsize == 0):
            timings.append(time_acc)
            sizes.append(size_acc)
            time_acc = 0
            size_acc = 0
    return batchsize, np.average(timings), np.max(timings)

if __name__ == "__main__":
    dataset = sys.argv[1]
    num_rounds = 0
    batch = 0

    if(dataset=="dblp"):
        num_rounds = 20
        batch = 5
    elif(dataset=="livejournal"):
        num_rounds = 85
        batch = 6
    else:
        print("wrong dataset name")
        quit()
    
    epsilons=[0.2, 0.4, 0.8, 1.6, 3.2, 6.4]
    lambdas=[3, 6, 12, 24, 48, 96] 

    for eps in epsilons:
        for lam in lambdas:
            try:
                med_ind, med_val = get_median_batch_time(dataset, eps, lam, int(10**batch))
                batchsize,avg_t, max_t = get_timing2(batch, dataset, med_ind, eps, lam)
                print(eps, lam, med_val, avg_t, max_t)
            except:
                print(eps, lam, -1, -1, -1)
    
    