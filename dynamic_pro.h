#ifndef DYNAMIC_PRO_H
#define DYNAMIC_PRO_H

#include <vector>
#include <climits>
#include <iostream>
#include <utility>
#include <iomanip>
#include "quicksort.h"
#include "string"
#include "datastruct.h"
#include "quicksort.h"
#include <algorithm>
#include <sstream>
#include <cctype>

template<typename T>
T max1(const T& t1,const T& t2){
    return t1>t2? t1: t2;
}

//int ct = 0;

using ul = unsigned long;

ul cut_rod(ul lg,const std::vector<ul> &prc,int *ct = nullptr){
    if(lg == 0)
        return 0;
    unsigned long q = 0;
    for(size_t i = 1;i <= lg;++ i)
        q = max<ul>(q,prc[i-1] + cut_rod(lg-i,prc,ct));

    if(ct)
        std::cout << ++ (*ct) << std::endl;

    return q;
}



ul memoized_cut_rod_aux(ul lg,
                      std::vector<ul> &r,
                        std::vector<ul> &s,
                      const std::vector<ul> &prc,
                        int *ct = nullptr){
    ul q = 0;
    if(ct)
        std::cout << ++ (*ct) << std::endl;

    if(lg == 0)
        return 0;
    else if(r[lg-1] > 0)
        return r[lg-1];
    else{
        q = 0;
        for(ul i = 1;i <= lg;++ i){
            auto t = max<ul>(q,prc[i-1] + memoized_cut_rod_aux(lg-i,r,s,prc,ct));
            if(t != q)
                s[lg-1] = i;
            q = t;
        }
    }
    r[lg-1] = q;
    return q;
}

std::pair<std::vector<ul>,std::vector<ul>> memoized_cut_rod(ul lg,const std::vector<ul> &prc){
    std::vector<ul> r(lg,0),s(lg,0);
    std::cout << r.size() << std::endl;
    //int i = 0;  //count
    memoized_cut_rod_aux(lg,r,s,prc);
    return make_pair(r,s);
}

std::pair<std::vector<ul>,std::vector<ul>> bottom_up_cut_rod(ul lg,const std::vector<ul> prc){
    std::vector<ul> r(lg+1,0),s(lg+1,0);

    for(size_t j = 1;j <= lg;++ j){
        ul q = 0;
        size_t i;
        for(i =1;i <= j;++ i){
            auto t = max<ul>(q,prc[i-1] + r[j-i]);
            if(t != q)
                s[j] = i;
            q = t;
        }
        r[j] = q;
    }
    return make_pair(r,s);
}

std::pair<std::vector<ul>,std::vector<ul>> bottom_up_cut_rod2(ul lg,
                       const std::vector<ul> prc,unsigned cc = 0){
    std::vector<ul> r(lg+1,0),s(lg+1,0);

    for(size_t j = 1;j <= lg;++ j){
        ul q = 0;
        size_t i;
        for(i =1;i <= j;++ i){
            ul t;
            if(i == j)
                t = max<ul>(q,prc[i-1] + r[j-i]);
            else
                t = max<ul>(q,prc[i-1] - cc + r[j-i]);
            if(t != q)
                s[j] = i;
            q = t;
        }
        r[j] = q;
    }
    return make_pair(r,s);
}

void test_cutrod(){
    std::vector<ul> p = {1,5,8,9,10,17,18,20,24,30,31,35};
    //int i = 0;
    auto sz = p.size();
    //std::cout << "cut rod:(12): " << cut_rod(12,p) <<
    //          std::endl;
    //auto x = ct;
    //std::cout << "cut rod:(12): " << memoized_cut_rod(12,p) <<
    //          std::endl;
    auto re = bottom_up_cut_rod(sz,p);
    for(auto x:re.first)
        std::cout << std::setw(2)<< x << " | ";
    std::cout << std::endl;
    for(auto x:re.second)
        std::cout << std::setw(2) << x << " | ";
    std::cout << std::endl;

    //std::vector<ul> cut(sz);
    Stack<ul> deal;
    ul lth = 5;
    std::cout << "L = "<< lth <<":\n";
    if(re.second[lth] != lth)
        deal.push(re.second[lth]);
    while(deal){
        auto temp = deal.top();
        deal.pop();
        std::cout << temp << "// ";
        temp = lth - temp;
        if(re.second[temp] != temp)
            deal.push(re.second[temp]);
    }
    std::cout << std::endl;
}

std::vector<ul> fob(ul i){
    std::vector<ul> ret(i,1);
    if(i < 3)
        return ret;
    for(size_t j = 2;j != i;++ j)
        ret[j] = ret[j-2] + ret[j-1];
    return ret;
}

