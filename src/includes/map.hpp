/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.h"
#include "exceptions.h"

namespace sjtu {

    template<
            class Key,
            class T,
            class Compare = std::less<Key>
    >
    class map {
    public:

        enum COLOR {
            red, black
        };

        class RedBlackNode {
        public:
            COLOR nodeColor;
            RedBlackNode *next, *pre, *parent, *lch, *rch;
            pair<const Key, T> record;

            RedBlackNode(const Key &k, const T &v, COLOR col = red) : record(k, v),
                                                                      parent(nullptr), lch(nullptr),
                                                                      rch(nullptr), next(nullptr),
                                                                      pre(nullptr), nodeColor(col) {}

            explicit RedBlackNode(const RedBlackNode *other) : record(other->record), nodeColor(other->nodeColor),
                                                               parent(nullptr),
                                                               lch(nullptr), rch(nullptr), next(nullptr), pre(nullptr) {}

            ~RedBlackNode() {
                if (next)
                    next->pre = pre;
                if (pre)
                    pre->next = next;
            }

        };

        //false for failing to insert because same key has existed
        typedef pair<RedBlackNode *, bool> pointer;

        class RBT {
        public:
            RedBlackNode *head, *Beg, *End;
            size_t count;

            //first is the smallest one in the subtree and vice versa
            pair<RedBlackNode *, RedBlackNode *>
            build_tree(RedBlackNode *&ptr, RedBlackNode *other_ptr, RedBlackNode *pre,
                       RedBlackNode *next) {
                ptr = new RedBlackNode(other_ptr);
                ptr->pre = pre;
                ptr->next = next;
                pair<RedBlackNode *, RedBlackNode *> ptr_pair(ptr, ptr);
                if (other_ptr->lch) {
                    pair<RedBlackNode *, RedBlackNode *> tmp = build_tree(ptr->lch, other_ptr->lch, pre, ptr);
                    ptr->lch->parent = ptr;
                    ptr->pre = tmp.second;
                    ptr_pair.first = tmp.first;
                }
                if (other_ptr->rch) {
                    pair<RedBlackNode *, RedBlackNode *> tmp = build_tree(ptr->rch, other_ptr->rch, ptr, next);
                    ptr->rch->parent = ptr;
                    ptr->next = tmp.first;
                    ptr_pair.second = tmp.second;
                }
                return ptr_pair;
            }

            RBT(const RBT &other) : head(nullptr), Beg(nullptr), End(nullptr), count(other.count) {
                if (other.head == nullptr) return;
                pair<RedBlackNode *, RedBlackNode *> tmp = build_tree(head, other.head, nullptr, nullptr);
                Beg = tmp.first;
                End = tmp.second;
            }

            RBT &operator=(const RBT &other) {
                if (this == &other) return *this;
                this->Clear();
                count = other.count;
                if (other.head == nullptr) return *this;
                pair<RedBlackNode *, RedBlackNode *> tmp = build_tree(head, other.head, nullptr, nullptr);
                Beg = tmp.first;
                End = tmp.second;
                return *this;
            }

            RBT() : head(nullptr), Beg(nullptr), End(nullptr), count(0) {}

            ~RBT() {
                Clear();
            }

            template<class Q>
            void Swap(Q &a, Q &b) {
                Q c = a;
                a = b, b = c;
            }

            void Clear() {
                if (head) {
                    for (RedBlackNode *ptr = Beg, *j; ptr; ptr = j) {
                        j = ptr->next;
                        delete ptr;
                    }
                }
                head = Beg = End = nullptr;
                count = 0;
            }

            void makeEmpty(RedBlackNode *ptr) {
                if (ptr->rch)
                    makeEmpty(ptr->rch);
                if (ptr->lch)
                    makeEmpty(ptr->lch);
                delete ptr;
            }

            void Del(RedBlackNode *ptr) {
                if (ptr == Beg) Beg = ptr->next;
                if (ptr == End) End = ptr->pre;
                delete ptr;
            }

