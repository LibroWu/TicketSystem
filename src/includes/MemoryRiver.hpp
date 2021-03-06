//
// Created by Leon on 2021/4/9.
//

#ifndef BPT_MEMORYRIVER_HPP
#define BPT_MEMORYRIVER_HPP
//#define cache
#define back_up

#include "unordered_map.hpp"
#include <fstream>

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

//the first info is for the head of empty node chain
//the second info is for the number of empty node in the chain
template<class T, int info_len = 2, int poolSize = 64>
class MemoryRiver {
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);
#ifdef back_up
    int tot, inc;
#endif

#ifdef cache

    struct L_heap {
        struct heapElement {
            int frequency, position, offset;

            heapElement() = default;

            heapElement(int frequency, int offset) : frequency(frequency), offset(offset), position(0) {}
        } *p[poolSize + 1];

        int top;

        void swap(heapElement *&a, heapElement *&b) {
            heapElement *c = a;
            a = b, b = c;
            int cc = a->position;
            a->position = b->position, b->position = cc;
        }

        void pop() {
            delete p[1];
            p[1] = p[top--];
            if (top) {
                p[1]->position = 1;
                int pos = 1, nxt;
                while ((pos << 1) <= top) {
                    if (((pos << 1 | 1) > top) || p[pos << 1]->frequency < p[pos << 1 | 1]->frequency) nxt = pos << 1;
                    else nxt = pos << 1 | 1;
                    if (p[pos]->frequency > p[nxt]->frequency) {
                        swap(p[pos], p[nxt]);
                        pos = nxt;
                    } else break;
                }
            }
        }

        void push(heapElement *ptr) {
            p[++top] = ptr;
            p[top]->position = top;
            int pos = top;
            while (pos >> 1) {
                if (p[pos]->frequency < p[pos >> 1]->frequency) {
                    swap(p[pos], p[pos >> 1]);
                    pos >>= 1;
                } else break;
            }
        }

        void modify(heapElement *ptr) {
            int pos = ptr->position, nxt;
            ++ptr->frequency;
            while ((pos << 1) <= top) {
                if (((pos << 1 | 1) > top) || p[pos << 1]->frequency < p[pos << 1 | 1]->frequency) nxt = pos << 1;
                else nxt = pos << 1 | 1;
                if (p[pos]->frequency > p[nxt]->frequency) {
                    swap(p[pos], p[nxt]);
                    pos = nxt;
                } else break;
            }
        }

        void erase(int pos) {
            while (pos >> 1) {
                swap(p[pos], p[pos >> 1]);
                pos >>= 1;
            }
            pop();
        }

        L_heap() : top(0) {}

        void clear() {
            for (int i = 1; i <= top; ++i) delete p[i];
            top = 0;
        }

        ~L_heap() {
            for (int i = 1; i <= top; ++i) delete p[i];
            top = 0;
        }
    } Anna;

    struct poolElement {
        typename L_heap::heapElement *joint;
        T *t;
        bool temporary;

        poolElement() = default;

        poolElement(typename L_heap::heapElement *joint, const T &t) : joint(joint), t(new T(t)), temporary(false) {}

        ~poolElement() {
            if (!temporary) delete t;
        }
    };

    LaMetropole::unordered_map<int, poolElement> Elsa;

    typedef typename L_heap::heapElement HeapElement;

    int infoList[info_len];
#endif

#ifdef back_up

    std::string toString(int x) {
        std::string res;
        if (x == 0) return "0";
        while (x) {
            res += char(x % 10) + '0';
            x /= 10;
        }
        for (int i = 0; i < res.length() / 2; ++i) {
            char c = res[i];
            res[i] = res[res.length() - 1 - i];
            res[res.length() - 1 - i] = c;
        }
        return res;
    }

#endif
public:

    MemoryRiver() = default;

    MemoryRiver(const string &file_name) : file_name(file_name) {}

#ifdef cache

    ~MemoryRiver() {
        file.open(file_name);
        for (int i = 0; i < info_len; ++i)
            file.write(reinterpret_cast<char *>(&infoList[i]), sizeof(int));
        while (Anna.top) {
            typename L_heap::heapElement *ptr = Anna.p[1];
            poolElement tmp = Elsa[ptr->offset];
            tmp.temporary = true;
            file.seekp(ptr->offset + sizeof(int));
            file.write(reinterpret_cast<char *>(&ptr->frequency), sizeof(int));
            file.write(reinterpret_cast<char *>(tmp.t), sizeofT);
            Elsa.erase(ptr->offset);
            Anna.pop();
        }
        file.close();
    }

