#include<iostream>
using namespace std;

template<class T>
class queue {
private:
    int queuefront; // 第一个数前一个位置的索引
    int queueback;  // 最后一个数的索引
    int arraylength; // 队列长度
    T* queue0;
public:
    queue(int l) {
        arraylength = l;
        queue0 = new T[arraylength];
        queuefront = queueback = 0;
    }
    ~queue() { delete[] queue0; }
    bool empty()const { return queuefront == queueback; } // 是否为空
    int qsize()const { return queueback - queuefront; }   // 列表大小
    T& front() { return queue0[queuefront]; }             // 第一个位置
    void pop() { queuefront++; }                          // 删除
    void push(T& theelement) {                            // 插入
        queue0[queueback] = theelement;
        queueback++;
    }
};

template<class T>
class arrayStack {
private:
    int stackTop;   // 栈顶
    int arrayLength; // 栈的容量
    T* stack;       // 元素数组
public:
    arrayStack(int al=2000) {
        arrayLength = al;
        stack = new T[al];
        stackTop = -1; // 栈为空
    }
    ~arrayStack() { delete[] stack; }
    bool empty()const { return stackTop == -1; }          // 判断是否为空
    int Stacksize()const { return stackTop + 1; }         // 元素个数
    T& top() { return stack[stackTop]; }                  // 返回栈顶元素
    void pop() { stack[stackTop--] = 0; }                 // 删除栈顶元素
    void push(const T& theElement) { stack[++stackTop] = theElement; } // 在栈顶后加入元素
};

struct binaryTreeNode {   // 树节点 
    int element;
    binaryTreeNode* leftchild;
    binaryTreeNode* rightchild;
    binaryTreeNode(int a) {
        element = a;
        leftchild = rightchild = NULL;
    }
};

class linkedBinaryTree {  // 二叉树链表 
private:
    binaryTreeNode* root;  // 根节点 
    binaryTreeNode** treenode; // 节点的指针数组 
    int treesize;          // 树的节点个数 

public:
    linkedBinaryTree(int t) {
        treesize = t;
        root = new binaryTreeNode(1); // 初始化根节点 
        treenode = new binaryTreeNode*[treesize + 1]; // 给指针数组分配空间 
        treenode[1] = root;
        for(int i = 1; i <= treesize; i++) {
            int a, b;
            cin >> a >> b;
            binaryTreeNode* x = treenode[i];
            if (a != -1) {
                x->leftchild = new binaryTreeNode(a);
                treenode[a] = x->leftchild;
            }
            if (b != -1) {
                x->rightchild = new binaryTreeNode(b);
                treenode[b] = x->rightchild;
            }
        }
    }

    bool empty()const { return treesize == 0; }

    void preOrder(binaryTreeNode *t) { // 前序遍历
        if (t != NULL) {
            cout << t->element << " ";
            preOrder(t->leftchild);
            preOrder(t->rightchild);
        }
    }

    void outputpreOrder() { // 前序遍历输出 
        preOrder(root);
        cout << endl;
    }

    void inOrder() { // 中序遍历 
        arrayStack<binaryTreeNode*> s(treesize);
        binaryTreeNode* t = root;
        while (t != NULL || !s.empty()) {
            while (t != NULL) {
                s.push(t);
                t = t->leftchild;
            }
            t = s.top();
            s.pop();
            cout << t->element << " ";
            t = t->rightchild;
        }
    }

    void postOrder() { // 后序遍历 
        arrayStack<binaryTreeNode*> s1(treesize);
        arrayStack<binaryTreeNode*> s2(treesize);
        binaryTreeNode* t = root;
        while (t != NULL || !s1.empty()) {
            while (t != NULL) {
                s2.push(t);
                s1.push(t);
                t = t->rightchild;
            }
            t = s1.top();
            s1.pop();
            t = t->leftchild;
        }
        while (!s2.empty()) {
            binaryTreeNode* a = s2.top();
            cout << a->element << " ";
            s2.pop();
        }
    }

    void levelOrder();
    int number(binaryTreeNode* t);
    void outputnumber();
    int height(binaryTreeNode* t); 
    void outputheight();
};

void linkedBinaryTree::levelOrder() { // 层次遍历
    queue<binaryTreeNode*> q(treesize);
    binaryTreeNode* t;
    q.push(root);
    while (!q.empty()) {
        t = q.front();
        cout << t->element << " ";
        q.pop();
        if (t->leftchild != NULL) q.push(t->leftchild);
        if (t->rightchild != NULL) q.push(t->rightchild);
    }
    cout << endl;
}

int linkedBinaryTree::number(binaryTreeNode* t) {
    if (t == NULL) return 0;
    int nl = number(t->leftchild);
    int nr = number(t->rightchild);
    return nl + nr + 1;
}

void linkedBinaryTree::outputnumber() {
    queue<binaryTreeNode*> q(treesize);
    binaryTreeNode* t;
    int b[treesize + 1];
    q.push(root);
    while (!q.empty()) {
        t = q.front();
        b[t->element] = number(t);
        q.pop();
        if (t->leftchild != NULL) q.push(t->leftchild);
        if (t->rightchild != NULL) q.push(t->rightchild);
    }
    for (int i = 1; i <= treesize; i++) {
        cout << b[i] << " ";
    }
    cout << endl;
}

int linkedBinaryTree::height(binaryTreeNode* t) {
    if (t == NULL) return 0;
    int hl = height(t->leftchild);
    int hr = height(t->rightchild);
    return max(hl, hr) + 1;
}

void linkedBinaryTree::outputheight() {
    queue<binaryTreeNode*> q(treesize);
    binaryTreeNode* t;
    int b[treesize + 1];
    q.push(root);
    while (!q.empty()) {
        t = q.front();
        b[t->element] = height(t);
        q.pop();
        if (t->leftchild != NULL) q.push(t->leftchild);
        if (t->rightchild != NULL) q.push(t->rightchild);
    }
    for (int i = 1; i <= treesize; i++) {
        cout << b[i] << " ";
    }
    cout << endl;
}

int main() {
    int n;
    cin >> n;
    linkedBinaryTree tree(n);
    tree.outputpreOrder();
    tree.inOrder();
    cout << endl;
    tree.postOrder();
    cout << endl;
    tree.levelOrder();
    tree.outputnumber();
    tree.outputheight();
    return 0;
}
