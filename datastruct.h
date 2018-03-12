#ifndef DATASTRUCT_H
#define DATASTRUCT_H

#include <list>
#include <vector>
#include <exception>
#include <iostream>
#include <memory>
#include <new>
#include <climits>

template <typename T>
struct Stack{
    std::vector<T> s;
    size_t size;
    Stack():s(),size(0){}

    bool isempty(){return size == 0;}

    void push(const T &t){
        s.push_back(t);
        ++ size;
    }
    T pop(){
        if(isempty())
            throw std::range_error("Stack is empty");
        -- size;
        auto bk = s.back();
        s.pop_back();
        return bk;
    }
    T top(){
        if(isempty())
            throw std::range_error("Stack is empty");
        return s.back();
    }

    explicit operator bool() const{
        return size != 0;
    }
};

template <typename T,unsigned N>
struct Queue{
    std::vector<T> q;
    size_t size,head,tail;
    size_t length;
    Queue():q(N),size(0),length(N),head(0),tail(0){}
    bool isempty(){return size == 0;}
    void enqueue(const T &t){
        if(size == length)
            throw std::range_error("Queue is full");
        if(++tail == length){
            q[0] = t;
            tail = 0;
        }
        else
            q[tail-1] = t;
        ++ size;
    }
    T dequeue(){
        if(size == 0)
            throw std::range_error("Queue is empty.");
        -- size;

        auto temp = q[head];
        ++ head;
        if(head == length)
            head = 0;
        return temp;
    }
};

template <typename T,unsigned N>
struct Deque{
    T d[N];
    size_t head,tail,size,length;

    Deque():size(0),length(N),head(0),tail(0){}

    bool push_back(const T &t){
        if(size == length){
            std::cout << "Deque is full.";
            return false;
        }
        ++ size;
        if(tail == N-1)
            tail = 0;
        else ++ tail;
        d[tail] = t;
        return true;
    }

    T pop_back(){
        if(size == 0){
            std::cout << "Deque is empty.";
            return T();
        }
        -- size;
        auto t = d[tail];
        if(tail == 0)
            tail = N-1;
        else
            -- tail;
        return t;
    }

    bool push_front(const T &t){
        if(size == length){
            std::cout << "Deque is full.";
            return false;
        }

        if(head == 0 && size != 0)
            head = N-1;
        else if(head != 0) -- head;
        d[head] = t;
        ++ size;
        return true;
    }

    T pop_front(){
        if(size == 0){
            std::cout << "Deque is empty.";
            return T();
        }
        -- size;
        auto t = d[head];
        if(head == N-1)
            head = 0;
        else
            ++ head;
        return t;
    }
};


template <typename T>
struct linklist{
    struct Node{
        Node *next,*prev;
        T key;
        Node():next(nullptr),prev(nullptr){}
    };

    Node *head,*end;
    size_t size;
    linklist():head(nullptr),end(nullptr),size(0){
    }

    bool Insert(const T &k){
        if(head == nullptr){
            head = end = new Node();
            head->key = k;
            ++ size;
            return true;
        }
        auto temp = head;
        head = new Node();
        head->key = k;
        ++ size;
        head->next = temp;
        temp->prev = head;
        return true;
    }

    bool push_back(const T &x){
        auto temp = new Node;
        temp->key = x;
        temp->prev = end;
        end->next = temp;
        end = temp;
        ++ size;
        return true;
    }
    bool push_front(const T &x){
        auto temp = new Node;
        temp->key = x;
        temp->next = head;
        head->prev = temp;
        head = temp;
        ++ size;
        return true;
    }

    T pop_front(){
        auto temp = head;
        auto ret = temp->key;
        if(head != end){
            head = head->next;
            head->prev = nullptr;
        }
        else head = end = nullptr;
        -- size;
        delete temp;
        return ret;
    }

    T pop_back(){
        if(size == 0){
            std::cerr << "empty list.\n";
            return T();
        }
        auto temp = end;
        auto ret = temp->key;
        if(end != head){
            end = end->prev;
            end->next = nullptr;
        }
        else
            head = end = nullptr;
        -- size;
        delete temp;
        return ret;
    }

    T back(){
        return end->key;
    }

    T front(){
        return head->key;
    }

    bool Delete(const T &k){
        if(size == 0)
            return false;
        auto temp = Search(k);
        if(temp == nullptr){
            std::cout << k << " not in list,so can\'t be deleted.\n";
            return false;
        }
        if(temp->prev != nullptr)
            temp->prev->next = temp->next;
        else
            head = head->next;
        if(temp == end){
            end = end->prev;
            end->next = nullptr;
        }
        else
            temp->next->prev = temp->prev;

        delete temp;
        -- size;
        return true;
    }

