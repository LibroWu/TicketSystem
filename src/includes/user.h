//
// Created by Libro on 2021/4/17.
//

#ifndef TICKETSYSTEM_USER_H
#define TICKETSYSTEM_USER_H

#include <cstring>
#include <string>

using std::string;
namespace LaMetropole {

    class user {
        char username[24];
        char password[32];
        char privilege;
        char name[20];
        char mailAddr[32];
        user()=default;
        user(const string& userName,const string& pswd,const string& Name,const string& mail,char privilege);
    };

    class userManager{

    };
}

#endif //TICKETSYSTEM_USER_H
