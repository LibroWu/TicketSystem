//
// Created by Leon on 2021/5/5.
//

#include "train.h"

namespace LaMetropole{

    void trainManager::initialise(bool flag) {}

    void trainManager::clear() {}

    trainManager::~trainManager() {}

    trainManager::trainManager() {}

    bool trainManager::addTrain(parser::PaperCup *cup) {}

    bool trainManager::queryOrder(parser::PaperCup *cup) {}

    bool trainManager::queryTicket(parser::PaperCup *cup) {}

    bool trainManager::queryTrain(parser::PaperCup *cup) {}

    bool trainManager::queryTransfer(parser::PaperCup *cup) {}

    bool trainManager::refundTicket(parser::PaperCup *cup) {}

    bool trainManager::releaseTrain(parser::PaperCup *cup) {}

    bool trainManager::deleteTrain(parser::PaperCup *cup) {}

    char trainManager::buyTicket(parser::PaperCup *cup) {}
}