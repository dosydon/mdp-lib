#!/usr/bin/env python

import sys
import json
import matplotlib.pyplot as plt

if __name__ == '__main__':
    with open(sys.argv[1]) as f:
        data = f.read()
        dict = json.loads(data)
        y = dict["execution_cost"]
        x = dict["max_time"]
        err = dict["stds"]
        plt.ylabel("Execution Costs")
        plt.xlabel("Time per Re-Planning")
        plt.xscale('log')
#         plt.errorbar(x, y, err, fmt='-o')
        plt.plot(x, y)
        plt.savefig(sys.argv[2])
