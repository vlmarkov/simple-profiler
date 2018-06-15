# This utility helps to detected memory leak
1. Override by macros standart glibc malloc(), calloc(), realloc(), free() functions
2. Additional expansion to get size of allocated/free pointer
3. Static and real-time aproach* (no recursive protection be aware)
4. Python script can analyze and detected (alloc()/free()) mismatch and report about it
