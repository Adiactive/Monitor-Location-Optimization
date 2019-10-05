//
// Created by Rongzhi on 2019/10/4.
//

#include <cstdlib>
#include "Graph.h"

template<class T>
Graph<T>::Graph(int mSize) {
    size = mSize;
    head = new Node<T>*[size];
    for (size_t i = 0; i < size; i++)
        head[i]=NULL;
}

template<class T>
Graph<T>::~Graph() {
    Node<T>*p,*q;
    for(size_t i = 0; i < size; i++)
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
bool Graph<T>::insert(int u, int v, T &w) {
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




