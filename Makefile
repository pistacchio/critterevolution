CXX=clang++
FLAGS=-std=c++11 -stdlib=libc++ -lsfml-window -lsfml-graphics -lsfml-system

all:
    $(CXX) $(FLAGS) main.cpp -o cevolution

clean:
    rm -rf cevolution

.PHONY: all clean