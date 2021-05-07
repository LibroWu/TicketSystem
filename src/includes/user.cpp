//
// Created by Leon on 2021/4/17.
//

#include "user.h"

namespace LaMetropole {

    user::user(const string &userName, const string &pswd, const string &Name, const string &mail, char privilege)
            : privilege(privilege), orderNum(0) {
        strcpy(username, userName.c_str());
        strcpy(password, pswd.c_str());
        strcpy(name, Name.c_str());
        strcpy(mailAddr, mail.c_str());
    }

    user::user() : privilege(-1), orderNum(0) {
        username[0] = '\0';
        password[0] = '\0';
        name[0] = '\0';
        mailAddr[0] = '\0';
    }

    user &user::operator=(const user &other) {
        privilege = other.privilege;
        strcpy(username, other.username);
        strcpy(password, other.password);
        strcpy(name, other.name);
        strcpy(mailAddr, other.mailAddr);
        return *this;
    }

    userManager::userManager() : Leon(selfHash), Mathilda("user_crystal.file", "user_index.file"),
                                 Sabine("Sabine_crystal.file", "Sabine_index.file") {}

    userManager::~userManager() {}

    void userManager::initialise(bool flag) {
        if (flag) {
            has_user = Mathilda.empty();
        } else {
            Sabine.initialise();
            Mathilda.initialise();
            has_user = false;
        }
    }

    void userManager::clear() {
        Mathilda.initialise();
        Sabine.initialise();
        Leon.clear();
        has_user = false;
    }

    bool userManager::add_user(parser::PaperCup *cup) {
        if (!has_user) {
            user tmp(*cup->arg['u' - 'a'], *cup->arg['p' - 'a'], *cup->arg['n' - 'a'], *cup->arg['m' - 'a'],
                     10);
            Mathilda.insert(HASH(*cup->arg['u' - 'a']), tmp);
            Sabine.insert(userIdTime(HASH(*cup->arg['u'-'a']),-1),orderRecord());
            return (has_user = true);
        } else {
            long long Hc = HASH(*cup->arg['c' - 'a']), Hu = HASH(*cup->arg['u' - 'a']);
            char level = toInt(cup->arg['g' - 'a']);
            if (Leon.count(Hc))
                if (Leon[Hc].privilege > level)
                    if (!Mathilda.count(Hu)) {
                        user tmp(*cup->arg['u' - 'a'], *cup->arg['p' - 'a'], *cup->arg['n' - 'a'], *cup->arg['m' - 'a'],
                                 level);
                        Mathilda.insert(Hu, tmp);
                        Sabine.insert(userIdTime(Hu,-1),orderRecord());
                        return true;
                    }
            return false;
        }
    }

    bool userManager::logout(parser::PaperCup *cup) {
        long long Hu = HASH(*cup->arg['u' - 'a']);
        return Leon.erase(Hu);
    }

    bool userManager::login(parser::PaperCup *cup) {
        long long Hu = HASH(*cup->arg['u' - 'a']);
        if (Leon.count(Hu) == 0) {
            user tmp = Mathilda.Find(Hu);
            tmp.mailAddr[0] = 0, tmp.name[0] = 0, tmp.username[0] = 0;
            if (tmp.privilege != -1) {
                if (strcmp(tmp.password, cup->arg['p' - 'a']->c_str()) == 0) {
                    Leon[Hu] = tmp;
                    return true;
                }
            }
        }
        return false;
    }

    bool userManager::modify(parser::PaperCup *cup) {
        long long Hu = HASH(*cup->arg['u' - 'a']), Hc = HASH(*cup->arg['c' - 'a']);
        if (Leon.count(Hc)) {
            user tmp = Mathilda.Find(Hu);
            if (tmp.privilege != -1 && (Hu == Hc || Leon[Hc].privilege > tmp.privilege)) {
                if (cup->arg['p' - 'a'] != nullptr) {
                    strcpy(tmp.password, cup->arg['p' - 'a']->c_str());
                }
                if (cup->arg['n' - 'a'] != nullptr) {
                    strcpy(tmp.name, cup->arg['n' - 'a']->c_str());
                }
                if (cup->arg['m' - 'a'] != nullptr) {
                    strcpy(tmp.mailAddr, cup->arg['m' - 'a']->c_str());
                }
                if (cup->arg['g' - 'a'] != nullptr) {
                    tmp.privilege = toInt(cup->arg['g' - 'a']);
                }
                Mathilda.modify(Hu, tmp);
                cout << tmp.username << ' ' << tmp.name << ' ' << tmp.mailAddr << ' ' << int(tmp.privilege) << '\n';
                return true;
            }
        }
        return false;
    }

    bool userManager::query_profile(parser::PaperCup *cup) {
        long long Hu = HASH(*cup->arg['u' - 'a']), Hc = HASH(*cup->arg['c' - 'a']);
        if (Leon.count(Hc)) {
            user tmp = Mathilda.Find(Hu);
            if (tmp.privilege != -1 && (Hu == Hc || Leon[Hc].privilege > tmp.privilege)) {
                cout << tmp.username << ' ' << tmp.name << ' ' << tmp.mailAddr << ' ' << int(tmp.privilege) << '\n';
                return true;
            }
        }
        return false;
    }
}