#ifndef RADIXSORT_H
#define RADIXSORT_H

#include "countsort.h"
#include <iostream>
#include <utility>
#include <vector>
#include "ita1_4.h"

template <class T>
T ctsort(const T&t,unsigned max,size_t lg)
{
    unsigned rd = 1;
    for(size_t i = 1;i <= lg;++ i)
        rd *= max;
    unsigned dd = rd/max;
    std::vector<size_t> index(max+1,0);

    T ut = t;
    for(size_t i = 0;i != t.size();++ i)
        ut[i] = (t[i]%rd)/dd;
    std::cout << std::endl << "ut: " << std::endl; showvec(ut);

    for(size_t i = 0;i < t.size();++ i){
        ++ index[ut[i]];
    }
    for(size_t i = 1;i <= max;++ i){
        index[i] += index[i-1];
    }
    /*
    for(auto x:t)
        std::cout << x << " : " << index[x] << std::endl;*/
    T ret(t.size());
    for(size_t j = t.size();j != 0;-- j){
        ret[index[ut[j-1]]-1] = t[j-1];
        -- index[ut[j-1]];
    }
    std::cout << std::endl << "ret::" << std::endl;showvec(ret) ;
    return ret;
}

template <class T>
void radixsort(T &t,unsigned long base,size_t lg){
    for(int a = 1;a <= lg;++ a)
        t = ctsort(t,base,a);
}

#endif // RADIXSORT_H