void test_fob(){
    auto r = fob(15);
    for(auto x : r)
        std::cout << x << " < " ;
    std::cout << std::endl;
}


struct matrix{
    std::vector<vector<ul>> x;
    matrix(const ul r,const ul c){
        x = std::vector<vector<ul>>(r,std::vector<ul>(c,0));
    }

    std::vector<ul>& operator[](const ul i){
        return x[i];
    }
};

matrix matrix_multiply(const matrix &m1,const matrix &m2){
    if(m1.x[0].size() != m2.x.size()){
        std::cerr << "Can\'t multip.\n";
        return matrix(0,0);
    }else{
        auto row = m1.x.size();
        auto col = m2.x[0].size();
        matrix c(row,col);
        for(size_t i = 0;i != row;++ i)
            for(size_t j = 0;j != col;++ j){
                ul r = 0;
                for(size_t k = 0;k != m1.x[0].size();++ k)
                    r += m1.x[i][k]*m2.x[k][j];
                c[i][j] = r;
            }
        return c;
    }
}

void show_matrix(const matrix &m,size_t w = 6){
    std::cout << "<<<<<<<<<<<" << m.x.size() <<
                 " X " << m.x[0].size() << std::endl;
    for(auto z:m.x){
        for(auto y:z)
            std::cout << std::setw(w) << y << " ";
        std::cout << std::endl;
    }
}

void test_matrix(){
    std::default_random_engine en(std::time(0));
    std::uniform_int_distribution<unsigned long> rdm(0,20);

    ul r = 6,q = 8,c = 7;
    matrix m1(r,q),m2(q,c);
    for(auto &z :m1.x)
        for(auto &y:z){
        y = rdm(en);
    }
    for(auto &z :m2.x)
        for(auto &y:z){
        y = rdm(en);
    }
    matrix mr(0,0);
    mr.x = matrix_multiply(m1,m2).x;
    show_matrix(m1);
    show_matrix(m2);

    show_matrix(mr);
}

std::pair<matrix,matrix>
matrix_chain_order(const std::vector<ul> &p){
    size_t n = p.size()-1;
    matrix m(n,n),s(n,n);

    for(size_t i = 0;i != n;++ i)
        m.x[i][i] = 0;

    for(size_t l = 2;l != n+1;++ l)
        for(size_t i = 1;i <= n-l+1;++ i){
            size_t j = i+l-1;
            m.x[i-1][j-1] = ULONG_MAX;
            for(size_t k = i;k != j;++ k){//kth colum, so [i~k]*[k+1~j]
                size_t q = m.x[i-1][k-1] + m.x[k][j-1] +p[i-1]*p[k]*p[j];
                if(q < m.x[i-1][j-1]){
                    m[i-1][j-1] = q;
                    s[i-1][j-1] = k;
                }
            }
    }
    return make_pair(m,s);
}

ul lookup_chain(matrix &m,matrix &s,const std::vector<ul> &p
                ,size_t i,size_t j){
    if(m[i-1][j-1] < ULONG_MAX)
        return m.x[i-1][j-1];
    if(i == j)
        m.x[i-1][j-1] = 0;
    else
        for(size_t k = i;k != j;++ k){
            ul q = lookup_chain(m,s,p,i,k)
                    + lookup_chain(m,s,p,k+1,j)
                    +p[i-1]*p[k]*p[j];
            if(q < m[i-1][j-1]){
                m[i-1][j-1] = q;
                s[i-1][j-1] = k;
            }
        }
    return m[i-1][j-1];
}

std::pair<matrix,matrix>
matrix_chain_order_recursive(const std::vector<ul> &p){
    size_t n = p.size() - 1;
    matrix m(n,n),s(n,n);

    for(size_t i = 0;i != n;++ i)
        for(size_t j = 0;j != n;++ j)
            m.x[i][j] = ULONG_MAX;
    lookup_chain(m,s,p,1,n);
    return make_pair(m,s);
}


void print_optimal_parens(matrix s,size_t i,size_t j){
    if(i == j)
        std::cout << "M" << i;
    else{
        std::cout << "(";
        print_optimal_parens(s,i,s.x[i-1][j-1]);
        print_optimal_parens(s,s.x[i-1][j-1]+1,j);
        std::cout << ")";
    }
}


