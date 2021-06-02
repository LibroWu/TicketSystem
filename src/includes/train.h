//
// Created by Leon on 2021/5/5.
//

#ifndef TICKETSYSTEM_TRAIN_H
#define TICKETSYSTEM_TRAIN_H

#include <cstring>
#include <string>
#include "user.h"

//#define debugs1

using std::sort;

namespace LaMetropole {
    class train {
    public:
        char ID[21], stationNum, stations[101][35];

        int pricePrefixSum[100], start_hour, start_minute, maxSeatNum;
        unsigned short leavingTime[100], stopoverTimes[100];
        char beginDay, beginMonth, endDay, endMonth, Type;

        train(std::string Id, char station_num, char type);

        train() = default;
    };

    class trainManager {
    public:
        userManager *Libro;

        //station+trainOffset -> offset of the train and the number of the station within the train
        struct stationTrain {
            long long key;
            int trainOffset;

            stationTrain(long long stationHash = 0, int trainOffset = -1) : key(stationHash),
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

#ifdef debugs
            friend std::ostream & operator<<(std::ostream& out,const stationTrain& obj) {
                out<<obj.key<<' '<<obj.trainOffset<<' ';
                return out;
            }
#endif
        };

        struct offsetNum {
            int offset;
            char num;

            offsetNum(int offset = 0, char num = 0) : offset(offset), num(num) {}
#ifdef debugs
            friend std::ostream & operator<<(std::ostream& out,const offsetNum& obj) {
                out<<obj.offset<<' '<<int(obj.num)<<' ';
                return out;
            }
#endif

        };

        BPT<stationTrain, offsetNum, 300, 200> Nancy;

        //trainID -> offset & released or not of the train & pendingNum
        struct offsetFlag {
            int offset;
            bool flag;

            offsetFlag(int offset = -1, bool flag = false) : offset(offset), flag(flag){}
#ifdef debugs
            friend std::ostream & operator<<(std::ostream& out,const offsetFlag& obj) {
                out<<obj.offset<<' '<<obj.flag<<' ';
                return out;
            }
#endif
        };

        BPT<long long, offsetFlag, 200, 200> Jason;

        //information of each train
        MemoryRiver<train,3> trainRecorder;

        //pending list of each train
        struct trainIDOrder {
            //todo trainId + day
            struct IdDay {
                long long Id;
                unsigned char day;

                IdDay() = default;

                IdDay(long long Id, unsigned char day) : Id(Id), day(day) {}

                bool operator<(const IdDay &other) const {
                    if (Id < other.Id) return true;
                    if (Id > other.Id) return false;
                    return day < other.day;
                }

                bool operator<=(const IdDay &other) const {
                    if (Id < other.Id) return true;
                    if (Id > other.Id) return false;
                    return day <= other.day;
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

            trainIDOrder(long long trainIdHash = 0, int dayN = 0, int orderNum = -1) : key(trainIdHash, dayN),
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

        struct seatStruct {
            char num;
            L_integer maxSeatNum;
            L_integer seat[100];
        };

        BPT<trainIDOrder::IdDay, int, 200, 400> Yuki;

        MemoryRiver<seatStruct,2,8> Yoshikawa;

        BPT<trainIDOrder, pendingRecord, 200, 200> Arya;
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

    std::ostream &operator<<(std::ostream &out, const trainManager::seatStruct &obj);

    std::ostream &operator<<(std::ostream &out, const trainManager::trainIDOrder::IdDay &obj);
}
#endif //TICKETSYSTEM_TRAIN_H
