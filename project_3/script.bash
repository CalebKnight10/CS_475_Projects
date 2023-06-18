# /usr/bin/clang++ -fcolor-diagnostics -fansi-escape-codes -Xpreprocessor -fopenmp -I/usr/local/opt/libomp/include/ -L/usr/local/opt/libomp/lib/ -lomp -DNUMT=$t -DNUMTRIALS=$n main.cpp -o main
# ./main

#!/bin/bash
for n in 1024 2048 4096 8192 16384 32768
do
  for m in true true true true true false false false false false false false false false false
  do
        /usr/bin/clang++  -fcolor-diagnostics -fansi-escape-codes -Xpreprocessor -fopenmp  -I/usr/local/opt/libomp/include/ -L/usr/local/opt/libomp/lib/ -lomp -DNUMT=$n -DNUMMUTEX=$m main.cpp -o main
      ./main
  done
done