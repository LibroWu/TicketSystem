//
// Created by Leon on 2021/5/5.
//

#ifndef TICKETSYSTEM_TRAIN_H
#define TICKETSYSTEM_TRAIN_H

#include <cstring>
#include <string>
#include "Algorithm.h"
#include "unordered_map.hpp"
#include "BPT.hpp"
#include "parser.h"

namespace LaMetropole {
    class train {
    public:
        char ID[22], stationNum, stations[101][35];
        int seatNum[101], prices[101], start_hour, start_minute;
        int travelTimes[101], stopoverTimes[101];
        char beginDay, beginMonth, endDay, endMonth, type;

        train(std::string ID, char stationNum, char type);

        train() = default;
    };

    class trainManager {
    private:
        //station+trainID -> offset of the train
        class stationTrain{

        };
        BPT<stationTrain,int,288,288> Nancy;

        //trainID -> offset of the train
        BPT<long long, int , 288, 288> Jason;

        MemoryRiver<train> trainRecorder;

        //for pending
        //userId + Time -> offset
        class userIdTime{

        };
        BPT<userIdTime,int ,288,288> Sabine;

        //unrolled linked list table for each train's pending list
        //maybe use bpt
        class trainIDOrder{

        };
        BPT<userIdTime,int ,288,288> Arya;
    public:
        trainManager();

        ~trainManager();

        void initialise(bool flag = false);

        void clear();

        bool addTrain(parser::PaperCup* cup);

        bool refundTicket(parser::PaperCup* cup);

        bool queryTrain(parser::PaperCup* cup);

        bool queryTicket(parser::PaperCup* cup);

        bool releaseTrain(parser::PaperCup* cup);

        char buyTicket(parser::PaperCup* cup);

        bool queryOrder(parser::PaperCup* cup);

        bool deleteTrain(parser::PaperCup* cup);

        bool queryTransfer(parser::PaperCup* cup);
    };
}
#endif //TICKETSYSTEM_TRAIN_H
