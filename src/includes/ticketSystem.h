//
// Created by Libro on 2021/4/21.
//

#ifndef TICKETSYSTEM_TICKETSYSTEM_H
#define TICKETSYSTEM_TICKETSYSTEM_H
#include "parser.h"
namespace LaMetropole {

    namespace LM{
        void addUser(parser::PaperCup *cup);

        void login(parser::PaperCup *cup);

        void logout(parser::PaperCup *cup);

        void queryProfile(parser::PaperCup *cup);

        void modifyProfile(parser::PaperCup *cup);

        void addTrain(parser::PaperCup *cup);

        void releaseTrain(parser::PaperCup *cup);

        void queryTrain(parser::PaperCup *cup);

        void deleteTrain(parser::PaperCup *cup);

        void queryTicket(parser::PaperCup *cup);

        void queryTransfer(parser::PaperCup *cup);

        void buyTicket(parser::PaperCup *cup);

        void queryOrder(parser::PaperCup *cup);

        void refundTicket(parser::PaperCup *cup);

        void Clean(parser::PaperCup *cup);

        void Exit(parser::PaperCup *cup);
        void None(parser::PaperCup *cup);
    }
    void (*commandMap[17])(parser::PaperCup *) ={
            LM::addUser, LM::login, LM::logout, LM::queryProfile, LM::modifyProfile, LM::addTrain,
            LM::releaseTrain,LM::queryTrain, LM::deleteTrain, LM::queryTicket, LM::queryTransfer,
            LM::buyTicket, LM::queryOrder,LM::refundTicket, LM::Clean, LM::Exit, LM::None
    };
    class ticketSystem {
    public:
        parser Apollo;
        ticketSystem();

    };
}

#endif //TICKETSYSTEM_TICKETSYSTEM_H
