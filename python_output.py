import numpy as np
import pandas as pd

if __name__ == "__main__":

    for eps in [0.2]:
        for lam in [96]:
            data = pd.read_csv("./python_outputs/livejournal_eps_lam/livejournal_%s_%s.txt"%(eps,lam) , skiprows=1, header = None, sep = " ")
            print(eps, lam, np.average(data[1]), np.max(data[2]))