    ~linklist(){
        auto temp = head;
        if(size != 0){
            while(temp->next)
                temp = temp->next;
            while(temp->prev){
                temp = temp->prev;
                delete temp->next;
                -- size;
            }

            if(size != 1)
             std::cout << "trouble\n";

            if(size != 0)
            {
                delete head;
            }
        }
    }
    std::ostream& show(std::ostream & os){
        auto temp = head;
        size_t size = 0;
        while(temp){
            ++ size;
            os << temp->key << "->";
            if(size%6 == 0)
                os << std::endl;
            temp = temp->next;
        }
        os << "END" << std::endl;
        return os;
    }

    Node* Search(const T&k){
        if(size == 0){
            std::cout << "Null list...\n";
            return nullptr;
        }
        auto temp = head;
        while(temp){
            if(temp->key == k){
                break;
            }
            else
                temp = temp->next;
        }
        if(temp == nullptr){
            std::cout << "Can\'t find :)\n";
        }
        return temp;
    }
    void reverse(){
        auto temp1 = head;
        end = head;
        auto temp2 = head->next;
        temp1->next = nullptr;
        temp1->prev = temp2;
        auto temp3 = temp2->next;
        while(temp2){
            temp3 = temp2->next;
            temp2->next = temp1;
            temp1->prev = temp2;
            temp1 = temp2;
            temp2 = temp3;
            temp1->prev = temp2;
        }
        head = temp1;
    }

    bool isempty(){
        return size == 0;
    }
};



/*
template <typename T>
struct linklist{
    struct Node{
        Node *next,*prev;
        T key;
        Node():next(nullptr),prev(nullptr){}
    };

    Node nil;
    size_t size;
    linklist():size(0){
        nil.next = nil.prev = &nil;
    }

    bool Insert(const T &k){
        auto temp = new Node();
        temp->key = k;
        temp->next = nil.next;
        nil.next->prev = temp;
        nil.next = temp;
        temp->prev = &nil;
        ++ size;
    }
    Node* Search(const T&k){
        if(nil.next == &nil){
            std::cout << "Null list...\n";
            return nullptr;
        }
        auto temp = nil.next;
        while(temp != &nil){
            if(temp->key == k){
                break;
            }
            else
                temp = temp->next;
        }
        if(temp == &nil){
            std::cout << "Can\'t find :)\n";
            return nullptr;
        }
        return temp;
    }

    bool Delete(const T &k){
        if(size == 0)
            return false;
        auto temp = Search(k);
        if(temp == nullptr){
            std::cout << k << " not in list,so can\'t be deleted.\n";
            return false;
        }
        temp->prev->next = temp->next;

        temp->next->prev = temp->prev;

        delete temp;
        -- size;
        return true;
    }

    ~linklist(){
        auto temp = &nil;
        while(temp->next != &nil)
            temp = temp->next;
        while(temp != &nil){
            temp = temp->prev;
            delete temp->next;
            -- size;
        }
        if(size != 0)
            std::cout << "Trouble.";
        size = 0;
    }
    std::ostream& show(std::ostream & os){
        auto temp = nil.next;

        while(temp != &nil){
            os << temp->key << "->";
            if(size%6 == 0)
                os << std::endl;
            temp = temp->next;
        }
        os << "END" << std::endl;
    }


};*/

template <typename T,unsigned N>
struct mlarray{
    struct Ele{
        size_t next,prev;
        T key;
        Ele():next(npos),prev(npos){}
    };
    size_t size,length;
    size_t L,free;
    size_t end;
    Ele *ele;
    static const size_t npos = ULONG_MAX;
    mlarray(){
        ele = new Ele[N];
        for(size_t i = 0;i != N-1;++ i){
            ele[i].next = i+1;
            ele[i].prev = npos;
        }
        ele[N-1].next = npos;
        free = 0;
        size = 0;
        L = end = npos;
        length = N;
    }
    ~mlarray(){
        delete [] ele;
    }

    size_t allocate_o(const T  &t){
        if(free == npos){
            std::cerr << "No space.\n";
            return npos;
        }
        auto x = free;
        free = ele[free].next;
        ele[x].next = npos;
        ele[x].key = t;

        if(size == 0){
            L = end = x;
        }
        else{
            auto temp = L;
            while(ele[temp].next != npos)
                temp = ele[temp].next;
            ele[x].prev = temp;
            ele[temp].next = x;
            if(x > end)
                end = x;
        }
        ++ size;
        return x;
    }

