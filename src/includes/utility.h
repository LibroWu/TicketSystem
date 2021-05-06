//
// Created by Leon on 2021/4/17.
//

#ifndef TICKETSYSTEM_UTILITY_H
#define TICKETSYSTEM_UTILITY_H
namespace LaMetropole{

    template<class T1, class T2>
    class pair {
    public:
        T1 first;
        T2 second;
        constexpr pair() : first(), second() {}
        pair(const T1 &x, const T2 &y) : first(x), second(y) {}
        template<class U1, class U2>
        pair(U1 &&x, U2 &&y) : first(x), second(y) {}
        template<class U1, class U2>
        pair(const pair<U1, U2> &other) : first(other.first), second(other.second) {}
        template<class U1, class U2>
        pair(pair<U1, U2> &&other) : first(other.first), second(other.second) {}
    };
}
#endif //TICKETSYSTEM_UTILITY_H
