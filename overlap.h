#ifndef OVERLAP_H
#define OVERLAP_H

#include <iostream>

struct rbtree{
    const static bool red = false;
    const static bool black = true;
    struct Node{
        int num;
        int fg;   //-1 or 1
        int sf;
        bool color;//red or black
        unsigned ns;//number of children
        int pmn;//the biggest ns of all nodes
        Node* pm;//the node has the biggest ns
        Node *left,*right,*p;

        Node():num(0),fg(0),sf(0),color(red),
            ns(1),pmn(0),pm(nullptr){}
        Node(int i,int f):num(i),fg(f),sf(f),color(red),ns(1),
            pmn(f),pm(nullptr){}
    };

    Node *root,*nil;
    size_t sz;

    rbtree(){
        nil = new Node(0,0);
        nil->pm = nil->left = nil->right = nil->p = nil;
        nil->color = black;
        nil->ns = 0;
        root = nil;
        sz = 0;
    }

    void org(Node *s){
        s->sf = s->fg + s->left->sf + s->right->sf;
        s->ns = 1 + s->left->ns + s->right->ns;
        if(s->sf > s->left->pmn){
            if(s->pmn > s->right->pmn){
                s->pm = s;
                s->pmn = s->sf;
            }
            else{
                s->pm = s->right->pm;
                s->pmn = s->right->pmn;
            }
        }else{
            if(s->left->pmn > s->right->pmn){
                s->pm = s->left->pm;
                s->pmn = s->left->pmn;
            }else{
                s->pm = s->right->pm;
                s->pmn = s->right->pmn;
            }
        }
    }

    void trans(Node *a,Node *b){
        if(a == a->p->left)
            a->p->left = b;
        else
            a->p->right = b;
        b->p = a->p;
    }

    Node * leftrotate(Node *t){
        auto z = t->right;

        if(t->p == nil){
            root = z;
        }
        trans(t,z);
        t->p = z;
        if(z->left != nil){
            z->left->p = t;
        }
        t->right = z->left;
        z->left = t;
        org(t);
        org(z);

        return t;
    }

    Node * rightrotate(Node *t){
        auto z = t->left;

        if(t->p == nil){
            root = z;
        }
        trans(t,z);
        t->p = z;
        if(z->right != nil){
            z->right->p = t;
        }
        t->left = z->right;
        z->right = t;
        org(t);
        org(z);
        return t;
    }

    void insert_fixup(Node *z){
        while(z->p->color == red){
            if(z == z->p->left){
                auto uncle = z->p->p->right;
                if(uncle->color == red){
                    uncle->color = black;
                    z->p->color = black;
                    z = z->p->p;
                    z->color = red;
                }else{
                    if(z == z->p->right){
                        z = leftrotate(z->p);
                    }
                    z->p->color = black;
                    z->p->p->color = red;
                    rightrotate(z->p->p);
                }
            }else{
                auto uncle = z->p->p->left;
                if(uncle->color == red){
                    uncle->color = black;
                    z->p->color = black;
                    z = z->p->p;
                    z->color = red;
                }else{
                    if(z == z->p->left){
                        z = rightrotate(z->p);
                    }
                    z->p->color = black;
                    z->p->p->color = red;
                    leftrotate(z->p->p);
                }
            }
        }
        root->color = black;
    }

    Node* insert(int i,int f){
        auto z = new Node(i, f);
        z->left = z->right = z-> pm = nil;

        if(root == nil){
            root = z;
            root ->p = nil;
        }
        else{
            auto t = root;
            auto pt = root;
            while(t != nil){
                pt = t;
                if(i > t->num)
                    t = t->right;
                else
                    t = t->left;
            }
            z->p = pt;
            if(i > pt->num)
                pt->right = z;
            else
                pt->left = z;
        }
        auto tp = z->p;
        while(tp != nil){
            org(tp);
            tp = tp->p;
        }

        ++ sz;
        insert_fixup(z);
        return z;
    }

    void free(Node *r){
        if(r != nil){
            free(r->left);
            free(r->right);
            delete r;
            sz--;
        }
    }

    ~rbtree(){
        if(sz != 0)
            free(root);
        if(sz != 0)
            std::cerr<<"Trouble!\n";
        delete nil;
    }

    void inorder_treewalk(Node* x){
        if(x != nil){
            inorder_treewalk(x->left);
            std::cout << x->num << "(" << x->pmn
                      << "|" << x->pm->num << ") ^ ";
            //std::cout << x->key << " ^ ";
            inorder_treewalk(x->right);
            //std::cout << std::endl;
        }
    }
};

#endif // OVERLAP_H
