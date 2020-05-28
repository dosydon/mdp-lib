#!/usr/bin/env python

import sys
import re
import json

if __name__ == '__main__':
    merged = {
        "execution_cost": [],
        "max_time": [],
        "stds": [],
        "total_time": [],
    }
    for filename in sys.argv[1:]:
        with open(filename) as f:
            data = f.read()
            try:
                dict = json.loads(data)
                merged["execution_cost"].append(dict["execution_cost"])
                merged["stds"].append(dict["stds"])
                merged["max_time"].append(dict["time_per_decision"])
                merged["total_time"].append(dict["total_time"])
            except ValueError:
                pass
    print(json.dumps(merged))
