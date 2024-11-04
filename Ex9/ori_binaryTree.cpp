#include <iostream>
using namespace std;

// 节点类声明
template <class T>
class binaryTreeNode {
public:
    T element;
    binaryTreeNode<T>* left_child;
    binaryTreeNode<T>* right_child;

    binaryTreeNode() {
        left_child = right_child = NULL;
    }

    binaryTreeNode(const T& ele) {
        element = ele;
        left_child = right_child = NULL;
    }

    binaryTreeNode(const T& ele, binaryTreeNode<T>* left_child, binaryTreeNode<T>* right_child) {
        element = ele;
        this->left_child = left_child;
        this->right_child = right_child;
    }
};

// 二叉树类声明
template <class T>
class binaryTree {
public:
    binaryTreeNode<T>* root;
    int tree_size;

    binaryTree() {
        root = NULL;
        tree_size = 0;
    }

    void buildTree(int n) {
        if (n <= 0) return;
        root = new binaryTreeNode<T>(1); // 根节点编号1
        tree_size = n;

        binaryTreeNode<T>** nodes = new binaryTreeNode<T>*[n + 1];//存放结点的指针数组
        for (int i = 1; i <= n; i++) {
            nodes[i] = new binaryTreeNode<T>(i);//肯定存在n个元素
        }

        int index = 2;
        int left_ele, right_ele;
        for (int i = 1; i <= n; i++) {
            cin >> left_ele >> right_ele;
            if (left_ele != -1) {
                nodes[i]->left_child = nodes[index];
                nodes[index]->element = left_ele;
                index++;
            }
            if (right_ele != -1) {
                nodes[i]->right_child = nodes[index];
                nodes[index]->element = left_ele;
                index++;
            }
        }

        root = nodes[1]; // 根节点设为编号1
        delete[] nodes;
    }

    //前序遍历
    void preOrder(binaryTreeNode<T>* node) {
        if (node) {//node非空
            cout << node->element << " ";
            preOrder(node->left_child);
            preOrder(node->right_child);
        }
    }

    //中序遍历
    void inOrder(binaryTreeNode<T>* node) {
        binaryTreeNode<T>* current = node;
        binaryTreeNode<T>* pre;
        while (current != NULL) {
            if (current->left_child == NULL) {
                cout << current->element << " ";
                current = current->right_child;
            } else {
                pre = current->left_child;
                while (pre->right_child != NULL && pre->right_child != current) {
                    pre = pre->right_child;
                }
                if (pre->right_child == NULL) {
                    //第一次访问节点时，如果存在子节点，在左子树的最右面的叶子
                    //建立一个指向该节点的线程
                    pre->right_child = current;
                    current = current->left_child;
                } else {
                    //发现最右面的节点指向原节点，证明访问过
                    //说明左子树便利完成，断开线程并移动到右节点
                    pre->right_child = NULL;
                    cout << current->element << " ";
                    current = current->right_child;
                }
            }
        }
    }

    //后序遍历
    void postOrder(binaryTreeNode<T>* node) {
        if (!node) return;
        binaryTreeNode<T>* dummyRoot = new binaryTreeNode<T>(0);
        dummyRoot->left_child = node;
        binaryTreeNode<T>* current = dummyRoot;
        binaryTreeNode<T>* pre;
        
        while (current != NULL) {
            if (current->left_child == NULL) {
                current = current->right_child;
            } else {
                pre = current->left_child;
                while (pre->right_child != NULL && pre->right_child != current) {
                    pre = pre->right_child;
                }
                if (pre->right_child == NULL) {
                    pre->right_child = current;
                    current = current->left_child;
                } else {
                    pre->right_child = NULL;
                    printReverse(current->left_child, pre);
                    current = current->right_child;
                }
            }
        }
        delete dummyRoot;
    }

    void printReverse(binaryTreeNode<T>* from, binaryTreeNode<T>* to) {
        reversePrint(from, to);
        binaryTreeNode<T>* p = to;
        while (true) {
            cout << p->element << " ";
            if (p == from) break;
            p = p->right_child;
        }
        reversePrint(to, from);
    }

    void reversePrint(binaryTreeNode<T>* from, binaryTreeNode<T>* to) {
        if (from == to) return;
        binaryTreeNode<T>* x = from;
        binaryTreeNode<T>* y = from->right_child;
        binaryTreeNode<T>* z;
        while (x != to) {
            z = y->right_child;
            y->right_child = x;
            x = y;
            y = z;
        }
    }

    void levelOrder(binaryTreeNode<T>* node) {
        if (!node) return;
        binaryTreeNode<T>** queue = new binaryTreeNode<T>*[tree_size];
        int front = 0, back = 0;
        queue[back++] = node;
        while (front < back) {
            binaryTreeNode<T>* current = queue[front++];
            cout << current->element << " ";
            if (current->left_child != NULL) queue[back++] = current->left_child;
            if (current->right_child != NULL) queue[back++] = current->right_child;
        }
        delete[] queue;
    }

    void numLevelOrder(binaryTreeNode<T>* node) {
        if (!node) return;
        binaryTreeNode<T>** queue = new binaryTreeNode<T>*[tree_size];
        int front = 0, back = 0;
        queue[back++] = node;
        while (front < back) {
            binaryTreeNode<T>* current = queue[front++];
            cout << treeNum(current) << " ";
            if (current->left_child != NULL) queue[back++] = current->left_child;
            if (current->right_child != NULL) queue[back++] = current->right_child;
        }
        delete[] queue;
    }

    void hetigtLevelOrder(binaryTreeNode<T>* node) {
        if (!node) return;
        binaryTreeNode<T>** queue = new binaryTreeNode<T>*[tree_size];
        int front = 0, back = 0;
        queue[back++] = node;
        while (front < back) {
            binaryTreeNode<T>* current = queue[front++];
            cout << treeHeight(current) << " ";
            if (current->left_child != NULL) queue[back++] = current->left_child;
            if (current->right_child != NULL) queue[back++] = current->right_child;
        }
        delete[] queue;
    }



    int treeNum(binaryTreeNode<T>* node){
        if(node == NULL){
            return 0;
        }
        else{
            return 1 + treeNum(node->left_child) + treeNum(node->right_child);
        }
    }

    int treeHeight(binaryTreeNode<T>* node){
        if(node == NULL){
            return 0;
        }
        int h_left = treeHeight(node -> left_child);
        int h_right = treeHeight(node -> right_child);
        if(h_left > h_right){
            return ++h_left;
        }
        else{
            return ++h_right;
        }
    }
};

int main() {
    int n;
    cin >> n;

    binaryTree<int> tree;
    tree.buildTree(n);

    // 输出各种遍历
    tree.preOrder(tree.root);
    cout << endl;
    tree.inOrder(tree.root);
    cout << endl;
    tree.postOrder(tree.root);
    cout << endl;
    tree.levelOrder(tree.root);
    cout << endl;
    tree.numLevelOrder(tree.root);
    cout<<endl;
    tree.hetigtLevelOrder(tree.root);
    cout<<endl;
    
    return 0;
}