void print_optimal_parens3(matrix s,size_t i,size_t j){
    using namespace std;
    std::string ret;
    Stack<std::pair<size_t,size_t>> stk;
    auto k = s.x[i-1][j-1];
    Stack<pair<size_t,size_t>> q1;
    vector<pair<size_t,size_t>> q2;
    vector<string> qts(9);//size is 9
    q1.push(make_pair(k+1,j));
    q1.push(make_pair(i,k));
    //size_t p = 0;
    qts[i-1] += "(";
    qts[j-1] += ")";
    while(q1){
        auto tt1 = q1.top();
        q1.pop();

        if(tt1.first == tt1.second)
            q2.push_back(make_pair(0,tt1.first));
        else{
            qts[tt1.first-1] += "(";
            qts[tt1.second-1] += ")";
            k = s.x[tt1.first-1][tt1.second-1];
            q1.push(make_pair(k+1,tt1.second));
            q1.push(make_pair(tt1.first,k));
        }

    }

    size_t in=0;
    for(auto x:qts){
        if(x[0] == '(')
            cout << x+"m" << ++in;
        else
            cout << "m" << ++in << x;
    }
    cout << endl;
}

std::string print_optimal_parens_ss(matrix s,size_t i,size_t j){
    if(i == j){
        //std::cout << "M" << i;
        std::stringstream ss;
        ss << "M" << i;
        return ss.str();
    }
    else{
        //::cout << "(";
        return
        "("+ print_optimal_parens_ss(s,i,s.x[i-1][j-1])
        +"*"+print_optimal_parens_ss(s,s.x[i-1][j-1]+1,j)+")";
        //std::cout << ")";
    }

}

void test_chainorder(size_t seed = 0){
    if(seed == 0)
        seed = std::time(0);

    std::default_random_engine en(seed);
    std::uniform_int_distribution<unsigned long> rdm(10,100);

    std::vector<ul> vl;
    for(size_t i = 0;i != 10;++ i)
        vl.push_back(rdm(en));
    for(auto x:vl)
        std::cout << x << " | ";
    std::cout << std::endl;

    auto x = matrix_chain_order_recursive(vl);

    //show_matrix(x.first);
    //show_matrix(x.second);

    //auto ct = x.first;
    auto pk = x.second;
    std::stringstream ss;

    print_optimal_parens(pk,1,vl.size()-1);
    std::cout << std::endl;
    //print_optimal_parens3(pk,1,vl.size()-1);
    //std::cout << print_optimal_parens_ss(pk,1,vl.size()-1)
    //          << std::endl;
    std::cout << std::endl;
}

////////////////////find the longest common sequence
template<typename T>
std::pair<matrix,matrix>
lcs_length(const T x,const T y){
    size_t m = x.size();
    size_t n = y.size();

    matrix b(m,n),c(m+1,n+1);

    for(size_t i = 1;i <= m;++ i)
        for(size_t j = 1;j <= n;++ j){
            if(x[i-1] == y[j-1]){
                c.x[i][j] = c.x[i-1][j-1] +1;
                b.x[i-1][j-1] = 1;//diagonal up
            }else if(c.x[i-1][j] >= c.x[i][j-1]){
                c.x[i][j] = c.x[i-1][j];
                b.x[i-1][j-1] = 2;//up
            }else{
                c.x[i][j] = c.x[i][j-1];
                b.x[i-1][j-1] = 3;//left
            }
        }
    return make_pair(std::move(c),std::move(b));
}

template<typename T>
size_t recur_cacu(matrix &b,matrix &c,
                  const T &x,const T &y,
                  size_t m,size_t n){
    if(c.x[m][n] != ULONG_MAX)
        return c.x[m][n];
    if(x[m-1] == y[n-1]){
        size_t i = recur_cacu(b,c,x,y,m-1,n-1);
        c.x[m][n] = i + 1;
        b.x[m-1][n-1] = 1;
        return c.x[m][n];
    }
    size_t left = recur_cacu(b,c,x,y,m,n-1);
    size_t up   = recur_cacu(b,c,x,y,m-1,n);

    if(up >= left){
        c.x[m][n] = up;
        b.x[m-1][n-1] = 2;
        return c.x[m][n];
    }
    c.x[m][n] = left;
    b.x[m-1][n-1] = 3;
    return c.x[m][n];
}

template<typename T>
std::pair<matrix,matrix>
lcs_length2(const T x,const T y){
    size_t m = x.size();
    size_t n = y.size();

    matrix b(m,n),c(m+1,n+1);
    for(size_t i = 1;i != m+1;++ i)
        for(size_t j = 1;j != n+1;++ j){
            c.x[i][j] = ULONG_MAX;
        }

    recur_cacu(b,c,x,y,m,n);
    //return make_pair(c,b);
    return make_pair(std::move(c),std::move(b));
}

