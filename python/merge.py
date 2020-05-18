#!/usr/bin/env python

import sys
import re
import json

def parse(data):
    dict = {}
    m = re.search("Exec cost (\d+\.*\d*)", data)
    dict["execution_cost"] = m.group(1)

    m = re.search("Num\. decisions (\d+)", data)
    dict["num_decisions"] = m.group(1)

    m = re.search("Dev\. (\d+\.\d+)", data)
    dict["std"] = m.group(1)

    m = re.search("Avg\. time per decision (\d+\.*\d*)", data)
    dict["time_per_decision"] = m.group(1)
    return dict

if __name__ == '__main__':
    merged = {
        "execution_cost": [],
        "max_time": [],
        "stds": [],
    }
    for filename in sys.argv[1:]:
        with open(filename) as f:
            data = f.read()
            dict = json.loads(data)
            merged["execution_cost"].append(dict["execution_cost"])
            merged["stds"].append(dict["stds"])
            merged["max_time"].append(dict["time_per_decision"])
    print(json.dumps(merged))
