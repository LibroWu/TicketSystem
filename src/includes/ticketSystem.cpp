//
// Created by Libro on 2021/4/21.
//

#include "ticketSystem.h"

namespace LaMetropole{
    void (ticketSystem::*commandMap[17])(parser::PaperCup *) ={
            &ticketSystem::addUser, &ticketSystem::login, &ticketSystem::logout, &ticketSystem::queryProfile, &ticketSystem::modifyProfile, &ticketSystem::addTrain,
            &ticketSystem::releaseTrain, &ticketSystem::queryTrain, &ticketSystem::deleteTrain, &ticketSystem::queryTicket, &ticketSystem::queryTransfer,
            &ticketSystem::buyTicket, &ticketSystem::queryOrder, &ticketSystem::refundTicket, &ticketSystem::Clean, &ticketSystem::Exit, &ticketSystem::None
    };
    ticketSystem::ticketSystem() {
        std::fstream file("user_crystal.file", std::fstream::in);
        if (!file.is_open()) {
            Libro.initialise();
        } else Libro.initialise(true);
        while (1) {
            LaMetropole::parser::PaperCup* cup=Apollo.listen();
            (this->*commandMap[cup->keyType])(cup);
            delete cup;
        }
    }
    void ticketSystem::addUser(parser::PaperCup *cup) {
        cout<<(Libro.add_user(cup)?0:-1)<<'\n';
    }

    void ticketSystem::Clean(parser::PaperCup *cup) {
        Libro.clear();
        cout<<0<<'\n';
    }

    void ticketSystem::Exit(parser::PaperCup *cup) {
        cout<<"bye\n";
        exit(0);
    }

    void ticketSystem::addTrain(parser::PaperCup *cup) {}

    void ticketSystem::buyTicket(parser::PaperCup *cup) {}

    void ticketSystem::deleteTrain(parser::PaperCup *cup) {}

    void ticketSystem::login(parser::PaperCup *cup) {
        cout<<(Libro.login(cup)?0:-1)<<'\n';
    }

    void ticketSystem::logout(parser::PaperCup *cup) {
        cout<<(Libro.logout(cup)?0:-1)<<'\n';
    }

    void ticketSystem::refundTicket(parser::PaperCup *cup) {}

    void ticketSystem::releaseTrain(parser::PaperCup *cup) {}

    void ticketSystem::queryTransfer(parser::PaperCup *cup) {}

    void ticketSystem::queryTrain(parser::PaperCup *cup) {}

    void ticketSystem::queryTicket(parser::PaperCup *cup) {}

    void ticketSystem::queryProfile(parser::PaperCup *cup) {
        if (!Libro.query_profile(cup)) cout<<"-1\n";
    }

    void ticketSystem::queryOrder(parser::PaperCup *cup) {}

    void ticketSystem::modifyProfile(parser::PaperCup *cup) {
        if (!Libro.modify(cup)) cout<<"-1\n";
    }

    void ticketSystem::None(parser::PaperCup *cup) {}
}