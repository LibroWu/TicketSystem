//
// Created by Libro on 2021/4/19.
//

#include <iostream>
#include "includes/unordered_map.hpp"
using namespace std;

bool is_prime(int x){
    for (int i = 2; i*i<=x; ++i) {
        if (x%i==0) return false;
    }
    return true;
}

int get_next_prime(int n){
    n*=2;
    while (!is_prime(n)) ++n;
    return n;
}

int main(){
    int n=48;
    for (int i = 0; i < 25; ++i)
        cout<<(n= get_next_prime(n))<<','<<((i%10==9)?'\n':' ');
    /*for (int i = 0; i < s.length(); ++i) {
        cout<<s[i];
    }*/
}