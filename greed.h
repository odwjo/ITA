#ifndef GREED_H
#define GREED_H

#include <vector>
#include <algorithm>
#include <utility>
#include <memory>
#include "dynamic_pro.h"

struct event{
    unsigned b,e;
    event(unsigned bg,unsigned ed):b(bg),e(ed){}
};

bool notoverlap(const event &e1, const event &e2){
    return (e1.e <= e2.b || e1.b >= e2.e);
}

void sort_event(std::vector<event> &ve){
    std::sort(ve.begin(),ve.end(),
              [](const event &e1,
              const event &e2)
              {return e1.e < e2.e;});
}

std::pair<matrix,matrix>    ///use dyamic programing:}
mste(std::vector<event> &ve){
    sort_event(ve);
    matrix c(ve.size(),ve.size());
    matrix pd(ve.size(),ve.size());
    size_t n = ve.size();

    for(size_t i = 0;i != n-1;++ i)
        if(notoverlap(ve[i],ve[i+1])){
            //c.x[i][i+1]  = 2;
            pd.x[i][i+1] = i+1;
        }else{
            pd.x[i][i+1] = i;
            //c.x[i][i+1]  = 1;
        }
    for(size_t i = 0;i != n;++ i){
        pd.x[i][i] = i;
        c.x[i][i]  = 0;
    }

    for(size_t l = 1;l != n;++ l)
        for(size_t i = 0;i != n-l;++ i){
            size_t j = i+l;
            if(notoverlap(ve[i],ve[j])){
                size_t temp = 1;
                size_t temp2 = i;
                for(size_t k = i+1;k != j;++ k){
                    size_t t = c.x[i][k] + c.x[k][j];
                    if(notoverlap(ve[k-1],ve[k]) &&
                        notoverlap(ve[k+1],ve[k]))
                        ++ t;
                    if(t > temp){
                        temp = t;
                        temp2 = k;
                    }
                }
                c.x[i][j] = temp;
                pd.x[i][j] = temp2;
            }else{
                c.x[i][j] = 0;
                pd.x[i][j] = i;
            }
        }
    return make_pair(c,pd);
}

void print_event(const matrix &pd,size_t b,size_t e){
    size_t k = pd.x[b][e];

    if(k != b){
        print_event(pd, b, k);
        print_event(pd, k, e);
    }else
        std::cout << k+1 << ", ";
}

void test_dyna_event(){
    std::vector<size_t> vib = {1,3,0,5,3,5,6 ,8 ,8 ,2 ,12};
    std::vector<size_t> vie = {4,5,6,7,9,9,10,11,12,14,16};
    std::vector<event> ve;
    for(size_t i = 0;i != vib.size();++ i){
        ve.push_back(event(vib[i],vie[i]));
    }
    auto rt = mste(ve);
    show_matrix(rt.first);
    show_matrix(rt.second);
    std::cout << std::endl;
    print_event(rt.second,0,vib.size()-1);
}

////////////////////greed selector
void    ///use recursive greed method:}
recur_act_selector(const std::vector<event> &ve,
                   size_t k,size_t n,
                   std::vector<size_t> &ret){
    size_t m = k+1;
    while(m <= n && ve[m].b < ve[k].e){
        ++ m;
    }
    if(m <= n){
        ret.push_back(m);
        recur_act_selector(ve,m,n,ret);
    }
}
///use recursive greed method:}
std::pair<std::vector<size_t>,std::vector<event>>
act_selector1(
        std::vector<size_t> vib,
        std::vector<size_t> vie){
    std::vector<event> ve;
    for(size_t i = 0;i != vib.size();++ i){
        ve.push_back(event(vib[i],vie[i]));
    }
    sort_event(ve);
    std::vector<size_t> vi;
    vi.push_back(0);
    recur_act_selector(ve,0,vib.size()-1,vi);
    return make_pair(vi,ve);
}
///use no recur,by ealiest ended act
std::pair<std::vector<size_t>,std::vector<event>>
act_selector2(
        std::vector<size_t> vib,
        std::vector<size_t> vie){
    std::vector<event> ve;
    for(size_t i = 0;i != vib.size();++ i){
        ve.push_back(event(vib[i],vie[i]));
    }
    sort_event(ve);

    std::vector<size_t> ret;
    size_t b = 0;
    size_t n = vib.size();
    while(b != n){
        size_t temp = ve[b].e;
        ret.push_back(b++);
        while(b != n && ve[b].b < temp)
            ++ b;
    }
    return make_pair(ret,ve);
}
///use no recur,by latest begined act
std::pair<std::vector<size_t>,std::vector<event>>
act_selector3(
        std::vector<size_t> vib,
        std::vector<size_t> vie){
    std::vector<event> ve;
    for(size_t i = 0;i != vib.size();++ i){
        ve.push_back(event(vib[i],vie[i]));
    }
    std::sort(ve.begin(),ve.end(),
              [](const event &e1,const event &e2)
                {return e1.b > e2.b;});

    std::vector<size_t> ret;
    size_t b = 0;
    size_t n = vib.size();
    while(b != n){
        size_t temp = ve[b].b;
        ret.push_back(b++);
        while(b != n && ve[b].e > temp)
            ++ b;
    }
    return make_pair(ret,ve);
}

