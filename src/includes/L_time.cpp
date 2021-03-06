//
// Created by Leon on 2021/5/1.
//

#include "L_time.h"

namespace LaMetropole {

    const int Month[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    L_time::L_time(char mm, char dd, int hour, int minute, bool flag) : month(mm), day(dd), hour(hour),
                                                                        minute(minute), flag(flag) {}

    //the base is 01-01
    L_time::L_time(int t) {
        *this = (L_time() + t);
    }

    L_time L_time::operator+(int t) {
        L_time tmp(*this);
        tmp.minute += t;
        tmp.hour += tmp.minute / 60;
        tmp.minute %= 60;
        tmp.day += tmp.hour / 24;
        tmp.hour %= 24;
        while (tmp.day > Month[tmp.month])
            tmp.day -= Month[tmp.month++];
        return tmp;
    }

    L_time L_time::operator-(int t) {
        return L_time((this->operator int()) - t);
    }

    int L_time::operator-(const L_time &other) {
        return (int(*this) - int(other));
    }

    L_time::operator int() const {
        int mm = 5, sum = 0;
        while (mm < month) sum += Month[mm++];
        sum += day - 1;
        return (sum * 1440 + hour * 60 + minute);
    }

    L_time &L_time::operator+=(int t) {
        *this = *this + t;
        return *this;
    }

    L_time &L_time::operator-=(int t) {
        *this = *this - t;
        return *this;
    }

    bool L_time::less(const L_time &other) {
        if (month == other.month) return day < other.day;
        return month < other.month;
    }

    bool L_time::lessEqual(const L_time &other) {
        if (month == other.month) return day <= other.day;
        return month < other.month;
    }

    bool L_time::operator<(const L_time &other) {
        if (month < other.month) return true;
        else if (month > other.month) return false;
        if (day < other.day) return true;
        else if (day > other.day) return false;
        if (hour < other.hour) return true;
        else if (hour > other.hour) return false;
        if (minute < other.minute) return true;
        else if (minute > other.minute) return false;
        return false;
    }

    bool L_time::operator>(const L_time &other) {
        if (month < other.month) return false;
        else if (month > other.month) return true;
        if (day < other.day) return false;
        else if (day > other.day) return true;
        if (hour < other.hour) return false;
        else if (hour > other.hour) return true;
        if (minute < other.minute) return false;
        else if (minute > other.minute) return true;
        return false;
    }

    std::ostream &operator<<(std::ostream &out, const L_time &obj) {
        if (obj.flag) out << "xx-xx xx:xx";
        else
            out << '0' << obj.month << ((obj.day < 10) ? "-0" : "-") << obj.day << ((obj.hour < 10) ? " 0" : " ")
                << obj.hour << ((obj.minute < 10) ? ":0" : ":") << obj.minute;
        return out;
    }

    orderRecord::orderRecord(int price, int n, int trainOrderNum, int dayN, char sStation, char arStation,
                             char *trainId,
                             char *start, char *target,
                             const L_time &st,
                             const L_time &arv) : price(price), n(n),
                                                  startTime(st),
                                                  arrivalTime(arv), pendingNum(trainOrderNum), st(sStation),
                                                  arv(arStation), dayN(dayN) {
        status = 'a';
        strcpy(trainID, trainId);
        strcpy(startStation, start);
        strcpy(targetStation, target);
    }

    void orderRecord::set(int Price, int N, int TrainOrderNum, int DayN, char SStation, char ArStation, char *TrainId,
                          char *Start, char *Target, const L_time &St, const L_time &Arv) {
        price = Price, n = N,
        startTime = St,
        arrivalTime = Arv, pendingNum = TrainOrderNum, st = SStation,
        arv = ArStation, dayN = DayN;
        status = 'a';
        strcpy(trainID, TrainId);
        strcpy(startStation, Start);
        strcpy(targetStation, Target);
    }

    orderRecord::orderRecord() {
        //e for empty
        status = 'e';
        trainID[0] = 0;
        startStation[0] = 0;
        targetStation[0] = 0;
    }

    orderRecord::orderRecord(const orderRecord &other) : price(other.price), n(other.n),
                                                         startTime(other.startTime),
                                                         arrivalTime(other.arrivalTime), status(other.status),
                                                         st(other.st), arv(other.arv), pendingNum(other.pendingNum),
                                                         dayN(other.dayN) {
        strcpy(trainID, other.trainID);
        strcpy(startStation, other.startStation);
        strcpy(targetStation, other.targetStation);
    }

    pendingRecord::pendingRecord(int price, int n, int pendingNum, int dayN, char st, char arv, int orderNum,
                                 long long hashUserId)
            : price(price), n(n),
              pendingNum(pendingNum), st(st),
              arv(arv), orderNum(orderNum), hashUserId(hashUserId), dayN(dayN) {}

    L_integer::operator int() const {
        int res=a;
        res*=10000;
        res+=b;
        return res;
    }

    L_integer::L_integer(int other) {
        a=other/10000;
        b=other%10000;
    }

    L_integer::L_integer(const L_integer &other):a(other.a),b(other.b) {}

    L_integer & L_integer::operator-=(const L_integer &other) {
        if (this!=&other) {
            int res = int(*this) - int(other);
            *this=L_integer(res);
        }
        return *this;
    }

    L_integer & L_integer::operator+=(const L_integer &other) {
        if (this!=&other) {
            int res = int(*this) + int(other);
            *this=L_integer(res);
        }
        return *this;
    }
}