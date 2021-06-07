//
// Created by Leon on 2021/4/18.
//

#ifndef TICKETSYSTEM_PARSER_H
#define TICKETSYSTEM_PARSER_H

#include <string>
#include <cstring>
#include <iostream>

//#define debugs3

namespace LaMetropole {
    using std::string;
    using std::getline;

    class parser {
    public:
        enum commandType {
            ADD_USER, LOGIN, LOGOUT, QUERY_PROFILE, MODIFY_PROFILE, ADD_TRAIN,
            RELEASE_TRAIN, QUERY_TRAIN, DELETE_TRAIN, QUERY_TICKET, QUERY_TRANSFER,
            BUY_TICKET, QUERY_ORDER, REFUND_TICKET, CLEAN, EXIT, NONE,ROLLBACK,BACKUP,REBUILD,COMMAND_LIST,STATUS
        };

        const string commandSet[22] = {"query_profile", "query_ticket", "buy_ticket", "login", "logout",
                                     "modify_profile", "query_order", "add_user", "add_train", "release_train",
                                     "query_train", "delete_train", "query_transfer", "refund_ticket", "exit", "clean",
                                     "N","roll_back","backup","rebuild","command_list","status"
        };
        const commandType setToType[22] = {QUERY_PROFILE, QUERY_TICKET, BUY_TICKET, LOGIN, LOGOUT,
                                       MODIFY_PROFILE, QUERY_ORDER, ADD_USER, ADD_TRAIN, RELEASE_TRAIN,
                                       QUERY_TRAIN, DELETE_TRAIN, QUERY_TRANSFER, REFUND_TICKET, EXIT, CLEAN, NONE,
                                       ROLLBACK,BACKUP,REBUILD,COMMAND_LIST,STATUS
        };

        int commandNumber;

        //simple tokenScanner with none robust
        class tokenScanner {
        public:
            string *s;
            int pos, len;
            char division;

            tokenScanner(string *s_ptr,char DIVISION=' ');

            void set_ptr(string *s_ptr);

            void set_division(char Division);

            inline bool isSpace(char ch);

            string *nextToken();

            inline bool hasNextToken();
        };

        class PaperCup {
        public:
            commandType keyType;
            string *arg[26];
            std::string origin;
            char key[26];
            char arv;

            PaperCup();

            ~PaperCup();

            void push_back(char keyType, string *argument);

        };

        PaperCup *listen();
    };
}
#endif //TICKETSYSTEM_PARSER_H