    bool free_o(size_t x){
        if(size == 0 || ele[x].prev == npos){
            std::cerr << "Invalid free.";
            return false;
        }

        if(x != L)
            ele[ele[x].prev].next = ele[x].next;
        else
            L = ele[x].next;
        if(ele[x].next != npos){
            ele[ele[x].next].prev = ele[x].prev;
        }
        else if(x == end){
            size_t te = L;
            size_t max = ele[x].prev;

            while(te != npos){
                max = max>te? max: te;
                te = ele[te].next;
                if(te == end)
                    te = ele[te].next;
            }
            end = max;
        }
        -- size;
        ele[x].next = free;
        free = x;
        ele[free].prev = npos;
        return true;
    }

    bool complex_free_o(size_t x){
        if(size == 0 || ele[x].prev == npos){
            std::cerr << "Invalid free.";
            return false;
        }

        if(x == end){
            free_o(x);
            return true;
        }
        //auto k = ele[end].key;
        //free_o(end);
        if(ele[end].prev != npos)
            ele[ele[end].prev].next = x;
        if(ele[end].next != npos)
            ele[ele[end].next].prev = x;
        ele[x].key = ele[end].key;
        ele[x].next = ele[end].next;
        ele[x].prev = ele[end].prev;

        ele[end].next = free;
        free = end;
        ele[free].prev = npos;

        size_t te = L;
        size_t max = ele[x].prev;

        while(te != npos){
            max = max>te? max: te;
            te = ele[te].next;
            if(te == end)
                te = ele[te].next;
        }
        end = max;
        -- size;
        return true;
    }

    size_t compact(){
        size_t t = 0;
        auto h = L;
        while(ele[h].next != npos){
            while(ele[t].prev != npos || t == L)
                ++ t;
            while(h < t && ele[h].next != npos){
                h = ele[h].next;
            }
            if(ele[h].next == npos){
                if(h < t)
                    break;

                if(ele[h].prev != npos)
                    ele[ele[h].prev].next = t;
                else
                    L = t;

                //ele[ele[h].next].prev = t;

                ele[t].key = ele[h].key;
                ele[t].next = ele[h].next;
                ele[t].prev = ele[h].prev;

                ++ t;
                ele[h].prev = npos;

                break;
            }

            if(ele[h].prev != npos)
                ele[ele[h].prev].next = t;
            else
                L = t;

            ele[ele[h].next].prev = t;


            ele[t].key = ele[h].key;
            ele[t].next = ele[h].next;
            ele[t].prev = ele[h].prev;


            ele[h].prev = npos;

            h = ele[t].next;
            ++ t;
        }

        //
        if(ele[h].next == npos){
            if(ele[h].prev != npos)
                ele[ele[h].prev].next = t;
            else
                L = t;

            ele[t].key = ele[h].key;
            ele[t].next = ele[h].next;
            ele[t].prev = ele[h].prev;

            ++ t;
            ele[h].prev = npos;
        }
        //
        end = t-1;
        auto f = t;
        free = f;
        while(f != length){
            ele[f].next = f+1;
            ele[f].prev = npos;
            ++ f;
        }

        return t;
    }

    void show(){
        auto x = L;

        if(size != 0)
            std::cout << x << " - " << ele[x].key << "->\n";

        while(ele[x].next != npos){
            x = ele[x].next;
            std::cout << x << " - " << ele[x].key << "->\n";
        }
        std::cout << std::endl;
        for(size_t x = 0;x != N;++ x){
             std::cout << x << " - " << ele[x].next << " : "
                       << ele[x].key << " : "<< ele[x].prev << "->\n";
        }
    }
};
/*
//Binary tree in a array named bTree.
void show1042(size_t i){
    std::cout << Tree[i].key << " ";
    if(bTree[i].left != nil)
        show1042(Tree[i].left);
    if(bTree[i].right != nil)
        show1042(Tree[i].right);
}
//Binary tree in a array named bTree.
template <typename T>
void show1043(size_t i){
    Stack<T> s;
    s.push(i);
    while(!s.isempty()){
        auto t = s.top();
        s.pop();
        std::cout << Tree[t].key << " ";
        if(bTree[i].left != nil)
            s.push(Tree[i].left);
        if(bTree[i].right != nil)
            s.push(Tree[i].right);
    }
}
//Unbounding tree in a array named Tree.
void show1043(size_t i){
    std::cout << Tree[i].key << " ";
    if(bTree[i].right_sibling != nil)
        show1042(Tree[i].right_sibling);
    if(bTree[i].left_child != nil)
        show1042(Tree[i].left_child);
}
*/
#endif // DATASTRUCT_H
