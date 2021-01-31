import numpy as np
import pandas as pd
import sys, os

def compute_error(true_file, approx_file):
    
    if(os.stat(approx_file).st_size == 0):
        # print("empty")
        approx_vals = [[0,1]]
    else:
        approx_vals = pd.read_csv(approx_file, header = None, sep = " ")
        approx_vals = approx_vals.to_numpy() 
    
    true_vals = pd.read_csv(true_file, header = None, sep = " ").to_numpy()

    ind_app = 0
    ind_true = 0
    error = 0
    max_error = 0
    cnt = 0
    u,cu = approx_vals[ind_app]
    v,cv = true_vals[ind_true]
    # for i in range(max(len(approx_vals), len(true_vals))):
    while (ind_true < len(true_vals)):
        # print("::", ind_app+1, ind_true+1, )
        if(u==v):
            # print(u,v)
            error += max(cu/cv, cv/cu)
            max_error = max(max_error, max(cu/cv, cv/cu))
            cnt = cnt + 1
            ind_app = ind_app + 1
            ind_true = ind_true + 1
        elif(u > v or ind_app == len(approx_vals)): #approx v is  0, true v is not 0, count as approx is 1
            # print(v,v)
            error += cv
            max_error = max(max_error, cv)
            cnt = cnt + 1
            ind_true = ind_true + 1
        else:
            ind_app = ind_app + 1
            
        if(ind_app< len(approx_vals)):
            u,cu = approx_vals[ind_app]
        if(ind_true< len(true_vals)):
            v,cv = true_vals[ind_true]

    assert(cnt == len(true_vals))
    return error/cnt, max_error

def compute_error_naive(true_file, approx_file):
    approx_vals = pd.read_csv(approx_file, header = None, sep = " ").to_numpy() 
    true_vals = pd.read_csv(true_file, header = None, sep = " ").to_numpy()

    ind_app = 0
    ind_true = 0
    error = 0
    max_error = 0
    cnt = 0

    # for i in range(max(len(approx_vals), len(true_vals))):
    while (ind_true < len(true_vals)):
        u,cu = approx_vals[ind_app]
        v,cv = true_vals[ind_true]
        if(cv != 0):
            if(cu == 0):
                cu = 1
            error += max(cu/cv, cv/cu)
            max_error = max(max_error, max(cu/cv, cv/cu))
            cnt = cnt + 1

        ind_app = ind_app + 1 
        ind_true = ind_true + 1

    # assert(cnt == len(true_vals))
    print(ind_true)
    return error/cnt, max_error

if __name__ == "__main__":
    eps = sys.argv[1]
    lam = sys.argv[2]
    num_rounds = 0
    dataset = sys.argv[3]
    batch = 0

    if(dataset=="dblp"):
        num_rounds = 20
        batch = int(1e5)
    elif(dataset=="livejournal"):
        num_rounds = 85
        batch = int(1e6)
    else:
        print("wrong dataset name")
        quit()
    print(eps)
    # delta = 3
    error_rounds = 0
    max_error_rounds = 0
    for i in range(1,num_rounds+1):
        count = batch * i
        # error, max_error = compute_error_naive("~/outputs0/dblp/dblp_true_%s" % count, "~/forked/DynHyperCoreDecomp/outputs0/dblp_eps/dblp_%s_%s.out" % (eps, count))

        # error, max_error = compute_error("~/outputs/%s/%s_true_%s" % (dataset, dataset, count), "~/forked/DynHyperCoreDecomp/outputs/dblp_alpha_3_%s_round_0_%s.out" % (eps, count))


        error, max_error = compute_error("~/outputs/%s/%s_true_%s" % (dataset, dataset, count), "/home/sy/forked/DynHyperCoreDecomp/outputs/%s_eps_lam/%s_%s_%s_round_0_%s.out" % (dataset, dataset, eps, lam, count))
        print(i, error, max_error )
        error_rounds = error_rounds + error
        max_error_rounds = max(max_error_rounds, max_error)
    print((error_rounds+1)/(num_rounds + 1), max_error_rounds)