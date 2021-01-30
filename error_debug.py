import numpy as np
import pandas as pd

def compute_error(true_file, approx_file):
    approx_vals = pd.read_csv(approx_file, header = None, sep = " ").to_numpy() 
    true_vals = pd.read_csv(true_file, header = None, sep = " ").to_numpy()

    ind_app = 0
    ind_true = 0
    # error = 0
    errors = []
    max_error = 0
    cnt = 0
    zero_cnt = 0
    u,cu = approx_vals[ind_app]
    v,cv = true_vals[ind_true]
    for i in range(max(len(approx_vals), len(true_vals))):
        # print(u,v)
        if(u==v):
            e = max(cu/cv, cv/cu)
            # if(e > 1):
            #     print(e)
            # error += e
            errors.append(e)
            max_error = max(max_error, e)
            cnt = cnt + 1
            ind_app = ind_app + 1
            ind_true = ind_true + 1
        elif(u > v): #approx v is  0, true v is not 0, count as approx is 1
            # error += cv
            print(cv)
            errors.append(cv)
            max_error = max(max_error, cv)
            cnt = cnt + 1
            zero_cnt = zero_cnt + 1
            ind_true = ind_true + 1
        else:
            # print(0)
            ind_app = ind_app + 1
   
        if(ind_app< len(approx_vals)):
            u,cu = approx_vals[ind_app]
        if(ind_true< len(true_vals)):
            v,cv = true_vals[ind_true]
    print(zero_cnt, cnt)
    return np.mean(errors), np.var(errors), max_error


if __name__ == "__main__":
    batch = 100000
    eps = 6.4
    # delta = 3
    error_rounds = []
    error_var_rounds = []
    max_error_rounds = 0
    for i in range(1,20):
        count = batch * i
        error_mean, error_var, max_error = compute_error("../outputs/dblp/dblp_true_%s" % count, "./outputs/dblp/dblp_eps/dblp_%s_%s.out" % (eps, count))
        # error, max_error = compute_error("../outputs/orkut/orkut_true_%s" % count, "./outputs/orkut_eps/orkut_%s_%s.out" % (eps, count))
        print(i, error_mean, error_var, max_error )
        error_rounds.append(error_mean) 
        error_var_rounds.append(error_var) 
        max_error_rounds = max(max_error_rounds, max_error)
    error_rounds.append(1) 
    print(np.mean(error_rounds),np.var(error_rounds), np.mean(error_var_rounds), max_error_rounds)