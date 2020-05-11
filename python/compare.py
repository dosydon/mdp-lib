#!/usr/bin/env python

import sys
import json
import os
import matplotlib.pyplot as plt

if __name__ == '__main__':
    dirname = sys.argv[1]
    methods = ["lrtdp", "flare0", "flare1"]
    for method in methods:
        filename = os.path.join(dirname, "{}.json".format( method))
        with open(filename) as f:
            data = f.read()
            dict = json.loads(data)
            y = dict["execution_cost"]
            x = dict["max_time"]
            plt.ylabel("Execution Costs")
            plt.xlabel("Time per Re-Planning")
            plt.plot(x, y, label=method)
    plt.legend()
#     plt.show()
    plt.savefig(sys.argv[2])
