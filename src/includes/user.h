//
// Created by Leon on 2021/4/17.
//

#ifndef TICKETSYSTEM_USER_H
#define TICKETSYSTEM_USER_H

#include <cstring>
#include <string>
#include "Algorithm.h"
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

        user();
        user(const string &userName, const string &pswd, const string &Name, const string &mail, char privilege);
        user& operator=(const user& other);
    };

    class userManager {
    private:
        BPT<long long, user, 288, 32> Mathilda;
        unordered_map<long long, user> Leon;
        bool has_user;
    public:
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
