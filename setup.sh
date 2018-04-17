#!/usr/bin/env bash

set -eu

# make llvm ir
#clang --std=c++11  -emit-llvm -S -o main.ll main.cpp
#g++ -g easypass.cpp `llvm-config-3.8 --cxxflags --ldflags --libs` -c  -o easypass.o
#g++ -g -shared -o easypass.so easypass.o
#opt -load ./easypass.so -easypass main.ll
nasm -g -f elf -o helloworld.o helloworld.asm && \
gcc -L/usr/lib/x86_64-linux-gnu -m32 helloworld.o -o hello && \
g++ -g -Wall -std=c++11 main.cpp &&./a.out hello
