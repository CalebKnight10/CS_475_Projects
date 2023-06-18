for t in 1 3 5 7 9 11 13 15 17 19 21 23 25 27 29 31 33 35
do
    /usr/bin/clang++ -fcolor-diagnostics -fansi-escape-codes -Xpreprocessor -fopenmp -I/usr/local/opt/libomp/include/ -L/usr/local/opt/libomp/lib/ -lomp -DNUMT=$t -DNUMTRIALS=$n main.cpp -o main
    ./main
done