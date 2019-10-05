//
// Created by Rongzhi on 2019/10/4.
//

#ifndef ECE650_A2_GRAPH_H
#define ECE650_A2_GRAPH_H


//struct for linked list node
//import template class and weight in case there are additional requirements
template<class T>
struct Node
{
    Node() {
        nextArc=NULL;
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
    bool insert(int u, int v,T& w);
    int size;
protected:
    Node<T> ** head;
};



#endif //ECE650_A2_GRAPH_H
