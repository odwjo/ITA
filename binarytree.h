#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <memory>
#include <iostream>
#include <vector>
#include <climits>
#include "datastruct.h"

template <typename T>
struct binarytree{
    struct Node{
        T key;
        Node *left,*right,*p;
        Node():left(nullptr),right(nullptr),p(nullptr){}
        Node(const T&t):key(t),left(nullptr),right(nullptr),p(nullptr){}
    };
    Node* root;
    size_t size;
    size_t layer;

    binarytree():layer(0),size(0),root(nullptr){}

    size_t insert(const T &t){
        if(size == 0)
            root = new Node(t);
        else{
            auto temp = root;
            auto tp = temp;
            //auto tp1 = tp;
            int fg = 0;
            while(temp){
                auto s = temp->key;
                //tp1 = tp;
                tp = temp;
                if(s >= t){
                    temp = temp->left;
                    fg = 1;
                }
                else{
                    temp = temp->right;
                    fg = 2;
                }
            }

            if(fg == 1){
                if(tp->right != nullptr)
                    -- layer;
                tp->left = new Node(t);
                tp->left->p = tp;
            }else{
                if(tp->left != nullptr)
                    -- layer;
                tp->right = new Node(t);
                tp->right->p = tp;
            }

        }
        ++ size;
        ++ layer;
        return size;
    }

    void check_insert(Node *p,Node **t,const T &tp){
        if(!(*t)){
            *t = new Node(tp);
            (*t)->p = p;
            (*t)->key = tp;
            if(t == &(p->left)){
                p->left = (*t);
                if(!p->right)
                    -- layer;
            }
            else{
                p->right = (*t);
                if(!p->left)
                    -- layer;
            }
        }
        else if((*t)->key > tp)
            check_insert(*t,&((*t)->left),tp);
        else
            check_insert(*t,&((*t)->right),tp);
    }

    size_t insert2(const T &t){
        if(size == 0){
            root = new Node(t);
        }
        else{
            if(t > root->key)
                check_insert(root,&(root->right),t);
            else
                check_insert(root,&(root->left),t);
        }
        ++ size;
        ++ layer;
        return size;
    }

    T del(Node *t){

        auto ret = t->key;
        auto temp = t;
        if(!t->left){
            temp = t->right;
        }
        else if(!t->right){
            temp = t->left;
        }
        else {
            auto y = successor(t);
            if(y == t->right){      //y has no left child
                y->left = t->left;
                temp = y;
            }
            else{
                y->p->left = y->right;
                y->right->p = y->p;

                y->right = t->right;
                y->left = t->left;
                temp = y;
            }
        }
        transplant(t,temp);
        delete t;
        -- size;
        return ret;
    }

    void transplant(Node *t,Node *temp){
        if(t->p)
            temp->p = t->p;
        else
            root = temp;
        if(t == t->p->left)
            t->p->left = temp;
        else
            t->p->right = temp;
    }

    void free(Node* p){
        if(p){
            auto t1 = p->left;
            auto t2 = p->right;
            delete p;
            -- size;
            free(t1);
            free(t2);
        }
    }

    ~binarytree(){
        free(root);
        if(size != 0)
            std::cerr << "Trouble.\n";
    }

    void inorder_treewalk(Node* x){
        if(x){
            inorder_treewalk(x->left);
            std::cout << x->key << " ^ ";
            inorder_treewalk(x->right);
            //std::cout << std::endl;
        }
    }
    void inorder_treewalk2(Node* x){
        Stack<Node*> stk;
        size_t fg;
        fg = 0;

        while(true){
            while((x->left!=nullptr) && fg == 0){
                stk.push(x);

                x = x->left;
            }
            //std::cout << std::endl;
            std::cout << x->key << " ^ ";
            if(x->right){

                x = x->right;
                fg = 0;
            }
            else if(stk){
                x = stk.top();
                stk.pop();
                fg = 1;
            }
            else
                break;
        }
    }
    void inorder_treewalk3(Node* x){

        size_t fg;
        fg = 0;
        auto p = x;
        while(true){
            while((x->left!=nullptr) && fg == 0){
                x = x->left;
                p = x;
            }

            std::cout << x->key << " ^ " << std::endl;
            if(x->right){
                p = x;
                x = x->right;
                fg = 0;
            }
            else if(p->p){
                if(p == p->p->right)
                    break;
                p = p->p;
                x = p;
                fg = 1;
            }
            else
                break;
        }
    }
    void preorder_treewalk(Node* x){
        if(x){
            std::cout << x->key << " ^ ";
            inorder_treewalk(x->left);
            inorder_treewalk(x->right);
        }
    }
    void postorder_treewalk(Node* x){
        if(x){
            inorder_treewalk(x->left);
            inorder_treewalk(x->right);
            std::cout << x->key << " ^ ";
        }
    }

    Node* search(const T &t){
        auto x = root;
        while(x){
            if(x->key == t)
                return x;
            else if(x->key < t)
                x = x->right;
            else
                x = x->left;
        }
        return nullptr;
    }

    Node* maximum(Node* t){
        auto x = t;
        while(x->right)
            x = x->right;
        return x;
    }

    Node* minimum(Node* t){
        Node* x = t;
        while(x->left)
            x = x->left;
        return x;
    }

    Node* successor(Node* t){  //ret->key <= t->key
        if(t->right)
            return minimum(t->right);
        auto y = t->p;
        while(y && t == y->right){   //if t== y->left then return y
            t = y;
            y = y->p;
        }
        return y;
    }

    Node* predecessor(Node* t){
        if(t->left)
            return maximum(t->left);
        auto y = t->p;
        while(y && t == y->left){   //if t== y->right then return y
            t = y;
            y = y->p;
        }
        return y;
    }

    void asminheap(const Node* x){
        if(x->left)
            as(x->left);
        std::cout << x->key << "^";
        if(x->right)
            as(x->right);
    }


    void show(){
        std::vector<int> vi;
        auto l = layer;

    }
};

void testbitree(){

}

#endif // BINARYTREE_H
