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
        Arya.initialise();
        trainRecorder.initialise();
    }

    void trainManager::clear() {
        Jason.initialise();
        Nancy.initialise();
        Arya.initialise();
        trainRecorder.initialise();
    }

    trainManager::~trainManager() {}

    trainManager::trainManager(userManager *libro) : Libro(libro), trainRecorder("train.file"),
                                                     Jason("ID_crystal.file", "ID_index.file"),
                                                     Nancy("stationID_crystal.file", "stationID_index.file"),
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
        int beginN = (tmp.beginMonth - 6) * 31 + tmp.beginDay, endN = (tmp.endMonth - 6) * 31 + tmp.endDay;
        //stations and travelTimes and prices and stopoverTimes
        int seatNum = toLong(cup->arg['n' - 'a']);
        for (char i = 0; i < tmp.stationNum - 2; ++i) {
            tmp.stopoverTimes[i] = toLong(stopoverTimes.nextToken(), true);
            tmp.prices[i] = toLong(prices.nextToken(), true);
            tmp.travelTimes[i] = toLong(travelTimes.nextToken(), true);
            s_tmp = stations.nextToken();
            strcpy(tmp.stations[i], s_tmp->c_str());
            delete s_tmp;
            for (int j = beginN; j <= endN; ++j)tmp.seatNum[j][i] = seatNum;
        }
        for (char i = tmp.stationNum - 2; i < tmp.stationNum; ++i) {
            s_tmp = stations.nextToken();
            strcpy(tmp.stations[i], s_tmp->c_str());
            delete s_tmp;
        }
        tmp.prices[tmp.stationNum - 1] = toLong(prices.nextToken(), true);
        tmp.travelTimes[tmp.stationNum - 1] = toLong(travelTimes.nextToken(), true);
        for (int j = beginN; j <= endN; ++j) tmp.seatNum[j][tmp.stationNum - 1] = seatNum;
        int offset = trainRecorder.write(tmp);
        Jason.insert(HashID, offsetFlag(offset, false));
        Arya.insert(trainIDOrder(HashID), pendingRecord());
        return true;
    }

    bool trainManager::queryOrder(parser::PaperCup *cup) {
        long long Hu = HASH(*cup->arg['u' - 'a']);
        if (!Libro->Leon.count(Hu)) return false;
        vector<orderRecord> *tmp = Libro->Sabine.multipleFind(userManager::userIdTime(Hu));
        cout << tmp->size() - 1 << '\n';
        for (int i = tmp->size() - 1; i > 0; ++i) {
            orderRecord &t = tmp->operator[](i);
            switch (t.status) {
                case 's':
                    cout << "[success] ";
                    break;
                case 'p':
                    cout << "[pending] ";
                    break;
                case 'r':
                    cout << "[refunded] ";
            }
            cout << t.trainID << ' ' << t.startStation << ' ' << t.startTime << " -> " << t.targetStation << ' '
                 << t.arrivalTime << ' ' << t.price << ' ' << t.n << '\n';
        }
        delete tmp;
        return true;
    }

    bool trainManager::queryTicket(parser::PaperCup *cup) {
        long long HashStart = HASH(*cup->arg['s' - 'a']), HashEnd = HASH(*cup->arg['t' - 'a']);
        parser::tokenScanner tS(cup->arg['d' - 'a'], '-');
        char Month = toInt(tS.nextToken(), true), Day = toInt(tS.nextToken(), true);
        vector<int> *start_vec = Nancy.multipleFind(stationTrain(HashStart));
        vector<int> *end_vec = Nancy.multipleFind(stationTrain(HashEnd));
        vector<int> same_vec(0);
        unordered_map<int, bool> mapTable(selfHashInt);
        int len = start_vec->size();
        for (int i = 0; i < len; ++i) {
            mapTable[start_vec->operator[](i)] = true;
        }
        len = end_vec->size();
        for (int i = 0; i < len; ++i) {
            if (mapTable.count(end_vec->operator[](i)))
                same_vec.push_back(end_vec->operator[](i));
        }
        if (same_vec.empty()) {
            delete start_vec;
            delete end_vec;
            return false;
        }
        len = same_vec.size();
        train trainTmp;
        int sumPrice, seatNum, dayN = (Month - 6) * 31 + Day;
        bool flag;
        for (int i = 0; i < len; ++i) {
            trainRecorder.read(trainTmp, same_vec[i]);
            if (trainTmp.beginMonth < Month || trainTmp.beginMonth == Month && trainTmp.beginDay <= Day)
                if (trainTmp.endMonth > Month || trainTmp.endMonth == Month && trainTmp.endDay >= Day) {
                    sumPrice = 0, seatNum == trainTmp.maxSeatNum, flag = false;
                    for (char j = 0; j < trainTmp.stationNum; ++j) {
                        if (strcmp(trainTmp.stations[j], cup->arg['s' - 'a']->c_str()) == 0) flag = true;
                        if (strcmp(trainTmp.stations[j], cup->arg['t' - 'a']->c_str()) == 0) {
                            if (!flag) break;
                            //todo:add record
                        }
                        if (flag) {
                            sumPrice += trainTmp.prices[j];
                            seatNum = min(seatNum, trainTmp.seatNum[dayN][j]);
                        }
                    }
                }
        }
        //todo delete!
        //todo empty return false
        //todo sort and print
        delete start_vec;
        delete end_vec;
        return true;
    }

    bool trainManager::queryTrain(parser::PaperCup *cup) {
        long long HashID = HASH(*cup->arg['i' - 'a']);
        parser::tokenScanner tS(cup->arg['d' - 'a'], '-');
        char Month = toInt(tS.nextToken(), true), Day = toInt(tS.nextToken(), true);
        offsetFlag tmp = Jason.Find(HashID);
        if (tmp.offset == -1) return false;
        train trainTmp;
        trainRecorder.read(trainTmp, tmp.offset);
        int dayN = (Month - 6) * 31 + Day;
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
                    cout << Lt << ' ' << trainTmp.prices[i - 1] << ' ' << trainTmp.seatNum[dayN][i] << '\n';
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
        tmp.flag = true;
        Jason.modify(HashID, tmp);
        long long HashTrainId = HASH(trainTmp.ID), HashStation;
        for (char i = 0; i < trainTmp.stationNum; ++i) {
            Nancy.insert(stationTrain(HASH(trainTmp.stations[i])), -1);
            Nancy.insert(stationTrain(HASH(trainTmp.stations[i]), tmp.offset), tmp.offset);
        }
        return true;
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

    char trainManager::buyTicket(parser::PaperCup *cup) {
        long long Hu = HASH(*cup->arg['u' - 'a']);
        //user does not login
        if (!Libro->Leon.count(Hu)) return 'f';
        long long hashTrainId = HASH(*cup->arg['i' - 'a']);
        offsetFlag tmp = Jason.Find(hashTrainId);
        //not find the train
        if (tmp.offset == -1) return 'f';
        parser::tokenScanner tS(cup->arg['d' - 'a'], '-');
        char Month = toInt(tS.nextToken(), true), Day = toInt(tS.nextToken(), true);
        train trainTmp;
        trainRecorder.read(trainTmp, tmp.offset);
        //not in the date interval
        if ((trainTmp.beginMonth > Month || trainTmp.beginMonth == Month && trainTmp.beginDay > Day) ||
            (trainTmp.endMonth < Month || trainTmp.endMonth == Month && trainTmp.endDay < Day))
            return 'f';
        int dayN = (Month - 6) * 31 + Day;
        user userTmp = Libro->Mathilda.Find(Hu);
        int st = -1;
        int seatNum = trainTmp.maxSeatNum, sumPrice = 0;
        L_time timeTmp(Month, Day, trainTmp.start_hour, trainTmp.start_minute), stTime;
        for (char i = 0; i < trainTmp.stationNum; ++i) {
            if (strcmp(cup->arg['t' - 'a']->c_str(), trainTmp.stations[i]) == 0) {
                if (st == -1) return 'f';
                int Need = toLong(cup->arg['n' - 'a']);
                if (seatNum > Need)
                    if (cup->arv == 6 || cup->arg['q' - 'a']->operator[](0) == 'f') return 'f';
                orderRecord orderTmp(sumPrice, Need, tmp.pendingNum, trainTmp.ID, trainTmp.stations[st],
                                     trainTmp.stations[i], stTime,
                                     timeTmp);
                if (seatNum > Need) {
                    orderTmp.status = 'p';
                    Libro->Sabine.insert(userManager::userIdTime(Hu, userTmp.orderNum), orderTmp);
                    ++userTmp.orderNum;
                    Libro->Mathilda.modify(Hu, userTmp);
                    Arya.insert(trainIDOrder(hashTrainId, tmp.pendingNum),
                                pendingRecord(sumPrice, seatNum, tmp.pendingNum, st, i));
                    ++tmp.pendingNum;
                    Jason.modify(hashTrainId, tmp);
                    return 'p';
                }
                orderTmp.status = 's';
                Libro->Sabine.insert(userManager::userIdTime(Hu, userTmp.orderNum), orderTmp);
                ++userTmp.orderNum;
                Libro->Mathilda.modify(Hu, userTmp);
                cout << orderTmp.price * orderTmp.n << '\n';
                return 's';
            }
            if (i > 0) timeTmp += trainTmp.stopoverTimes[i - 1];
            if (strcmp(cup->arg['f' - 'a']->c_str(), trainTmp.stations[i]) == 0) {
                st = i;
                stTime = timeTmp;
            }
            stTime += trainTmp.travelTimes[i];
            if (st != -1) {
                sumPrice += trainTmp.prices[i];
                seatNum = min(seatNum, trainTmp.seatNum[dayN][i]);
            }
        }
        return 'f';
    }
}