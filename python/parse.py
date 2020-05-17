#!/usr/bin/env python

import sys
import re
import json

def parse(data):
    dict = {}
    costs = re.findall("Exec cost (\d+\.*\d*)", data)
    dict["execution_cost"] = [float(cost) for cost in costs]

    maxTimes = re.findall("maxTime:(\d+)", data)
    dict["max_time"] = [int(maxTime) for maxTime in maxTimes]

    numDecisions = re.findall("Num\. decisions (\d+)", data)
    dict["num_decisions"] = [int(numDecision) for numDecision in numDecisions]

    stds = re.findall("Dev\. (\d+\.\d+)", data)
    dict["stds"] = [float(std) for std in stds]
    return dict

if __name__ == '__main__':
    with open(sys.argv[1]) as f:
        data = f.read()
        dict = parse(data)
        print(json.dumps(dict))
