//
// Created by Leon on 2021/4/17.
//

#ifndef TICKETSYSTEM_USER_H
#define TICKETSYSTEM_USER_H

#include <cstring>
#include <string>
#include "Algorithm.h"
#include "L_time.h"
#include "unordered_map.hpp"
#include "BPT.hpp"
#include "parser.h"

using std::string;
namespace LaMetropole {

    class user {
    public:
        char username[24];
        char password[32];
        char privilege;
        char name[20];
        char mailAddr[32];
        int orderNum;

        user();

        user(const string &userName, const string &pswd, const string &Name, const string &mail, char privilege);

        user &operator=(const user &other);
    };

#ifdef debugs

    std::ostream &operator<<(std::ostream &out, const user &obj);

#endif

    class userManager {
    private:
        bool has_user;

    public:
        BPT<long long, user, 288, 32> Mathilda;
        unordered_map<long long, user> Leon;

        //pending list of each user
        //userId + Time -> offset
        struct userIdTime {
            long long key;
            int orderNum;

            userIdTime(long long userIdHash = 0, long long orderNum = -1) : key(userIdHash),
                                                                            orderNum(orderNum) {}

            bool operator<(const userIdTime &other) const {
                if (key < other.key) return true;
                if (key > other.key) return false;
                return (orderNum < other.orderNum);
            }

            bool operator<=(const userIdTime &other) const {
                return (key < other.key || key == other.key && orderNum <= other.orderNum);
            }

            bool operator==(const userIdTime &other) const {
                return ((key == other.key && orderNum == other.orderNum));
            }
        };

        BPT<userIdTime, orderRecord, 288, 288> Sabine;

        userManager();

        ~userManager();

        void initialise(bool flag = false);

        void clear();

        bool add_user(parser::PaperCup *cup);

        bool login(parser::PaperCup *cup);

        bool logout(parser::PaperCup *cup);

        bool query_profile(parser::PaperCup *cup);

        bool modify(parser::PaperCup *cup);
    };
}

#endif //TICKETSYSTEM_USER_H
