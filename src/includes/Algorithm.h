//
// Created by Leon on 2021/4/18.
//

#ifndef TICKETSYSTEM_ALGORITHM_H
#define TICKETSYSTEM_ALGORITHM_H

#include <string>
#include "L_time.h"

namespace LaMetropole {

    template<class T>
    void Swap(T &a, T &b) {
        T c = a;
        a = b, b = c;
    }

    template<class T>
    T min(const T &a, const T &b) {
        return ((a < b) ? a : b);
    }

    template<class T>
    T max(const T &a, const T &b) {
        return ((a > b) ? a : b);
    }

    template<class T>
    T *lower_bound(T *l, T *r, T t) {
        T *mid;
        while (l < r) {
            mid = l + (r - l) / 2;
            if (*mid < t) l = mid + 1;
            else r = mid;
        }
        return l;
    }

    template<class T>
    T *upper_bound(T *l, T *r, T t) {
        T *mid;
        while (l < r) {
            mid = l + (r - l) / 2;
            if (*mid <= t) l = mid + 1;
            else r = mid;
        }
        return l;
    }

    void sort(sortStruct **l, sortStruct **r);

    long long selfHash(const long long &input);

    long long selfHashInt(const int &input);

    long long HASH(const std::string &input);

    char toInt(std::string *s, bool label = false);

    int toLong(std::string *s, bool label = false);

    std::string toString(int x);
}
#endif //TICKETSYSTEM_ALGORITHM_H
