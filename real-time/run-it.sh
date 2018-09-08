#!/bin/bash

LD_PRELOAD=$PWD/build/lib_hook_malloc.so build/sample 2> build/log

echo "Running: sample..."
cat build/log
echo "Running: sample [complete]"

LD_PRELOAD=$PWD/build/lib_hook_malloc.so build/leak-sample 2> build/log

echo "Running: leak-sample..."
cat build/log
echo "Running: leak-sample [complete]"