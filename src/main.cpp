//
// Created by Libro on 2021/4/19.
//

#include <iostream>
#include "includes/unordered_map.hpp"
#include "includes/parser.h"

LaMetropole::parser Apollo;

void addUser(LaMetropole::parser::PaperCup *cup);

void login(LaMetropole::parser::PaperCup *cup);

void logout(LaMetropole::parser::PaperCup *cup);

void queryProfile(LaMetropole::parser::PaperCup *cup);

void modifyProfile(LaMetropole::parser::PaperCup *cup);

void addTrain(LaMetropole::parser::PaperCup *cup);

void releaseTrain(LaMetropole::parser::PaperCup *cup);

void queryTrain(LaMetropole::parser::PaperCup *cup);

void deleteTrain(LaMetropole::parser::PaperCup *cup);

void queryTicket(LaMetropole::parser::PaperCup *cup);

void queryTransfer(LaMetropole::parser::PaperCup *cup);

void buyTicket(LaMetropole::parser::PaperCup *cup);

void queryOrder(LaMetropole::parser::PaperCup *cup);

void refundTicket(LaMetropole::parser::PaperCup *cup);

void Clean(LaMetropole::parser::PaperCup *cup);

void Exit(LaMetropole::parser::PaperCup *cup){
    exit(0);
}

void (*commandMap[])(LaMetropole::parser::PaperCup *) ={
        addUser, login, logout, queryProfile, modifyProfile, addTrain,
        releaseTrain,queryTrain, deleteTrain, queryTicket, queryTransfer,
        buyTicket, queryOrder,refundTicket, Clean, Exit
};

int main() {
    while (1) {
        LaMetropole::parser::PaperCup* cup=Apollo.listen();
        commandMap[cup->keyType](cup);
        delete cup;
    }
}