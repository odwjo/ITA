#ifndef BUCKETSORT_H
#define BUCKETSORT_H

#include "ita1_4.h"
#include <vector>
#include <functional>
#include <iterator>
#include <algorithm>

template <class T>
T bucketsort(T &t,size_t nb,std::function<size_t(typename T::value_type)> f){ //t[i] ~ (0, 1)
    T ret;
    std::vector<T> bks(nb);
    for(size_t i = 0;i < t.size();++ i){
        bks[f(t[i])].push_back(t[i]);
    }
    for(auto x:bks){
        insertion_sort(x,0,x.size());
        std::copy(x.begin(),x.end(),std::back_inserter(ret));
    }
    return ret;
}

#endif // BUCKETSORT_H
