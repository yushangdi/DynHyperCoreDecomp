import numpy as np
import pandas as pd
import re
import sys

total_time_find = re.compile("# PBBS time: Finished!!!: .+?\n")

# median batch time excluding first run
def get_median_batch_time(dataset, eps, lam, batchsize):
    f1 = open("./timing/dynamic_outputs/%s_eps_alpha/%s_%s_%s_%s.txt" % (dataset, dataset, eps, lam, batchsize))  
    data1 = f1.read()
    times = []
    for line in total_time_find.findall(data1+"\n"):
        times.append(float(line[25:-1]))
    med_ind = np.argsort(times[1:])[1]
    med_val = np.median(times[med_ind+1])
    return med_ind, med_val

# min batch time for all runs
def get_min_batch_time(dataset, eps, lam, batchsize):
    f1 = open("./timing/dynamic_outputs/%s_eps_alpha/%s_%s_%s_%s.txt" % (dataset, dataset, eps, lam, batchsize))  
    data1 = f1.read()
    times = []
    for line in total_time_find.findall(data1+"\n"):
        times.append(float(line[25:-1]))
    min_ind = np.argsort(times[:2])[0]
    min_val = np.median(times[min_ind])
    return min_ind, min_val

def get_batch_time(dataset, eps, lam, batchsize, ind):
    f1 = open("./timing/dynamic_outputs/%s_eps_alpha/%s_%s_%s_%s.txt" % (dataset, dataset, eps, lam, batchsize))  
    data1 = f1.read()
    times = []
    for line in total_time_find.findall(data1+"\n"):
        times.append(float(line[25:-1]))
    return times[ind]

def get_timing2(i, dataset, r, eps, lam):
    batchsize = 10**i
    data_timing = pd.read_csv("timing/outputs/%s_eps_alpha/%s_%s_%s_round_%s_1e+0%s_timing.out" % (dataset,dataset, eps, lam, r, i), header = None, sep = " ").to_numpy()
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
    elif(dataset=="dblp_insertion"):
        num_rounds = 10
        batch = 5
    elif(dataset=="livejournal_insertion"):
        num_rounds = 42
        batch = 6
    else:
        print("wrong dataset name")
        quit()
    
    epsilons=[0.1, 0.2, 0.4, 0.8, 1.6, 3.2, 6.4]
    lambdas=[0, 1.1] 

    for lam in lambdas:
        for eps in epsilons:
            ind, val = get_min_batch_time(dataset, eps, lam, int(10**batch))
            batchsize,avg_t, max_t = get_timing2(batch, dataset, ind, eps, lam)
            print(eps, lam, val, avg_t, max_t)
            # try:
            #     val = get_batch_time(dataset, eps, lam, int(10**batch), 0)
            #     batchsize,avg_t, max_t = get_timing2(batch, dataset, 0, eps, lam)
            #     print(eps, lam, val, avg_t, max_t)
            # except:
            #     print(eps, lam, -1, -1, -1)
    
    