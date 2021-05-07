//
// Created by Leon on 2021/5/5.
//

#include "train.h"

namespace LaMetropole {

    train::train(std::string Id, char station_num, char type) : stationNum(station_num), Type(type) {
        strcpy(ID, Id.c_str());
    }

    void trainManager::initialise(bool flag) {
        Jason.initialise();
        Nancy.initialise();
        Sabine.initialise();
        Arya.initialise();
        trainRecorder.initialise();
    }

    void trainManager::clear() {
        Jason.initialise();
        Nancy.initialise();
        Sabine.initialise();
        Arya.initialise();
        trainRecorder.initialise();
    }

    trainManager::~trainManager() {}

    trainManager::trainManager(userManager *libro) : Libro(libro), trainRecorder("train.file"),
                                                     Jason("ID_crystal.file", "ID_index.file"),
                                                     Nancy("stationID_crystal.file", "stationID_index.file"),
                                                     Sabine("Sabine_crystal.file", "Sabine_index.file"),
                                                     Arya("Arya_crystal.file", "Arya_index.file") {}

    bool trainManager::addTrain(parser::PaperCup *cup) {
        long long HashID = HASH(*cup->arg['i' - 'a']);
        if (Jason.count(HashID)) return false;
        //ID stationNum TYPE
        train tmp(*cup->arg['i' - 'a'], toInt(cup->arg['m' - 'a']), cup->arg['y' - 'a']->operator[](0));
        parser::tokenScanner prices(cup->arg['p' - 'a'], '|');
        parser::tokenScanner travelTimes(cup->arg['t' - 'a'], '|');
        parser::tokenScanner stopoverTimes(cup->arg['o' - 'a'], '|');
        parser::tokenScanner stations(cup->arg['s' - 'a'], '|');
        parser::tokenScanner startTime(cup->arg['x' - 'a'], ':');
        //startTime
        tmp.start_hour = toLong(startTime.nextToken(), true);
        tmp.start_minute = toLong(startTime.nextToken(), true);
        //train interval
        startTime.set_ptr(cup->arg['d' - 'a']);
        string *s_tmp = startTime.nextToken();
        parser::tokenScanner date(s_tmp, '-');
        tmp.beginMonth = toInt(date.nextToken(), true);
        tmp.beginDay = toInt(date.nextToken(), true);
        delete s_tmp;
        s_tmp = startTime.nextToken();
        date.set_ptr(s_tmp);
        tmp.endMonth = toInt(date.nextToken(), true);
        tmp.endDay = toInt(date.nextToken(), true);
        delete s_tmp;
        //stations and travelTimes and prices and stopoverTimes
        int seatNum = toLong(cup->arg['n' - 'a']);
        for (char i = 0; i < tmp.stationNum - 2; ++i) {
            tmp.stopoverTimes[i] = toLong(stopoverTimes.nextToken(), true);
            tmp.prices[i] = toLong(prices.nextToken(), true);
            tmp.travelTimes[i] = toLong(travelTimes.nextToken(), true);
            s_tmp = stations.nextToken();
            strcpy(tmp.stations[i], s_tmp->c_str());
            delete s_tmp;
            tmp.seatNum[i] = seatNum;
        }
        for (char i = tmp.stationNum - 2; i < tmp.stationNum; ++i) {
            s_tmp = stations.nextToken();
            strcpy(tmp.stations[i], s_tmp->c_str());
            delete s_tmp;
        }
        tmp.prices[tmp.stationNum - 1] = toLong(prices.nextToken(), true);
        tmp.travelTimes[tmp.stationNum - 1] = toLong(travelTimes.nextToken(), true);
        tmp.seatNum[tmp.stationNum - 1] = seatNum;
        int offset = trainRecorder.write(tmp);
        Jason.insert(HashID, offsetFlag(offset, false));
        //todo
        return true;
    }

    bool trainManager::queryOrder(parser::PaperCup *cup) {}

    bool trainManager::queryTicket(parser::PaperCup *cup) {}

    bool trainManager::queryTrain(parser::PaperCup *cup) {
        long long HashID = HASH(*cup->arg['i' - 'a']);
        parser::tokenScanner tS(cup->arg['d' - 'a'], '-');
        char Month = toInt(tS.nextToken(), true), Day = toInt(tS.nextToken(), true);
        offsetFlag tmp = Jason.Find(HashID);
        if (tmp.offset == -1) return false;
        train trainTmp;
        trainRecorder.read(trainTmp, tmp.offset);
        if (trainTmp.beginMonth < Month || trainTmp.beginMonth == Month && trainTmp.beginDay <= Day)
            if (trainTmp.endMonth > Month || trainTmp.endMonth == Month && trainTmp.endDay >= Day) {
                cout << trainTmp.ID << ' ' << trainTmp.Type << '\n';
                L_time Lt(Month, Day, trainTmp.start_hour, trainTmp.start_minute);
                cout << trainTmp.stations[0] << " xx-xx xx:xx -> " << Lt << " 0 " << trainTmp.seatNum[0] << '\n';
                int i;
                for (i = 1; i < trainTmp.stationNum - 1; ++i) {
                    Lt += trainTmp.travelTimes[i - 1];
                    cout << trainTmp.stations[i] << ' ' << Lt << " -> ";
                    Lt += trainTmp.stopoverTimes[i - 1];
                    cout << Lt << ' ' << trainTmp.prices[i - 1] << ' ' << trainTmp.seatNum[i] << '\n';
                }
                Lt += trainTmp.travelTimes[i - 1];
                cout << trainTmp.stations[i] << ' ' << Lt << " -> xx-xx xx:xx " << trainTmp.prices[i - 1] << " x\n";
                return true;
            }
        return false;
    }

    bool trainManager::queryTransfer(parser::PaperCup *cup) {}

    bool trainManager::refundTicket(parser::PaperCup *cup) {}

    bool trainManager::releaseTrain(parser::PaperCup *cup) {
        long long HashID = HASH(*cup->arg['i' - 'a']);
        offsetFlag tmp = Jason.Find(HashID);
        if (tmp.offset == -1 || tmp.flag) return false;
        train trainTmp;
        trainRecorder.read(trainTmp, tmp.offset);
        Jason.modify(HashID, offsetFlag(tmp.offset, true));
        long long HashTrainId = HASH(trainTmp.ID), HashStation;
        for (char i = 0; i < trainTmp.stationNum; ++i) {
            Nancy.insert(stationTrain(HASH(trainTmp.stations[i]), HashTrainId), tmp.offset);
        }
    }

    bool trainManager::deleteTrain(parser::PaperCup *cup) {
        long long HashID = HASH(*cup->arg['i' - 'a']);
        offsetFlag tmp = Jason.Find(HashID);
        if (tmp.offset != -1 && !tmp.flag) {
            trainRecorder.Delete(tmp.offset);
            Jason.Delete(HashID);
            return true;
        }
        return false;
    }

    char trainManager::buyTicket(parser::PaperCup *cup) {}
}