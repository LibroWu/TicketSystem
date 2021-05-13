//
// Created by Leon on 2021/4/17.
//

#ifndef TICKETSYSTEM_UNORDERED_MAP_HPP
#define TICKETSYSTEM_UNORDERED_MAP_HPP

#include <cstring>
#include "utility.h"
#include "exceptions.h"
#include <iostream>

namespace LaMetropole {
    const int sizeSet[] = {97, 197, 397, 797, 1597, 3203, 6421, 12853, 25717, 51437,
                           102877, 205759, 411527, 823117, 1646237, 3292489, 6584983, 13169977,
                           26339969,
                           52679969, 105359939, 210719881, 421439783, 842879579, 1685759167};

    template<class Key, class T>
    class unordered_map {
    public:
        typedef pair<const Key, T> value_type;
    private:


        class hash_table {
        private:

            int Size, realSize, P;

            long long (*HashFunc)(const Key &);

            void doubleSpace() {
                Node **oldTable = table;
                int old_P = P;
                P = sizeSet[++realSize];
                table = new Node *[P];
                memset(table, 0, sizeof(Node * ) * P);
                for (int pos = 0; pos < old_P ;++pos) {
                    for (Node *i = oldTable[pos], *j; i; i = j) {
                        j = i->next;
                        insert(i->v);
                        delete i;
                    }
                }
                delete[] oldTable;
            }

        public:

            class Node {
            public:
                value_type v;
                Node *next;

                Node() = default;

                Node(const value_type &v, Node *ptr = nullptr) : v(v), next(ptr) {}
            } **table;

            typedef pair<Node *, bool> pointer;

            hash_table(long long (*hashFunc)(const Key &) = nullptr) : Size(0), realSize(0), P(sizeSet[0]),
                                                                       HashFunc(hashFunc),
                                                                       table(new Node *[sizeSet[0]]) {
                memset(table, 0, sizeof(Node *) * sizeSet[0]);
            }

            ~hash_table() {
                clear();
            }

            void clear(bool flag = false) {
                for (int pos = 0; pos < P; ++pos) {
                    for (Node *i = table[pos], *j; i; i = j) {
                        j = i->next;
                        delete i;
                    }
                }
                delete[] table;
                if (flag) {
                    P = sizeSet[realSize = 0];
                    table = new Node *[P];
                    memset(table, 0, sizeof(Node *) * P);
                } else table = nullptr;
            }

            hash_table &operator=(const hash_table &other) = delete;

            bool erase(const Key &key) {
                long long pos = HashFunc(key) % P;
                if (pos < 0) pos += P;
                if (table[pos])
                    for (Node *ptr = table[pos], *pre = nullptr; ptr; pre = ptr, ptr = ptr->next)
                        if (ptr->v.first == key) {
                            if (pre) pre->next = ptr->next;
                            else table[pos] = ptr->next;
                            delete ptr;
                            --Size;
                            return true;
                        }
                return false;
            }

            Node *find(const Key &key) const {
                long long pos = HashFunc(key) % P;
                if (pos < 0) pos += P;
                if (table[pos])
                    for (Node *ptr = table[pos]; ptr; ptr = ptr->next)
                        if (ptr->v.first == key) return ptr;
                return nullptr;
            }

            bool empty() const {
                return Size == 0;
            }

            int count() const {
                return Size;
            }

            pointer insert(const value_type &v) {
                if (Size == P) doubleSpace();
                long long pos = HashFunc(v.first) % P;
                if (pos < 0) pos += P;
                if (table[pos]) {
                    Node *ptr;
                    for (ptr = table[pos]; ptr->next; ptr = ptr->next)
                        if (ptr->v.first == v.first) return pointer(ptr, false);
                    if (ptr->v.first == v.first) return pointer(ptr, false);
                    ++Size;
                    return pointer(ptr->next = new Node(v), true);
                } else {
                    ++Size;
                    return pointer(table[pos] = new Node(v), true);
                }
            }
        } Nebula;


    public:

        unordered_map() {}

        unordered_map(long long ((*hashFunc)(const Key &)) = nullptr) : Nebula(hashFunc) {}

        unordered_map(const unordered_map &other) = delete;

        unordered_map &operator=(unordered_map &other) = delete;

        ~unordered_map() {}

        T &at(const Key &key) {
            typename hash_table::Node *tmp = Nebula.find(key);
            if (tmp) return tmp->v.second;
            else throw -1;
            //todo exceptions
        }

        const T &at(const Key &key) const {
            typename hash_table::Node *tmp = Nebula.find(key);
            if (tmp) return tmp->v.second;
            else throw -1;
            //todo exceptions
        }

        T &operator[](const Key &key) {
            typename hash_table::Node *tmp = Nebula.find(key);
            if (tmp) return tmp->v.second;
            else {
                return Nebula.insert(value_type(key, T())).first->v.second;
            }
        }

        const T &operator[](const Key &key) const {
            typename hash_table::Node *tmp = Nebula.find(key);
            if (tmp) return tmp->v.second;
            else throw -1;
            //todo exceptions
        }

        bool erase(const Key &key) {
            return Nebula.erase(key);
        }

        bool empty() const {
            return Nebula.empty();
        }

        size_t size() const {
            return Nebula.count();
        }

        void clear() {
            Nebula.clear(true);
        }

        int count(const Key &key) const {
            if (Nebula.find(key) == nullptr) return 0;
            return 1;
        }
    };
}

#endif //TICKETSYSTEM_UNORDERED_MAP_HPP
