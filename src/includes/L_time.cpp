//
// Created by Leon on 2021/5/1.
//

#include "L_time.h"

namespace LaMetropole {

    const int Month[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    L_time::L_time(char mm, char dd, char hour, char minute, bool flag) : month(mm), day(dd), hour(hour),
                                                                          minute(minute), flag(flag) {}

    //the base is 06-01
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
        int mon = 6, day = 1, sum = 0;
        while (mon < month) sum += Month[mon++];
        sum += day;
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
                << obj.hour << ((obj.hour < 10) ? ":0" : ":") << obj.minute;
        return out;
    }
}