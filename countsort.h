#ifndef COUNTSORT_H
#define COUNTSORT_H

#include <vector>
//#include <iostream>

template <class T>
T countsort(const T&t,unsigned max){
    std::vector<size_t> index(max+1,0);
    for(size_t i = 0;i < t.size();++ i){
        ++ index[t[i]];
    }
    for(size_t i = 1;i <= max;++ i){
        index[i] += index[i-1];
    }
    /*
    for(auto x:t)
        std::cout << x << " : " << index[x] << std::endl;*/
    T ret(t.size());
    for(size_t j = t.size();j != 0;-- j){
        ret[index[t[j-1]]-1] = t[j-1];
        -- index[t[j-1]];
    }
    return ret;
}

#endif // COUNTSORT_H
