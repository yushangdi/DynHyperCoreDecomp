import numpy as np
import pandas as pd
import sys

def compute_error(true_file, approx_file):
    approx_vals = pd.read_csv(approx_file, header = None, sep = " ").to_numpy() 
    true_vals = pd.read_csv(true_file, header = None, sep = " ").to_numpy()

    ind_app = 0
    ind_true = 0
    error = 0
    max_error = 0
    cnt = 0
    u,cu = approx_vals[ind_app]
    v,cv = true_vals[ind_true]
    for i in range(max(len(approx_vals), len(true_vals))):
        if(u==v):
            error += max(cu/cv, cv/cu)
            max_error = max(max_error, max(cu/cv, cv/cu))
            cnt = cnt + 1
            ind_app = ind_app + 1
            ind_true = ind_true + 1
        elif(u > v): #approx v is  0, true v is not 0, count as approx is 1
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

    return error/cnt, max_error


if __name__ == "__main__":
    batch = 10000000
    eps = sys.argv[1]
    print(eps)
    # delta = 3
    error_rounds = 0
    max_error_rounds = 0
    for i in range(1,24):
        count = batch * i
        # error, max_error = compute_error("../outputs/dblp/dblp_true_%s" % count, "./outputs/dblp/dblp_eps/dblp_%s_%s.out" % (eps, count))
        error, max_error = compute_error("../outputs/orkut/orkut_true_%s" % count, "./outputs/orkut/orkut_eps/orkut_%s_%s.out" % (eps, count))
        print(i, error, max_error )
        error_rounds = error_rounds + error
        max_error_rounds = max(max_error_rounds, max_error)
    print((error_rounds+1)/20, max_error_rounds)