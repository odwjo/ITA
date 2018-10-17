#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>
#include <cfloat>

using namespace std;
struct Point_2d{
    double x,y;
    Point_2d():x(0), y(0){}
    Point_2d(double x_, double y_):x(x_), y(y_){}
    Point_2d(const Point_2d &p):x(p.x),y(p.y){}
    Point_2d(Point_2d &&)=default;
    Point_2d &operator=(const Point_2d &)=default;
    Point_2d &operator=(Point_2d &&)=default;

    Point_2d &operator-=(const Point_2d &p){
        x -= p.x;
        y -= p.y;
        return *this;
    }
    Point_2d &operator+=(const Point_2d &p){
        x += p.x;
        y += p.y;
        return *this;
    }
};
bool operator==(const Point_2d &p1,
                    const Point_2d &p2){
    return p1.x==p2.x && p1.y==p2.y;
}
bool operator!=(const Point_2d &p1,
                    const Point_2d &p2){
    return !(p1==p2);
}
ostream &operator<<(ostream &os, const Point_2d &p){
    os << "(" << p.x << ", " << p.y << ")";
    return os;
}
Point_2d operator-(const Point_2d &p1,
                    const Point_2d &p2){
    auto p = p1;
    p -= p2;
    return p;
}
Point_2d operator+(const Point_2d &p1,
                    const Point_2d &p2){
    auto p = p1;
    p += p2;
    return p;
}

// + indicates p1 is at clockwise side.
// and p2 is at anti_clockwise side.
double cross_product(const Point_2d &p1,
                     const Point_2d &p2){
    return p1.x*p2.y - p1.y*p2.x;
}

struct Line_2d{
    Point_2d p1,p2;
    Line_2d(const Point_2d &p):p1(),p2(p){}
    Line_2d(const Point_2d &p1_,
            const Point_2d &p2_):p1(p1_),p2(p2_){}
    Line_2d(const Line_2d &l)=default;
    Point_2d vec()const{
        return Point_2d(p2.x-p1.x, p2.y-p1.y);
    }
};
//l1.p1 is above l2?
bool operator>(const Line_2d &l1,
               const Line_2d &l2){
    return cross_product(l1.p1-l2.p1, l2.vec()) > 0;
}
double cross_product(const Line_2d &l1,
                     const Line_2d &l2){
    return cross_product(l1.vec(), l2.vec());
}

bool is_turn_left(const Point_2d &p0,
                  const Point_2d &p1,
                  const Point_2d &p2){
    return cross_product(p1-p0, p2-p0) >= 0;
}
bool is_clockwise_side(const Point_2d &p,
                       const Line_2d &l){
    return cross_product(p-l.p1 ,l.vec()) >=0;
}

//////////////////intersect check /////////

double direction(const Point_2d &p0,
                const Point_2d &p1,
                const Point_2d &p2){
    return cross_product(p2-p0, p1-p0);
}
//if p2 is on line(p0,p1)
bool on_segment(const Point_2d &p0,
                 const Point_2d &p1,
                 const Point_2d &p2){
    if(std::min(p0.x, p1.x) <= p2.x &&
       p2.x <= std::max(p0.x, p1.x) &&
       std::min(p0.y, p1.y) <= p2.y &&
       p2.y <= std::max(p0.y, p1.y))
        return true;
    return false;
}
bool
segments_intersect(const Point_2d &p1,
                 const Point_2d &p2,
                 const Point_2d &p3,
                 const Point_2d &p4){
    auto d1 = direction(p3,p4,p1);
    auto d2 = direction(p3,p4,p2);
    auto d3 = direction(p1,p2,p3);
    auto d4 = direction(p1,p2,p4);
    //p1,p2 is on different side of line(p3,p4)
    //p3,p4 is on different side of line(p1,p2)
    if(((d1>0 && d2<0) || (d1<0 &&& d2>0)) &&
       ((d3>0 && d4<0) || (d3<0 &&& d4>0)))
        return true;
    else if(d1==0 && on_segment(p3,p4,p1))
        return true;
    else if(d2==0 && on_segment(p3,p4,p2))
        return true;
    else if(d3==0 && on_segment(p1,p2,p3))
        return true;
    else if(d4==0 && on_segment(p1,p2,p4))
        return true;
    return false;
}

