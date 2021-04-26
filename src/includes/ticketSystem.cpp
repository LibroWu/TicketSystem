//
// Created by Libro on 2021/4/21.
//

#include "ticketSystem.h"

namespace LaMetropole{
    ticketSystem::ticketSystem() {
        while (1) {
            LaMetropole::parser::PaperCup* cup=Apollo.listen();
            commandMap[cup->keyType](cup);
            delete cup;
        }
    }
    void LM::addUser(parser::PaperCup *cup) {

    }

    void LM::Clean(parser::PaperCup *cup) {

    }

    void LM::Exit(parser::PaperCup *cup) {

    }

    void LM::addTrain(parser::PaperCup *cup) {

    }

    void LM::buyTicket(parser::PaperCup *cup) {}

    void LM::deleteTrain(parser::PaperCup *cup) {}

    void LM::login(parser::PaperCup *cup) { }

    void LM::logout(parser::PaperCup *cup) {}


}