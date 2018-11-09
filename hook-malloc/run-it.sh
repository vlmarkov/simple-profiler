#!/bin/bash

LD_PRELOAD=$PWD/build/lib_hook_malloc.so build/sample 2> $PWD/build/log

echo "Running: sample..."
cat $PWD/build/log
echo "Running: sample [complete]"
echo ""

LD_PRELOAD=$PWD/build/lib_hook_malloc.so build/leak-sample 2> $PWD/build/log

echo "Running: leak-sample..."
cat $PWD/build/log
echo "Running: leak-sample [complete]"