template<typename T>
void print_lcs(const matrix &b,const T&t,size_t i,size_t j){
    if(i == 0 || j == 0)
        return ;
    if(b.x[i-1][j-1] == 1){//diag
        print_lcs(b,t,i-1,j-1);
        std::cout << t[i-1];
    }else if(b.x[i-1][j-1] == 2){//up
        print_lcs(b,t,i-1,j);
    }else//left
        print_lcs(b,t,i,j-1);
}
template<typename T>
void print_lcs2(const matrix &c,const T&t,size_t i,size_t j){
    if(i == 0 || j == 0)
        return ;
    if(c.x[i][j]>c.x[i][j-1] &&
            c.x[i][j]>c.x[i-1][j]){//diag
        print_lcs2(c,t,i-1,j-1);
        std::cout << t[i-1];
    }else if(c.x[i-1][j] >= c.x[i][j-1]){//up
        print_lcs2(c,t,i-1,j);
    }else//left
        print_lcs2(c,t,i,j-1);
}

void test_lcs(){
    std::string x = "ABCBDAB",y = "BDCABA";

    auto rx = lcs_length2(x,y);
    show_matrix(rx.first);
    show_matrix(rx.second);
    print_lcs(rx.second,x,x.size(),y.size());
    std::cout << std::endl;
    //print_lcs2(rx.first,x,x.size(),y.size());
    std::cout << std::endl;
}

////////////////////search the longest subsequence
template<typename T>
std::pair<matrix,matrix>
longest_incsub(const T x){
    auto cp = x;
    std::sort(cp.begin(),cp.end());
    cp.erase(std::unique(cp.begin(),cp.end()),cp.end());
    auto rx = lcs_length(x,cp);
    print_lcs(rx.second,x,x.size(),cp.size());
    return rx;
}

void test_longsub(){
    std::vector<int> vi1 = {1,2,3,4,5,6};
    std::vector<int> vi2 = {9,1,10,2,11,3,13,4,5,6};
    std::vector<int> vi3 = {8,1,10,1,12,2,3,6};

    longest_incsub(vi1);std::cout << std::endl;
    longest_incsub(vi2);std::cout << std::endl;
    longest_incsub(vi3);std::cout << std::endl;
}

////////////////////optimal binary search tree
std::pair<std::vector<std::vector<double>>,
            std::vector<std::vector<size_t>> >
optimal_bst(std::vector<double> p,
            std::vector<double> q){
    size_t n = p.size();
    std::vector<std::vector<double>>
            e(n+2,std::vector<double>(n+1,0)),//expectation[i,j]
            w(n+2,std::vector<double>(n+1,0));
    std::vector<std::vector<size_t>>
            root(n+1,std::vector<size_t>(n+1,0));
            //e[1...n+1,0...n],
            //w[1...n+1,0...n]

    for(size_t i = 1;i != n+2;++ i){
        e[i][i-1] = q[i-1];//only d[i-1]
        w[i][i-1] = q[i-1];
    }

    for(size_t l = 1;l <= n;++ l)
        for(size_t i = 1;i <= n-l+1;++ i){
            size_t j = i+l-1;
            e[i][j] = ULONG_MAX;
            w[i][j] = w[i][j-1] + p[j] + q[j];
            for(size_t r = i;r != j+1;++ r){
                double t = e[i][r-1] + e[r+1][j] + w[i][j];
                if(t < e[i][j]){
                    e[i][j] = t;
                    root[i][j] = r;
                }
            }
        }
    return make_pair(std::move(e),std::move(root));
}

template<typename T>
void dshow(const T &t){
    std::cout << "<<<<<<<<\n";
    for(auto x:t){
        for(auto y:x)
            std::cout << std::setw(6) << y;
        std::cout << std::endl;
    }
}

std::pair<std::vector<std::vector<double>>,
            std::vector<std::vector<size_t>> >
