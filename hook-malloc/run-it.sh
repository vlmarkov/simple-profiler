#!/bin/bash

LD_PRELOAD=$PWD/build/lib_hook_malloc.so build/sample 2> $PWD/log

echo "Running: sample..."
cat $PWD/log
echo "Running: sample [complete]"

LD_PRELOAD=$PWD/build/lib_hook_malloc.so build/leak-sample 2> $PWD/log

echo "Running: leak-sample..."
cat $PWD/log
echo "Running: leak-sample [complete]"