void test_greed_selector(){
    std::vector<size_t> vib = {1,3,0,5,3,5,6 ,8 ,8 ,2 ,12};
    std::vector<size_t> vie = {4,5,6,7,9,9,10,11,12,14,16};
    auto x = act_selector3(vib,vie);
    for(auto y:x.first){
        std::cout << y+1<< "(" << x.second[y].b << ","
                  << x.second[y].e << "), ";
    }
    std::cout << std::endl;
}

///////////////classroom choose
void classroom_choose(
        std::vector<size_t> vib,
        std::vector<size_t> vie){
    std::vector<event> ve;
    for(size_t i = 0;i != vib.size();++ i){
        ve.push_back(event(vib[i],vie[i]));
    }
    sort_event(ve);

    size_t i = 0;
    size_t n = vib.size();
    do{
        size_t b = 0;
        std::vector<size_t> ret;
        std::vector<event> bk;
        while(b != n){
            size_t temp = ve[b].e;
            ret.push_back(b++);
            while(b != n && ve[b].b < temp){
                bk.push_back(ve[b]);
                ++ b;
            }
        }
        std::cout << "No." << i+1 << "classroom.......\n";
        for(auto y:ret){
            std::cout << y+1<< "(" << ve[y].b << ","
                      << ve[y].e << "), ";
        }
        std::cout << std::endl;

        ++ i;
        ve = bk;
        n = ve.size();
    }while(n != 0);

    //return make_pair(ret,ve);
}

void test_classroom_choose(){
    std::vector<size_t> vib = {1,3,0,5,3,5,6 ,8 ,8 ,2 ,12};
    std::vector<size_t> vie = {4,5,6,7,9,9,10,11,12,14,16};
    classroom_choose(vib,vie);
    std::cout << std::endl;
}

/////////////////// 0-1 bag
struct gds{
    unsigned weight;
    unsigned value;
    gds(unsigned w,unsigned v):weight(w),value(v){}
};

std::pair<matrix,matrix>
knapsack_problem(std::vector<gds> gs,unsigned bag_size){
    size_t n = gs.size();
    //matrix vs(n,n),ws(n,n),ms(n,n);
    matrix m(n+1,bag_size+1);
    matrix mk(n+1,bag_size+1);

    for(size_t i = 0;i <= bag_size;++ i){
        m.x[0][i] = 0;
    }

    for(size_t i = 1;i <= n;++ i)
        for(size_t j = 0;j <= bag_size;++ j){
            if(gs[i-1].weight > j)
                m.x[i][j] = m.x[i-1][j];
            else{                                    //j >= gs[i-1].weight
                //put ith in or not
                m.x[i][j] = max(m.x[i-1][j], m.x[i-1][j-gs[i-1].weight] + gs[i-1].value);
                if(m.x[i][j] == (m.x[i-1][j-gs[i-1].weight] + gs[i-1].value))
                    mk.x[i][j] = 1;

            }
        }


    return make_pair(m,mk);
}

void print_knapsack(const std::vector<gds> &bg,size_t W,
                    const matrix &mk,const matrix &w){
    size_t i = bg.size();
    std::cout << "Sum value: " << w.x[i][W] << std::endl;
    while(i != 0 && W != 0){
        if(mk.x[i][W] == 1){
            std::cout << "w: " << bg[i-1].weight << ", " <<
                         "v: " << bg[i-1].value  << std::endl;
            W -= bg[i-1].weight;
        }
        -- i;
    }
}

void test_knapsack_problem(){
    std::vector<unsigned> vs = {60,120,120,100};
    std::vector<unsigned> ws = {10,40 ,30 ,20 };
    const size_t bgsz = 70;

    std::vector<gds> bg;
    for(size_t i = 0;i != vs.size();++ i)
        bg.push_back(gds(ws[i], vs[i]));

    auto x = knapsack_problem(bg, bgsz);

    print_knapsack(bg,bgsz,x.second,x.first);
    //show_matrix(x.first);
    //show_matrix(x.second);
    //std::cout << std::endl;
}

