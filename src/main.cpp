//
// Created by Libro on 2021/4/19.
//

#include <iostream>
#include "includes/unordered_map.hpp"
using namespace std;

int f(const int& t){
    return t;
}

LaMetropole::unordered_map<int,int> u_map(f);

int main(){
    int n=10000;
    for (int i = 0; i < n; ++i) {
        u_map[i]=n-i;
    }
    for (int i = 0; i < n; i+=2) {
        cout<<u_map[i]<<' ';
    }
}