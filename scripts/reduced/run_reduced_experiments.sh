#!/bin/bash

pddl_folder=../../data/ppddl/ippc2008
domain=ex-blocksworld
problem=p01
k=2

# Creating all possible determinizations
./create_all_determinizations.py -d $pddl_folder/$domain/domain.pddl \
  -o /tmp/$domain
  
num_det=`ls /tmp/${domain}_det*.pddl -l | wc -l`
let "num_det=$num_det-1"

# Evaluating all possible determinizations in the first problem of this domain
all_successes=""
all_costs=""
for i in `seq 0 $num_det`; do
  successes_and_costs=`./run_experiment.sh $pddl_folder $domain p01 $i $k \
    | tail -n 1`
  echo $successes_and_costs
  all_successes=$all_successes`echo $successes_and_costs | awk '{print $1}'`,
  all_costs=$all_costs`echo $successes_and_costs | awk '{print $2}'`,
done
echo $all_successes
echo $all_costs

# Getting the best determinizations
n=3
best_n_determinizations=`./get_index_n_best_results.py \\
  -s ${all_successes::-1} -c ${all_costs::-1} -n $n `
best_determinization=`echo $best_n_determinizations | { read x _ ; echo $x; }`

# Solving all problems with the best determinization
for i in {07..10}; do
  echo `./run_experiment.sh $pddl_folder $domain p$i $best_determinization $k \\
    | tail -n 1`
done
