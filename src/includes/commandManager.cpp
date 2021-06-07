//
// Created by Libro on 2021/6/6.
//

#include "commandManager.h"

namespace LaMetropole {
    commandRecord::commandRecord(char *Cmd) {
        time_t t = time(NULL);
        strcpy(Time, ctime(&t));
        strcpy(command, Cmd);
    }

    commandManager::commandManager() {

    }

    void commandManager::initialise(bool flag) {}

    void commandManager::push(const commandRecord &cR) {}

    void commandManager::ls() {}

    void commandManager::pageUp() {}

    void commandManager::pageDown() {}
}