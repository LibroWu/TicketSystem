//
// Created by Libro on 2021/4/18.
//

#ifndef TICKETSYSTEM_ALGORITHM_H
#define TICKETSYSTEM_ALGORITHM_H
namespace LaMetropole{
    template<class T>
    T* lower_bound(T* st,T* ed,T);

    template<class T>
    T* upper_bound(T* st,T* ed,T);

    template<class T>
    void sort(T* st,T* ed);

    void Hash();
}
#endif //TICKETSYSTEM_ALGORITHM_H
