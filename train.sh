#!/usr/bin/env sh
LOG=./try0426/model/log-`date +%Y-%m-%d-%H-%M-%S`.log 
set -e
export PYTHONPATH=/home/pengshanzhen/ssd/caffe-ssd/build/install/python:$PYTHONPATH

#export PYTHONPATH=./://media/d/work/SSD/caffe/python:$PYTHONPATH
/home/pengshanzhen/ssd/caffe-ssd/build/tools/caffe train -solver solver.prototxt -weights /media/pengshanzhen/bb42233c-19d1-4423-b161-e5256766be8e/SSD/trafficlight/shanzhen/crop/2/try2/model/snapshots/model_iter_108000.caffemodel 2>&1  | tee $LOG $@
