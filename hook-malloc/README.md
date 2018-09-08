# This simple-utility helps to detected memory leak
* By overriding standard glibc malloc(), calloc(), realloc(), free() functions

* Sample output

ADDRESS | NAME | SIZE | POINTER | HEAP
--- | --- | --- | --- | ---
 |  |  |  |  | 0
0x4005c0 | malloc | 111 | 0x1a84018 | 111
0x4005d7 | calloc | 222 | 0x1a84098 | 333
0x4005eb | realloc | 333 | 0x1a84188 | 666
0x4005fd | realloc | 444 | 0x1a84188 | 777
0x40060a | free | 444 | 0x1a84188 | 777
0x400612 | free | 222 | 0x1a84098 | 333
0x40061a | free | 111 | 0x1a84018 | 111
 |  |  |  |  | 0 <- **No leak!**


ADDRESS | NAME | SIZE | POINTER | HEAP
--- | --- | --- | --- | ---
 |  |  |  |  | 0
0x4004ee | malloc | 111 | 0xa62018 | 111
 |  |  |  |  | 111   <- **Leak detected!**