#endif

    void initialise(string FN = "") {
        if (FN != "") file_name = FN;
        file.open(file_name, std::ios::out);
        int tmp = sizeof(int) * info_len;
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        tmp = 0;
        for (int i = 1; i < info_len; ++i)
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
#ifdef back_up
        tot = -1;
#endif
#ifdef cache
        infoList[0] = sizeof(int) * info_len;
        for (int i = 1; i < info_len; ++i)
            infoList[i] = 0;
        Anna.clear();
        Elsa.clear();
#endif
    }

#ifdef cache

    void initialize() {
        file.open(file_name);
        for (int i = 0; i < info_len; ++i)
            file.read(reinterpret_cast<char *>(&infoList[i]), sizeof(int));
        file.close();
    }

#endif

    void get_info(int &tmp, int n) {
        if (n > info_len) return;
#ifndef cache
        file.open(file_name);
        file.seekg(sizeof(int) * (n - 1));
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
#endif
#ifdef cache
        tmp = infoList[n - 1];
#endif
    }

    void write_info(int tmp, int n) {
#ifndef cache
        if (n > info_len) return;
        file.open(file_name);
        file.seekp(sizeof(int) * (n - 1));
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
#endif
#ifdef cache
        infoList[n - 1] = tmp;
#endif
    }

    int write(T &t) {
#ifndef cache
        file.open(file_name);
        int pos, num, r_index;
        file.seekg(0);
        file.read(reinterpret_cast<char *>(&pos), sizeof(int));
        r_index = pos;
        file.read(reinterpret_cast<char *>(&num), sizeof(int));
        //no empty node exists
        if (!num) {
            file.seekp(0);
#ifdef back_up
            if (tot != -1) {
                string road = "environment/totalBackup" + toString(tot) + "/" + file_name + "Vec";
                fstream vecFile(road);
                int Num;
                vecFile.read(reinterpret_cast<char *>(&Num), sizeof(int));
                vecFile.seekp(sizeof(int) * (Num + 1));
                vecFile.write(reinterpret_cast<char *>(&pos), sizeof(int));
                vecFile.close();
            }
#endif
            pos += sizeofT + sizeof(int);
            file.write(reinterpret_cast<char *>(&pos), sizeof(int));
            file.seekp(pos - sizeofT - sizeof(int));
            file.write(reinterpret_cast<char *>(&num), sizeof(int));
            file.write(reinterpret_cast<char *>(&t), sizeofT);
        } else {
            --num;
#ifdef back_up
            if (tot != -1) {
                string road = "environment/totalBackup" + toString(tot) + "/" + file_name + "Vec";
                fstream vecFile(road);
                int Num;
                vecFile.read(reinterpret_cast<char *>(&Num), sizeof(int));
                vecFile.seekp(sizeof(int) * (Num + 1));
                vecFile.write(reinterpret_cast<char *>(&pos), sizeof(int));
                vecFile.close();
            }
#endif
            file.seekp(pos);
            file.read(reinterpret_cast<char *>(&pos), sizeof(int));
            file.write(reinterpret_cast<char *>(&t), sizeofT);
            file.seekp(0);
            file.write(reinterpret_cast<char *>(&pos), sizeof(int));
            file.write(reinterpret_cast<char *>(&num), sizeof(int));
        }
        file.close();
        return r_index;
#endif
#ifdef cache
        file.open(file_name);
        int pos = infoList[0], num = infoList[1], r_index;
        r_index = pos;
        //no empty node exists
        if (!num) {
            infoList[0] = pos + sizeofT + 2 * sizeof(int);
            file.seekp(pos);
            file.write(reinterpret_cast<char *>(&num), sizeof(int));
            file.write(reinterpret_cast<char *>(&num), sizeof(int));
            file.write(reinterpret_cast<char *>(&t), sizeofT);
        } else {
            --num;
            file.seekp(pos);
            file.read(reinterpret_cast<char *>(&pos), sizeof(int));
            int tmp = 0;
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
            file.write(reinterpret_cast<char *>(&t), sizeofT);
            infoList[0] = pos;
            infoList[1] = num;
        }
        file.close();
        return r_index;
#endif
    }

    void update(T &t, const int index) {
#ifndef cache
#ifdef back_up
        if (tot != -1) {
            string road = "environment/totalBackup" + toString(tot) + "/" + file_name + "Vec";
            fstream vecFile(road);
            int Num;
            vecFile.read(reinterpret_cast<char *>(&Num), sizeof(int));
            vecFile.seekp(sizeof(int) * (Num + 1));
            vecFile.write(reinterpret_cast<const char *>(&index), sizeof(int));
            vecFile.close();
        }
#endif
        file.open(file_name);
        file.seekp(index + sizeof(int));
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
#endif
#ifdef cache
        if (poolSize == 0) {
            file.open(file_name);
            file.seekp(index + 2 * sizeof(int));
            file.write(reinterpret_cast<char *>(&t), sizeofT);
            file.close();
        } else if (Elsa.count(index)) {
            poolElement tmp = Elsa[index];
            tmp.temporary = true;
            *tmp.t = t;
            Anna.modify(tmp.joint);
        } else {
            file.open(file_name);
            file.seekp(index + 2 * sizeof(int));
            file.write(reinterpret_cast<char *>(&t), sizeofT);
            file.close();
        }
#endif
    }

    void read(T &t, const int index) {
#ifndef cache
        file.open(file_name);
        file.seekg(index + sizeof(int));
        file.read(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
#endif
#ifdef cache
        if (poolSize == 0) {
            file.open(file_name);
            file.seekg(index + 2 * sizeof(int));
            file.read(reinterpret_cast<char *>(&t), sizeofT);
            file.close();
        } else if (Elsa.count(index)) {
            poolElement tmp = Elsa[index];
            tmp.temporary = true;
            t = *tmp.t;
            Anna.modify(tmp.joint);
        } else {
            int frequency;
            file.open(file_name);
            file.seekg(index + sizeof(int));
            file.read(reinterpret_cast<char *>(&frequency), sizeof(int));
            ++frequency;
            file.read(reinterpret_cast<char *>(&t), sizeofT);
            if (Anna.top < poolSize) {
                HeapElement *ptr = new HeapElement(frequency, index);
                Anna.push(ptr);
                poolElement tmp(ptr, t);
                Elsa[index] = tmp;
                tmp.temporary = true;
            } else {
                if (frequency < Anna.p[1]->frequency) {
                    file.seekp(index + sizeof(int));
                    file.write(reinterpret_cast<char *>(&frequency), sizeof(int));
                } else {
                    HeapElement *ptr = Anna.p[1];
                    poolElement tmp = Elsa[ptr->offset];
                    tmp.temporary = true;
                    file.seekp(ptr->offset + sizeof(int));
                    file.write(reinterpret_cast<char *>(&ptr->frequency), sizeof(int));
                    file.write(reinterpret_cast<char *>(tmp.t), sizeofT);
                    Elsa.erase(ptr->offset);
                    Anna.pop();
                    ptr = new HeapElement(frequency, index);
                    Anna.push(ptr);
                    tmp.t = new T(t);
                    tmp.joint = ptr;
                    tmp.temporary = false;
                    Elsa[index] = tmp;
                    tmp.temporary = true;
                }
            }
            file.close();
        }
#endif
    }

    void Delete(int index) {
#ifndef cache
#ifdef back_up
        if (tot != -1) {
            string road = "environment/totalBackup" + toString(tot) + "/" + file_name + "Vec";
            fstream vecFile(road);
            int Num;
            vecFile.read(reinterpret_cast<char *>(&Num), sizeof(int));
            vecFile.seekp(sizeof(int) * (Num + 1));
            vecFile.write(reinterpret_cast<char *>(&index), sizeof(int));
            vecFile.close();
        }
#endif
        int a, num;
        file.open(file_name);
        file.read(reinterpret_cast<char *>(&a), sizeof(int));
        file.read(reinterpret_cast<char *>(&num), sizeof(int));
        ++num;
        file.seekg(index);
        file.write(reinterpret_cast<char *>(&a), sizeof(int));
        file.seekp(0);
        file.write(reinterpret_cast<char *>(&index), sizeof(int));
        file.write(reinterpret_cast<char *>(&num), sizeof(int));
        file.close();
#endif
#ifdef cache
        if (poolSize && Elsa.count(index)) {
            poolElement tmp = Elsa[index];
            tmp.temporary = true;
            Anna.erase(tmp.joint->position);
            Elsa.erase(index);
        }
        int a = infoList[0], num = infoList[1];
        file.open(file_name);
        ++num;
        file.seekg(index);
        file.write(reinterpret_cast<char *>(&a), sizeof(int));
        infoList[0] = index;
        infoList[1] = num;
        file.close();
#endif
    }

#ifdef cache

    void clearCache() {
        file.open(file_name);
        for (int i = 0; i < info_len; ++i)
            file.write(reinterpret_cast<char *>(&infoList[i]), sizeof(int));
        while (Anna.top) {
            typename L_heap::heapElement *ptr = Anna.p[1];
            poolElement tmp = Elsa[ptr->offset];
            tmp.temporary = true;
            file.seekp(ptr->offset + sizeof(int));
            file.write(reinterpret_cast<char *>(&ptr->frequency), sizeof(int));
            file.write(reinterpret_cast<char *>(tmp.t), sizeofT);
            Elsa.erase(ptr->offset);
            Anna.pop();
        }
        Anna.clear();
        Elsa.clear();
        file.close();
    }

#endif

#ifdef back_up

    void setVersion(int Tot, int Inc) {
        tot = Tot;
        inc = Inc;
    }

    void backup(int op) {
        //tot
        if (op == 0) {
            string road = "environment/totalBackup" + toString(tot) + "/" + file_name + "Vec";
            string sysCom = "cp " + file_name + " environment/totalBackup" + toString(tot) + "/base/" + file_name;
            file.open(road, std::ios::out);
            file.write(reinterpret_cast<char *>(&op), sizeof(int));
            file.close();
            system(sysCom.c_str());
        } //inc
        else if (op == 1) {
            LaMetropole::unordered_map<int, bool> tmpHashTable;
            string road = "environment/totalBackup" + toString(tot) + "/" + file_name + "Vec";
            string road2 =
                    "environment/totalBackup" + toString(tot) + "/Inc" + toString(inc) + "/" + file_name + "Inc";
            fstream vecFile(road), originFile(file_name);
            file.open(road2, std::ios::out);
            int Num, v, cnt = 0;
            file.write(reinterpret_cast<char *>(&cnt), sizeof(int));
            for (int i = 0; i < info_len; ++i) {
                originFile.read(reinterpret_cast<char *>(&v), sizeof(int));
                file.write(reinterpret_cast<char *>(&v), sizeof(int));
            }
            T tmp;
            int pos;
            vecFile.read(reinterpret_cast<char *>(&Num), sizeof(int));
            for (int i = 0; i < Num; ++i) {
                vecFile.read(reinterpret_cast<char *>(&v), sizeof(int));
                if (!tmpHashTable.count(v)) {
                    tmpHashTable[v] = true;
                    originFile.seekg(v);
                    originFile.read(reinterpret_cast<char *>(&v), sizeof(int));
                    originFile.read(reinterpret_cast<char *>(&pos), sizeof(int));
                    originFile.read(reinterpret_cast<char *>(&tmp), sizeofT);
                    file.write(reinterpret_cast<char *>(&pos), sizeof(int));
                    file.write(reinterpret_cast<char *>(&tmp), sizeofT);
                    ++cnt;
                }
            }
            file.seekp(0);
            file.write(reinterpret_cast<char *>(&cnt), sizeof(int));
            file.close();
            originFile.close();
            vecFile.close();
        }
    }

    void rollBack(int tot, int inc) {
        string sysCom = "rm " + file_name;
        system(sysCom.c_str());
        sysCom = "cp environment/totalBackup" + toString(tot) + "/base/" + file_name + " " + file_name;
        system(sysCom.c_str());
        if (inc != -1) {
            string road2 =
                    "environment/totalBackup" + toString(tot) + "/Inc" + toString(inc) + "/" + file_name + "Inc";
            system(sysCom.c_str());
            file.open(file_name);
            fstream incFile(road2);
            int cnt, v;
            incFile.read(reinterpret_cast<char *>(&cnt), sizeof(int));
            for (int i = 0; i < info_len; ++i) {
                incFile.read(reinterpret_cast<char *>(&v), sizeof(int));
                file.write(reinterpret_cast<char *>(&v), sizeof(int));
            }
            T tmp;
            int pos;
            for (int i = 0; i < cnt; ++i) {
                incFile.read(reinterpret_cast<char *>(&v), sizeof(int));
                incFile.read(reinterpret_cast<char *>(&pos), sizeof(int));
                incFile.read(reinterpret_cast<char *>(&tmp), sizeofT);
                file.seekp(v);
                file.write(reinterpret_cast<char *>(&pos), sizeof(int));
                file.write(reinterpret_cast<char *>(&tmp), sizeofT);
            }
            file.close();
            incFile.close();
        }
    }

#endif

};


#endif //BPT_MEMORYRIVER_HPP
