import numpy as np
import pandas as pd

def get_timing(batchsize, dataset, r):
    data_timing = pd.read_csv("timing/outputs/%s_eps_lam/%s_0.4_3_round_%s_100_timing.out" % (dataset,dataset, r), header = None, sep = " ").to_numpy()
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
    print(batchsize, np.average(timings), np.max(timings))

# for i in [2,3,4,5,6]:
#     get_timing(10**i, "dblp", 2)
# for i in [2,3,4,5,6,7]:
#     get_timing(10**i, "livejournal", 1)

def get_timing2(batchsize, dataset, r):
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
    print(batchsize, np.average(timings), np.max(timings))

get_timing2(10**6, "orkut", 2)