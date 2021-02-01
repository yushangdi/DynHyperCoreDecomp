import numpy as np
import pandas as pd
import re
# import matplotlib.pyplot as plt

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

def get_timing(batchsize, dataset, r):
    data_timing = pd.read_csv("timing/outputs/%s_eps_lam/%s_0.4_3_round_%s_1e+06_timing.out" % (dataset,dataset, r), header = None, sep = " ").to_numpy()
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

    # timings.append(time_acc)
    # sizes.append(size_acc)
    return batchsize, np.average(timings), np.max(timings)

if __name__ == "__main__":
    dataset = sys.argv[1]

    med_ind, med_val = get_median_batch_time(dataset, 0.4, 3)
    batchsize,avg_t, max_t = get_timing(10**6, dataset, med_ind)
    print(batchsize, med_val, avg_t, max_t)