////////////////////凸包查找////////////
//using namespace std;
/*
vector<Point_2d>
incremental_method(const vector<Point_2d> &v){
    auto vp = v;
    sort(vp.begin(), vp.end(),
         [](const Point_2d &p1,
            const Point_2d &p2){
            return p1.x <= p2.x;});
    vector<Point_2d> upv, downv;
    upv.push_back(vp[0]);
    downv.push_back(vp[0]);

    if(vp[1].y < vp[2].y){
        upv.push_back(vp[2]);
        downv.push_back(vp[1]);
    }else{
        upv.push_back(vp[1]);
        downv.push_back(vp[2]);
    }
    //for_each(vp.begin(),vp.end(),
    //         [](const Point_2d&p){cout << p << endl;});
    for(size_t i=3;i != vp.size(); ++ i){
        while(upv.size()>1 && !is_turn_left(vp[i],upv.back(),upv[upv.size()-2]))
            upv.pop_back();
        upv.push_back(vp[i]);
        while(downv.size()>1 && !is_turn_left(upv[upv.size()-2],upv.back(),vp[i]))
            downv.pop_back();
        downv.push_back(vp[i]);
    }
    downv.pop_back();
    upv.insert(upv.end(), downv.crbegin(), downv.crend());
    return upv;
}
*/
template <typename Container=vector<Point_2d>>
deque<Point_2d>
incremental_method(const Container &v){
    auto vp = v;
    sort(vp.begin(), vp.end(),
         [](const Point_2d &p1,
            const Point_2d &p2){
            return p1.x <= p2.x;});
    deque<Point_2d> ret;
    ret.push_back(vp[0]);

    if(vp[1].y < vp[2].y){
        ret.push_front(vp[2]);
        ret.push_back(vp[1]);
    }else{
        ret.push_front(vp[1]);
        ret.push_back(vp[2]);
    }
    //for_each(vp.begin(),vp.end(),
    //         [](const Point_2d&p){cout << p << endl;});
    for(size_t i=3;i != vp.size(); ++ i){
        while(ret.front()!=vp[0] && !is_turn_left(vp[i],ret.front(),ret[1]))
            ret.pop_front();
        ret.push_front(vp[i]);
        while(ret.back()!=vp[0] && !is_turn_left(ret[ret.size()-2],ret.back(),vp[i]))
            ret.pop_back();
        ret.push_back(vp[i]);
    }
    return ret;
}
template<typename Container=vector<Point_2d>>
vector<Point_2d>
graham_scan(const Container &t){
    auto q = t;
    vector<Point_2d> vp;
    nth_element(q.begin(), q.begin(), q.end(),
                [](const Point_2d &p1,
                   const Point_2d &p2){
                   if(p1.y == p2.y)
                        return p1.x < p2.x;
                   return p1.y<p2.y;});
    sort(q.begin()+1, q.end(),
         [&q](const Point_2d &p1,
            const Point_2d &p2){
            return is_turn_left(*(q.begin()),p1,p2);});
    if(t.size()<3){
        cerr << "Too little points.";
        return vp;
    }
    vp.insert(vp.end(), q.begin(), q.begin()+3);
    for(size_t i = 3;i != q.size();++ i){
        while(!is_turn_left(vp[vp.size()-2],vp.back(),q[i]))
            vp.pop_back();
        vp.push_back(q[i]);
    }
    return vp;
}

