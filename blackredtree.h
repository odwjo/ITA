#ifndef BLACKREDTREE_H
#define BLACKREDTREE_H

#include "binarytree.h"
#include <utility>

template <typename T>
struct blackredtree{
    const static bool red = false;
    const static bool black = true;
    struct Node{
        bool color;
        T key,h_key,mx;
        Node *left,*right,*p;
        size_t bh;
        size_t nsize;
        Node():color(red),left(nullptr),right(nullptr)
          ,p(nullptr),bh(0),nsize(0){}
        Node(const T &t,const T &t2):color(red),key(t),h_key(t2),mx(t2),
            left(nullptr),right(nullptr),p(nullptr),bh(0),nsize(1){}
        Node(const T &t):color(red),key(t),h_key(t),mx(t),
            left(nullptr),right(nullptr),p(nullptr),bh(0),nsize(1){}
    };
    Node *root,*nil;
    size_t size;
    size_t b_high;

    blackredtree():size(0),b_high(0){
        nil = new Node();
        nil->color = black;
        root = nil;
    }

    template <typename N>
    N mmax(const N &n1,const N &n2,const N &n3){
        return n1>n2?(n1>n3?n1:n3):(n2>n3?n2:n3);
    }

    void nsizemaxupdate(Node *s){
        s->mx = mmax(s->h_key,s->left->mx,s->right->mx);
        s->nsize = s->left->nsize + s->right->nsize +1;
    }

    Node* leftrotate(Node* t){
        auto x = t->right;
        t->right = x->left;
        if(x->left != nil)
            x->left->p = t;
        x->p = t->p;
        if(t->p == nil)
            root = x;
        else if(t == t->p->left)
            t->p->left = x;
        else t->p->right = x;
        x->left = t;
        t->p = x;

        nsizemaxupdate(t);
        nsizemaxupdate(x);

        return x;
    }

    Node* rightrotate(Node* t){
        auto x = t->left;
        t->left = x->right;
        x->p = t->p;
        if(x->right != nil)
            x->right->p = t;
        if(t->p == nil)
            root = x;
        else if(t == t->p->left)
            t->p->left = x;
        else t->p->right = x;
        x->right = t;
        t->p = x;

        nsizemaxupdate(t);
        nsizemaxupdate(x);

        return x;
    }

    void rb_insert_fixup(Node* z){
        while(z->p->color == red){
            if(z->p == z->p->p->left){
                auto y = z->p->p->right;
                if(y->color == red){
                    z->p->color = black;
                    y->color = black;
                    z->p->p->color = red;
                    z = z->p->p;
                    ++ z->bh;
                }else if(z == z->p->right){
                    z = z->p;
                    leftrotate(z);
                }else{
                    z->p->color = black;
                    //++ z->p->bh;
                    z->p->p->color = red;
                    rightrotate(z->p->p);
                }
            }else{
                auto y = z->p->p->left;
                if(y->color == red){
                    z->p->color = black;
                    y->color = black;
                    z->p->p->color = red;
                    z = z->p->p;
                    ++ z->bh;
                }else if(z == z->p->left){
                    z = z->p;
                    rightrotate(z);
                }else{
                    z->p->color = black;
                    //++ z->p->bh;
                    z->p->p->color = red;
                    leftrotate(z->p->p);
                }
            }
        }
        if(root->color != black){
            root->color = black;
            //++ root->bh;
        }
        b_high = root->bh;
    }

    void rb_insert(const T& t){
        auto z = new Node(t);
        auto x = root;
        auto temp = nil;
        while(x != nil){
            temp = x;
            if(x->key > t)
                x = x->left;
            else
                x = x->right;
        }
        z->p = temp;
        if(temp == nil)
            root = z;
        else if(t <= temp->key)
            temp->left = z;
        else temp->right = z;

        z->left = z->right = nil;
        z->color = red;

        auto tp = z;
        while(tp->p != nil){
            tp = tp->p;
            ++ tp->nsize;
        }

        auto up = z->p;
        while(up != nil){
            auto upmx = mmax(z->h_key,up->left->mx,up->right->mx);
            if(upmx < z->mx)
                up->mx = upmx;
            else
                break;

            up = up->p;
        }

        rb_insert_fixup(z);

        ++ size;
    }

    void rb_insert(const T& t,const T& t2){
        auto z = new Node(t,t2);
        auto x = root;
        auto temp = nil;
        while(x != nil){
            temp = x;
            if(x->key > t)
                x = x->left;
            else
                x = x->right;
        }
        z->p = temp;
        if(temp == nil)
            root = z;
        else if(t <= temp->key)
            temp->left = z;
        else temp->right = z;

        z->left = z->right = nil;
        z->color = red;

        auto tp = z;
        while(tp->p != nil){
            tp = tp->p;
            ++ tp->nsize;
        }

        auto up = z->p;
        while(up != nil){
            auto upmx = mmax(z->h_key,up->left->mx,up->right->mx);
            if(upmx < z->mx)
                up->mx = upmx;
            else
                break;

            up = up->p;
        }

        rb_insert_fixup(z);

        ++ size;
    }

    void rb_transplant(Node *u,Node *v){
        //v->nsize = u->nsize;
        //v->max = u->max;
        if(u->p == nil){
            root = v;
        }
        else if(u == u->p->left)
            u->p->left = v;
        else
            u->p->right = v;
        v->p = u->p;
    }



