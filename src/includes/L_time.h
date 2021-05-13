//
// Created by Leon on 2021/5/1.
//

#ifndef TICKETSYSTEM_L_TIME_H
#define TICKETSYSTEM_L_TIME_H

#include <ostream>
#include "cstring"

namespace LaMetropole {

    class L_time {
        friend std::ostream &operator<<(std::ostream &out, const L_time &obj);

    public:
        int month, day, hour, minute;
        bool flag;
    public:
        L_time(char mm = 5, char dd = 1, char hour = 0, char minute = 0, bool flag = false);

        L_time(int t);

        L_time operator+(int t);

        L_time operator-(int t);

        int operator-(const L_time &other);

        bool operator<(const L_time &other);

        bool operator>(const L_time &other);

        L_time &operator+=(int t);

        L_time &operator-=(int t);

        explicit operator int() const;
    };

    std::ostream &operator<<(std::ostream &out, const L_time &obj);

    struct orderRecord {
        char status;
        char trainID[22], startStation[35], targetStation[35];
        //price means the single ticket
        int price, n, pendingNum, dayN;
        char st, arv;
        L_time startTime, arrivalTime;

        orderRecord(int price, int n, int trainOrderNum, int dayN, char sStation, char arStation, char *trainId,
                    char *start, char *target, const L_time &st,
                    const L_time &arv);
        void set(int Price, int N, int TrainOrderNum, int DayN, char SStation, char ArStation, char *TrainId,
                 char *Start, char *Target, const L_time &St,
                 const L_time &Arv);
        orderRecord();

        orderRecord(const orderRecord &other);
    };

    struct pendingRecord {
        int price, n, pendingNum, orderNum, dayN;
        char st, arv;
        long long hashUserId;

        pendingRecord(int price, int n, int pendingNum, int dayN, char st, char arv, int orderNum,
                      long long hashUserId);

        pendingRecord() = default;
    };

    struct sortStruct {
        int keyPrice, keyTime, num;
        char trainID[22];
        //if compareFlag then sort by cost
        bool flag;

        sortStruct() = default;

        sortStruct(int keyPrice, int keyTime, int num, bool flag,const char* train):keyPrice(keyPrice),keyTime(keyTime),num(num),flag(flag) {
            strcpy(trainID,train);
        }

        bool operator<(const sortStruct& other) {
            if (flag) {
                if (keyPrice<other.keyPrice) return true;
                if (keyPrice>other.keyPrice) return false;
            } else {
                if (keyTime<other.keyTime) return true;
                if (keyTime>other.keyTime) return false;
            }
            return (strcmp(trainID,other.trainID)<0);
        }
    };
}

#endif //TICKETSYSTEM_L_TIME_H