template<typename Container=vector<Point_2d>>
deque<Point_2d>
Javis_step(const Container &t){
    auto ps = t;
    nth_element(ps.begin(),ps.begin(),ps.end(),
                [](const Point_2d &p1,
                   const Point_2d &p2){
                    if(p1.x == p2.x)
                        return p1.y<p2.y;
                    return p1.x < p2.x;});
    nth_element(ps.begin()+1,ps.begin()+1,ps.end(),
                [](const Point_2d &p1,
                   const Point_2d &p2){
                    if(p1.x == p2.x)
                        return p1.y<p2.y;
                    return p1.x > p2.x;});
    deque<Point_2d> ret;
    auto cp = *(ps.begin());
    auto top = ps[1];
    size_t index = 0;
    while(cp != top){
        ret.push_back(cp);
        ++ index;
        nth_element(ps.begin()+index,ps.begin()+index,ps.end(),
                    [&cp](const Point_2d &p1,
                          const Point_2d &p2){
                        return is_turn_left(cp,p1,p2);});
        cp = *(ps.begin()+index);
    }
    cp = ret[0];
    while(cp != top){
        nth_element(ps.begin()+index,ps.begin()+index,ps.end(),
                    [&cp](const Point_2d &p1,
                          const Point_2d &p2){
                        return !is_turn_left(cp,p1,p2);});
        cp = *(ps.begin()+index);
        ret.push_front(cp);
        ++ index;
    }
    return ret;
}

////////////////////find closest point_pair/////////////
double distance_square(const Point_2d& p1,
                       const Point_2d& p2){
    return (p1.x-p2.x)*(p1.x-p2.x) +
            (p1.y-p2.y)*(p1.y-p2.y);
}
double distance(const Point_2d& p1,
                const Point_2d& p2){
    return std::sqrt(distance_square(p1,p2));
}
double find_median_x(vector<Point_2d> vp){
    nth_element(vp.begin(),vp.begin()+vp.size()/2,vp.end(),
                [](const Point_2d& p1,
                const Point_2d& p2){
                    return p1.x < p2.x;});
    return vp[vp.size()/2].x;
}

void check_vp(const vector<Point_2d> &ps){
    for(auto &a:ps)
        cout << a;
    cout << "-------\n";
}

double closest_distance(vector<Point_2d> ps,
                        bool st = true){
    //check_vp(ps);
    if(ps.size() < 2)
        return DBL_MAX;
    if(ps.size()==2){
        return distance_square(ps[0], ps[1]);
    }
    double pp = find_median_x(ps);
    if(st)
        sort(ps.begin(), ps.end(),
         [](const Point_2d &p1,
            const Point_2d &p2){
         return p1.y > p2.y;});
    vector<Point_2d> pls, prs, mids;
    for(size_t i=0;i != ps.size();++ i){
        if(ps[i].x < pp)
            pls.push_back(ps[i]);
        else if(ps[i].x > pp)
            prs.push_back(ps[i]);
    }
    double ld = closest_distance(pls, false);
    double rd = closest_distance(prs, false);
    double min_dis = ld;
    if(ld > rd)
        min_dis = rd;
    for(size_t i=0;i != ps.size();++ i){
        double d = pp - ps[i].x;
        if(d*d <= min_dis)
            mids.push_back(ps[i]);
    }
    if(mids.size() > 1)
        for(size_t i=0;i != mids.size()-2;++i){
            for(size_t j=i+1;j != mids.size() && j != j+8;
                ++ j){
                double dis = distance_square(mids[i],mids[j]);
                if(dis < min_dis)
                    min_dis = dis;
            }
        }
    if(st)
        return sqrt(min_dis);
    return min_dis;
}

int main(){
    Point_2d p1(1,1),p2(4,2),p3(2,4),p4(3,3),p5(4,4),
        p6(0,3),p7(5,3),p8(3,0),p9(5,10);
    vector<Point_2d> vp{p1,p2,p3,p4,p5,p6,p7,p8,p9};
    //auto v = incremental_method(vp);
    //auto v = graham_scan(vp);
    //auto v = Javis_step(vp);
    //for_each(v.begin(),v.end(),[](const Point_2d&p){cout << p << endl;});
    //std::cout << cross_product(p1,p2) << std::endl;
    //cout << *vp.begin() << endl;
    cout << closest_distance(vp) << endl;
    return 0;
}
