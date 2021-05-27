//
// Created by Leon on 2021/4/18.
//

#include "parser.h"

namespace LaMetropole {
    void parser::PaperCup::push_back(char keyType, string *argument) {
        arg[keyType - 'a'] = argument;
    }

    parser::PaperCup::~PaperCup() {
        for (char i = 0; i < 26; ++i) delete arg[i];
    }

    parser::PaperCup::PaperCup() : arv(0) {
        memset(arg, 0, sizeof(arg));
    }

    string *parser::tokenScanner::nextToken() {
        while (pos < len && isSpace(s->operator[](pos))) ++pos;
        int pre = pos;
        while (pos < len && !isSpace(s->operator[](pos))) ++pos;
        return (new string(*s, pre, pos - pre));
    }

    bool parser::tokenScanner::isSpace(char ch) {
        return (ch == division || ch == '\r' || ch == '\n');
    }

    parser::tokenScanner::tokenScanner(string *s_ptr, char DIVISION) : s(s_ptr), pos(0), len(s_ptr->length()),
                                                                       division(DIVISION) {}

    inline bool parser::tokenScanner::hasNextToken() {
        while (pos < len && isSpace(s->operator[](pos))) ++pos;
        return (pos < len);
    }

    void parser::tokenScanner::set_division(char Division) {
        division=Division;
    }

    void parser::tokenScanner::set_ptr(string *s_ptr) {
        s = s_ptr;
        pos = 0;
        len = s->length();
    }

    parser::PaperCup *parser::listen() {
        string s, *sKey, *sArg;
        PaperCup *tmp = new PaperCup;
        bool flag = false;
        if (getline(std::cin, s)) flag = true;
        if (!flag) {
            tmp->keyType = EXIT;
            return tmp;
        }
#ifdef debugs
        std::cout<<s<<'\n';
        tmp->origin=s;
#endif
        tokenScanner tS(&s);
        sKey = tS.nextToken();
        int i;
        for (i = 0; i < 16; ++i) if (commandSet[i] == *sKey) break;
        tmp->keyType = setToType[i];
        delete sKey;
        while (tS.hasNextToken()) {
            sKey = tS.nextToken();
            if (!tS.hasNextToken()) {
                delete sKey;
                return tmp;
                //todo error
            }
            sArg = tS.nextToken();
            tmp->push_back(sKey->operator[](1), sArg);
            delete sKey;
        }
#ifdef debugs
//        std::cout<<++commandNumber<<std::endl;
//        ++commandNumber;
#endif
        std::cout.flush();
        return tmp;
    }
}