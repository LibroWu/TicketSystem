//
// Created by Leon on 2021/5/5.
//

#ifndef TICKETSYSTEM_TRAIN_H
#define TICKETSYSTEM_TRAIN_H

#include <cstring>
#include <string>
#include "user.h"
using std::sort;

namespace LaMetropole {
    class train {
    public:
        char ID[22], stationNum, stations[101][35];

        int seatNum[94][101], pricePrefixSum[101], start_hour, start_minute, maxSeatNum;
        int leavingTime[101], stopoverTimes[101];
        char beginDay, beginMonth, endDay, endMonth, Type;

        train(std::string Id, char station_num, char type);

        train() = default;
    };

    class trainManager {
    private:
        userManager *Libro;

        //station+trainOffset -> offset of the train and the number of the station within the train
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

        struct offsetNum {
            int offset;
            char num;

            offsetNum(int offset = 0, char num = 0) : offset(offset), num(num) {}
        };

        //todo take down the station's number of the train can optimise the program
        BPT<stationTrain, offsetNum, 288, 288> Nancy;

        //trainID -> offset & released or not of the train & pendingNum
        struct offsetFlag {
            int offset, pendingNum;
            bool flag;

            offsetFlag(int offset = -1, bool flag = false, int pendingNum = 0) : offset(offset), flag(flag),
                                                                                 pendingNum(pendingNum) {}
        };

        BPT<long long, offsetFlag, 288, 288> Jason;

        //information of each train
        MemoryRiver<train> trainRecorder;

        //pending list of each train
        struct trainIDOrder {
            //todo trainId + day
            struct IdDay {
                long long Id;
                int day;

                IdDay() = default;

                IdDay(long long Id, int day) : Id(Id), day(day) {}

                bool operator<(const IdDay &other) const {
                    if (Id < other.Id) return true;
                    if (Id > other.Id) return false;
                    return day < other.day;
                }

                bool operator>(const IdDay &other) const {
                    if (Id > other.Id) return true;
                    if (Id < other.Id) return false;
                    return day > other.day;
                }

                bool operator==(const IdDay &other) const {
                    return (Id == other.Id && day == other.day);
                }
            } key;

            int pendingNum;

            trainIDOrder(long long trainIdHash = 0, int dayN = 0, long long orderNum = -1) : key(trainIdHash, dayN),
                                                                                             pendingNum(orderNum) {}

            bool operator<(const trainIDOrder &other) const {
                if (key < other.key) return true;
                if (key > other.key) return false;
                return (pendingNum < other.pendingNum);
            }

            bool operator<=(const trainIDOrder &other) const {
                return (key < other.key || key == other.key && pendingNum <= other.pendingNum);
            }

            bool operator==(const trainIDOrder &other) const {
                return ((key == other.key && pendingNum == other.pendingNum));
            }
        };

        BPT<trainIDOrder, pendingRecord, 288, 288> Arya;
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
