//
// Created by Libro on 2021/4/17.
//

#include "user.h"

namespace LaMetropole{

    user::user(const string &userName, const string &pswd, const string &Name, const string &mail, char privilege):privilege(privilege) {
        strcpy(username,userName.c_str());
        strcpy(password,pswd.c_str());
        strcpy(name,Name.c_str());
        strcpy(mailAddr,mail.c_str());
    }
}