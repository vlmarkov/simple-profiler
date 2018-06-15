#!/bin/bash

./main_static

LD_PRELOAD=$PWD/lib_alloc.so ./main_dynamic