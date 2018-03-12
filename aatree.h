#ifndef AATREE_H
#define AATREE_H

#include <memory>

template <typename T>
struct aatree{
    const static bool red = false;
    const static bool black = true;
    struct Node{
        T key;
        Node *left,*right,*p;
        bool color;
        int bh;

        Node():left(nullptr),right(nullptr),p(nullptr),color(red),bh(0){}
        Node(const T &t):key(t),
            left(nullptr),right(nullptr),p(nullptr),color(red),bh(0){}
    };

    Node *root,*nil;
    size_t size,b_high;

    aatree():size(0),b_high(0){
        nil = new Node();
        nil->color = black;
        root = nil;
        root->p = root->left = root->right = nil;
    }

    bool isred(Node* s){
        return s->color == red;
    }

    Node* leftrotate(Node* s){
        auto x = s->right;
        x->color = s->color;
        s->color = red;
        x->p = s->p;

        if(s->p == nil)
            root = x;
        s->p = x;
        s->right = x->left;
        if(x->left != nil)
            x->left->p = s;
        x->left = s;
        return x;
    }
    Node* rightrotate(Node* s){
        auto x = s->left;
        x->color = s->color;
        s->color = red;
        x->p = s->p;

        if(s->p == nil)
            root = x;
        s->p = x;
        s->left = x->right;
        if(x->right != nil)
            x->right->p = s;
        x->right = s;
        return x;
    }
    void flipcolors(Node* s){
        s->color = red;
        s->left->color = s->right->color = black;
    }

    Node* ainsert(Node *p,Node *z){
        if(p == nil){
            z->left = z->right = nil;
            return z;
        }

        if(p->key < z->key){
            p->right = ainsert(p->right,z);
            p->right->p = p;
        }
        else {
            p->left = ainsert(p->left,z);
            p->left->p = p;
        }

        if(isred(p->right) && !isred(p->left)) p = leftrotate(p);
        if(isred(p->left) && isred(p->left->left)) p = rightrotate(p);
        if(isred(p->right) && isred(p->left)){
            ++ p->bh;
            flipcolors(p);
        }

        return p;
    }

    void insert(const T &t){
        auto z = new Node(t);

        root = ainsert(root,z);

        root->color = black;

        b_high = root->bh;
        ++ size;
    }

    void rb_transplant(Node *u,Node *v){
        if(u->p == nil)
            root = v;
        else if(u == u->p->left)
            u->p->left = v;
        else
            u->p->right = v;
        v->p = u->p;
    }

    void fix_up(Node *z){
        auto p = z;
        if(z->color == red)
            z->color = black;
        else{
            if(isred(p->right) && !isred(p->left)) p = leftrotate(p);
            if(isred(p->left) && isred(p->left->left)) p = rightrotate(p);
            if(isred(p->right) && isred(p->left)){
                ++ p->bh;
                flipcolors(p);
            }
        }
    }

    void del(Node* z){
        auto cc = z->color;
        auto y = z;
        auto x = z;
        if(z->left == nil)
            x = z = z->right;
        else if(z->right == nil)
            x = z = z->left;
        else{
            y = tree_minimum(z->right);
            cc = y->color;
            x = y->right;

            if(y->p == z)       //use successor replace t
                x->p = y;       //if x is nil
            else{
                rb_transplant(y,y->right);
                y->right = z->right;
                y->right->p = y;
            }

            rb_transplant(z,y);
            y->left = z->left;
            y->left->p = y;
            y->color = z->color;//so t's color doesn't change
        }
        delete z;
        -- size;
        if(cc == black)
           fix_up(x);
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

    ~aatree(){
        free(root);
        if(size != 0)
            std::cerr << "Trouble.\n";
        delete nil;
    }

    void inorder_treewalk(Node* x){
        if(x != nil){
            inorder_treewalk(x->left);
            std::cout << x->key << "(" << x->bh
                      << "|" << x->color << ") ^ ";
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

#endif // AATREE_H
