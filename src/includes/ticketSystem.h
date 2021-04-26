//
// Created by Libro on 2021/4/21.
//

#ifndef TICKETSYSTEM_TICKETSYSTEM_H
#define TICKETSYSTEM_TICKETSYSTEM_H

#include "parser.h"
#include "user.h"

namespace LaMetropole {


    class ticketSystem {
    public:
        parser Apollo;
        userManager Libro;

        ticketSystem();

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

    };

}

#endif //TICKETSYSTEM_TICKETSYSTEM_H
