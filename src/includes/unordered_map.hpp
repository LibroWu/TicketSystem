//
// Created by Libro on 2021/4/17.
//

#ifndef TICKETSYSTEM_UNORDERED_MAP_HPP
#define TICKETSYSTEM_UNORDERED_MAP_HPP

#include <cstring>
#include "utility.h"
#include "exceptions.h"

namespace LeMetropole {

    template<class Key, class T>
    class unordered_map {
    public:
        typedef pair<const Key, T> value_type;
    private:
        constexpr const static int sizeSet[] = {97, 197, 397, 797, 1597, 3203, 6421, 12853, 25717, 51437,
                                                102877, 205759, 411527, 823117, 1646237, 3292489, 6584983, 13169977,
                                                26339969,
                                                52679969, 105359939, 210719881, 421439783, 842879579, 1685759167};

        class hash_table {
        private:
            class occupyChain {
            public:
                class Node {
                public:
                    int pos;
                    Node *next;

                    Node(int p, Node *ptr = nullptr) : pos(p), next(ptr) {}
                } *head;

                occupyChain() : head(nullptr) {}

                bool empty() {
                    return (head == nullptr);
                }

                void push(int pos) {
                    head = new Node(pos, head);
                }

                inline int front() {
                    return head->pos;
                }

                void pop() {
                    if (head) {
                        Node *tmp = head;
                        head = head->next;
                        delete tmp;
                    }
                }
            } chain;

            int Size, realSize, P;

            int (*HashFunc)(const Key &);

            void doubleSpace() {
                Node **oldTable = table;
                P = sizeSet[++realSize];
                table = new Node *[P];
                memset(table, 0, sizeof(Node * ) * P);
                occupyChain oldChain;
                oldChain.head = chain.head;
                int pos;
                while (!oldChain.empty()) {
                    pos = oldChain.front();
                    oldChain.pop();
                    for (Node *i = oldTable[pos], *j; i; i = j) {
                        j = i->next;
                        insert(i->v);
                        delete i;
                    }
                    oldTable[pos] = nullptr;
                }
                delete[] oldTable;
            }

        public:
            class Node {
            public:
                value_type v;
                Node *next;

                Node(const value_type &v, Node *ptr = nullptr) : v(v), next(ptr) {}
            } **table;

            typedef pair<Node *, bool> pointer;

            hash_table(int (*hashFunc)(const Key &) = nullptr) : Size(0), realSize(0), P(sizeSet[0]),
                                                                 HashFunc(hashFunc), table(new Node *[sizeSet[0]]) {
                memset(table, 0, sizeof(Node *) * sizeSet[0]);
            }

            ~hash_table() {
                clear();
            }

            void clear() {
                int pos;
                while (!chain.empty()) {
                    pos = chain.front();
                    chain.pop();
                    for (Node *i = table[pos], *j; i; i = j) {
                        j = i->next;
                        delete i;
                    }
                    table[pos] = nullptr;
                }
            }

            hash_table &operator=(const hash_table &other) {
                clear();
                //todo
                return *this;
            }

            bool erase(const Key &key) {
                int pos = HashFunc(key) % P;
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

            Node *find(const Key &key) {
                int pos = HashFunc(key) % P;
                if (table[pos])
                    for (Node *ptr = table[pos]; ptr; ptr = ptr->next)
                        if (ptr->v.first == key) return ptr;
                return nullptr;
            }

            pointer *insert(const value_type &v) {
                if (Size == P) doubleSpace();
                int pos = HashFunc(v.first) % P;
                if (table[pos]) {
                    Node *ptr;
                    for (ptr = table[pos]; ptr->next; ptr = ptr->next)
                        if (ptr->v.first == v.first) return pointer(ptr, false);
                    if (ptr->v.first == v.first) return pointer(ptr, false);
                    ++Size;
                    return pointer(ptr->next = new Node(v), true);
                } else {
                    ++Size;
                    chain.push(pos);
                    return pointer(table[pos] = new Node(v), true);
                }
            }
        } Nebula;


    public:

        unordered_map() {}

        unordered_map(const unordered_map &other) : Nebula(other.Nebula) {}

        unordered_map &operator=(unordered_map &other) {
            Nebula = other.Nebula;
            return *this;
        }

        ~unordered_map() {}

        T &at(const Key &key) {
            pointer ptr = Nebula.get(key);
            if (ptr.second)
                return ptr.first->record.second;
            else throw index_out_of_bound();
        }

        const T &at(const Key &key) const {
            pointer ptr = Nebula.get(key);
            if (ptr.second)
                return ptr.first->record.second;
            else throw index_out_of_bound();
        }

        T &operator[](const Key &key) {
            pointer ptr = Nebula.get(key);
            if (ptr.second)
                return ptr.first->record.second;
            else {
                pointer ptrr = Nebula.insert(key);
                return ptrr.first->record.second;
            }
        }

        const T &operator[](const Key &key) const {
            pointer ptr = Nebula.get(key);
            if (ptr.second)
                return ptr.first->record.second;
            else throw index_out_of_bound();
        }

        bool empty() const {
            return (Nebula.head == nullptr);
        }

        size_t size() const {
            return Nebula.count;
        }

        void clear() {
            Nebula.Clear();
        }

        void erase(iterator pos) {
            if (&Nebula != pos.source || pos == end()) throw invalid_iterator();
            Nebula.Delete(pos.ptr->record.first);
        }

        size_t count(const Key &key) const {
            return Nebula.get(key).second;
        }
    };
}

#endif //TICKETSYSTEM_UNORDERED_MAP_HPP
