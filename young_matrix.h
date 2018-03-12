#ifndef YOUNG_MATRIX_H
#define YOUNG_MATRIX_H

#include <utility>

using od = std::pair<size_t,size_t>;

inline od up(od i){
    return od(i.first-1,i.second);
}
inline od down(od i){
    return od(i.first+1,i.second);
}
inline od left(od i){
    return od(i.first,i.second-1);
}
inline od right(od i){
    return od(i.first,i.second+1);
}


template<class T>
void fillb(T &t,od i,od max = od(0,0))
{
    auto r = right(i);
    auto d = down(i);
    //od largest = i;
    if(max == od(0,0))
        max = od(t.size(),t[0].size());
    if(d.first == max.first && r.second == max.second){
        t[i.first][i.second] = INT_MAX;
        return;
    }
    if(d.first == max.first || t[r.first][r.second]>t[d.first][d.second]){

        t[i.first][i.second] = t[r.first][r.second];
        fillb(t, r, max);
    }
    else {
        t[i.first][i.second] = t[d.first][d.second];
        fillb(t, d, max);
    }

    //show_heap(t);
}

template<class T>
typename T::value_type::value_type extract_min(T &t,od max = od(0,0)){
    auto ret = t[0][0];
    fillb(t,od(0,0),max);
    return ret;
}

template <class T>
void showYm(const T& t,od max=od(0,0)){
    for(auto x:t){
        for(auto y:x)
            std::cout << y << " ";
        std::cout << endl;
    }
}

#endif // YOUNG_MATRIX_H