            void singleRotate(RedBlackNode *ptr) {
                RedBlackNode *P = ptr->parent;
                if (P == nullptr) return;
                bool flag_ptr = isLeftChild(ptr);
                if (P == head) {
                    head = ptr;
                    ptr->parent = nullptr;
                }
                else {
                    if (isLeftChild(P)) P->parent->lch = ptr;
                    else P->parent->rch = ptr;
                    ptr->parent = P->parent;
                }
                P->parent = ptr;
                if (flag_ptr) {
                    if (ptr->rch) ptr->rch->parent = P;
                    P->lch = ptr->rch,ptr->rch = P;
                }
                else {
                    if (ptr->lch) ptr->lch->parent = P;
                    P->rch = ptr->lch,ptr->lch = P;
                }
            }

            void rotate(RedBlackNode *ptr, RedBlackNode *P, RedBlackNode *G, int dye_pattern = 0) {
                //LL & RR
                if (P->lch == ptr && G->lch == P || P->rch == ptr && G->rch == P) {
                    singleRotate(P);
                    if (dye_pattern == 0) P->nodeColor = black,G->nodeColor = red;
                    else if (dye_pattern == 1) ptr->nodeColor = black,G->nodeColor = black,P->nodeColor = red;
                }
                //LR & RL
                if (P->rch == ptr && G->lch == P || P->lch == ptr && G->rch == P) {
                    singleRotate(ptr),singleRotate(ptr);
                    if (dye_pattern == 0) ptr->nodeColor = black, G->nodeColor = red;
                    else if (dye_pattern == 1) ptr->nodeColor = red, G->nodeColor = black, P->nodeColor = black;
                }
            }

            pointer insert(const Key &key, const T &value = T()) {
                Compare cmp;
                if (!head) {
                    head = new RedBlackNode(key, value, black),Beg = End = head,++count;
                    return pointer(head, true);
                }
                RedBlackNode *ptr = head, *child, *P, *G, *pre, *next;
                pre = next = nullptr;
                while (1) {
                    //have existed?
                    if (!(cmp(ptr->record.first, key) || cmp(key, ptr->record.first))) return pointer(ptr, false);
                    //avoid red uncle node
                    if (ptr->rch && ptr->lch)
                        if (ptr->rch->nodeColor == red && ptr->lch->nodeColor == red) {
                            ptr->rch->nodeColor = ptr->lch->nodeColor = black;
                            P = ptr->parent;
                            if (P) {
                                G = P->parent;
                                ptr->nodeColor = red;
                                if (P->nodeColor == red) {
                                    //rotate
                                    rotate(ptr, P, G);
                                }
                            }
                        }
                    //insert into left tree
                    if (cmp(key, ptr->record.first)) {
                        next = ptr;
                        if (ptr->lch) {
                            ptr = ptr->lch;
                        }//insert
                        else {
                            ptr->lch = new RedBlackNode(key, value);
                            child = ptr->lch;
                            child->pre = pre;
                            child->next = next;
                            if (pre) pre->next = child;
                            else Beg = child;
                            if (next) next->pre = child;
                            else End = child;
                            child->parent = ptr;
                            if (ptr->nodeColor == red) {
                                rotate(child, ptr, ptr->parent);
                            }
                            ++count;
                            return pointer(child, true);
                        }
                    }//insert into right tree
                    else {
                        pre = ptr;
                        if (ptr->rch) {
                            ptr = ptr->rch;
                        }//insert
                        else {
                            ptr->rch = new RedBlackNode(key, value);
                            child = ptr->rch;
                            child->pre = pre;
                            child->next = next;
                            if (pre) pre->next = child;
                            else Beg = child;
                            if (next) next->pre = child;
                            else End = child;
                            child->parent = ptr;
                            if (ptr->nodeColor == red) {
                                rotate(child, ptr, ptr->parent);
                            }
                            ++count;
                            return pointer(child, true);
                        }
                    }
                }
            }

