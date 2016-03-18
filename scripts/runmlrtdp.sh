#!/bin/bash

tracks=(known/barto-big-error known/square-4-error known/square-5-error \
        known/ring-5-error known/ring-6-error)
nsims=100

for track in ${tracks[@]}; do
  echo "${track}|lrtdp"
  ../testsolver.out --track=../data/tracks/$track.track \
  --algorithm=lrtdp --v=0 --n=1
  
  echo "${track}|mlrtdp"
  ../testsolver.out --track=../data/tracks/$track.track \
  --algorithm=mlrtdp --v=0 --n=1 --optimal
  
  horizon=1
  for i in `seq 0 4`; do
    let "horizon *= 2"
    echo "${track}|mlrtdp|hor=${horizon}"
      ../testsolver.out --track=../data/tracks/$track.track \
      --algorithm=mlrtdp --horizon=$horizon --v=0 --n=$nsims --no-initial-plan
  done
done