//
// Created by Leon on 2021/5/5.
//

#include "train.h"

namespace LaMetropole {

    train::train(std::string Id, char station_num, char type) : stationNum(station_num), Type(type) {
        strcpy(ID, Id.c_str());
    }

    void trainManager::initialise(bool flag) {
        if (!flag) {
            Jason.initialise();
            Nancy.initialise();
            Arya.initialise();
            Yuki.initialise();
            trainRecorder.initialise();
        } else {
#ifdef cache
            Jason.initialize();
            Nancy.initialize();
            Arya.initialize();
            Yuki.initialize();
            trainRecorder.initialize();
#endif
        }
    }

    void trainManager::clear() {
        Jason.initialise();
        Nancy.initialise();
        Arya.initialise();
        Yuki.initialise();
        trainRecorder.initialise();
    }

    trainManager::~trainManager() {}

    trainManager::trainManager(userManager *libro) : Libro(libro), trainRecorder("train.file"),
                                                     Jason("ID_crystal.file", "ID_index.file"),
                                                     Nancy("stationID_crystal.file", "stationID_index.file"),
                                                     Arya("Arya_crystal.file", "Arya_index.file"),
                                                     Yuki("Yuki_crystal.file", "Yuki_index.file") {}

    bool trainManager::addTrain(parser::PaperCup *cup) {
        long long HashID = HASH(*cup->arg['i' - 'a']);
        if (Jason.count(HashID)) return false;
        //ID stationNum TYPE
        train trainTmp(*cup->arg['i' - 'a'], toInt(cup->arg['n' - 'a']), cup->arg['y' - 'a']->operator[](0));
        parser::tokenScanner prices(cup->arg['p' - 'a'], '|');
        parser::tokenScanner travelTimes(cup->arg['t' - 'a'], '|');
        parser::tokenScanner stopoverTimes(cup->arg['o' - 'a'], '|');
        parser::tokenScanner stations(cup->arg['s' - 'a'], '|');
        parser::tokenScanner startTime(cup->arg['x' - 'a'], ':');
        //startTime
        trainTmp.start_hour = toLong(startTime.nextToken(), true);
        trainTmp.start_minute = toLong(startTime.nextToken(), true);
        //train interval
        startTime.set_ptr(cup->arg['d' - 'a']);
        startTime.set_division('|');
        string *s_tmp = startTime.nextToken();
        parser::tokenScanner date(s_tmp, '-');
        trainTmp.beginMonth = toInt(date.nextToken(), true);
        trainTmp.beginDay = toInt(date.nextToken(), true);
        delete s_tmp;
        s_tmp = startTime.nextToken();
        date.set_ptr(s_tmp);
        trainTmp.endMonth = toInt(date.nextToken(), true);
        trainTmp.endDay = toInt(date.nextToken(), true);
        delete s_tmp;
        //stations and leavingTime and pricePrefixSum and stopoverTimes
        int seatNum = toLong(cup->arg['m' - 'a']);
        if (seatNum == 0) return false;
        trainTmp.maxSeatNum = seatNum;
        trainTmp.pricePrefixSum[0] = 0;
        trainTmp.leavingTime[0] = 0;
        for (char i = 0; i < trainTmp.stationNum - 2; ++i) {
            trainTmp.stopoverTimes[i] = toLong(stopoverTimes.nextToken(), true);
            trainTmp.pricePrefixSum[i + 1] = trainTmp.pricePrefixSum[i] + toLong(prices.nextToken(), true);
            trainTmp.leavingTime[i + 1] =
                    trainTmp.leavingTime[i] + trainTmp.stopoverTimes[i] + toLong(travelTimes.nextToken(), true);
            s_tmp = stations.nextToken();
            strcpy(trainTmp.stations[i], s_tmp->c_str());
            delete s_tmp;
        }
        for (char i = trainTmp.stationNum - 2; i < trainTmp.stationNum; ++i) {
            s_tmp = stations.nextToken();
            strcpy(trainTmp.stations[i], s_tmp->c_str());
            delete s_tmp;
        }
        trainTmp.stopoverTimes[trainTmp.stationNum - 2] = 0;
        trainTmp.pricePrefixSum[trainTmp.stationNum - 1] =
                trainTmp.pricePrefixSum[trainTmp.stationNum - 2] + toLong(prices.nextToken(), true);
        trainTmp.leavingTime[trainTmp.stationNum - 1] =
                trainTmp.leavingTime[trainTmp.stationNum - 2] + toLong(travelTimes.nextToken(), true);
        int offset = trainRecorder.write(trainTmp);
        Jason.insert(HashID, offsetFlag(offset, false));
        return true;
    }

