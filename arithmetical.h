#include <iostream>
#include <vector>

using num = int;

struct Gcd_a{
    using num = int;
    num d,x,y;
    Gcd_a(num d_,num x_,num y_):d(d_),x(x_),y(y_){}
};

Gcd_a extended_euclid(num a, num b){
    if(b == 0)
        return Gcd_a(a, 1, 0);
    else{
        auto g = extended_euclid(b, a%b);
        return Gcd_a(g.d, g.y, g.x-(a/b)*g.y);
    }
}

void modular_linear_equation_solver(num a, num b, num n){
    auto g = extended_euclid(a, n);
    if(b%g.d){
        std::cout << "no solutions" << std::endl;
    }else{
        auto x0 = (g.x * (b % g.d)) % n;
        for (num i = 0; i != g.d; ++ i)
            std::cout << (x0 + i * (n/g.d))%n << " / ";
        std::cout << std::endl;
    }

}

//calculate the binary representation of |n|
class Mybitset{
public:
    Mybitset(num n_):n(n_){
        int bits = (8 * sizeof n)-1;
        auto tn = n;
        if(n < 0)
            tn = -n;
        while(bits && !(tn & (1<<bits)))
              -- bits;
        if(bits){
            fg.push_back(true);
            -- bits;
            while(bits >= 0){
                if(tn & (1 << bits))
                    fg.push_back(true);
                else
                    fg.push_back(false);
                -- bits;
            }
        }else if(tn == 1){
            fg.push_back(true);
        }
    }
    size_t size()const{
        return fg.size();
    }
    int get(size_t n)const{
        return fg[n];
    }
private:
    num n;
    std::vector<bool> fg;
};

//calc (a^b) mod n
num modular_exponentiation(num a, num b, num n){
    num c = 0;
    num d = 1;
    Mybitset bs(b);
    //d = a^c %n,  c = <bs[0]...bs[i]>
    for(size_t i = 0; i != bs.size(); ++ i){
        c *= 2;
        d = (d*d)%n;
        if(bs.get(i)){
            c += 1;
            d = (d*a)%n;
        }
    }
    return d;
}

int main_test_number(){
    //auto g = extended_euclid(21, 98);
    //std::cout << g.d << ' ' << g.x << ' ' << g.y << std::endl;
    //modular_linear_equation_solver(44, 22,77);
    //std::cout << Mybitset(3).size() << std::endl;
    std::cout << modular_exponentiation(7, 560, 561) << std::endl;
    return 0;
}
