#ifndef EXAM_LIST_LIST_HPP
#define EXAM_LIST_LIST_HPP

#include <bits/stl_iterator_base_types.h>
#include <iterator>

template<typename T>
struct list {
private:
    struct node {
        node* prev;
        node* next;

        node() : prev(this), next(this) {}
        node(node* const prev, node* const next) : prev(prev), next(next) {}

        void swap(node& that) {
            if (next == this && that.next == &that) {
                return;
            }
            if (next != this && that.next == &that) {
                next->prev = &that;
                prev->next = &that;
                that.next = next;
                that.prev = prev;
                next = this;
                prev = this;
                return;
            }
            if (next == this && that.next != &that) {
                that.next->prev = this;
                that.prev->next = this;
                next = that.next;
                prev = that.prev;
                that.next = &that;
                that.prev = &that;
                return;
            }
            if (next != this && that.next != &that) {
                next->prev = &that;
                prev->next = &that;
                that.next->prev = this;
                that.prev->next = this;
                std::swap(next, that.next);
                std::swap(prev, that.prev);
                return;
            }
        }
    };

    struct payload_node : node {
        T payload;

        payload_node() = delete;
        payload_node(node* const prev, node* const next, const T& val) : node(prev, next), payload(val) {}
    };

    void connect_nodes(node* a, node* b) {
        a->next = b;
        b->prev = a;
    }

public:
    struct node_iterator {
        friend struct list;

        typedef T value_type;
        typedef T& reference;
        typedef ptrdiff_t difference_type;
        typedef T* pointer;
        typedef std::bidirectional_iterator_tag iterator_category;

        node_iterator() = delete;
        node_iterator(const node_iterator& that) = default;
        node_iterator& operator=(const node_iterator &that) = default;

        node_iterator& operator++() {
            ptr = ptr->next;
            return *this;
        }

        node_iterator& operator--() {
            ptr = ptr->prev;
            return *this;
        }

        const node_iterator operator++(int) {
            node_iterator ret(*this);
            ++*this;
            return ret;
        }

        const node_iterator operator--(int) {
            node_iterator ret(*this);
            --*this;
            return  ret;
        }

        reference operator*() const {
            return (static_cast<payload_node*>(ptr))->payload;
        }

        pointer operator->() const {
            return &(static_cast<payload_node*>(ptr)->payload);
        }

        friend bool operator==(const node_iterator& a, const node_iterator& b) {
            return a.ptr == b.ptr;
        }

        friend bool operator!=(const node_iterator& a, const node_iterator& b) {
            return a.ptr != b.ptr;
        }

    private:
        node* ptr;

        explicit node_iterator(node* that) : ptr(that) {}
    };

    struct node_const_iterator {
        friend struct list;

        typedef T value_type;
        typedef const T& reference;
        typedef ptrdiff_t difference_type;
        typedef const T* pointer;
        typedef std::bidirectional_iterator_tag iterator_category;

        node_const_iterator() = delete;
        node_const_iterator(const node_const_iterator& that) = default;
        node_const_iterator& operator=(const node_const_iterator &that) = default;

        node_const_iterator(const node_iterator& that) : ptr(that.ptr) {}

        node_const_iterator& operator++() {
            ptr = ptr->next;
            return *this;
        }

        node_const_iterator& operator--() {
            ptr = ptr->prev;
            return *this;
        }

        const node_const_iterator operator++(int) {
            node_const_iterator ret(*this);
            ++*this;
            return ret;
        }

        const node_const_iterator operator--(int) {
            node_const_iterator ret(*this);
            --*this;
            return  ret;
        }

        reference operator*() const {
            return static_cast<payload_node*>(ptr)->payload;
        }

        pointer operator->() const {
            return &(static_cast<payload_node*>(ptr)->payload);
        }

        friend bool operator==(const node_const_iterator& a, const node_const_iterator& b) {
            return a.ptr == b.ptr;
        }

        friend bool operator!=(const node_const_iterator& a, const node_const_iterator& b) {
            return a.ptr != b.ptr;
        }

    private:
        node* ptr;

        node_const_iterator(node* that) : ptr(that) {}
    };

    typedef T value_type;

    typedef node_iterator iterator;
    typedef node_const_iterator const_iterator;
    typedef std::reverse_iterator<node_iterator> reverse_iterator;
    typedef std::reverse_iterator<node_const_iterator> const_reverse_iterator;

    list() : head() {}

    list(const list& that) {
        for (auto that_val : that) {
            try {
                push_back(that_val);
            } catch (...) {
                clear();
                throw std::exception();
            }
        }
    }

    ~list() {
        clear();
    }

    list& operator=(const list& that) {
        list tmp(that);
        swap(*this, tmp);
        return *this;
    }

    void push_back(const T& val) {
        insert(end(), val);
    }

    void push_front(const T& val) {
        insert(begin(), val);
    }

    void pop_back() {
        erase(--end());
    }

    void pop_front() {
        erase(begin());
    }

    T& front() {
        return *begin();
    }

    T& back() {
        return *(--end());
    }

    const T& front() const {
        return *cbegin();
    }

    const T& back() const {
        return *(--cend());
    }

    bool empty() const {
        return begin() == end();
    }

    void clear() {
        while (begin() != end()) {
            erase(begin());
        }
    }

    void splice(const_iterator pos, list& other, const_iterator first, const_iterator last) {
        if (first == last) {
            return;
        }

        const_iterator prev_pos = pos;
        --prev_pos;
        const_iterator prev_first = first;
        --prev_first;
        const_iterator prev_last = last;
        --prev_last;

        connect_nodes(prev_pos.ptr, first.ptr);
        connect_nodes(prev_last.ptr, pos.ptr);
        connect_nodes(prev_first.ptr, last.ptr);
    }

    iterator insert(const_iterator pos, const T& val) {
        const_iterator prev = pos;
        --prev;
        auto tmp = new payload_node(prev.ptr, pos.ptr, val);
        prev.ptr->next = tmp;
        pos.ptr->prev = tmp;
        return iterator(tmp);
    }

    iterator erase(const_iterator pos) {
        const_iterator prev = pos;
        --prev;
        const_iterator next = pos;
        ++next;
        delete static_cast<payload_node*>(prev.ptr->next);
        prev.ptr->next = next.ptr;
        next.ptr->prev = prev.ptr;
        return iterator(next.ptr);
    }

    iterator begin() {
        return iterator(head.next);
    }

    iterator end() {
        return iterator(&head);
    }

    const_iterator begin() const {
        return const_iterator(head.next);
    }

    const_iterator end() const {
        return const_iterator(const_cast<node*>(&head));
    }

    const_iterator cbegin() const {
        return begin();
    }

    const_iterator cend() const {
        return end();
    }

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(cend());
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(cbegin());
    }

    const_reverse_iterator rcbegin() const {
        return rbegin();
    }

    const_reverse_iterator rcend() const {
        return rend();
    }

    friend void swap(list& a, list& b) {
        a.head.swap(b.head);
    }

private:
    node head;
};

#endif
