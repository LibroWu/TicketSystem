//
// Created by Leon on 2021/4/18.
//

#include "Algorithm.h"

namespace LaMetropole {
    template<class T>
    void swap(T &a, T &b) {
        T c = a;
        a = b, b = c;
    }

    namespace ForSort {

    }

    template<class T>
    void sort(T *l, T *r) {

    }

    long long selfHashInt(const int &input) { return input; }

    long long selfHash(const long long &input) { return input; }

    long long HASH(const std::string &input) {
        unsigned int xor_num[4] = {1073758344, 268960770, 16779332, 268468481};
        int len = input.length();
        long long tmp = 0;
        //sub_hash1 197
        for (int i = 0; i < len; ++i) {
            tmp = (tmp << 7) + (tmp << 6) + (tmp << 2) + tmp + input[i];
            tmp ^= xor_num[input[i] % 4];
            if (input[i] % 7 == 0) tmp <<= 1;
            if (input[i] % 13 == 0) tmp >>= 1;
        }
        tmp <<= 32;
        long long tmp2 = 0;
        //sub_hash2 157
        for (int i = 0; i < len; ++i) {
            tmp2 = (tmp2 << 7) + (tmp2 << 4) + (tmp2 << 3) + (tmp2 << 2) + tmp2 + input[i];
            tmp2 ^= xor_num[3 - input[i] % 4];
            if (input[i] % 2 == 0) tmp2 <<= 1;
            if (input[i] % 5 == 0) tmp2 >>= 1;
        }
        tmp += tmp2;
        return tmp;
    }

    char toInt(std::string *s, bool label) {
        char tmp = 0, len = s->length();
        for (char i = 0; i < len; ++i)
            tmp = tmp * 10 + s->operator[](i) - '0';
        if (label) delete s;
        return tmp;
    }

    int toLong(std::string *s, bool label) {
        int tmp = 0, len = s->length();
        for (int i = 0; i < len; ++i)
            tmp = tmp * 10 + s->operator[](i) - '0';
        if (label) delete s;
        return tmp;
    }
}