optimal_bst2(std::vector<double> p,
            std::vector<double> q){
    size_t n = p.size();
    std::vector<std::vector<double>>
            e(n+2,std::vector<double>(n+1,0)),//expectation[i,j]
            w(n+2,std::vector<double>(n+1,0));
    std::vector<std::vector<size_t>>
            root(n+1,std::vector<size_t>(n+1,0));
            //e[1...n+1,0...n],
            //w[1...n+1,0...n]

    for(size_t i = 1;i != n+2;++ i){
        e[i][i-1] = q[i-1];//only d[i-1]
        w[i][i-1] = q[i-1];
        root[i-1][i-1] = i-1;
    }
    for(size_t i = 2;i != n+1;++ i){
        e[i-1][i-1] = 2*q[i-2] + 2*q[i-1] + p[i-1];
        w[i-1][i-1] = q[i-2] + q[i-1] + p[i-1];
    }
    dshow(root);
    dshow(e);
    for(size_t l = 1;l <= n-1;++ l)
        for(size_t i = 1;i <= n-l;++ i){
            size_t j = i+l;
            e[i][j] = ULONG_MAX;
            w[i][j] = w[i][j-1] + p[j] + q[j];
            dshow(root);
            size_t r = root[i][j-1];
            while(r <= root[i+1][j]){
                double t = e[i][r-1] + e[r+1][j] + w[i][j];
                if(t < e[i][j]){
                    e[i][j] = t;
                    root[i][j] = r;
                }
                ++ r;
            }
            dshow(e);
        }
    return make_pair(std::move(e),std::move(root));
}


size_t extract(std::vector<std::vector<size_t>> roots,
               size_t i,size_t j){
    std::cout << roots[i][j] << ", " << std::flush;
    if(j != i){
        if(roots[i][j] != i)
            extract(roots,i,roots[i][j]-1);
        if(roots[i][j] != j)
            extract(roots,roots[i][j]+1,j);
    }
    return roots[i][j];
}

void test_optimal_bst(){
    std::vector<double> pi = {0   ,0.15,0.1 ,0.05,0.1 ,0.2};
    std::vector<double> qi = {0.05,0.1 ,0.05,0.05,0.05,0.1};

    auto rt = optimal_bst2(pi,qi);
    std::cout << "E[i, j]...\n";
    for(auto x:rt.first){
        for(auto y:x)
            std::cout << std::setw(6) << y;
        std::cout << std::endl;
    }
    std::cout << "Root[i, j]...\n";
    for(auto x:rt.second){
        for(auto y:x)
            std::cout << std::setw(6) << y;
        std::cout << std::endl;
    }
    //extract(rt.second,1,5);
    std::cout << std::endl;
}

////////////////////palindrome
std::string palindrome(const std::string &str){
    size_t b,e,c,l;
    b = e = c = 0;
    l = 1;
    for(size_t i = 1;i != str.size()-l;++ i){
        size_t j = 1;
        for(;j != i+1 && j != str.size()-i;++ j){
            if(str[i-j] != str[i+j])
                break;
        }
        if(j > l){
            l = j-1;
            b = i-l;
            e = i+l;
            c = i;
        }
    }
    return str.substr(b,e-b+1);
}

////////////////// palindrome2
std::pair<matrix,matrix>
palindrome2(const std::string &s){
    size_t n = s.size();
    matrix sl(n,n);
    matrix mk(n,n);
    for(size_t i = 0;i != n;++ i){
        sl.x[i][i] = 1;
        mk.x[i][i] = 0;//self
    }
    for(size_t l = 1;l != n;++ l)
        for(size_t i = 0;i != n-l;++ i){
            size_t j = i + l;
            if(s[i] == s[j]){
                sl.x[i][j] = sl.x[i+1][j-1] + 1;
                mk.x[i][j] = 1;//s[i] ... s[j]
            }
            else{
                if(l == 1){
                    sl.x[i][j] = 1;
                    mk.x[i][j] = 3;//++ i
                }else{
                    size_t r = sl.x[i+1][j];
                    size_t l = sl.x[i][j-1];
                    if(r < l){
                        sl.x[i][j] = l;
                        mk.x[i][j] = 2;//-- j
                    }else{
                        sl.x[i][j] = r;
                        mk.x[i][j] = 3;//++ i
                    }
                }
            }
        }
    return make_pair(sl,mk);
}

void print_palindrome(std::string s,matrix mk){
    size_t b,e;
    b = 0;
    e = s.size()-1;
    std::string tp;
    size_t sb;
    sb = 0;
    while(mk.x[b][e] != 0 && b < e){
        auto m = mk.x[b][e];
        if(m == 1){
            tp.push_back(s[b++]);
            -- e;
        }
        else if(m == 2)
            -- e;
        else
            ++ b;
    }
    std::string x(tp.rbegin(), tp.rend());
    if(sb != tp.size()-1){
        tp += s[b] + x;
    }else{
        tp += x;
    }
    std::cout << s << " :palindrome:" << endl;
    std::cout << tp << endl;
}

void test_palindrome(){
    std::string s = "character";
    auto pr = palindrome2(s);
    show_matrix(pr.first);
    show_matrix(pr.second);
    std::cout << std::endl;
    print_palindrome(s,pr.second);
}

#endif // DYNAMIC_PRO_H