    bool trainManager::queryOrder(parser::PaperCup *cup) {
        long long Hu = HASH(*cup->arg['u' - 'a']);
        if (!Libro->Leon.count(Hu)) return false;
        vector<orderRecord> *tmp = Libro->Sabine.multipleFind(userManager::userIdTime(Hu, 0));
        if (!tmp) {
            cout << 0 << '\n';
            return true;
        }
        cout << tmp->size() << '\n';
        for (int i = tmp->size() - 1; i >= 0; --i) {
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
        if (Month < 6) return false;
        vector<offsetNum> *start_vec = Nancy.multipleFind(stationTrain(HashStart));
        if (!start_vec) return false;
        vector<offsetNum> *end_vec = Nancy.multipleFind(stationTrain(HashEnd));
        if (!end_vec) {
            delete start_vec;
            return false;
        }
        vector<pair<int, int>> same_vec;
        vector<orderRecord> result;
        vector<sortStruct> resultSort;
        unordered_map<int, int> mapTable(selfHashInt);
        int len = start_vec->size();
        for (int i = 1; i < len; ++i) mapTable[start_vec->operator[](i).offset] = i;
        len = end_vec->size();
        for (int i = 1; i < len; ++i) {
            if (mapTable.count(end_vec->operator[](i).offset))
                same_vec.push_back(pair<int, int>(mapTable[end_vec->operator[](i).offset], i));
        }
        len = same_vec.size();
        train trainTmp;
        int sumPrice, seatNum, dayN;
        //if compareFlag then sort by cost
        bool compareFlag = false;
        if (cup->arg['p' - 'a'] && cup->arg['p' - 'a']->operator[](0) == 'c') compareFlag = true;
        for (int i = 0; i < len; ++i) {
            char startC = start_vec->operator[](same_vec[i].first).num;
            char arvC = end_vec->operator[](same_vec[i].second).num;
            if (arvC <= startC) continue;
            trainRecorder.read(trainTmp, start_vec->operator[](same_vec[i].first).offset);
            int keyTime = trainTmp.leavingTime[arvC] - trainTmp.stopoverTimes[arvC - 1] -
                          trainTmp.leavingTime[startC];
            sumPrice = trainTmp.pricePrefixSum[arvC] - trainTmp.pricePrefixSum[startC], seatNum = trainTmp.maxSeatNum;
            L_time stTime(trainTmp.beginMonth, trainTmp.beginDay, trainTmp.start_hour, trainTmp.start_minute),
                    endTime(trainTmp.endMonth, trainTmp.endDay, trainTmp.start_hour, trainTmp.start_minute),
                    checkTime(Month, Day, trainTmp.start_hour, trainTmp.start_minute);
            stTime += trainTmp.leavingTime[startC];
            endTime += trainTmp.leavingTime[startC];
            if (stTime.lessEqual(checkTime) && checkTime.lessEqual(endTime)) {
                checkTime.hour=stTime.hour,checkTime.minute=stTime.minute;
                checkTime -= trainTmp.leavingTime[startC];
                dayN = (checkTime.month - 6) * 31 + checkTime.day;
                checkTime += trainTmp.leavingTime[startC];
                seatStruct seatArray(Yuki.Find(trainIDOrder::IdDay(HASH(trainTmp.ID), dayN)));
                for (char j = startC; j < arvC; ++j) seatNum = min(seatNum, int(seatArray.seat[j]));
                result.push_back(orderRecord(sumPrice, seatNum, -1, dayN, startC, arvC, trainTmp.ID,
                                             trainTmp.stations[startC], trainTmp.stations[arvC],
                                             checkTime, checkTime + keyTime));
                resultSort.push_back(
                        sortStruct(sumPrice, keyTime, result.size() - 1, compareFlag,
                                   trainTmp.ID));
            }
        }
        sort(resultSort.begin(), resultSort.end());
        int Len = resultSort.size();
        cout << Len << '\n';
        for (int i = 0; i < Len; ++i) {
            orderRecord &R = result[resultSort[i].num];
            cout << R.trainID << ' ' << R.startStation << ' ' << R.startTime << " -> " << R.targetStation << ' '
                 << R.arrivalTime << ' ' << R.price << ' ' << R.n << '\n';
        }
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
        seatStruct seatArray;
        if (tmp.flag) seatArray = Yuki.Find(trainIDOrder::IdDay(HashID, dayN));
        if (trainTmp.beginMonth < Month || trainTmp.beginMonth == Month && trainTmp.beginDay <= Day)
            if (trainTmp.endMonth > Month || trainTmp.endMonth == Month && trainTmp.endDay >= Day) {
                cout << trainTmp.ID << ' ' << trainTmp.Type << '\n';
                L_time Lt(Month, Day, trainTmp.start_hour, trainTmp.start_minute);
                cout << trainTmp.stations[0] << " xx-xx xx:xx -> " << Lt << " 0 "
                     << ((tmp.flag) ? int(seatArray.seat[0]) : trainTmp.maxSeatNum) << '\n';
                int i;
                for (i = 1; i < trainTmp.stationNum - 1; ++i) {
                    cout << trainTmp.stations[i] << ' '
                         << Lt + (trainTmp.leavingTime[i] - trainTmp.stopoverTimes[i - 1]) << " -> ";
                    cout << Lt + trainTmp.leavingTime[i] << ' '
                         << trainTmp.pricePrefixSum[i] << ' '
                         << ((tmp.flag) ? int(seatArray.seat[i]) : trainTmp.maxSeatNum) << '\n';
                }
                cout << trainTmp.stations[i] << ' ' << Lt + trainTmp.leavingTime[i] << " -> xx-xx xx:xx "
                     << trainTmp.pricePrefixSum[i]
                     << " x\n";
                return true;
            }
        return false;
    }

    bool trainManager::queryTransfer(parser::PaperCup *cup) {
        //todo target: O(N^3)
        //use unorderedMap to find the cross Node of two train
        //if compareFlag then sort by cost
        bool compareFlag = false;
        if (cup->arg['p' - 'a'] && cup->arg['p' - 'a']->operator[](0) == 'c') compareFlag = true;
        long long HashStart = HASH(*cup->arg['s' - 'a']), HashEnd = HASH(*cup->arg['t' - 'a']);
        parser::tokenScanner tS(cup->arg['d' - 'a'], '-');
        char Month = toInt(tS.nextToken(), true), Day = toInt(tS.nextToken(), true);
        if (Month < 6) return false;
        vector<offsetNum> *start_vec = Nancy.multipleFind(stationTrain(HashStart));
        if (!start_vec || start_vec->size() == 1) {
            delete start_vec;
            return false;
        }
        vector<offsetNum> *end_vec = Nancy.multipleFind(stationTrain(HashEnd));
        if (!end_vec || end_vec->size() == 1) {
            delete start_vec;
            delete end_vec;
            return false;
        }
        vector<pair<int, int>> same_vec(0);
        unordered_map<long long, char> mapTableOfStation(selfHash);
        int l1 = start_vec->size(), l2 = end_vec->size();
        train train_st, train_arv;
        orderRecord startResult, arvResult;
        sortStruct forCMP;
        for (int i = 1; i < l1; ++i) {
            trainRecorder.read(train_st, start_vec->operator[](i).offset);
            char firstStartStation = start_vec->operator[](i).num;
            for (int j = 1; j < l2; ++j) {
                int preFirstTimeConsume;
                if (start_vec->operator[](i).offset == end_vec->operator[](j).offset) continue;
                //todo cache this
                trainRecorder.read(train_arv, end_vec->operator[](j).offset);
                char secondArvStation = end_vec->operator[](j).num;
                mapTableOfStation.clear();
                for (char k = firstStartStation + 1; k < train_st.stationNum; ++k)
                    mapTableOfStation[HASH(train_st.stations[k])] = k;
                for (char k = secondArvStation - 1; k > -1; --k)
                    //find the cross node of the two routine
                    if (mapTableOfStation.count(HASH(train_arv.stations[k]))) {
                        int firstDayN, secondDayN;
                        char firstArvStation = mapTableOfStation[HASH(train_arv.stations[k])];
                        L_time firstStTime(train_st.beginMonth, train_st.beginDay, train_st.start_hour,
                                           train_st.start_minute),
                                firstCheckTime(Month, Day, train_st.start_hour, train_st.start_minute),
                                firstEndTime(train_st.endMonth, train_st.endDay, train_st.start_hour,
                                             train_st.start_minute),
                                firstArvTime;
                        firstStTime += train_st.leavingTime[firstStartStation];
                        firstEndTime += train_st.leavingTime[firstStartStation];
                        if (firstCheckTime.less(firstStTime) || firstEndTime.less(firstCheckTime)) continue;
                        firstStTime.month = Month, firstStTime.day = Day;
                        firstCheckTime = firstStTime;
                        firstArvTime = firstCheckTime + (train_st.leavingTime[firstArvStation] -
                                                      train_st.leavingTime[firstStartStation] -
                                                      train_st.stopoverTimes[firstArvStation - 1]);
                        firstCheckTime -= train_st.leavingTime[firstStartStation];
                        firstDayN = (firstCheckTime.month - 6) * 31 + firstCheckTime.day;
                        L_time secondStTime(train_arv.beginMonth, train_arv.beginDay, train_arv.start_hour,train_arv.start_minute),
                        secondArvTime, secondEndTime(train_arv.endMonth,train_arv.endDay,train_arv.start_hour,train_arv.start_minute),
                        secondCheckTime;
                        secondStTime += train_arv.leavingTime[k];
                        secondEndTime += train_arv.leavingTime[k];
                        if (firstArvTime > secondEndTime) continue;
                        if (!(firstArvTime < secondStTime)) {
                            secondStTime.month = firstArvTime.month;
                            secondStTime.day = firstArvTime.day;
                            //wait whole day
                            if ( secondStTime < firstArvTime )
                                secondStTime += 1440;
                        }
                        secondCheckTime = secondStTime - train_arv.leavingTime[k];
                        secondArvTime = secondStTime + (train_arv.leavingTime[secondArvStation] -
                                                        train_arv.leavingTime[k] -train_arv.stopoverTimes[secondArvStation - 1]);
                        secondDayN = (secondCheckTime.month - 6) * 31 + secondCheckTime.day;
                        int timeConsume = secondArvTime - firstStTime, firstTimeConsume = firstArvTime - firstStTime;
                        int firstPrice =
                                train_st.pricePrefixSum[firstArvStation] - train_st.pricePrefixSum[firstStartStation];
                        int secondPrice = train_arv.pricePrefixSum[secondArvStation] - train_arv.pricePrefixSum[k];
                        int firstSeatNum = train_st.maxSeatNum, secondSeatNum = train_arv.maxSeatNum;
                        seatStruct stArray(Yuki.Find(trainIDOrder::IdDay(HASH(train_st.ID), firstDayN))),
                                arvArray(Yuki.Find(trainIDOrder::IdDay(HASH(train_arv.ID), secondDayN)));
                        for (char l = firstStartStation; l < firstArvStation; ++l)
                            firstSeatNum = min(firstSeatNum, int(stArray.seat[l]));
                        for (char l = k; l < secondArvStation; ++l)
                            secondSeatNum = min(secondSeatNum, int(arvArray.seat[l]));
                        if (startResult.status == 'e') {
                            startResult.status = 'a';
                            startResult.set(firstPrice, firstSeatNum, 0, firstDayN, firstStartStation, firstArvStation,
                                            train_st.ID, train_st.stations[firstStartStation],
                                            train_st.stations[firstArvStation], firstStTime, firstArvTime);
                            arvResult.set(secondPrice, secondSeatNum, 0, secondDayN, k, secondArvStation, train_arv.ID,
                                          train_arv.stations[k], train_arv.stations[secondArvStation], secondStTime,
                                          secondArvTime);
                            forCMP.keyTime = timeConsume, forCMP.keyPrice = firstPrice + secondPrice;
                            preFirstTimeConsume = firstTimeConsume;
                        } else {
                            if (compareFlag && ((firstPrice + secondPrice < forCMP.keyPrice) ||
                                                (firstPrice + secondPrice == forCMP.keyPrice &&
                                                 firstTimeConsume < preFirstTimeConsume))) {
                                startResult.set(firstPrice, firstSeatNum, 0, firstDayN, firstStartStation,
                                                firstArvStation, train_st.ID, train_st.stations[firstStartStation],
                                                train_st.stations[firstArvStation], firstStTime, firstArvTime);
                                arvResult.set(secondPrice, secondSeatNum, 0, secondDayN, k, secondArvStation,
                                              train_arv.ID, train_arv.stations[k], train_arv.stations[secondArvStation],
                                              secondStTime, secondArvTime);
                                forCMP.keyTime = timeConsume, forCMP.keyPrice = firstPrice + secondPrice;
                                preFirstTimeConsume = firstTimeConsume;
                            }
                            if (!compareFlag && ((timeConsume < forCMP.keyTime) ||
                                                 (timeConsume == forCMP.keyTime &&
                                                  firstTimeConsume < preFirstTimeConsume))) {
                                startResult.set(firstPrice, firstSeatNum, 0, firstDayN, firstStartStation,
                                                firstArvStation, train_st.ID, train_st.stations[firstStartStation],
                                                train_st.stations[firstArvStation], firstStTime, firstArvTime);
                                arvResult.set(secondPrice, secondSeatNum, 0, secondDayN, k, secondArvStation,
                                              train_arv.ID, train_arv.stations[k], train_arv.stations[secondArvStation],
                                              secondStTime, secondArvTime);
                                forCMP.keyTime = timeConsume, forCMP.keyPrice = firstPrice + secondPrice;
                                preFirstTimeConsume = firstTimeConsume;
                            }
                        }
                    }
            }
        }
        if (startResult.status == 'e') cout << "0\n";
        else {
            cout << startResult.trainID << ' ' << startResult.startStation << ' ' << startResult.startTime << " -> "
                 << startResult.targetStation << ' '
                 << startResult.arrivalTime << ' ' << startResult.price << ' ' << startResult.n << '\n';
            cout << arvResult.trainID << ' ' << arvResult.startStation << ' ' << arvResult.startTime << " -> "
                 << arvResult.targetStation << ' '
                 << arvResult.arrivalTime << ' ' << arvResult.price << ' ' << arvResult.n << '\n';
        }
        delete start_vec;
        delete end_vec;
        return true;
    }

    bool trainManager::refundTicket(parser::PaperCup *cup) {
        long long Hu = HASH(*cup->arg['u' - 'a']);
        if (!Libro->Leon.count(Hu)) return false;
        int n = 1;
        if (cup->arg['n' - 'a']) n = toLong(cup->arg['n' - 'a']);
        user userTmp = Libro->Mathilda.Find(Hu);
        if (n > userTmp.orderNum) return false;
        userManager::userIdTime refundKey(Hu, userTmp.orderNum - n);
        orderRecord orderTmp = Libro->Sabine.Find(refundKey), orderGet;
        if (orderTmp.status == 'r') return false;
        if (orderTmp.status == 'p') {
            orderTmp.status = 'r';
            Libro->Sabine.modify(refundKey, orderTmp);
            Arya.Delete(trainIDOrder(HASH(orderTmp.trainID), orderTmp.dayN, orderTmp.pendingNum));
        }
        if (orderTmp.status == 's') {
            orderTmp.status = 'r';
            long long hashTrainID = HASH(orderTmp.trainID);
            Libro->Sabine.modify(refundKey, orderTmp);
            seatStruct seatArray(Yuki.Find(trainIDOrder::IdDay(hashTrainID, orderTmp.dayN)));
            for (char i = orderTmp.st; i < orderTmp.arv; ++i)
                seatArray.seat[i] += orderTmp.n;
            vector<pendingRecord> *vec_ptr = Arya.multipleFind(trainIDOrder(hashTrainID, orderTmp.dayN));
            if (vec_ptr) {
                for (int i = 1; i < vec_ptr->size(); ++i) {
                    pendingRecord &pr = vec_ptr->operator[](i);
                    int seatNum = int(seatArray.maxSeatNum);
                    if (pr.arv <= orderTmp.st || pr.st >= orderTmp.arv) continue;
                    for (char j = pr.st; j < pr.arv; ++j) seatNum = min(seatNum, int(seatArray.seat[j]));
                    if (seatNum >= pr.n) {
                        Arya.Delete(trainIDOrder(hashTrainID, pr.dayN, pr.pendingNum));
                        for (char j = pr.st; j < pr.arv; ++j) seatArray.seat[j] -= pr.n;
                        //todo optimise
                        orderGet = Libro->Sabine.Find(userManager::userIdTime(pr.hashUserId, pr.orderNum));
                        orderGet.status = 's';
                        Libro->Sabine.modify(userManager::userIdTime(pr.hashUserId, pr.orderNum), orderGet);
                    }
                }
                delete vec_ptr;
            }
            Yuki.modify(trainIDOrder::IdDay(hashTrainID, orderTmp.dayN), seatArray);
        }
        return true;
    }

    bool trainManager::releaseTrain(parser::PaperCup *cup) {
        long long HashID = HASH(*cup->arg['i' - 'a']);
        offsetFlag tmp = Jason.Find(HashID);
        if (tmp.offset == -1 || tmp.flag) return false;
        train trainTmp;
        trainRecorder.read(trainTmp, tmp.offset);
        tmp.flag = true;
        Jason.modify(HashID, tmp);
        for (char i = 0; i < trainTmp.stationNum; ++i) {
            Nancy.insert(stationTrain(HASH(trainTmp.stations[i])), -1);
            Nancy.insert(stationTrain(HASH(trainTmp.stations[i]), tmp.offset), offsetNum(tmp.offset, i));
        }
        int beginN = (trainTmp.beginMonth - 6) * 31 + trainTmp.beginDay,
                endN = (trainTmp.endMonth - 6) * 31 + trainTmp.endDay;
        seatStruct seatArray;
        seatArray.num = trainTmp.stationNum;
        for (char i = 0; i < seatArray.num - 1; ++i) seatArray.seat[i] = trainTmp.maxSeatNum;
        seatArray.maxSeatNum = trainTmp.maxSeatNum;
        for (int i = beginN; i <= endN; ++i) {
            Yuki.insert(trainIDOrder::IdDay(HashID, i), seatArray);
            Arya.insert(trainIDOrder(HashID, i), pendingRecord());
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
        //not find the train or not release
        if (tmp.offset == -1 || !tmp.flag) return 'f';
        parser::tokenScanner tS(cup->arg['d' - 'a'], '-');
        char Month = toInt(tS.nextToken(), true), Day = toInt(tS.nextToken(), true);
        train trainTmp;
        trainRecorder.read(trainTmp, tmp.offset);
        //beyond the train's capacity
        int Need = toLong(cup->arg['n' - 'a']);
        if (Need > trainTmp.maxSeatNum || Need <= 0) return 'f';
        int dayN;
        user userTmp = Libro->Mathilda.Find(Hu);
        char st = -1;
        int seatNum = trainTmp.maxSeatNum,pendingNum;
        trainRecorder.get_info(pendingNum, 3);
        L_time timeTmp(Month, Day, trainTmp.start_hour, trainTmp.start_minute), st_Time;
        for (char i = 0; i < trainTmp.stationNum; ++i) {
            if (st == -1 && strcmp(cup->arg['f' - 'a']->c_str(), trainTmp.stations[i]) == 0) st = i;
            else if (st != -1 && strcmp(cup->arg['t' - 'a']->c_str(), trainTmp.stations[i]) == 0) {
                timeTmp += trainTmp.leavingTime[st], timeTmp.day = Day, timeTmp.month = Month;
                st_Time = timeTmp - trainTmp.leavingTime[st];
                //not in the date interval
                if ((trainTmp.beginMonth > st_Time.month ||
                     trainTmp.beginMonth == st_Time.month && trainTmp.beginDay > st_Time.day) ||
                    (trainTmp.endMonth < st_Time.month ||
                     trainTmp.endMonth == st_Time.month && trainTmp.endDay < st_Time.day))
                    return 'f';
                dayN = (st_Time.month - 6) * 31 + st_Time.day;
                seatStruct seatArray(Yuki.Find(trainIDOrder::IdDay(hashTrainId, dayN)));
                for (char j = st; j < i; ++j) seatNum = min(seatNum, int(seatArray.seat[j]));
                if (seatNum < Need)
                    if (!cup->arg['q' - 'a'] || cup->arg['q' - 'a']->operator[](0) == 'f') return 'f';
                orderRecord orderTmp(trainTmp.pricePrefixSum[i] - trainTmp.pricePrefixSum[st], Need, pendingNum,
                                     dayN, st, i, trainTmp.ID, trainTmp.stations[st],
                                     trainTmp.stations[i], timeTmp,
                                     timeTmp + (trainTmp.leavingTime[i] - trainTmp.stopoverTimes[i - 1] -
                                                trainTmp.leavingTime[st]));
                if (seatNum < Need) {
                    orderTmp.status = 'p';
                    Libro->Sabine.insert(userManager::userIdTime(Hu, userTmp.orderNum), orderTmp);
                    Arya.insert(trainIDOrder(hashTrainId, dayN, pendingNum),
                                pendingRecord(trainTmp.pricePrefixSum[i] - trainTmp.pricePrefixSum[st], Need,
                                              pendingNum, dayN, st, i, userTmp.orderNum, Hu));
                    ++userTmp.orderNum;
                    Libro->Mathilda.modify(Hu, userTmp);
                    ++pendingNum;
                    trainRecorder.write_info(pendingNum,3);
                    return 'p';
                }
                orderTmp.status = 's';
                Libro->Sabine.insert(userManager::userIdTime(Hu, userTmp.orderNum), orderTmp);
                for (char j = st; j < i; ++j) seatArray.seat[j] -= Need;
                Yuki.modify(trainIDOrder::IdDay(hashTrainId, dayN), seatArray);
                ++userTmp.orderNum;
                Libro->Mathilda.modify(Hu, userTmp);
                cout << (long long) (orderTmp.price) * orderTmp.n << '\n';
                return 's';
            }
        }
        return 'f';
    }

#ifdef debugs

    std::ostream &operator<<(std::ostream &out, const trainManager::seatStruct &obj) {
        out << int(obj.num) << '\n';
        for (char i = 0; i < obj.num; ++i) {
            cout << int(obj.seat[i]);
        }
        return out;
    }

    std::ostream &operator<<(std::ostream &out, const trainManager::trainIDOrder::IdDay &obj) {
        out << obj.Id << ' ' << obj.day << '\n';
        return out;
    }

#endif
}