    void rb_delete_fixup(Node *x){
        -- x->bh;
        while(x != root && x->color == black){// the side is deleted
            if(x == x->p->left){                //is one black dot short
                auto w = x->p->right;
                if(w->color == red){
                    w->color = black;
                    x->p->color = red;
                    leftrotate(x->p);
                    w = x->p->right;
                }
                if(w->left->color == black && w->right->color == black){
                    w->color = red;
                    x = x->p;
                    -- x->bh;
                }else{
                    if(w->right->color == black){
                        w->left->color = black;
                        w->color = red;
                        rightrotate(w);
                        w = x->p->right;
                    }
                    w->color = x->p->color;
                    x->p->color = black;
                    w->right->color = black;
                    leftrotate(x->p);
                    x = root;
                }
            }else{
                auto w = x->p->left;
                if(w->color == red){
                    w->color = black;
                    x->p->color = red;
                    rightrotate(x->p);
                    w = x->p->left;
                }
                if(w->right->color == black && w->left->color == black){
                    w->color = red;
                    x = x->p;
                }else{
                    if(w->left->color == black){
                        w->right->color = black;
                        w->color = red;
                        leftrotate(w);
                        w = x->p->left;
                    }
                    w->color = x->p->color;
                    x->p->color = black;
                    w->left->color = black;
                    rightrotate(x->p);
                    x = root;
                }
            }
        }
        x->color = black;
        b_high = root->bh;
    }

    void rb_delete(Node *t){
        auto y = t;
        auto ocolor = y->color;
        auto x = t;
        auto pp = t->p;
        if(t->left == nil){
            x = t->right;
            rb_transplant(t,t->right);
        }
        else if(t->right == nil){
            x = t->left;
            rb_transplant(t,t->left);
        }else{

            y = tree_minimum(t->right);
            pp = y->p;
            ocolor = y->color;
            x = y->right;

            if(y->p == t)       //use successor replace t
                x->p = y;       //if x is nil
            else{
                rb_transplant(y,y->right);
                y->right = t->right;
                y->right->p = y;
            }

            rb_transplant(t,y);
            y->left = t->left;
            y->left->p = y;
            y->color = t->color;//so t's color doesn't change
        }
        //std::cout << x->key << x->color << std::endl;

        auto tmax = t->mx;
        auto pmax = pp->mx;
        if(x != nil)
            nsizemaxupdate(x);

        if(pp != nil && (tmax != x->mx||x==nil||pmax != x->mx)){
            auto up = pp;
            if(pmax == x->mx)
                up = y->p;
            while(up != nil){
                auto upmx = mmax(up->h_key,up->left->mx,up->right->mx);
                if(upmx != up->mx)
                    up->mx = upmx;
                else break;
                up = up->p;
            }
        }

        auto temp = x->p;
        while(temp != nil){
            -- temp->nsize;
            temp = temp->p;
        }

        delete t;
        -- size;
        if(ocolor == black)     //It's the color being deleted
            rb_delete_fixup(x);
    }

    Node* os_select(Node *t,size_t i){
        if(i > t->nsize)
            return nullptr;
        auto r = t->left->nsize + 1;
        if(r == i)
            return t;
        else if(r > i)
            return os_select(t->left,i);
        else
            return os_select(t->right,i-r);
    }

    Node* os_select2(Node *t,size_t i){
        if(i > t->nsize)
            return nullptr;
        auto r = t->left->nsize + 1;
        if(r == i)
            return t;
        auto temp = t;
        while(r != i){
            if(r > i){
                temp = temp->left;
                r = temp->left->nsize + 1;
            }
            else{
                temp = temp->right;
                i -= r;
                r = temp->left->nsize + 1;
            }
        }

        return temp;
    }

    size_t os_rank(Node *e,Node* rt = nullptr){
        if(rt == nullptr)
            rt = root;
        auto r = e->left->nsize + 1;
        auto y = e;
        while(y != rt){
            if(y == y->p->right)
                r+= y->p->left->nsize +1;
            y = y->p;
        }
        return r;
    }

    bool overlap(const T &t1,const T &t2,Node *t){
        return (t1 >= t->key && t1 <= t->h_key) ||
                (t2 >= t->key && t2 <= t->h_key);
    }

    Node* interval_search(const T &t1,const T &t2,Node *rt = nullptr){
        if(rt == nullptr)
            rt = root;
        while(rt != nil && !overlap(t1,t2,rt)){
            if(rt->left != nil && rt->left->mx >= t1)
                rt = rt->left;
            else
                rt = rt->right;
        }
        return rt;//if can not find,return nil
    }

    // borrow from binary tree
    void free(Node* p){
        if(p != nil){
            auto t1 = p->left;
            auto t2 = p->right;
            delete p;
            -- size;
            free(t1);
            free(t2);
        }
    }

    ~blackredtree(){
        free(root);
        if(size != 0)
            std::cerr << "Trouble.\n";
        delete nil;
    }

    void inorder_treewalk(Node* x){
        if(x != nil){
            inorder_treewalk(x->left);
            std::cout << x->key << "(" << x->h_key
                      << "|" << x->mx << ") ^ ";
            //std::cout << x->key << " ^ ";
            inorder_treewalk(x->right);
            //std::cout << std::endl;
        }
    }

    Node* tree_maximum(Node* t){
        auto x = t;
        while(x->right != nil)
            x = x->right;
        return x;
    }

    Node* tree_minimum(Node* t){
        Node* x = t;
        while(x->left != nil)
            x = x->left;
        return x;
    }

    Node* search(const T &t){
        auto x = root;
        while(x != nil){
            if(x->key == t)
                return x;
            else if(x->key < t)
                x = x->right;
            else
                x = x->left;
        }
        return nullptr;
    }
};

#endif // BLACKREDTREE_H
