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
#ifdef debugs
        cout << "command add train:\n";
        cout << cup->origin << '\n';
        cout << "Hash Of train ID:" << HashID << '\n';
#endif
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
        int beginN = (trainTmp.beginMonth - 6) * 31 + trainTmp.beginDay, endN =
                (trainTmp.endMonth - 6) * 31 + trainTmp.endDay;
        //stations and leavingTime and pricePrefixSum and stopoverTimes
        int seatNum = toLong(cup->arg['m' - 'a']);
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
            for (int j = beginN; j <= endN; ++j)trainTmp.seatNum[j][i] = seatNum;
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
        for (int j = beginN; j <= endN; ++j) trainTmp.seatNum[j][trainTmp.stationNum - 2] = seatNum;
        int offset = trainRecorder.write(trainTmp);
        Jason.insert(HashID, offsetFlag(offset, false));
        return true;
    }

    bool trainManager::queryOrder(parser::PaperCup *cup) {
        long long Hu = HASH(*cup->arg['u' - 'a']);
#ifdef debugs
        cout << "command  query order:\n";
        cout << cup->origin << '\n';
        cout << "Hash Of user ID:" << Hu << '\n';
#endif
        if (!Libro->Leon.count(Hu)) return false;
        vector<orderRecord> *tmp = Libro->Sabine.multipleFind(userManager::userIdTime(Hu));
        cout << tmp->size() - 1 << '\n';
        for (int i = tmp->size() - 1; i > 0; --i) {
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
        if (Month<6 || Month>8) return false;
        vector<offsetNum> *start_vec = Nancy.multipleFind(stationTrain(HashStart));
#ifdef debugs
        cout << "command  query ticket:\n";
        cout << cup->origin << '\n';
        cout << "Hash Of start station:" << HashStart << '\n';
        cout << "Hash of arrival station:" << HashEnd << '\n';
#endif
        if (!start_vec) return false;
        vector<offsetNum> *end_vec = Nancy.multipleFind(stationTrain(HashEnd));
        if (!end_vec) return false;
        vector<pair<int, int>> same_vec(0);
        vector<orderRecord> result(0);
        vector<sortStruct> resultSort(0);
        unordered_map<int, int> mapTable(selfHashInt);
        int len = start_vec->size();
        for (int i = 1; i < len; ++i) {
            mapTable[start_vec->operator[](i).offset] = i;
        }
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
        if (cup->arv == 4 && cup->arg['p' - 'a']->operator[](0) == 'c') compareFlag = true;
        for (int i = 0; i < len; ++i) {
            char startC = start_vec->operator[](same_vec[i].first).num;
            char arvC = end_vec->operator[](same_vec[i].second).num;
            if (arvC <= startC) continue;
            trainRecorder.read(trainTmp, start_vec->operator[](same_vec[i].first).offset);
            int keyTime = trainTmp.leavingTime[arvC] - trainTmp.stopoverTimes[arvC - 1] -
                          trainTmp.leavingTime[startC];
            sumPrice = trainTmp.pricePrefixSum[arvC] - trainTmp.pricePrefixSum[startC], seatNum = trainTmp.maxSeatNum;
            L_time timeTmp(Month, Day, trainTmp.start_hour, trainTmp.start_minute), st_time;
            timeTmp += trainTmp.leavingTime[startC];
            timeTmp.month = Month, timeTmp.day = Day;
            st_time = timeTmp - trainTmp.leavingTime[startC];
            if (trainTmp.beginMonth < st_time.month ||
                trainTmp.beginMonth == st_time.month && trainTmp.beginDay <= st_time.day)
                if (trainTmp.endMonth > st_time.month ||
                    trainTmp.endMonth == st_time.month && trainTmp.endDay >= st_time.day) {
                    dayN = (st_time.month - 6) * 31 + st_time.day;
                    for (char j = startC; j < arvC; ++j) seatNum = min(seatNum, trainTmp.seatNum[dayN][j]);
                    result.push_back(orderRecord(sumPrice, seatNum, -1, dayN, startC, arvC, trainTmp.ID,
                                                 trainTmp.stations[startC], trainTmp.stations[arvC],
                                                 timeTmp, timeTmp + keyTime));
                    resultSort.push_back(
                            sortStruct(sumPrice, keyTime, result.size() - 1, compareFlag,
                                       trainTmp.ID));
                }
        }
        if (result.empty()) cout << "0\n";
        else {
            sort(resultSort.begin(), resultSort.end());
            int Len = resultSort.size();
            cout << Len << '\n';
            for (int i = 0; i < Len; ++i) {
                orderRecord &R = result[resultSort[i].num];
                cout << R.trainID << ' ' << R.startStation << ' ' << R.startTime << " -> " << R.targetStation << ' '
                     << R.arrivalTime << ' ' << R.price << ' ' << R.n << '\n';
            }
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
#ifdef debugs
        cout << "command  query train:\n";
        cout << cup->origin << '\n';
        cout << "Hash Of train ID:" << HashID << '\n';
#endif
        if (tmp.offset == -1) return false;
        train trainTmp;
        trainRecorder.read(trainTmp, tmp.offset);
        int dayN = (Month - 6) * 31 + Day;
        if (trainTmp.beginMonth < Month || trainTmp.beginMonth == Month && trainTmp.beginDay <= Day)
            if (trainTmp.endMonth > Month || trainTmp.endMonth == Month && trainTmp.endDay >= Day) {
                cout << trainTmp.ID << ' ' << trainTmp.Type << '\n';
                L_time Lt(Month, Day, trainTmp.start_hour, trainTmp.start_minute);
                cout << trainTmp.stations[0] << " xx-xx xx:xx -> " << Lt << " 0 " << trainTmp.seatNum[dayN][0] << '\n';
                int i;
                for (i = 1; i < trainTmp.stationNum - 1; ++i) {
                    cout << trainTmp.stations[i] << ' '
                         << Lt + (trainTmp.leavingTime[i] - trainTmp.stopoverTimes[i - 1]) << " -> ";
                    cout << Lt + trainTmp.leavingTime[i] << ' '
                         << trainTmp.pricePrefixSum[i] << ' '
                         << trainTmp.seatNum[dayN][i] << '\n';
                }
                cout << trainTmp.stations[i] << ' ' << Lt + trainTmp.leavingTime[i] << " -> xx-xx xx:xx "
                     << trainTmp.pricePrefixSum[i]
                     << " x\n";
                return true;
            }
        return false;
    }

    bool trainManager::queryTransfer(parser::PaperCup *cup) {
#ifdef debugs
        cout << "*** 1\n";
        cout.flush();
#endif
        //todo target: O(N^3)
        //use unorderedMap to find the cross Node of two train
        //if compareFlag then sort by cost
        bool compareFlag = false;
        if (cup->arv == 4 && cup->arg['p' - 'a']->operator[](0) == 't') compareFlag = true;
        long long HashStart = HASH(*cup->arg['s' - 'a']), HashEnd = HASH(*cup->arg['t' - 'a']);
        parser::tokenScanner tS(cup->arg['d' - 'a'], '-');
        char Month = toInt(tS.nextToken(), true), Day = toInt(tS.nextToken(), true);
        if (Month<6 || Month>8) return false;
#ifdef debugs
        cout << "command  query transfer:\n";
        cout << cup->origin << '\n';
        cout << "Hash Of start station:" << HashStart << '\n';
        cout << "Hash of arrival station:" << HashEnd << '\n';
#endif
        //todo may have bugs here
        vector<offsetNum> *start_vec = Nancy.multipleFind(stationTrain(HashStart));
        if (!start_vec || start_vec->size() == 1) return false;
        vector<offsetNum> *end_vec = Nancy.multipleFind(stationTrain(HashEnd));
        if (!end_vec || end_vec->size() == 1) return false;
        vector<pair<int, int>> same_vec(0);
        unordered_map<long long, char> mapTableOfStation(selfHash);
        int l1 = start_vec->size(), l2 = end_vec->size();
        train train_st, train_arv;
        orderRecord startResult, arvResult;
        sortStruct forCMP;
#ifdef debug_transfer
        cout << "#debug_transfer"<<start_vec->size()<<' '<<end_vec->size()<<'\n';
        cout.flush();
#endif
        for (int i = 1; i < l1; ++i) {
            trainRecorder.read(train_st, start_vec->operator[](i).offset);
            char firstStartStation = start_vec->operator[](i).num;
#ifdef debug_transfer
            cout << "#debug_transfer i:" << i << "\n";
            cout << train_st.ID << '\n';
            for (char w = 0; w < train_st.stationNum; ++w) {
                cout << train_st.stations[w] << ' ';
            }
            cout << '\n';
            cout.flush();
#endif
            for (int j = 1; j < l2; ++j) {
                if (start_vec->operator[](i).offset==end_vec->operator[](j).offset) continue;
                //todo cache this
                trainRecorder.read(train_arv, end_vec->operator[](j).offset);
#ifdef debug_transfer
                cout << "#debug_transfer j:" << j << "\n";
                cout << train_arv.ID << '\n';
                for (char w = 0; w < train_arv.stationNum; ++w) {
                    cout << train_arv.stations[w] << ' ';
                }
                cout << '\n';
                cout<<int(train_arv.beginMonth)<<' '<<int(train_arv.beginDay)<<' '<<int(train_arv.endMonth)<<' '<<int(train_arv.endDay)<<'\n';
                cout.flush();
#endif
                char secondArvStation = end_vec->operator[](j).num;
#ifdef debug_transfer
                cout << "#debug_transfer @4\n";
                cout.flush();
#endif
                mapTableOfStation.clear();
#ifdef debug_transfer
                cout << "#debug_transfer @3\n";
                cout<<int(firstStartStation)<<' '<<int(train_st.stationNum)<<'\n';
                cout.flush();
#endif
                for (char k = firstStartStation + 1; k < train_st.stationNum; ++k) {
                    mapTableOfStation[HASH(train_st.stations[k])] = k;
                }

#ifdef debug_transfer
                cout << "#debug_transfer @1\n";
                cout.flush();
#endif
                for (char k = secondArvStation - 1; k > -1; --k)
                    //find the cross node of the two routine
                    if (mapTableOfStation.count(HASH(train_arv.stations[k]))) {
#ifdef debug_transfer
                        cout << "#debug_transfer ***\n";
                        cout << int(k) << ' ' << int(mapTableOfStation.count(HASH(train_arv.stations[k]))) << '\n';
                        cout << train_arv.stations[k] << '\n';
                        cout.flush();
#endif
                        int firstDayN, secondDayN;
                        char firstArvStation = mapTableOfStation[HASH(train_arv.stations[k])];
                        L_time firstStTime(6, 1, train_st.start_hour, train_st.start_minute), checkTime, firstArvTime;
                        firstStTime += train_st.leavingTime[firstStartStation], firstStTime.month = Month, firstStTime.day = Day;
                        firstArvTime = firstStTime + (train_st.leavingTime[firstArvStation] -
                                                      train_st.leavingTime[firstStartStation] -
                                                      train_st.stopoverTimes[firstArvStation - 1]);
                        checkTime = firstStTime - train_st.leavingTime[firstStartStation];
                        if (train_st.beginMonth > checkTime.month ||
                            train_st.beginMonth == checkTime.month && train_st.beginDay > checkTime.day ||
                            train_st.endMonth < checkTime.month ||
                            train_st.endMonth == checkTime.month && train_st.endDay < checkTime.day)
                            break;
#ifdef debug_transfer
                        cout << "#debug_transfer --=--\n";
                        cout.flush();
#endif
                        firstDayN = (checkTime.month - 6) * 31 + checkTime.day;
                        L_time secondStTime(6, 1, train_arv.start_hour, train_arv.start_minute), secondArvTime;
                        secondStTime += train_arv.leavingTime[k];
                        //todo bugs here
                        //secondStTime.month = max(char(firstArvTime.month),train_arv.beginMonth), secondStTime.day = max(char(firstArvTime.day),train_arv.beginDay);
                        if (firstArvTime.month < train_arv.beginMonth ||
                            firstArvTime.month == train_arv.beginMonth && firstArvTime.day < train_arv.beginDay) {
                            secondStTime.month = train_arv.beginMonth;
                            secondStTime.day = train_arv.beginDay;
                        }else {
                            secondStTime.month=firstArvTime.month;
                            secondStTime.day=firstArvTime.day;
                        }
#ifdef debug_transfer
                        cout << "#debug_transfer -***-\n";
                        cout << firstStTime << ' ' << firstArvTime << ' ' << secondStTime << '\n';
                        cout.flush();
#endif
                        //wait whole day
                        if (!(firstArvTime<secondStTime))
                            secondStTime += 1440;
                        checkTime = secondStTime - train_arv.leavingTime[k];
                        secondArvTime = secondStTime + (train_arv.leavingTime[secondArvStation] -
                                                        train_arv.leavingTime[k] -
                                                        train_arv.stopoverTimes[secondArvStation - 1]);
#ifdef debug_transfer
                        cout << "#debug_transfer -*^*-\n";
                        cout << firstStTime << ' ' << firstArvTime << '\n' << secondStTime << ' ' << secondArvTime
                             << '\n' << checkTime << '\n';
                        cout.flush();
#endif
                        if (train_arv.beginMonth > checkTime.month ||
                            train_arv.beginMonth == checkTime.month && train_arv.beginDay > checkTime.day ||
                            train_arv.endMonth < checkTime.month ||
                            train_arv.endMonth == checkTime.month && train_arv.endDay < checkTime.day)
                            break;
#ifdef debug_transfer
                        cout << "#debug_transfer ==-==\n";
                        cout.flush();
#endif
                        secondDayN = (checkTime.month - 6) * 31 + checkTime.day;
                        //todo add record
                        int timeConsume = secondArvTime - firstStTime;
                        int firstPrice =
                                train_st.pricePrefixSum[firstArvStation] - train_st.pricePrefixSum[firstStartStation];
                        int secondPrice = train_arv.pricePrefixSum[secondArvStation] - train_arv.pricePrefixSum[k];
                        int firstSeatNum = train_st.maxSeatNum, secondSeatNum = train_arv.maxSeatNum;
                        for (char l = firstStartStation; l < firstArvStation; ++l)
                            firstSeatNum = min(firstSeatNum, train_st.seatNum[firstDayN][l]);
                        for (char l = k; l < secondArvStation; ++l)
                            secondSeatNum = min(secondSeatNum, train_arv.seatNum[secondDayN][l]);
                        if (startResult.status == 'e') {
#ifdef debug_transfer
                            cout << "#debug_transfer ww\n";
                            cout.flush();
#endif
                            startResult.status='a';
                            startResult.set(firstPrice, firstSeatNum, 0, firstDayN, firstStartStation, firstArvStation,
                                            train_st.ID, train_st.stations[firstStartStation],
                                            train_st.stations[firstArvStation], firstStTime, firstArvTime);
                            arvResult.set(secondPrice, secondSeatNum, 0, secondDayN, k, secondArvStation, train_arv.ID,
                                          train_arv.stations[k], train_arv.stations[secondArvStation], secondStTime,
                                          secondArvTime);
                            forCMP.keyTime = timeConsume, forCMP.keyPrice = firstPrice + secondPrice;
                        } else {
#ifdef debug_transfer
                            cout << "#debug_transfer qwq\n";
                            cout.flush();
#endif
                            if (compareFlag && ((firstPrice + secondPrice < forCMP.keyPrice) ||
                                                (firstPrice + secondPrice == forCMP.keyPrice &&
                                                 firstPrice < startResult.price))) {
                                startResult.set(firstPrice, firstSeatNum, 0, firstDayN, firstStartStation,
                                                firstArvStation, train_st.ID, train_st.stations[firstStartStation],
                                                train_st.stations[firstArvStation], firstStTime, firstArvTime);
                                arvResult.set(secondPrice, secondSeatNum, 0, secondDayN, k, secondArvStation,
                                              train_arv.ID, train_arv.stations[k], train_arv.stations[secondArvStation],
                                              secondStTime, secondArvTime);
                                forCMP.keyTime = timeConsume, forCMP.keyPrice = firstPrice + secondPrice;
                            }
                            if (!compareFlag && ((timeConsume < forCMP.keyTime) ||
                                                 (timeConsume == forCMP.keyTime && firstPrice < startResult.price))) {
                                startResult.set(firstPrice, firstSeatNum, 0, firstDayN, firstStartStation,
                                                firstArvStation, train_st.ID, train_st.stations[firstStartStation],
                                                train_st.stations[firstArvStation], firstStTime, firstArvTime);
                                arvResult.set(secondPrice, secondSeatNum, 0, secondDayN, k, secondArvStation,
                                              train_arv.ID, train_arv.stations[k], train_arv.stations[secondArvStation],
                                              secondStTime, secondArvTime);
                                forCMP.keyTime = timeConsume, forCMP.keyPrice = firstPrice + secondPrice;
                            }
                        }
                        break;
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
        return true;
    }

    bool trainManager::refundTicket(parser::PaperCup *cup) {
        long long Hu = HASH(*cup->arg['u' - 'a']);
        if (!Libro->Leon.count(Hu)) return false;
        int n;
        if (cup->arv == 1) n = 1;
        else n = toLong(cup->arg['n' - 'a']);
#ifdef debugs
        cout << "command  refund ticket:\n";
        cout << cup->origin << '\n';
        cout << "Hash Of user ID:" << Hu << '\n';
        cout << "refund number:" << n << '\n';
#endif
        user userTmp = Libro->Mathilda.Find(Hu);
        if (n > userTmp.orderNum) return false;
        userManager::userIdTime refundKey(Hu, userTmp.orderNum - n);
        orderRecord orderTmp = Libro->Sabine.Find(refundKey), orderGet;
        if (orderTmp.status == 'r') return false;
#ifdef debugs3

        cout << "||^||" << *cup->arg['u' - 'a'] << ' ' << Hu << '\n' << orderTmp.status << ' ' << orderTmp.trainID
             << ' '
             << orderTmp.startStation << ' ' << orderTmp.targetStation << "||^||\n";

#endif
        if (orderTmp.status == 'p') {
            orderTmp.status = 'r';
            Libro->Sabine.modify(refundKey, orderTmp);
            Arya.Delete(trainIDOrder(HASH(orderTmp.trainID), orderTmp.dayN, orderTmp.pendingNum));
        }
        if (orderTmp.status == 's') {
            orderTmp.status = 'r';
            long long hashTrainID = HASH(orderTmp.trainID);
            offsetFlag offsetTmp = Jason.Find(hashTrainID);
            Libro->Sabine.modify(refundKey, orderTmp);
            train trainTmp;
            trainRecorder.read(trainTmp, offsetTmp.offset);
            for (char i = orderTmp.st; i < orderTmp.arv; ++i)
                trainTmp.seatNum[orderTmp.dayN][i] += orderTmp.n;
#ifdef debugs1
            if (strcmp(trainTmp.ID, "LeavesofGrass") == 0 && orderTmp.dayN == 28) {
                cout<<int(orderTmp.st)<<' '<<int(orderTmp.arv)<<' '<<orderTmp.n<<'\n';
                cout<<orderTmp.startStation<<' '<<orderTmp.targetStation<<' '<<*cup->arg['u'-'a']<<'\n';
                cout<<"$$$$\n";
                for (char j = 11; j < 13; ++j) {
                    cout << trainTmp.seatNum[orderTmp.dayN][j] << ' ';
                }
                cout << "*|*|*|*\n";
            }
#endif
            vector<pendingRecord> *vec_ptr = Arya.multipleFind(trainIDOrder(hashTrainID, orderTmp.dayN));
            if (vec_ptr) {
                for (int i = 1; i < vec_ptr->size(); ++i) {
                    pendingRecord &pr = vec_ptr->operator[](i);
                    //todo search exact day's train
                    int seatNum = trainTmp.maxSeatNum;
                    for (char j = pr.st; j < pr.arv; ++j) seatNum = min(seatNum, trainTmp.seatNum[pr.dayN][j]);
#ifdef debugs1
                    cout<<int(pr.st)<<' '<<int(pr.arv)<<"^^^^\n";
#endif
                    if (seatNum >= pr.n) {
                        Arya.Delete(trainIDOrder(hashTrainID, pr.dayN, pr.pendingNum));
                        for (char j = pr.st; j < pr.arv; ++j) trainTmp.seatNum[pr.dayN][j] -= pr.n;
#ifdef debugs1
                        if (strcmp(trainTmp.ID, "LeavesofGrass") == 0 && pr.dayN == 28) {
                            for (char j = 11; j < 13; ++j) {
                                cout << trainTmp.seatNum[pr.dayN][j] << ' ';
                            }
                            cout << "*|*|*|*\n";
                        }
#endif
                        //todo optimise
                        orderGet = Libro->Sabine.Find(userManager::userIdTime(pr.hashUserId, pr.orderNum));
                        orderGet.status = 's';
                        Libro->Sabine.modify(userManager::userIdTime(pr.hashUserId, pr.orderNum), orderGet);
#ifdef debugs3
                        cout << pr.hashUserId << ' ' << int(pr.st) << ' ' << int(pr.arv) <<' '<<pr.n << '\n';
#endif
                    }
                }
                delete vec_ptr;
            }
            trainRecorder.update(trainTmp, offsetTmp.offset);
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
#ifdef debugs
        cout << "command  release train:\n";
        cout << cup->origin << '\n';
        cout << "Hash Of train ID:" << HashID << '\n';
#endif
        for (char i = 0; i < trainTmp.stationNum; ++i) {
#ifdef debugs
            cout<<trainTmp.stations[i]<<' '<<HASH(trainTmp.stations[i])<<'\n';
#endif
            Nancy.insert(stationTrain(HASH(trainTmp.stations[i])), -1);
            Nancy.insert(stationTrain(HASH(trainTmp.stations[i]), tmp.offset), offsetNum(tmp.offset, i));
        }
        int beginN = (trainTmp.beginMonth - 6) * 31 + trainTmp.beginDay, endN =
                (trainTmp.endMonth - 6) * 31 + trainTmp.endDay;
        for (int i = beginN; i <= endN; ++i)
            Arya.insert(trainIDOrder(HashID, i), pendingRecord());
#ifdef debugs1
        if (strcmp(trainTmp.ID, "LeavesofGrass") == 0) {
            for (char j = 11; j < 13; ++j) {
                cout << trainTmp.seatNum[28][j] << ' ';
            }
            cout << "*|*|*|*\n";
        }
#endif
        return true;
    }

    bool trainManager::deleteTrain(parser::PaperCup *cup) {
        long long HashID = HASH(*cup->arg['i' - 'a']);
        offsetFlag tmp = Jason.Find(HashID);
#ifdef debugs
        cout << "command delete train:\n";
        cout << cup->origin << '\n';
        cout << "Hash Of train ID:" << HashID << '\n';
#endif
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
        if (tmp.offset == -1 || !tmp.flag) return 'f';
        parser::tokenScanner tS(cup->arg['d' - 'a'], '-');
        char Month = toInt(tS.nextToken(), true), Day = toInt(tS.nextToken(), true);
        train trainTmp;
        trainRecorder.read(trainTmp, tmp.offset);
        //beyond the train capacity
        int Need = toLong(cup->arg['n' - 'a']);
        if (Need > trainTmp.maxSeatNum) return false;
        int dayN;
        user userTmp = Libro->Mathilda.Find(Hu);
        char st = -1;
#ifdef debugs
        cout << "command buyTicket:\n";
        cout << cup->origin << '\n';
        cout << "Hash Of user ID:" << Hu << '\n';
        cout << "Hash Of train ID:" << hashTrainId << '\n';
#endif
        int seatNum = trainTmp.maxSeatNum;
        L_time timeTmp(Month, Day, trainTmp.start_hour, trainTmp.start_minute), st_Time;
        for (char i = 0; i < trainTmp.stationNum; ++i) {
            if (st == -1 && strcmp(cup->arg['f' - 'a']->c_str(), trainTmp.stations[i]) == 0) st = i;
            else if (strcmp(cup->arg['t' - 'a']->c_str(), trainTmp.stations[i]) == 0) {
                if (st == -1) return 'f';
                timeTmp += trainTmp.leavingTime[st], timeTmp.day = Day, timeTmp.month = Month;
                st_Time = timeTmp - trainTmp.leavingTime[st];
                //not in the date interval
                if ((trainTmp.beginMonth > st_Time.month ||
                     trainTmp.beginMonth == st_Time.month && trainTmp.beginDay > st_Time.day) ||
                    (trainTmp.endMonth < st_Time.month ||
                     trainTmp.endMonth == st_Time.month && trainTmp.endDay < st_Time.day))
                    return 'f';
                dayN = (st_Time.month - 6) * 31 + st_Time.day;
                for (char j = st; j < i; ++j) seatNum = min(seatNum, trainTmp.seatNum[dayN][j]);
#ifdef debugs1
                if (strcmp(trainTmp.ID, "LeavesofGrass") == 0 && dayN == 28) {
                    for (char j = 11; j < 13; ++j) {
                        cout << trainTmp.seatNum[dayN][j] << ' ';
                    }
                    cout << "*|*|*|*\n";
                    cout << int(st) << ' ' << int(i) << '\n';
                    cout << seatNum << ' ' << Need << '\n';
                }
#endif
#ifdef debugs
                cout<<dayN<<' '<<seatNum<<'\n';
#endif
                if (seatNum < Need)
                    if (cup->arv == 6 || cup->arg['q' - 'a']->operator[](0) == 'f') return 'f';
                orderRecord orderTmp(trainTmp.pricePrefixSum[i] - trainTmp.pricePrefixSum[st], Need, tmp.pendingNum,
                                     dayN, st, i, trainTmp.ID, trainTmp.stations[st],
                                     trainTmp.stations[i], timeTmp,
                                     timeTmp + (trainTmp.leavingTime[i] - trainTmp.stopoverTimes[i - 1] -
                                                trainTmp.leavingTime[st]));
                if (seatNum < Need) {
                    orderTmp.status = 'p';
                    Libro->Sabine.insert(userManager::userIdTime(Hu, userTmp.orderNum), orderTmp);
                    Arya.insert(trainIDOrder(hashTrainId, dayN, tmp.pendingNum),
                                pendingRecord(trainTmp.pricePrefixSum[i] - trainTmp.pricePrefixSum[st], Need,
                                              tmp.pendingNum, dayN, st, i, userTmp.orderNum, Hu));
                    ++userTmp.orderNum;
                    Libro->Mathilda.modify(Hu, userTmp);
                    ++tmp.pendingNum;
                    Jason.modify(hashTrainId, tmp);
                    return 'p';
                }
                orderTmp.status = 's';
                Libro->Sabine.insert(userManager::userIdTime(Hu, userTmp.orderNum), orderTmp);
                for (char j = st; j < i; ++j) trainTmp.seatNum[dayN][j] -= Need;

#ifdef debugs1
                if (strcmp(trainTmp.ID, "LeavesofGrass") == 0 && dayN == 28) {
                    for (char j = 11; j < 13; ++j) {
                        cout << trainTmp.seatNum[dayN][j] << ' ';
                    }
                    cout << "*|*|*|*\n";
                    cout << int(st) << ' ' << int(i) << '\n';
                    cout << seatNum << ' ' << Need << ' ' << orderTmp.n << '\n';
                }
#endif
                trainRecorder.update(trainTmp, tmp.offset);
                ++userTmp.orderNum;
                Libro->Mathilda.modify(Hu, userTmp);
                cout << orderTmp.price * orderTmp.n << '\n';
                return 's';
            }
        }
        return 'f';
    }
}