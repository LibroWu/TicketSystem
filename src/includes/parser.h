//
// Created by Libro on 2021/4/18.
//

#ifndef TICKETSYSTEM_PARSER_H
#define TICKETSYSTEM_PARSER_H

#include <string>
#include <iostream>
namespace LaMetropole {
    using std::string;
    using std::getline;

    class parser {
    public:
        enum commandType {
            ADD_USER, LOGIN, LOGOUT, QUERY_PROFILE, MODIFY_PROFILE, ADD_TRAIN,
            RELEASE_TRAIN, QUERY_TRAIN, DELETE_TRAIN, QUERY_TICKET, QUERY_TRANSFER,
            BUY_TICKET, QUERY_ORDER, REFUND_TICKET, CLEAN, EXIT, NONE
        };

        const string commandSet[17] = {"query_profile", "query_ticket", "buy_ticket", "login", "logout",
                                     "modify_profile", "query_order", "add_user", "add_train", "release_train",
                                     "query_train", "Delete_train", "query_transfer", "refund_ticket", "exit", "clean",
                                     "N"
        };
        const commandType setToType[17] = {QUERY_PROFILE, QUERY_TICKET, BUY_TICKET, LOGIN, LOGOUT,
                                       MODIFY_PROFILE, QUERY_ORDER, ADD_USER, ADD_TRAIN, RELEASE_TRAIN,
                                       QUERY_TRAIN, DELETE_TRAIN, QUERY_TRANSFER, REFUND_TICKET, EXIT, CLEAN, NONE
        };

        //simple tokenScanner with none robust
        class tokenScanner {
        public:
            string *s;
            int pos, len;
            char division;

            tokenScanner(string *s_ptr,char DIVISION=' ');

            void set_ptr(string *s_ptr);

            inline bool isSpace(char ch);

            string *nextToken();

            inline bool hasNextToken();
        };

        class PaperCup {
        public:
            commandType keyType;
            string *arg[26];
            char key[26];
            char arv;

            PaperCup();

            ~PaperCup();

            void push_back(char keyType, const string *argument);

        };

        PaperCup *listen();
    };
}
#endif //TICKETSYSTEM_PARSER_H
