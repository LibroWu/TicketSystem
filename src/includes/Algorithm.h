//
// Created by Leon on 2021/4/18.
//

#ifndef TICKETSYSTEM_ALGORITHM_H
#define TICKETSYSTEM_ALGORITHM_H

#include <string>

namespace LaMetropole {

    template<class T>
    void swap(T &a, T &b) {
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

    namespace forSort {
        int getLog2(int x);

        template<class T, class Compare=std::less<T>>
        void heapSort(T *l, T *r) {

        }

        template<class T, class Compare=std::less<T>>
        void insertSort(T *l, T *r) {

        }

        template<class T, class Compare=std::less<T>>
        void quicksort(T *l, T *r, int t) {
            T* mid;

            if (t == 0) forSort::heapSort(l,r);
            else {
                quicksort(l,mid,t-1);
                quicksort(mid,r,t-1);
            }
        }


    }

    template<class T, class Compare=std::less<T>>
    void sort(T *l, T *r) {
        if (l >= r) return;
        if (r - l <= 10) forSort::insertSort(l, r);
        else forSort::quicksort(l, r, forSort::getLog2(r - l));
    }

    long long selfHash(const long long &input);

    long long selfHashInt(const int &input);

    long long HASH(const std::string &input);

    char toInt(std::string *s, bool label = false);

    int toLong(std::string *s, bool label = false);
}
#endif //TICKETSYSTEM_ALGORITHM_H