////////////////////huffman code
struct charac{
    char c;
    unsigned freq;
    std::shared_ptr<charac> left,right;
    charac():c('^'),freq(0){}
    charac(char chr,unsigned f):c(chr),freq(f){}
    charac(const charac &cp):c(cp.c),freq(cp.freq),
        left(cp.left),right(cp.right){}
    ~charac(){}
};


inline size_t mleft(size_t i){
    return i*2;
}
inline size_t mright(size_t i){
    return i*2+1;
}

void min_heapify(std::vector<charac> &t,size_t i,size_t heap_size=0){
    size_t l = mleft(i);
    size_t r = mright(i);
    size_t smallest;
    if(heap_size == 0)
        heap_size = t.size();
    if(l <= heap_size && t[l-1].freq<t[i-1].freq)
        smallest = l;
    else smallest = i;
    if(r <= heap_size && t[r-1].freq<t[smallest-1].freq)
        smallest = r;
    if (smallest != i){
        std::swap(t[i-1], t[smallest-1]);
        min_heapify(t, smallest, heap_size);
    }
}

void build_minheap(std::vector<charac> &t){
    for(size_t i = t.size()/2;i > 0;-- i){
        min_heapify(t, i);
    }
}

void heap_insert(std::vector<charac> &t,const charac &c){
    t.push_back(c);
    build_minheap(t);
}

charac minheap_extract(std::vector<charac> &t){
    auto ret = t[0];
    t[0] = t.back();
    t.pop_back();
    min_heapify(t,1);
    return ret;
}

charac minimum(charac A,charac B,charac C){
    return (A.freq>B.freq) ? (C.freq>B.freq ? B:C)
                           : (A.freq>C.freq ? C:A);
}


charac huffman(std::vector<charac> &t){
    size_t n = t.size();
    build_minheap(t);
    for(size_t i = 1;i != n;++ i){
        auto x = minheap_extract(t);
        auto y = minheap_extract(t);
        charac z('*',x.freq + y.freq);
        shared_ptr<charac> sx(new charac(x));
        shared_ptr<charac> sy(new charac(y));
        z.left = sx;
        z.right = sy;
        heap_insert(t,z);
    }
    std::cout << t.size() << std::endl;
    return minheap_extract(t);
}

void print_huffman(charac c){
    std::cout << c.c << "$ " << std::endl;
    if(c.left)
        print_huffman(*(c.left));
    if(c.right)
        print_huffman(*(c.right));
}

void test_huffman(){
    std::vector<char> chs =    {'a','b','c','d','e','f','g','h'};
    std::vector<unsigned> fs = {  1,  1,  2,  3, 5 ,  8, 13, 21};
    std::vector<charac> cs;
    for(size_t i = 0;i != chs.size();++ i)
        cs.push_back(charac(chs[i],fs[i]));
    auto root = huffman(cs);
    auto temp = root;

    //print_huffman(temp);
    std::vector<string> strstack;
    std::vector<charac> chrstack;

    do{
        charac x(*(temp.left));
        charac y(*(temp.right));
        std::string ts;
        if(strstack.size() != 0){
            ts = strstack.back();
        }
        if(y.c != '*'){
            std::cout << y.c << " : ";
            std::cout << ts+"1" << std::endl;
            strstack.pop_back();
        }else{
            strstack.push_back(ts+"1");
            chrstack.push_back(y);
        }
        if(x.c != '*'){
            std::cout << x.c << " : ";
            std::cout << ts+"0" << std::endl;
        }else{
            strstack.push_back(ts+"0");
            chrstack.push_back(x);
        }

        if(chrstack.empty())
            break;
        temp = chrstack.back();
        //std::cout << temp.c << " : " << temp.freq << std::endl;
        chrstack.pop_back();
    }while(true);
}

/////////////////////// matroid
struct task{
    unsigned d;
    unsigned w;
    task():d(0),w(0){}
    task(unsigned dline, unsigned weight):
        d(dline),w(weight){}
    task(const task &t):d(t.d),w(t.w){}
    task& operator=(const task &t){
        d = t.d;
        w = t.w;
        return *this;
    }
};
/////////// Nt(A) <= t (t = 0,1,2,3,...)
bool independent(std::vector<task> &t,const task &e){
    if(t.size() > 1){
        auto temp = t.back();
        size_t i = t.size()-1;
        while(i != 0 && t[i-1].d > temp.d){
            t[i] = t[i-1];
            -- i;
        }
        t[i] = temp;
    }

    //std::sort(t.begin(),t.end(),
    //          [](const task &e1,const task &e2)
    //            {return e1.d < e2.d;});
    unsigned last;
    if(!t.empty())
        last = t.back().d;
    else
        last = 0;
    last = last>(t.size()+1) ? last:(t.size() +1);

    unsigned dd;
    if(!t.empty())
        dd = (t.back().d>e.d) ? t.back().d : e.d;
    else
        dd = e.d;

    if(last <= dd)
        return true;
    return false;
}


