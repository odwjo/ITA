#ifndef HEAPSORT_H
#define HEAPSORT_H

#include <iostream>
#include <string>
#include <exception>
#include "ita1_4.h"

inline size_t parent(size_t i){
    return i/2;
}
inline size_t left(size_t i){
    return i*2;
}
inline size_t right(size_t i){
    return i*2+1;
}

template<class T>
size_t height_heap(const T &t){
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
void show_heap(const T &t){
    using std::cout;
    auto ht = height_heap(t);
    size_t i = 1;
    size_t ls = 1;
    size_t index = 0;
    for(;i != ht;++ i){
        std::string sep = std::string(2*(ht-i),' ');
        cout << sep;
        for(size_t j = 0;j < ls;++ j)
            cout << t[index++] << sep;
        cout << std::endl;
        ls *= 2;
    }
    for(;index != t.size();++ index)
        cout << t[index] << " ";
    cout << std::endl;
}

template<class T>
void max_heapify(T &t,size_t i,size_t heap_size = 0)
{
    auto l = left(i);
    auto r = right(i);
    size_t largest;
    if(heap_size == 0)
        heap_size = t.size();
    if(l <= heap_size && t[l-1]>t[i-1])
        largest = l;
    else largest = i;
    if(r <= heap_size && t[r-1]>t[largest-1])
        largest = r;
    if (largest != i){
        std::swap(t[i-1], t[largest-1]);
        max_heapify(t, largest, heap_size);
    }

    //show_heap(t);
}

template<class T>
void max_heapify2(T &t,size_t i)
{
    size_t largest = i;
    do{
        i = largest;
        auto l = left(i);
        auto r = right(i);
        if(l <= t.size() && t[l-1]>t[i-1])
            largest = l;
        else largest = i;
        if(r <= t.size() && t[r-1]>t[largest-1])
            largest = r;
        if (largest != i){
            std::swap(t[i-1], t[largest-1]);
            //max_heapify(t, largest);
        }
    }while(largest != i);
}

template<class T>
void min_heapify(T &t,size_t i)
{
    auto l = left(i);
    auto r = right(i);
    size_t smallest;
    if(l <= t.size() && t[l-1]<t[i-1])
        smallest = l;
    else smallest = i;
    if(r <= t.size() && t[r-1]<t[smallest-1])
        smallest = r;
    if (smallest != i){
        std::swap(t[i-1], t[smallest-1]);
        min_heapify(t, smallest);
    }

    //show_heap(t);
}

template<class T>
void build_minheap(T &t){
    for(size_t i = t.size()/2;i > 0;-- i){
        min_heapify(t, i);
        //show_heap(t);
    }
}

template<class T>
void build_maxheap(T &t){
    for(size_t i = t.size()/2;i > 0;-- i){
        max_heapify(t, i);
        //show_heap(t);
    }
}

template<class T>
T heapsort2(T &t){
    T ret;
    build_minheap(t);
    for(size_t i = t.size();i != 1;-- i){
        ret.push_back(t[0]);
        std::swap(t[0], t[t.size() - 1]);
        t.erase(t.end()-1);
        min_heapify(t, 1);
    }
    return ret;
}

template<class T>
void heapsort(T &t){
    //T ret;
    build_maxheap(t);
    //size_t heapsize = t.size();
    for(size_t i = t.size();i != 1;-- i){
        std::swap(t[0], t[i-1]);            //must use the last to deduce the
                                                //biggest
        max_heapify(t,1,i-1);
    }
    //return ret;
}

template <class T>
auto heap_maximum(const T &t)->typename T::value_type{
    return t[0];
}

template <class T>
auto heap_max_extract(T &t)->typename T::value_type{
    if(t.size() < 1)
        throw std::runtime_error("empty heap!");
    auto max = t[0];
    t[0] = t[t.size()-1];
    t.pop_back();
    max_heapify(t, 1);
    return max;
}

template <class T>
void heap_increase_key(T &t,size_t i,typename T::value_type x){
    if(x < t[i])
        throw std::runtime_error("Insert too big element.");
    t[i] = x;
    while(i > 0 && t[parent(i)] < t[i])
    {
        std::swap(t[parent(i)], t[i]);
        i = parent(i);
    }
}

template <class T>
void max_heap_insert(T &t,typename T::value_type x){
    t.push_back(-1);
    heap_increase_key(t, t.size()-1, x);
}

template <class T>
T build_maxheap2(const T&t){
    T ret = {t[0]};
    for(size_t i = 1;i != t.size();++ i){
        max_heap_insert(ret,t[i]);
    }
    return ret;
}

template <class T>
void heap_increase_key_pro(T &t,size_t i,typename T::value_type x){
    if(x < t[i]){
        auto pos = i;
        while(t[i] > x && i < t.size()-1)
            ++ i;
        size_t j;
        for(j = pos;j != i;++ j)
            t[j] = t[j + 1];
        t[j] = x;
    }
    else{
        t[i] = x;
        while(i > 0 && t[parent(i)] < t[i])
        {
            std::swap(t[parent(i)], t[i]);
            i = parent(i);
        }
    }
}

template <class T>
typename T::value_type heapdelete(T &t, size_t i){
    auto ret = t[i];
    t[i-1] = t[t.size()-1];
    t.pop_back();
    max_heapify(t, i);
    return ret;
}



#endif // HEAPSORT_H
