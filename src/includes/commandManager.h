//
// Created by Libro on 2021/6/6.
//

#ifndef TICKETSYSTEM_COMMANDMANAGER_H
#define TICKETSYSTEM_COMMANDMANAGER_H
#include <ctime>
#include <cstring>
#include <fstream>
namespace LaMetropole {
    struct commandRecord {
        int tag;
        char Time[26];
        char command[300];

        commandRecord(char *Cmd);

        commandRecord() = default;
    };

    class commandManager {
        commandRecord queue[1000];
        int l, r, tagCnt;
        std::string file_name;
        std::fstream file;
    public:
        void initialise(bool flag=false);

        void push(const commandRecord &cR);

        void ls();

        void pageUp();

        void pageDown();

    public:
        commandManager();
    };

}
#endif //TICKETSYSTEM_COMMANDMANAGER_H
