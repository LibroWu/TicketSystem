//
// Created by Leon on 2021/4/19.
//

#include <iostream>
#include "includes/unordered_map.hpp"
#include "includes/parser.h"
#include "includes/BPT.hpp"
#include "includes/ticketSystem.h"
#include <ctime>
LaMetropole::ticketSystem ts;

int main() {
    //system("if [ ! -d \"Z\" ]; then \n mkdir Z \n fi");
    time_t t=time(NULL);
    char ch[26];
    strcpy(ch,ctime(&t));
    cout<<ch;
}