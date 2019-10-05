//
// Created by Rongzhi on 2019/10/4.
//

#ifndef ECE650_A2_GRAPH_H
#define ECE650_A2_GRAPH_H

#include <cstdlib>

//for error handling
struct Exception : std::runtime_error {
    explicit Exception(const char *msg) : std::runtime_error(msg) {}
};

//struct for linked list node
//import template class and weight in case there are additional requirements
template<class T>
struct Node
{
    Node() {
        adjVex = w = nextArc = NULL;
    }
    Node(int vertex, T weight, Node *next) {
        adjVex=vertex;
        w=weight;
        nextArc=next;
    }
    int adjVex;
    T w;
    Node* nextArc;
};

template<class T>
class Graph {
public:
    explicit Graph(int mSize);
    ~Graph();
    bool exist(int u, int v)const;
    bool insert(int u, int v, int w);
    void show(std::ostream &out);

    int size;
protected:
    Node<T> ** head;
};

template<class T>
Graph<T>::Graph(int mSize) {
    size = mSize;
    head = new Node<T>*[size];
    for (int i = 0; i < size; i++)
        head[i]=NULL;
}

template<class T>
Graph<T>::~Graph() {
    Node<T>*p,*q;
    for(int i = 0; i < size; i++)
    {
        p=head[i];
        q=p;
        while (p){
            p=p->nextArc;
            delete q;
            q=p;
        }
    }
    delete []head;
}

template<class T>
bool Graph<T>::insert(int u, int v, int w) {
    if (u < 0 || v < 0 || u > size - 1 || v > size - 1 || u == v)
        return false;
    if (exist(u, v))
        return false;
    auto* p = new Node<T>(v, w, head[u]);
    head[u] = p;
    return true;
}

template<class T>
bool Graph<T>::exist(int u, int v) const {
    if (u < 0 || v < 0 || u > size - 1 || v > size - 1 || u == v)
        return false;
    Node<T>* p = head[u];
    while (p) {
        if (p -> adjVex == v)
            return true;
        p=p->nextArc;
    }
    return false;
}

template<class T>
void Graph<T>::show(std::ostream &out) {
    Node<T> *p;
    out<<"Contents of LGraph:"<<std::endl;
    for(int i=0;i<size;i++)
    {
        out<<i;
        for(p=head[i];p;p=p->nextArc)
        {
            std::cout<<"->"<<p->adjVex;
        }
        out<<std::endl;
    }
    out<<"Total number of vertex(n): "<<size<<std::endl;
    out<<std::endl;
}

#endif //ECE650_A2_GRAPH_H
