//
// Created by Leon on 2021/5/1.
//

#ifndef TICKETSYSTEM_L_TIME_H
#define TICKETSYSTEM_L_TIME_H

#include <ostream>

namespace LaMetropole {

    class L_time {
        friend std::ostream &operator<<(std::ostream &out, const L_time &obj);

    private:
        int month, day, hour, minute;
        bool flag;
    public:
        L_time(char mm = 6, char dd = 1, char hour = 0, char minute = 0, bool flag = false);

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
}

#endif //TICKETSYSTEM_L_TIME_H