std::vector<task> matroid_greed(std::vector<task> &t){
    std::vector<task> temp;
    //using et = T::value_type;
    std::sort(t.begin(),t.end(),
              [](const task &e1,const task &e2)
                {return e1.w > e2.w;});
    for(auto &x:t)
        if(independent(temp,x))
            temp.push_back(x);
    return temp;
}

void test_matroid(){
    std::vector<unsigned> deadline = {4,2,4,3,1,4,6};
    std::vector<unsigned> weight   = {7,6,5,4,3,2,1};
    std::vector<task> vt;
    for(size_t i = 0;i != deadline.size();++ i)
        vt.push_back(task(deadline[i],weight[i]));
    auto y = matroid_greed(vt);
    for(auto x:y)
        std::cout << x.d << " | " << x.w << std::endl;

}

////////////////////change problem
std::vector<unsigned> change_problem_greed(std::vector<unsigned> &ms,
                                     unsigned sum){
    std::sort(ms.rbegin(),ms.rend());
    std::vector<unsigned> vs(ms.size(),0);
    int temp = sum;
    while(temp > 0){
        for(size_t i = 0;i != ms.size();++ i){
            if(temp >= ms[i]){
                ++ vs[i];
                temp -= ms[i];
                break;
            }
        }
    }
    if(temp != 0)
        std::cout << "Can\'t be changed.\n";
    return vs;
}

std::pair<matrix,matrix>
change_problem_dynamic_false(std::vector<unsigned> &ms,
                                     unsigned sum){
    std::sort(ms.begin(),ms.end());
    if(ms[0] != 1)
        std::cout << "Error, no 1 unit money.\n";
    matrix m(ms.size(),sum+1);
    matrix mk(ms.size(),sum);

    for(size_t i = 0;i != ms.size();++ i){
        m.x[i][0] = 0;
        mk.x[i][0] = 0;
    }

    for(size_t j = 1;j <= sum;++ j)
        for(size_t i = 0;i != ms.size();++ i){
            if(ms[i] >= j){
                if(ms[i] == j){
                    m.x[i][j] = 1;
                    mk.x[i][j-1] = 1;
                }else{
                    if(i == 0){
                        m.x[i][j] = m.x[i][j-1]+1;
                        mk.x[i][j-1] = 2;
                    }
                    else{
                        m.x[i][j] = m.x[i-1][j];
                        mk.x[i][j-1] = 3;
                    }
                }
            }
            else{
                auto pre = m.x[i][j-1] + 1;
                auto cur = 1 + m.x[i][j-ms[i-1]];
                if(pre < cur){
                    mk.x[i][j-1] = 2;
                    m.x[i][j] = pre;
                }
                else{
                    m.x[i][j] = cur;
                    mk.x[i][j-1] = 1;
                }
            }
        }
    return make_pair(m,mk);
}


std::pair<std::vector<unsigned>,std::vector<unsigned>>
change_problem_dynamic(std::vector<unsigned> &ms,
                                     unsigned sum){
    std::sort(ms.begin(),ms.end());
    if(ms[0] != 1)
        std::cout << "Error, no 1 unit money.\n";
    std::vector<unsigned> uv(sum+1);
    std::vector<unsigned> uk(sum+1);
    uv[0] = 0;
    for(size_t j = 1;j <= sum;++ j){
        unsigned temp = UINT_MAX;
        for(size_t i = 0;i != ms.size();++ i){
            if(ms[i] > j)
                break;
            unsigned t = uv[j-ms[i]] + 1;
            if(t < temp){
                temp = t;
                uk[j] = i;
            }
        }
        uv[j] = temp;
    }
    //for(auto x :uv)
        //std::cout << x << " < ";
    //::cout << std::endl;
    return make_pair(uv,uk);
}

void print_change_dy(std::vector<unsigned> vm,std::vector<unsigned> &mk,unsigned sum){
    std::sort(vm.begin(),vm.end());

    //size_t i = vm.size()-1;
    int j = sum-1;
    std::vector<size_t> ct(vm.size(),0);

    while(j > 0){
        ++ ct[mk.back()];
        j -= vm[mk.back()];
    }
    for(size_t i = 0;i != vm.size();++ i)
        std::cout << std::setw(3) << vm[i] << ", ";
    std::cout << std::endl;
    for(size_t i = 0;i != vm.size();++ i)
        std::cout << std::setw(3) << ct[i] << ", ";
}

