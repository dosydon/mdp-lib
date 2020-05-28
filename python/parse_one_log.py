#!/usr/bin/env python

import sys
import re
import json

def parse(data):
    dict = {}
    m = re.search("Exec cost (\d+\.*\d*)", data)
    dict["execution_cost"] = float(m.group(1))

    m = re.search("Num\. decisions (\d+)", data)
    dict["num_decisions"] = int(m.group(1))

    m = re.search("Dev\. (\d+\.\d+)", data)
    dict["stds"] = float(m.group(1))

    m = re.search("Avg\. time per decision (\d+\.*\d*)", data)
    dict["time_per_decision"] = float(m.group(1))

    m = re.search("Total time (\d+\.*\d*)", data)
    dict["total_time"] = float(m.group(1))
    return dict

if __name__ == '__main__':
    with open(sys.argv[1]) as f:
        data = f.read()
        dict = parse(data)
        print(json.dumps(dict))
