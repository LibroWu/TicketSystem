//
// Created by Leon on 2021/4/19.
//

#include <iostream>
#include "includes/unordered_map.hpp"
#include "includes/parser.h"
#include "includes/BPT.hpp"
#include "includes/ticketSystem.h"
//LaMetropole::ticketSystem ts;

using namespace std;
using LaMetropole::L_time;

int main() {
    L_time tmp(6,26,18,0);
    cout<<tmp<<'\n';
    tmp-=600;
    cout<<tmp<<'\n';
    tmp-=600;
    cout<<tmp<<'\n';
    tmp-=600;
    cout<<tmp<<'\n';
    tmp-=600;
    cout<<tmp<<'\n';
    return 0;
}