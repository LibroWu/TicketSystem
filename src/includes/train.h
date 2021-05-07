//
// Created by Leon on 2021/5/5.
//

#ifndef TICKETSYSTEM_TRAIN_H
#define TICKETSYSTEM_TRAIN_H

#include <cstring>
#include <string>
#include "L_time.h"
#include "user.h"
#include "Algorithm.h"
#include "unordered_map.hpp"
#include "BPT.hpp"
#include "parser.h"

namespace LaMetropole {
    class train {
    public:
        char ID[22], stationNum, stations[101][35];
        int seatNum[101], prices[101], start_hour, start_minute, maxSeatNum;
        int travelTimes[101], stopoverTimes[101];
        char beginDay, beginMonth, endDay, endMonth, Type;

        train(std::string Id, char station_num, char type);

        train() = default;
    };

    class trainManager {
    private:
        userManager *Libro;

        //station+trainOffset -> offset of the train
        struct stationTrain {
            long long key, trainOffset;

            stationTrain(long long stationHash = 0, long long trainOffset = -1) : key(stationHash),
                                                                                  trainOffset(trainOffset) {}

            bool operator<(const stationTrain &other) const {
                if (key < other.key) return true;
                if (key > other.key) return false;
                return (trainOffset < other.trainOffset);
            }

            bool operator<=(const stationTrain &other) const {
                return (key < other.key || key == other.key && trainOffset <= other.trainOffset);
            }

            bool operator==(const stationTrain &other) const {
                return ((key == other.key && trainOffset == other.trainOffset));
            }
        };

        BPT<stationTrain, int, 288, 288> Nancy;

        //trainID -> offset & released or not of the train
        struct offsetFlag {
            int offset;
            bool flag;

            offsetFlag(int offset = -1, bool flag = false) : offset(-1), flag(flag) {}
        };

        BPT<long long, offsetFlag, 288, 288> Jason;

        //information of each train
        MemoryRiver<train> trainRecorder;

        //pending list of each user
        //userId + Time -> offset
        class userIdTime {

        };

        BPT<userIdTime, int, 288, 288> Sabine;

        //pending list of each train
        //
        class trainIDOrder {

        };

        BPT<userIdTime, int, 288, 288> Arya;
    public:
        trainManager(userManager *libro);

        ~trainManager();

        void initialise(bool flag = false);

        void clear();

        bool addTrain(parser::PaperCup *cup);

        bool refundTicket(parser::PaperCup *cup);

        bool queryTrain(parser::PaperCup *cup);

        bool queryTicket(parser::PaperCup *cup);

        bool releaseTrain(parser::PaperCup *cup);

        char buyTicket(parser::PaperCup *cup);

        bool queryOrder(parser::PaperCup *cup);

        bool deleteTrain(parser::PaperCup *cup);

        bool queryTransfer(parser::PaperCup *cup);
    };
}
#endif //TICKETSYSTEM_TRAIN_H