void test_change_problem(){
    std::vector<unsigned> vm = {6,4,1};
    const unsigned sum = 8;

    auto gr = change_problem_greed(vm,sum);
    auto dr = change_problem_dynamic(vm,sum);

    for(auto x:gr)
        std::cout << x << ", ";
    std::cout << std::endl;

    //show_matrix(dr.first);
    //show_matrix(dr.second);
    print_change_dy(vm,dr.second,sum);
    std::cout << std::endl;
}

////////////////////Schedule to average minimun completion time
struct ctask{
    unsigned r;
    unsigned p;
    unsigned s;
    ctask():r(0),p(0),s(0){}
    ctask(unsigned rel,unsigned per):r(rel),p(per){
        s = r + p;
    }
};

void vecs(const std::vector<size_t> &vi,size_t ii=0){
    for(size_t i =0;i != vi.size();++ i){
        std::cout << vi[i] << ", ";
    }
    std::cout << std::endl;
    for(size_t i =0;i != ii;++ i){
        std::cout << "   ";
    }
    std::cout << "^";
    std::cout << std::endl;
}

void xyx(const std::vector<ctask> &ts){
    for(auto x:ts)
        std::cout << "<" << x.r << "(" << x.p
                  << ")" << x.r+x.p << "> ";
    std::cout << std::endl;
}

std::vector<size_t>
schedule(std::vector<ctask> &ts){
    auto cts = ts;
    size_t n = ts.size();
    std::vector<size_t> ret;
    std::vector<size_t> vir(n,0),
            vis(n,0),vif(n,0);
    for(size_t i = 1;i != n;++ i){
        vir[i] = i;
        vis[i] = i;
    }

    std::sort(vir.begin(),vir.end(),[&ts](const size_t &t1,
              const size_t &t2){return ts[t1].r < ts[t2].r;});
    std::sort(vis.begin(),vis.end(),[&ts,&vis](const size_t &t1,
              const size_t &t2){return (ts[t1].r+ts[t1].p) < (ts[t2].r+ts[t2].p);});

    vecs(vir);
    vecs(vis);
    xyx(cts);

    size_t is = 0;
    size_t ir = 0;
    unsigned t = cts[vir[is]].r;
    while(is != n){
        if(t < cts[vis[is]].r){//not to the top one
            if(t < cts[vir[ir]].r){//no use
                ret.push_back(0);
            }
            else{//use
                ret.push_back(vir[ir]+1);
                -- cts[vir[ir]].p;
            }
            ++ t;
        }
        else{//get over the top one
            t += cts[vis[is]].p;
            std::cout << ">" << t << std::endl;
            for(size_t i = 0;i != cts[vis[is]].p;++ i)
                ret.push_back(vis[is]+1);
            //auto x = cts[vis[is]].p;
            cts[vis[is]].p = 0;
            ++ is;
            if(is == n)
                break;
            //auto temp = is;
            for(size_t i = is;i != n;++ i){
                if(cts[vis[i]].r < t){
                    //cts[vis[i]].p += (t-cts[vis[i]].r);
                    cts[vis[i]].r = t;
                }
            }
            std::sort(vis.begin(),vis.end(),[&cts,&vis](const size_t &t1,
                      const size_t &t2){return (cts[t1].r+cts[t1].p) < (cts[t2].r+cts[t2].p);});
            is = 0;
            while(cts[vis[is]].p == 0)
                ++ is;

            ir = 0;
            std::sort(vir.begin(),vir.end(),[&cts](const size_t &t1,
                      const size_t &t2){return cts[t1].r < cts[t2].r;});
            while(cts[vir[ir]].p == 0)
                ++ ir;

            xyx(cts);
            vecs(vir,ir);
            vecs(vis,is);
        }

    }
    return ret;
}


void test_schedule(){
    std::vector<unsigned> rs = {1,1,4,11};
    std::vector<unsigned> ps = {4,2,2,1};
    std::vector<ctask> ts;
    for(size_t i = 0;i != rs.size();++ i)
        ts.push_back(ctask(rs[i],ps[i]));
    auto rt = schedule(ts);
    for(size_t i = 0;i != rt.size();++ i){
        std::cout << rt[i] << ". ";
        if(i%10 == 0)
            std::cout << std::endl;
    }
    std::cout << std::endl;
}



#endif // GREED_H
