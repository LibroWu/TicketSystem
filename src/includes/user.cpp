//
// Created by Libro on 2021/4/17.
//

#include "user.h"

namespace LaMetropole {

    user::user(const string &userName, const string &pswd, const string &Name, const string &mail, char privilege)
            : privilege(privilege) {
        strcpy(username, userName.c_str());
        strcpy(password, pswd.c_str());
        strcpy(name, Name.c_str());
        strcpy(mailAddr, mail.c_str());
    }

    user::user() : privilege(-1) {
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

    userManager::userManager() : Libro(selfHash), Nebula("user_crystal.file", "user_index.file") {}

    userManager::~userManager() {}

    void userManager::initialise(bool flag) {
        if (flag) {
            has_user = Nebula.empty();
        } else {
            Nebula.initialise();
            has_user = false;
        }
    }

    void userManager::clear() {
        Nebula.initialise();
        Libro.clear();
        has_user = false;
    }

    bool userManager::add_user(parser::PaperCup *cup) {
        if (!has_user) {
            user tmp(*cup->arg['u' - 'a'], *cup->arg['p' - 'a'], *cup->arg['n' - 'a'], *cup->arg['m' - 'a'],
                     10);
            Nebula.insert(HASH(*cup->arg['u' - 'a']), tmp);
            return (has_user = true);
        } else {
            long long Hc = HASH(*cup->arg['c' - 'a']), Hu = HASH(*cup->arg['u' - 'a']);
            char level = toInt(cup->arg['g' - 'a']);
            if (Libro.count(Hc))
                if (Libro[Hc].privilege > level)
                    if (Nebula.Find(Hu).privilege == -1) {
                        user tmp(*cup->arg['u' - 'a'], *cup->arg['p' - 'a'], *cup->arg['n' - 'a'], *cup->arg['m' - 'a'],
                                 level);
                        Nebula.insert(Hu, tmp);
                        return true;
                    }
            return false;
        }
    }

    bool userManager::logout(parser::PaperCup *cup) {
        long long Hu = HASH(*cup->arg['u' - 'a']);
        return Libro.erase(Hu);
    }

    bool userManager::login(parser::PaperCup *cup) {
        long long Hu = HASH(*cup->arg['u' - 'a']);
        if (Libro.count(Hu) == 0) {
            user tmp = Nebula.Find(Hu);
             tmp.mailAddr[0] = 0, tmp.name[0] = 0, tmp.username[0] = 0;
            if (tmp.privilege != -1) {
                if (strcmp(tmp.password, cup->arg['p' - 'a']->c_str()) == 0) {
                    Libro[Hu] = tmp;
                    return true;
                }
            }
        }
        return false;
    }

    bool userManager::modify(parser::PaperCup *cup) {
        long long Hu = HASH(*cup->arg['u' - 'a']), Hc = HASH(*cup->arg['c' - 'a']);
        if (Libro.count(Hc)) {
            user tmp = Nebula.Find(Hu);
            if (tmp.privilege != -1 && (Hu == Hc || Libro[Hc].privilege > tmp.privilege)) {
                if (cup->arg['p' - 'a'] != nullptr) {
                    strcpy(tmp.password,cup->arg['p' - 'a']->c_str());
                }
                if (cup->arg['n' - 'a'] != nullptr) {
                    strcpy(tmp.name,cup->arg['n' - 'a']->c_str());
                }
                if (cup->arg['m' - 'a'] != nullptr) {
                    strcpy(tmp.mailAddr,cup->arg['m' - 'a']->c_str());
                }
                if (cup->arg['g' - 'a'] != nullptr) {
                    tmp.privilege= toInt(cup->arg['g' - 'a']);
                }
                Nebula.modify(Hu, tmp);
                cout << tmp.username << ' ' << tmp.name << ' ' << tmp.mailAddr << ' ' << int(tmp.privilege) << '\n';
                return true;
            }
        }
        return false;
    }

    bool userManager::query_profile(parser::PaperCup *cup) {
        long long Hu = HASH(*cup->arg['u' - 'a']), Hc = HASH(*cup->arg['c' - 'a']);
        if (Libro.count(Hc)) {
            user tmp = Nebula.Find(Hu);
            if (tmp.privilege != -1 && (Hu == Hc || Libro[Hc].privilege > tmp.privilege)) {
                cout << tmp.username << ' ' << tmp.name << ' ' << tmp.mailAddr << ' ' << int(tmp.privilege) << '\n';
                return true;
            }
        }
        return false;
    }
}