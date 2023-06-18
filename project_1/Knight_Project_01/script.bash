#!/bin/bash
# for t in 1 2 4 8 12 16 20 24 32
# do
#   for n in 1 10 100 1000 10000 100000 500000 1000000
#   do
#      g++   proj01.cpp  -DNUMT=$t -DNUMTRIALS=$n  -o proj01  -lm  -fopenmp
#     ./proj01
#   done
# done



for t in 1 2 4 8 12 16 20 24 32
do
    for n in 1 10 100 1000 10000 100000 500000 1000000
    do
        /usr/bin/clang++ -fcolor-diagnostics -fansi-escape-codes -Xpreprocessor -fopenmp -I/usr/local/opt/libomp/include/ -L/usr/local/opt/libomp/lib/ -lomp -DNUMT=$t -DNUMTRIALS=$n main.cpp -o main
        ./main
  done
done
        # /usr/bin/clang++ -fcolor-diagnostics -fansi-escape-codes -Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include -L/opt/homebrew/opt/libomp/lib -lomp -DNUMT=$t -DNUMTRIALS=$n main.cpp -o main
