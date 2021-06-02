//
// Created by Leon on 2021/4/18.
//

#include "Algorithm.h"
#include <iostream>

namespace LaMetropole {


    int forSort::getLog2(int x) {
        int tmp = -1;
        while (x) {
            ++tmp;
            x >>= 1;
        }
        return tmp;
    }

    long long selfHashInt(const int &input) { return input; }

    long long selfHash(const long long &input) { return input; }

    long long HASH(const std::string &input) {
        unsigned int xor_num[4] = {1073758344, 268960770, 16779332, 268468481};
        int len = input.length();
        long long tmp = 0;
        //sub_hash1 197
        for (int i = 0; i < len; ++i) {
            tmp = (tmp << 7) + (tmp << 6) + (tmp << 2) + tmp + (input[i] << 2) + input[i] * input[i];
            tmp ^= xor_num[abs(input[i]) & 1];
            tmp ^= tmp>>16;
        }
        tmp <<= 32;
        long long tmp2 = 0;
        //sub_hash2 157
        for (int i = 0; i < len; ++i) {
            tmp2 = (tmp2 << 7) + (tmp2 << 4) + (tmp2 << 3) + (tmp2 << 2) + tmp2 + input[i];
            tmp2 ^= xor_num[3 - (abs(input[i]) & 1)];
            tmp2 ^= tmp2 << 16;
        }
        tmp ^= tmp2;
        return tmp;
    }

    char toInt(std::string *s, bool label) {
        char tmp = 0, len = s->length();
        for (char i = 0; i < len; ++i)
            if (s->operator[](i) <= '9' && s->operator[](i) >= '0')
                tmp = tmp * 10 + s->operator[](i) - '0';
        if (label) delete s;
        return tmp;
    }

    int toLong(std::string *s, bool label) {
        int tmp = 0, len = s->length();
        for (int i = 0; i < len; ++i)
            if (s->operator[](i) <= '9' && s->operator[](i) >= '0')
                tmp = tmp * 10 + s->operator[](i) - '0';
        if (label) delete s;
        return tmp;
    }

    std::string toString(int x){
        std::string res;
        if (x==0) return "0";
        while (x) {
            res+=char(x%10)+'0';
            x/=10;
        }
        for (int i = 0; i < res.length()/2; ++i) {
            Swap(res[i],res[res.length()-1-i]);
        }
        return res;
    }
}