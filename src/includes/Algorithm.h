//
// Created by Leon on 2021/4/18.
//

#ifndef TICKETSYSTEM_ALGORITHM_H
#define TICKETSYSTEM_ALGORITHM_H

#include <string>
namespace LaMetropole{

    template<class T>
    void swap(T& a,T& b);

    template<class T>
    T* lower_bound(T* l,T* r,T t);

    template<class T>
    T* upper_bound(T* l,T* r,T t);

    template<class T>
    void sort(T* l,T* r);

    long long selfHash(const long long & input);

    long long HASH(const std::string& input);

    char toInt(std::string* s);
}
#endif //TICKETSYSTEM_ALGORITHM_H