            void SwapTwoRBNode(RedBlackNode *a, RedBlackNode *b) {
                if (a->parent == b) {
                    SwapTwoRBNode(b, a);
                    return;
                }
                RedBlackNode *BP, *BR, *BL;
                Swap(a->nodeColor, b->nodeColor);
                bool A_lch, B_lch;
                BP = b->parent, BL = b->lch, BR = b->rch;
                if (a->parent) A_lch = isLeftChild(a);
                if (BP) B_lch = isLeftChild(b);
                if (BP == a) {
                    if (a->parent) {
                        if (A_lch) a->parent->lch = b;
                        else a->parent->rch = b;
                    }
                    else head = b;
                    b->parent = a->parent, a->parent = b;
                    if (B_lch) {
                        b->lch = a, b->rch = a->rch;
                        if (a->rch) a->rch->parent = b;
                    }
                    else {
                        b->rch = a, b->lch = a->lch;
                        if (a->lch) a->lch->parent = b;
                    }
                    if (BL) BL->parent = a;
                    if (BR) BR->parent = a;
                    a->lch = BL, a->rch = BR;
                }
                else {
                    if (BP) {
                        if (B_lch) BP->lch = a;
                        else BP->rch = a;
                    }
                    else head = a;
                    b->parent = a->parent;
                    if (a->parent) {
                        if (A_lch) a->parent->lch = b;
                        else a->parent->rch = b;
                    }
                    else head = b;
                    a->parent = BP;
                    if (BL) BL->parent = a;
                    if (BR) BR->parent = a;
                    b->lch = a->lch, b->rch = a->rch;
                    if (a->lch) a->lch->parent = b;
                    if (a->rch) a->rch->parent = b;
                    a->lch = BL, a->rch = BR;
                }
            }

            bool isLeftChild(RedBlackNode *ptr) {
                return (ptr->parent->lch == ptr);
            }

            RedBlackNode *getSibling(RedBlackNode *ptr) {
                if (ptr->parent) {
                    if (isLeftChild(ptr)) return ptr->parent->rch;
                    else return ptr->parent->lch;
                }
                return nullptr;
            }

            RedBlackNode *getSiblingsRedChild(RedBlackNode *ptr) {
                RedBlackNode *tmp = getSibling(ptr);
                if (tmp) {
                    if (isLeftChild(ptr)) {
                        if (tmp->rch && tmp->rch->nodeColor == red) return tmp->rch;
                        if (tmp->lch && tmp->lch->nodeColor == red) return tmp->lch;
                    }
                    else {
                        if (tmp->lch && tmp->lch->nodeColor == red) return tmp->lch;
                        if (tmp->rch && tmp->rch->nodeColor == red) return tmp->rch;
                    }
                }
                return nullptr;
            }

