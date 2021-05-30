//
// Created by Leon on 2021/4/9.
//

#ifndef BPT_MEMORYRIVER_HPP
#define BPT_MEMORYRIVER_HPP
//#define cache
#include <fstream>
using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

//the first info is for the head of empty node chain
//the second info is for the number of empty node in the chain
template<class T, int info_len = 2, int poolSize = 100>
class MemoryRiver {
private:
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);
#ifdef cache
    struct L_heap {
        struct heapElement {
            int frequency, position, offset;
        } *p[poolSize];
        int top;

        void swap(heapElement *&a, heapElement *&b) {
            heapElement *c = a;
            a = b, b = c;
            int cc = a->frequency;
            a->frequency = b->frequency, b->frequency = cc;
        }

        void pop() {
            if (top) {
                delete p[1];
                p[1] = p[top--];
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
            while (pos) {
                if (p[pos]->frequency < p[pos >> 1]->frequency) {
                    swap(p[pos], p[pos >> 1]);
                    pos >>= 1;
                } else break;
            }
        }

        void modify(heapElement *ptr) {
            int pos=ptr->position,nxt;
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

        L_heap() : top(0) {}

        void clear() {
            for (int i = 1; i <= top; ++i) delete p[i];
            top = 0;
        }

        ~L_heap() {
            for (int i = 1; i <= top; ++i) delete p[i];
            top = 0;
        }
    };

    struct poolElement{
        typename L_heap::heapElement* joint;
        T* t;
        poolElement()=default;
        poolElement(typename L_heap::heapElement* joint,const T & t):joint(joint),t(new T (t)) {}
        ~poolElement() {
            delete t;
        }
    };

    LaMetropole::unordered_map<int,poolElement> Elsa;
#endif
public:
    MemoryRiver() = default;

    MemoryRiver(const string &file_name) : file_name(file_name) {}

    void initialise(string FN = "") {
        if (FN != "") file_name = FN;
        file.open(file_name, std::ios::out);
        int tmp = sizeof(int) * info_len;
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        tmp = 0;
        for (int i = 1; i < info_len; ++i)
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    void get_info(int &tmp, int n) {
        if (n > info_len) return;
        file.open(file_name);
        file.seekg(sizeof(int) * (n - 1));
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    void write_info(int tmp, int n) {
        if (n > info_len) return;
        file.open(file_name);
        file.seekp(sizeof(int) * (n - 1));
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    int write(T &t) {
        file.open(file_name);
        int pos, num, r_index;
        file.seekg(0);
        file.read(reinterpret_cast<char *>(&pos), sizeof(int));
        r_index = pos;
        file.read(reinterpret_cast<char *>(&num), sizeof(int));
        //no empty node exists
        if (!num) {
            file.seekp(0);
            pos += sizeofT + sizeof(int);
            file.write(reinterpret_cast<char *>(&pos), sizeof(int));
            file.seekp(pos - sizeofT - sizeof(int));
            file.write(reinterpret_cast<char *>(&num), sizeof(int));
            file.write(reinterpret_cast<char *>(&t), sizeofT);
        } else {
            --num;
            file.seekp(pos);
            file.read(reinterpret_cast<char *>(&pos), sizeof(int));
            file.write(reinterpret_cast<char *>(&t), sizeofT);
            file.seekp(0);
            file.write(reinterpret_cast<char *>(&pos), sizeof(int));
            file.write(reinterpret_cast<char *>(&num), sizeof(int));
        }
        file.close();
        return r_index;
    }

    void update(T &t, const int &index) {
        file.open(file_name);
        file.seekp(index + sizeof(int));
        file.write(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
    }

    void read(T &t, const int &index) {
#ifndef cache
        file.open(file_name);
        file.seekg(index + sizeof(int));
        file.read(reinterpret_cast<char *>(&t), sizeofT);
        file.close();
#endif
#ifdef cache
#endif
    }

    void Delete(int index) {
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
    }
};


#endif //BPT_MEMORYRIVER_HPP
