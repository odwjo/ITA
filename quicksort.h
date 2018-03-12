#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <random>
#include <ctime>
#include <vector>
#include "ita1_4.h"
#include <utility>
#include <iostream>
#include "ita9.h"

template <class T>
size_t partition(T &t, size_t p,size_t r){
    auto x = t[r-1];
    int i = p-1;
    for(int j = p;j < r-1;++ j){
        if(t[j] <= x){
            i += 1;
            std::swap(t[i], t[j]);
        }
    }
    std::swap(t[++i], t[r-1]);
    return i;
}

template<typename T>
T rdm(T b,T e){
    std::default_random_engine en(std::time(0));
    std::uniform_int_distribution<unsigned long> u(b,e);
    return u(en);
}

template <class T>
size_t random_partition(T &t, size_t p,size_t r){
    if(r-p > 2)
        std::swap(t[r-1], t[rdm(p,r-2)]);

    auto x = t[r-1];
    int i = p-1;
    for(int j = p;j < r-1;++ j){
        if(t[j] <= x){
            i += 1;
            std::swap(t[i], t[j]);
        }
    }
    std::swap(t[++i], t[r-1]);
    return i;
}

template <class T>
void random_quicksort(T &t, size_t b,size_t e){
    if(e > b){
        auto q = random_partition(t,b,e);
        random_quicksort(t,b,q);
        random_quicksort(t,q+1,e);
    }
}

template <class T>
void i_quicksort(T &t, size_t b,size_t e){
    if(e > b+3){
        auto q = partition(t,b,e);
        i_quicksort(t,b,q);
        i_quicksort(t,q+1,e);
    }
    else if(e > b){
        insertion_sort(t,b,e);
    }
}

template <class T>
void quicksort(T &t, size_t b,size_t e){
    if(e > b){
        auto q = partition(t,b,e);
        quicksort(t,b,q);
        quicksort(t,q+1,e);
    }
}

template <class T>
size_t partition2(T &t, size_t p,size_t r){   //()
    auto x = t[r-1];
    size_t i = p-1;
    for(int j = p;j < r;++ j){
        if(t[j-1] <= x){
            i += 1;
            std::swap(t[i-1], t[j-1]);
        }
    }
    std::swap(t[i], t[r-1]);
    return i+1;
}

template <class T>
void quicksort2(T &t, size_t b,size_t e){
    if(e > b){
        auto q = partition(t,b,e);
        quicksort(t,b,q-1);
        quicksort(t,q+1,e);
    }
}



using Vi = std::vector<int>;


void g_partition(Vi &t,const size_t p,const size_t r,size_t &b,size_t &e){   //[p, r)
    auto x = t[r-1];
    int i = p-1;
    int k = r-1;
    //showvec(t);
    for(int j = p;j <= k;++ j){
        //showvec(t);
        if(t[j] < x){
            ++ i;
            std::swap(t[i], t[j]);
        }
        if(t[j] > x){
            std::swap(t[k], t[j]);
            -- k;
            -- j;
        }
        //std::cout <<std::endl <<  i << " : " << j << " : " << k << std::endl;
    }
    //std::swap(t[++i], t[r-1]);
    //return bound(size_t(i+1), size_t(k+1));
    b = i+1;
    e = k+1;

}


void g_quicksort(Vi &t,const size_t b,const size_t e){
    if(e > b+1){
        size_t f,s;
        g_partition(t,b,e,f,s);

        //showvec(t);
        //std::cout << std::endl << f << " : " << s << std::endl;
        g_quicksort(t,b,f);
        g_quicksort(t,s,e);
    }
}

//use ita9 to pick median to do partition
void sg_partition(Vi &t,const size_t p,const size_t r,size_t &b,size_t &e){   //[p, r)
    auto x = select(t,p,r,(r-p)/2);
    t[(r-p)/2-1] = t[r-1];
    t[r-1] = x;
    int i = p-1;
    int k = r-1;
    //showvec(t);
    for(int j = p;j <= k;++ j){
        //showvec(t);
        if(t[j] < x){
            ++ i;
            std::swap(t[i], t[j]);
        }
        if(t[j] > x){
            std::swap(t[k], t[j]);
            -- k;
            -- j;
        }

       // std::cout <<std::endl <<  i << " : " << j << " : " << k << std::endl;
    }
    //std::swap(t[++i], t[r-1]);
    //return bound(size_t(i+1), size_t(k+1));
    b = i+1;
    e = k+1;

}


void sg_quicksort(Vi &t,const size_t b,const size_t e){
    if(e > b+1){
        size_t f,s;
        sg_partition(t,b,e,f,s);

        //showvec(t);
        //std::cout << std::endl << f << " : " << s << std::endl;
        g_quicksort(t,b,f);
        g_quicksort(t,s,e);
    }
}

#endif // QUICKSORT_H
