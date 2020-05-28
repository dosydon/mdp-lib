#!/usr/bin/env python

import sys
import json
import os
import matplotlib.pyplot as plt

if __name__ == '__main__':
    dirname = sys.argv[1]
    methods = ["lrtdp", "flares0", "flares1", "flares", "brtdp", "brtdp-lb", "vpi-rtdp", "rtdp", "rtdp-ub"]
    plt.xscale('log')
    plt.ylabel("Execution Costs")
    plt.xlabel("Average Total Planning Time")
    for method in methods:
        filename = os.path.join(dirname, "{}.json".format( method))
        with open(filename) as f:
            data = f.read()
            dict = json.loads(data)
            y = dict["execution_cost"]
            x = dict["total_time"]
            err = dict["stds"]
            plt.errorbar(x, y, err, label=method, fmt='-o')
#             plt.plot(x, y, label=method)
    plt.legend()
#     plt.show()
    plt.savefig(sys.argv[2])
