# project_1:			main.cpp
# 					g++		main.cpp   -o	project_1	-lm		-fopenmp

proj1:         main.cpp
    /usr/local/Cellar/gcc/12.2.0/bin/g++-12     -lomp main.cpp   -o proj1 -I /usr/local/opt/libomp/include/ -L /usr/local/opt/libomp/lib/    -fopenmp

clang -Xpreprocessor -fopenmp -I/usr/local/opt/libomp/include/ -L/usr/local/opt/libomp/lib/ -lomp main.c -o main
