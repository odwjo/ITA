#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <utility>

using namespace std;

std::vector<size_t> naive_string_matcher(const string &t,
                                         const string &p){
    size_t n = t.size();
    size_t m = p.size();
    vector<size_t> ret;
    for(size_t s=0; s != n-m+1;++ s){
        size_t j = 0;
        while(j != m &&
              t[s+j] == p[j])
              ++ j;
        if(j == m)
            ret.push_back(s);
    }
    return ret;
}
//////////////incomplete :( /./././././
                           /*
vector<size_t>
rabin_farp_matcher(const string &t,
                   const string &p,
                   unsigned d,
                   unsigned q){
    size_t n = t.size();
    size_t m = p.size();
    //auto h = modular_exponentiation(d, m-1, q);
    unsigned pi = 0;
    unsigned ti;
}
*/

//////////////finite_automation_machine////////////////
class Trans{
    using key_type = pair<size_t, char>;
private:
    string p;
    set<char> stc;
    map<key_type, size_t> states;
    bool match(const string &p,
               size_t k,
               const string &pq,
               char a){
        if(p[--k] != a)
            return false;
        auto pi = pq.size()-1;
        while(k && p[k-1] == pq[pi]){
            -- k;
            -- pi;
        }
        if(k == 0)
            return true;
        else
            return false;
    }
public:
    Trans(const string& s):p(s){
        stc.insert(s.begin(),s.end());
        auto m = p.size();
        for(size_t i=0; i <= m;++ i){
            auto pq = p.substr(0,i);
            for(auto a : stc){
                size_t k = std::min(m, i+1);
                //k = i+1;
                while(k && !match(p,k,pq,a))
                    --k;
                states[make_pair(i,a)] = k;
            }
        }
    }
    size_t size()const{
        return p.size();
    }
    size_t operator()(size_t q, char c)const{
        if(stc.find(c)==stc.end())
            return 0;
        return states.find(make_pair(q,c))->second;
    }
};

vector<size_t>
finite_automation_machine(const string &t,
                          const Trans &s){
    size_t n = t.size();
    size_t q = 0;
    size_t m = s.size();
    vector<size_t> vs;
    for(size_t i = 0; i != n; ++ i){
        q = s(q, t[i]);
        if(q == m)
            vs.push_back(i-m+1);
    }
    return vs;
}

vector<size_t>
finite_automation_machine(const string &t,
                          const string &p){
    Trans s(p);
    return finite_automation_machine(t, s);
}

////////////////////KMP match algorithm//////////////

vector<size_t>
compute_prefix_function(const string &p){
    size_t m = p.size();
    vector<size_t> ret(m);
    size_t k = 0;
    for(size_t q=1;q != m;++ q){
        while(k && p[k] != p[q])
            k = ret[k-1];
        if(p[k] == p[q])
            ++ k;
        ret[q] = k;
    }
    return ret;
}

vector<size_t>
kmp_matcher(const string &t,
            const string &p){
    vector<size_t> ret;
    auto n = t.size();
    auto m = p.size();
    auto pi = compute_prefix_function(p);
    size_t q = 0;
    for(size_t i=0;i != n; ++ i){
        while(q && p[q] != t[i])
            q = pi[q-1];
        if(p[q] == t[i])
            ++ q;
        if(q == m){
            ret.push_back(i-m+1);
            q = pi[q-1];
        }
    }
    return ret;
}


int main_test_str(){
    //auto vi = naive_string_matcher("googole","go");
    Trans s("aabab");
    cout << finite_automation_machine("aaababaabaababaab",s)[1] << endl;
    cout << kmp_matcher("aaababaabaababaab","aabab")[1] << endl;
    return 0;
}
