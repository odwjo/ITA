#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <memory>
#include <iostream>
#include <cmath>
#include "datastruct.h"
#include <utility>
#include <functional>
#include <climits>
#include <iomanip>
#include <random>
#include <ctime>

template<typename T1,typename T2>
T2 rand(T1 b,T2 e){
    std::default_random_engine en(std::time(0));
    std::uniform_int_distribution<unsigned long> u(b,e);
    return u(en);
}

template <typename T,unsigned N=100>
struct direct_address_map{
    T m[N];
    T nil;
    direct_address_map(const T &n){
        nil = n;
        for(size_t i = 0;i != N;i ++)
            m[i] = nil;
    }

    T& direct_address_search(size_t k){
        return m[k];
    }

    bool direct_address_insert(const T &t,size_t i){
        if(m[i] != nil)
            return false;
        m[i] = t;
        return true;
    }

    bool direct_address_delete(size_t i){
        if(m[i] == nil)
            return false;
        else m[i] = nil;
        return true;
    }
};



template <typename K, typename T, size_t (*h)(const K &k),unsigned N = 100>
struct chain_hash_table{
    linklist<K> kys;
    linklist<T> hashtable[N];
    void chain_hash_insert(const K &key,const T&t){
        kys.Insert(key);
        hashtable[h(key)].Insert(t);
    }

    typename linklist<T>::Node* chain_hash_search(const K &key,const T&t){
        return hashtable[h(key)].Search(t);
    }

    bool chain_hash_delete(const K &key,const T &t){
        if(hashtable[h(key)].size == 1 && hashtable[h(key)].head->key == t)
            kys.Delete(key);
        return hashtable[h(key)].Delete(t);
    }
    void show(){
        while(!kys.isempty()){
            std::cout << kys.back() << " --- " ;
            hashtable[h(kys.pop_back())].show(std::cout);
        }
    }
};

size_t h(const size_t &t){
    return t;
}
size_t devid_h(const size_t &k,const size_t m){
    //const size_t s = 233; //should not be closed to 2^p
    return k%m;
}
size_t multi_h(const size_t &k,unsigned long p = 14){
    double A = (sqrt(5.0)-1)/2.;     //A = s/2^w, w is the wordsize,choosing s
                                    //better close to 0.618033 <- (5^0.5-1)/2
    unsigned int w = 32;
    unsigned long m = 1<<p;
    unsigned long s = A*(1<<(w-1));
    return m*((double)(s*k)/(1<<(w-1)));
}
size_t universal_h(size_t k,size_t p,size_t m,size_t a=0,size_t b=0){
    if(a == 0)
        size_t a = rand(1,p-1);
    if(b == 0)
        size_t b = rand(0,p-1);

    return ((a*k+b)%p)%m;
}


template <typename T>
size_t linear_probing(const size_t &k,size_t i,size_t m,T h){
    return (h(k) + i)%m;
}
template <typename T>
size_t quadratic_probing(const size_t &k,size_t i,size_t m,T h){
    return (h(k) + i + i*i)%m;
}
template <typename T>
size_t double_hash(const size_t &k,size_t i, T h1,//m is a power of 2 or prime,m` silghtly
                   T h2,size_t m){                  //smaller than m,h2 prduce odd
    return (h1(k) + i*h2(k))%m;
}

template<unsigned long N>
struct openaddress{
    size_t m;
    size_t kys[N];
    const size_t nil = ULONG_MAX;
    const size_t dlt = ULONG_MAX-1;
    openaddress():m(N){
        for(size_t i = 0;i != N;++ i)
            kys[i] = nil;
    }

    template<typename T>
    size_t hash_insert(size_t k, T h){
        size_t i = 0;
        while(i != m){
            auto j = h(k, i);
            if(kys[j] == nil || kys[j] == dlt){
                kys[j] = k;
                return j;
            }
            ++ i;
        }
        return nil;
    }

    template<typename T>
    size_t hash_search(size_t k,T h){
        size_t i = 0;
        size_t j = 0;
        while(i != m && kys[j != nil]){
            j = h(k,i);
            if(kys[j] == k)
                return j;
            ++ i;
        }
        return nil;
    }
    template<typename T>
    size_t hash_delete(size_t k,T h){
        size_t i = 0;
        size_t ret = nil;
        while(i != m){
            auto j = hash_search(k,h);
            if(j != nil){
                ret = j;
                kys[j] = dlt;
                break;
            }
            ++ i;
        }
        return ret;
    }

    void show(){
        std::cout << "------^_^------\n";
        for(size_t i = 0;i != N;++ i){
            std::cout << std::setw(3) << i << " :" << std::setw(16)
                      << kys[i]<< std::endl;
        }
    }
};

void test_hashtable(){
    chain_hash_table<size_t,char,*h,10> ht;
    for(size_t i = 0;i != 5;++ i)
        ht.chain_hash_insert(i,'D');
    ht.show();
}
void test_double_hash(){
    using namespace std::placeholders;
    using std::bind;

    for(size_t i = 0;i != 15;++ i){
        std::cout << double_hash(i,i,bind(devid_h,_1,15),
                                 bind(devid_h,_1,14),15) << std::endl;
    }
}

void test_openaddress(){
    using namespace std::placeholders;
    using std::bind;
    openaddress<15> o[3];
    auto z1 = bind(devid_h,_1,7);
    auto z2 = bind(devid_h,_1,6);
    //auto h1 = bind(linear_probing<decltype(z1)>,_1,_2,7,z1);
    auto l1 = [z1](size_t k,size_t i)->size_t{return (z1(k) + i)%15;};
    //auto h2 = bind(quadratic_probing<decltype(z1)>,_1,_2,7,z1);
    auto l2 = [z1](size_t k,size_t i)->size_t{return (z1(k) + i + i*i)%15;};
    //auto h3 = bind(double_hash<decltype(z1)>,_1,_2,z1,z2,7);
    auto l3 = [z1,z2](size_t k,size_t i)->size_t{return (z1(k) + i*z2(k))%15;};

    for(size_t i = 0;i != 15;++ i){
        o[0].hash_insert(i,l1);
        o[1].hash_insert(i,l2);
        o[2].hash_insert(i,l3);
    }
    o[0].hash_delete(7,l1);o[0].hash_delete(9,l1);
    o[1].hash_delete(7,l2);o[1].hash_delete(9,l2);
    o[2].hash_delete(7,l3);o[2].hash_delete(9,l3);

    for(size_t i = 0;i != 3;++ i)
        o[i].show();
}

#endif // HASHTABLE_H
