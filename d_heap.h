#ifndef D_HEAP_H
#define D_HEAP_H

#include <vector>
#include <iostream>
#include <exception>
#include "ita1_4.h"

inline size_t dparent(size_t i, size_t D = 3){
    return (i+D/2)/D;
}
inline std::vector<size_t> dchildren(size_t x, size_t D = 3){
    std::vector<size_t> ret;
    for(int i = 2-D;i != 2;++ i)
        ret.push_back(i+x*D);
    return ret;
}

template<class T>
size_t dheight_heap(const T &t,size_t D = 3){
    size_t ret, sz;
    ret = 1;
    sz = t.size();
    size_t temp = 1;
    size_t plus = 1;
    while(sz > temp)
    {
        ret += 1;
        temp += plus*D;
        plus *= D;
    }

    //ret += 1;
    return ret;
}


template <class T>
void dshow_heap(const T &t, size_t D = 3){
    using std::cout;
    auto ht = dheight_heap(t,D);
    size_t i = 1;
    size_t ls = 1;
    size_t index = 0;
    for(;i != ht;++ i){
        std::string sep = std::string(D*(ht-i),' ');
        cout << sep;
        for(size_t j = 0;j < ls;++ j)
            cout << t[index++] << sep;
        cout << std::endl;
        ls *= D;
    }
    for(;index != t.size();++ index)
        cout << t[index] << " ";
    cout << std::endl;
}

template<class T>
void dmax_heapify(T &t,size_t i,
                 size_t heap_size = 0,size_t D = 3)
{
    auto cd = dchildren(i,D);
    //std::cout << i << " : ";show(cd);std::cout << std::endl;
    size_t largest = i;
    typename T::value_type max = t[i-1];
    if(heap_size == 0)
        heap_size = t.size();
    for(size_t k = 0;k != cd.size();++ k)
        if(cd[k] <= heap_size && t[cd[k]-1] > max){
            largest = cd[k];
            max = t[cd[k]-1];
        }

    if (largest != i){
        std::swap(t[i-1], t[largest-1]);
        dmax_heapify(t, largest, heap_size, D);
    }

    //show_heap(t);
}

template<class T>
void build_dheap(T &t, size_t D = 3){
    auto last = dparent(t.size(),D);
    for(;last != 0;-- last){
        dmax_heapify(t,last,t.size(),D);
        //dshow_heap(t);
    }
}

template<class T>
typename T::value_type extract_max(T &t,size_t D = 3){
    auto max = t[0];
    t[0] = t.pop_back();
    if(t.size() > 1)
        dmax_heapify(t,1,t.size(),D);
    return max;
}

template <class T>
void increase_key(T &t,size_t i,typename T::value_type k,size_t D = 3){
    if(k < t[i-1])
        throw std::runtime_error("Inset too little.");
    t[i-1] = k;
    while(i != 1 && t[dparent(i)-1] < k){
        std::swap(t[i-1], t[dparent(i)-1]);
        i = dparent(i);
    }
}

template <class T>
void insert(T &t,typename T::value_type k,size_t D = 3){
    t.push_back(INT_MIN);
    increase_key(t,t.size(),k,D);
}


#endif // D_HEAP_H