            void Delete(const Key &key, bool flag = false) {
                Compare cmp;
                RedBlackNode *ptr = head, *child, *P, *G, *R, *Sib;
                while (1) {
                    //make current node red
                    if (ptr == nullptr) break;
                    if (ptr->nodeColor == black) {
                        if ((ptr->lch == nullptr || ptr->lch->nodeColor == black) &&
                            (ptr->rch == nullptr || ptr->rch->nodeColor == black)) {
                            R = getSiblingsRedChild(ptr);
                            Sib = getSibling(ptr);
                            if (R == nullptr) {
                                if (ptr->parent) ptr->parent->nodeColor = black;
                                ptr->nodeColor = red;
                                if (Sib) Sib->nodeColor = red;
                            }
                            else {
                                rotate(R, Sib, ptr->parent, 1);
                                ptr->nodeColor = red;
                            }
                        }
                        else {
                            if (!cmp(key, ptr->record.first) && !cmp(ptr->record.first, key)) {
                                if (flag)
                                    flag = 1;
                                if (ptr->rch == nullptr || ptr->lch == nullptr) {
                                    if (ptr->rch != nullptr) child = ptr->rch;
                                    else child = ptr->lch;
                                    singleRotate(child);
                                    child->nodeColor = black;
                                    ptr->nodeColor = red;
                                }
                                else {
                                    P = ptr->next, Sib = ptr->lch;
                                    SwapTwoRBNode(ptr, ptr->next);
                                    if (P->rch->nodeColor == black) {
                                        singleRotate(Sib);
                                        Sib->nodeColor = black;
                                        P->nodeColor = red;
                                    }
                                    ptr = P->rch;
                                }
                                continue;
                            }
                            else if (cmp(key, ptr->record.first)) {
                                P = ptr,Sib = P->rch,ptr = ptr->lch;
                                if (ptr && ptr->nodeColor == black) {
                                    singleRotate(Sib),P->nodeColor = red,Sib->nodeColor = black;
                                }
                                continue;
                            }
                            else {
                                P = ptr,Sib = P->lch,ptr = ptr->rch;
                                if (ptr && ptr->nodeColor == black) {
                                    singleRotate(Sib),P->nodeColor = red,Sib->nodeColor = black;
                                }
                                continue;
                            }
                        }
                    }
                    //delete the node
                    if (!cmp(key, ptr->record.first) && !cmp(ptr->record.first, key)) {
                        //leaf or has only one child
                        if (ptr->rch == nullptr) {
                            //has the left child
                            if (ptr->lch != nullptr) {
                                if (ptr->parent) {
                                    if (isLeftChild(ptr)) ptr->parent->lch = ptr->lch;
                                    else ptr->parent->rch = ptr->lch;
                                    ptr->lch->parent = ptr->parent;
                                }
                                else {
                                    head = ptr->lch;
                                    ptr->lch->parent = nullptr;
                                }
                            }//does not have a child
                            else {
                                if (ptr->parent) {
                                    if (isLeftChild(ptr)) ptr->parent->lch = nullptr;
                                    else ptr->parent->rch = nullptr;
                                }
                                else head = nullptr;
                            }
                            Del(ptr);
                            --count;
                            break;
                        }
                        else {
                            //has the right child
                            if (ptr->lch == nullptr) {
                                if (ptr->parent) {
                                    if (isLeftChild(ptr)) ptr->parent->lch = ptr->rch;
                                    else ptr->parent->rch = ptr->rch;
                                    ptr->rch->parent = ptr->parent;
                                }
                                else {
                                    head = ptr->rch;
                                    ptr->rch->parent = nullptr;
                                }
                                Del(ptr);
                                --count;
                                break;
                            }//has the right and left child
                            else {
                                child = ptr->next;
                                SwapTwoRBNode(ptr, ptr->next);
                                ptr = child->rch;
                            }
                        }
                    }
                    else ptr = cmp(key, ptr->record.first) ? ptr->lch : ptr->rch;
                }
                //adjust the root color
                if (head && head->nodeColor == red)
                    head->nodeColor = black;
            }

            //false for not found
            pointer get(const Key &key) const {
                Compare cmp;
                RedBlackNode *ptr = head;
                while (ptr) {
                    if (!(cmp(ptr->record.first, key) || cmp(key, ptr->record.first))) return pointer(ptr, true);
                    if (cmp(key, ptr->record.first)) ptr = ptr->lch;
                    else ptr = ptr->rch;
                }
                return pointer(nullptr, false);
            }
        } Nebula;

    public:
        typedef pair<const Key, T> value_type;

        class const_iterator;

        class iterator {
            friend map<Key, T, Compare>;
        private:
            RedBlackNode *ptr;
            const RBT *source;
        public:

            iterator(RedBlackNode *ptr, const RBT *source) : ptr(ptr), source(source) {}

            iterator() : ptr(nullptr), source(nullptr) {}

            iterator(const iterator &other) : ptr(other.ptr), source(other.source) {}

            iterator operator++(int) {
                if (ptr == nullptr) throw invalid_iterator();
                iterator tmp(*this);
                ptr = ptr->next;
                return tmp;
            }

            iterator &operator++() {
                if (ptr == nullptr) throw invalid_iterator();
                ptr = ptr->next;
                return *this;
            }

            iterator operator--(int) {
                if (source->count==0) throw invalid_iterator();
                if (ptr == nullptr) {
                    ptr = source->End;
                    return iterator(nullptr, source);
                }
                if (ptr->pre == nullptr) throw invalid_iterator();
                iterator tmp(*this);
                ptr = ptr->pre;
                return tmp;
            }

            iterator &operator--() {
                if (source->count==0) throw invalid_iterator();
                if (ptr == nullptr) {
                    ptr = source->End;
                    return *this;
                }
                if (ptr->pre == nullptr) throw invalid_iterator();
                ptr = ptr->pre;
                return *this;
            }

            value_type &operator*() const {
                if (ptr == nullptr) throw invalid_iterator();
                return ptr->record;
            }

            bool operator==(const iterator &rhs) const {
                return (ptr == rhs.ptr && source == rhs.source);
            }

