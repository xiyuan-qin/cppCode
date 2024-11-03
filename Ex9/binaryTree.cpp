#include<iostream>
using namespace std;

template<class T>
class binaryTreeNode{
public:
    T element;
    binaryTreeNode<T> *left_child;
    binaryTreeNode<T> *right_child;

public:
    binaryTreeNode(){
        left_child = right_child = NULL;
    }

    binaryTreeNode(const T& ele){
        left_child = right_child = NULL;
        element = ele;
    }

    binaryTreeNode(const T& ele , binaryTreeNode<T> *left_child , binaryTreeNode<T> *right_child){
        element = ele;
        this->left_child = left_child;
        this->right_child = right_child;
    }
};

template<class T>
class binaryTree{

public:
    binaryTreeNode<T> *root;//根节点
    int tree_size;

public:
    binaryTree(){
        root = new binaryTreeNode(1);//根节点为1，左右孩子为NULL
    }

    void insertLeft(){
        
    }

    //前序遍历,括号里放树根节点
    T preOrder(binaryTreeNode<T> *t){
        if(t != NULL){
            return  t->element;
            preOrder(t->left_child);
            preOrder(t->right_child);
        }
    }


};

int main(){
    int n;
    cin>>n;

    binaryTree<int> t;
    int left_ele , right_ele;
    for(int i = 0 ; i < n ; i++){
        cin>>left_ele>>right_ele;

    }
}