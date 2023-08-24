class range {
public:
    range(int begin, int end):m_begin(begin), m_end(end){}
    class iterator;
    iterator begin();
    iterator end();
private:
    int m_begin;
    int m_end;
};

class range::iterator {
public:
    iterator(int val):m_val(val) {}
    iterator& operator++()& {++m_val; return *this;}
    const iterator operator++(int)& {return iterator{m_val++};}
    bool operator!=(const iterator& t) {return m_val != t.m_val;}
    int operator*() {return m_val;}
private:
    int m_val;
};

range::iterator range::begin() {return range::iterator(m_begin);}
range::iterator range::end() {return range::iterator(m_end);}

#include<iostream>
int main() {
    for(int i : range(0, 10)) {
        std::cout << i << std::endl;
    }
}