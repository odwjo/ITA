#ifndef K_MERGE_H
#define K_MERGE_H

#include <iostream>
#include <string>
#include <exception>
#include "ita1_4.h"

inline size_t kparent(size_t i){
    return i/2;
}
inline size_t kleft(size_t i){
    return i*2;
}
inline size_t kright(size_t i){
    return i*2+1;
}

template<class T>
size_t kheight_heap(const T &t){
    size_t ret, sz;
    ret = 0;
    sz = t.size();
    while(sz != 0)
    {
        ret += 1;
        sz /= 2;
    }
    return ret;
}

template <class T>
void kshow_heap(const T &t){
    using std::cout;
    auto ht = kheight_heap(t);
    size_t i = 1;
    size_t ls = 1;
    size_t index = 0;
    for(;i != ht;++ i){
        std::string sep = std::string(2*(ht-i),' ');
        cout << sep;
        for(size_t j = 0;j < ls;++ j)
            cout << t[index++][0] << sep;
        cout << std::endl;
        ls *= 2;
    }
    for(;index != t.size();++ index)
        cout << t[index][0] << " ";
    cout << std::endl;
}

template<class T>
void kmin_heapify(T &t,size_t i)
{
    auto l = kleft(i);
    auto r = kright(i);
    size_t smallest;
    if(l <= t.size() && t[l-1][0]<t[i-1][0])
        smallest = l;
    else smallest = i;
    if(r <= t.size() && t[r-1][0]<t[smallest-1][0])
        smallest = r;
    if (smallest != i){
        std::swap(t[i-1], t[smallest-1]);
        kmin_heapify(t, smallest);
    }

    //kshow_heap(t);
}

template<class T>
void kbuild_minheap(T &t){
    for(size_t i = t.size()/2;i > 0;-- i){
        kmin_heapify(t, i);
    }
    //kshow_heap(t);
}

template <class T>
auto kheap_min_extract(T &t)->typename T::value_type::value_type{
    if(t.size() < 1)
        throw std::runtime_error("empty heap!");
    auto min = t[0][0];
    t[0].erase(t[0].begin());
    if(t[0].size() < 1)
        t[0].push_back(INT_MAX);
    kmin_heapify(t, 1);
    return min;
}

template <class T>
typename T::value_type K_merge(T &t){
    typename T::value_type ret;
    size_t sum_size = 0;
    for(auto x:t){
        sum_size += x.size();
    }
    kbuild_minheap(t);    //std::cout << "build completed...\n";
    for(auto i = 0;i != sum_size;++ i)
        ret.push_back(kheap_min_extract(t));
    return ret;
}

#endif // K_MERGE_H