            bool operator==(const const_iterator &rhs) const {
                return (ptr == rhs.ptr && source == rhs.source);
            }

            bool operator!=(const iterator &rhs) const {
                return (ptr != rhs.ptr || source != rhs.source);
            }

            bool operator!=(const const_iterator &rhs) const {
                return (ptr != rhs.ptr || source != rhs.source);
            }

            value_type *operator->() const noexcept {
                return &(ptr->record);
            }
        };

        class const_iterator {
            friend map<Key, T, Compare>;
        private:
            const RedBlackNode *ptr;
            const RBT *source;
        public:

            //const_iterator(RedBlackNode *ptr, RBT *source) : ptr(ptr), source(source) {}

            const_iterator() : ptr(nullptr), source(nullptr) {}

            const_iterator(const const_iterator &other) : ptr(other.ptr), source(other.source) {}

            const_iterator(const iterator &other) : ptr(other.ptr), source(other.source) {}

            const_iterator operator++(int) {
                if (ptr == nullptr) throw invalid_iterator();
                const_iterator tmp(*this);
                ptr = ptr->next;
                return tmp;
            }

            const_iterator &operator++() {
                if (ptr == nullptr) throw invalid_iterator();
                ptr = ptr->next;
                return *this;
            }

            const_iterator operator--(int) {
                if (source->count==0) throw invalid_iterator();
                if (ptr == nullptr) {
                    ptr = source->End;
                    return iterator(nullptr, source);
                }
                if (ptr->pre == nullptr) throw invalid_iterator();
                const_iterator tmp(*this);
                ptr = ptr->pre;
                return tmp;
            }

            const_iterator &operator--() {
                if (source->count==0) throw invalid_iterator();
                if (ptr == nullptr) {
                    ptr = source->End;
                    return *this;
                }
                if (ptr->pre == nullptr) throw invalid_iterator();
                ptr = ptr->pre;
                return *this;
            }

            const value_type &operator*() const {
                if (ptr == nullptr) throw invalid_iterator();
                return ptr->record;
            }

            bool operator==(const iterator &rhs) const {
                return (ptr == rhs.ptr && source == rhs.source);
            }

            bool operator==(const const_iterator &rhs) const {
                return (ptr == rhs.ptr && source == rhs.source);
            }

            bool operator!=(const iterator &rhs) const {
                return (ptr != rhs.ptr || source != rhs.source);
            }

            bool operator!=(const const_iterator &rhs) const {
                return (ptr != rhs.ptr || source != rhs.source);
            }

            const value_type *operator->() const noexcept {
                return &(ptr->record);
            }
        };

        map() {}

        map(const map &other) : Nebula(other.Nebula) {}

        map &operator=(const map &other) {
            Nebula = other.Nebula;
            return *this;
        }

        ~map() {}

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

        iterator begin() {
            iterator tmp;
            tmp.source = &Nebula;
            tmp.ptr = Nebula.Beg;
            return tmp;
        }

        const_iterator cbegin() const {
            const_iterator tmp;
            tmp.source = &Nebula;
            tmp.ptr = Nebula.Beg;
            return tmp;
        }

        iterator end() {
            iterator tmp;
            tmp.source = &Nebula;
            tmp.ptr = nullptr;
            return tmp;
        }

        const_iterator cend() const {
            iterator tmp;
            tmp.source = &Nebula;
            tmp.ptr = nullptr;
            return tmp;
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

        pair<iterator, bool> insert(const value_type &value) {
            pointer tmp = Nebula.insert(value.first, value.second);
            return pair<iterator, bool>(iterator(tmp.first, &Nebula), tmp.second);
        }

        void erase(iterator pos) {
            if (&Nebula != pos.source || pos == end()) throw invalid_iterator();
            Nebula.Delete(pos.ptr->record.first);
        }

        size_t count(const Key &key) const {
            return Nebula.get(key).second;
        }

        iterator find(const Key &key) {
            pointer tmp = Nebula.get(key);
            if (!tmp.second) return end();
            return iterator(tmp.first, &Nebula);
        }

        const_iterator find(const Key &key) const {
            pointer tmp = Nebula.get(key);
            if (!tmp.second) return cend();
            return iterator(tmp.first, &Nebula);
        }
    };

}

#endif
