/*
#include <iostream>
using namespace std;

template<class T>

class Stack {
private:
    T* stack;
    int stackTop;//栈顶
    int size;//栈的大小
public:
    Stack(int initial_size = 10) {//初始设定栈的大小为10
        stack = new T[initial_size];
        size = initial_size;
        stackTop = -1;//栈顶从-1开始
    }

    ~Stack() {
        delete[] stack;
    }

    bool empty() const {//判断是否为空
        return (stackTop == -1);
    }

    int Size() const {//拿取栈的大小
        return stackTop + 1;
    }

    T& top() {//返回栈顶的值
        return stack[stackTop];
    }

    void pop() {//删除栈顶的值
        if (stackTop == -1) {
            return ;
        }
        stackTop--;
    }

    void push(const T& the_ele) {//压入
        if (stackTop == size - 1) {//扩容
            size *= 2;
            T* temp = new T[size];
            for (int i = 0; i <= stackTop; i++) {
                temp[i] = stack[i];
            }
            delete[] stack;
            stack = temp;
        }

        stack[++stackTop] = the_ele;
    }
};

template<class T>

class Queue {
private:
	int queuefront;//第一个数前一个位置的索引
	int queueback;//最后一个数的索引
	int arraylength;//队列长度
	T* queue0;
public:
	Queue(int l) {
		arraylength = l;
		queue0 = new T[arraylength];
		queuefront = queueback = 0;
	}
	~Queue() { delete[] queue0; }
	bool empty()const { return queuefront==queueback; }//是否为空
	int qsize()const { return queueback - queuefront; }//列表大小
	T& front() { return queue0[queuefront]; }//第一个位置
	void pop() {//删除
		queuefront++;
	}
	void push(T& theelement) {//插入
		queue0[queueback] = theelement;
		queueback++; 
	}
};


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
    binaryTreeNode<T>** treenode; // 节点的指针数组 
    int tree_size;

    binaryTree(int t) {
        root = new binaryTreeNode<T>(1); // 初始化根节点
        tree_size = t;
        treenode = new binaryTreeNode<T>*[tree_size + 1]; // 给指针数组分配空间 
        treenode[1] = root;

        int index = 1;
        for(int i = 1; i <= tree_size; i++) {
            int a, b;
            cin >> a >> b;

            binaryTreeNode<T>* x = treenode[i]; // 当前节点

            if (a != -1) {
                x->left_child = new binaryTreeNode<T>(a);
                treenode[++index] = x->left_child; // 确保将左子节点存入正确位置
            }
            if (b != -1) {
                x->right_child = new binaryTreeNode<T>(b);
                treenode[++index] = x->right_child; // 确保将右子节点存入正确位置
            }
        }
    }

    bool empty()const { return tree_size == 0; }

    //前序遍历
    void preOrder(binaryTreeNode<T>* node) {
        if (node) {//node非空
            cout << node->element << " ";
            preOrder(node->left_child);
            preOrder(node->right_child);
        }
    }

    //中序遍历
    void inOrder() {
       Stack<binaryTreeNode<T>*> s(tree_size);
       binaryTreeNode<T>* cur = root;
       while (cur != NULL || !s.empty())
       {
            while (cur != NULL)
            {
                s.push(cur);
                cur = cur->left_child;//一直往里压直到压到空
            }
            cur = s.top();//回溯
            s.pop();
            cout<< cur->element<<" ";
            cur = cur->right_child;
       }
    }

    //后序遍历
    void postOrder(){
        Stack<binaryTreeNode<T>*> s1(tree_size);
        Stack<binaryTreeNode<T>*> s2(tree_size);
        binaryTreeNode<T>* cur = root;
        while (cur != NULL || !s1.empty()) {
            while (cur != NULL) {
                s2.push(cur);
                s1.push(cur);
                cur = cur->right_child;
            }
            cur = s1.top();
            s1.pop();
            cur = cur->left_child;
        }
        while (!s2.empty()) {
            binaryTreeNode<T>* a = s2.top();
            cout << a->element << " ";
            s2.pop();
        }
    }

    void levelOrder(){
       Queue<binaryTreeNode<T>*> q(tree_size);//队列类层次遍历 
        binaryTreeNode<T>* t; 
        q.push(root);
        while (!q.empty()) {//层次遍历 
            t = q.front();
            cout << t->element << " ";
            q.pop();
            if (t->left_child != NULL) {
                q.push(t->left_child);
            }
            if (t->right_child != NULL) {
                q.push(t->right_child);
            }
        }
    }

    void numLevelOrder() {
        Queue<binaryTreeNode<T>*> q(tree_size);//队列类层次遍历 
        binaryTreeNode<T>* t;
        int b[tree_size + 1];//以各节点为根的节点数 
        q.push(root);
        while (!q.empty()) {//层次遍历 
            t = q.front();
            b[t->element]=treeNum(t);//将节点数，存在对应节点的数组位置上 
            q.pop();
            if (t->left_child != NULL) {
                q.push(t->left_child);
            }
            if (t->right_child != NULL) {
                q.push(t->right_child);
            }
        }
        for(int i=1;i<=tree_size;i++){
            cout<<b[i]<<" ";
        }
        cout<<endl;
    }


    void heightLevelOrder() {
        Queue<binaryTreeNode<T>*> q(tree_size);
        binaryTreeNode<T>* t;
        int b[tree_size + 1];//各节点高度 
        q.push(root);
        while (!q.empty()) {//层次遍历 
            t = q.front();
            b[t->element]=treeHeight(t);//将高度存到数组的对应位置 
            q.pop();
            if (t->left_child != NULL) {
                q.push(t->left_child);
            }
            if (t->right_child != NULL) {
                q.push(t->right_child);
            }
        }
        for(int i=1;i<=tree_size;i++){
            cout<<b[i]<<" ";
        }
        cout<<endl;
    }

    int treeNum(binaryTreeNode<T>* node){
      	if (node == NULL) return 0;
        int nl = treeNum(node->left_child);//左子树节点数 
        int nr = treeNum(node->right_child);//右子树节点数 
        return (nl++)+(nr++)+1;
    }

    int treeHeight(binaryTreeNode<T>* node){
        if (node == NULL) return 0;
        int hl = treeHeight(node->left_child);//左子树的高 
        int hr = treeHeight(node->right_child);//右子树的高 
        if (hl > hr)return ++hl;
        else return ++hr;
    }
};

int main() {
    int n;
    cin >> n;

    binaryTree<int> tree(n);

    // 输出各种遍历
    tree.preOrder(tree.root);
    cout << endl;
    tree.inOrder();
    cout << endl;
    tree.postOrder();
    cout << endl;
    tree.levelOrder();
    cout << endl;
    tree.numLevelOrder();
    tree.heightLevelOrder();
    
    return 0;
}
*/

#include <iostream>
using namespace std;

template<class T>

class Stack {
private:
    T* stack;
    int stackTop;//栈顶
    int size;//栈的大小
public:
    Stack(int initial_size = 10) {//初始设定栈的大小为10
        stack = new T[initial_size];
        size = initial_size;
        stackTop = -1;//栈顶从-1开始
    }

    ~Stack() {
        delete[] stack;
    }

    bool empty() const {//判断是否为空
        return (stackTop == -1);
    }

    int Size() const {//拿取栈的大小
        return stackTop + 1;
    }

    T& top() {//返回栈顶的值
        return stack[stackTop];
    }

    void pop() {//删除栈顶的值
        if (stackTop == -1) {
            return ;
        }
        stackTop--;
    }

    void push(const T& the_ele) {//压入
        if (stackTop == size - 1) {//扩容
            size *= 2;
            T* temp = new T[size];
            for (int i = 0; i <= stackTop; i++) {
                temp[i] = stack[i];
            }
            delete[] stack;
            stack = temp;
        }

        stack[++stackTop] = the_ele;
    }
};

template<class T>

class Queue {
private:
	int queuefront;//第一个数前一个位置的索引
	int queueback;//最后一个数的索引
	int arraylength;//队列长度
	T* queue0;
public:
	Queue(int l) {
		arraylength = l;
		queue0 = new T[arraylength];
		queuefront = queueback = 0;
	}
	~Queue() { delete[] queue0; }
	bool empty()const { return queuefront==queueback; }//是否为空
	int qsize()const { return queueback - queuefront; }//列表大小
	T& front() { return queue0[queuefront]; }//第一个位置
	void pop() {//删除
		queuefront++;
	}
	void push(T& theelement) {//插入
		queue0[queueback] = theelement;
		queueback++; 
	}
};


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
    binaryTreeNode<T>** treenode; // 节点的指针数组 
    int tree_size;

    binaryTree(int t) {
        root = new binaryTreeNode<T>(1); // 初始化根节点
        tree_size = t;
        treenode = new binaryTreeNode<T>*[tree_size + 1]; // 给指针数组分配空间 
        treenode[1] = root;

        for(int i = 1; i <= tree_size; i++) {
            int a, b;
            cin >> a >> b;

            binaryTreeNode<T>* x = treenode[i]; // 当前节点

            if (a != -1) {
                x->left_child = new binaryTreeNode<T>(a);
                treenode[a] = x->left_child; // 确保将左子节点存入正确位置
            }
            if (b != -1) {
                x->right_child = new binaryTreeNode<T>(b);
                treenode[b] = x->right_child; // 确保将右子节点存入正确位置
            }
        }
    }

    bool empty()const { return tree_size == 0; }

    //前序遍历
    void preOrder(binaryTreeNode<T>* node) {
        if (node) {//node非空
            cout << node->element << " ";
            preOrder(node->left_child);
            preOrder(node->right_child);
        }
    }

    //中序遍历
    void inOrder() {
       Stack<binaryTreeNode<T>*> s(tree_size);//存放节点的栈
       binaryTreeNode<T>* cur = root;
       while (cur != NULL || !s.empty())
       {
            while (cur != NULL)
            {
                s.push(cur);
                cur = cur->left_child;//一直往左压直到压到空
            }
            cur = s.top();//回溯
            s.pop();
            cout<< cur->element<<" ";
            cur = cur->right_child;
       }
    }

    //后序遍历
    void postOrder(){
        Stack<binaryTreeNode<T>*> s1(tree_size);//辅助栈，记录节点位置
        Stack<binaryTreeNode<T>*> s2(tree_size);//存放节点顺序
        binaryTreeNode<T>* cur = root;
        while (cur != NULL || !s1.empty()) {
            while (cur != NULL) {//先压全部的右边
                s2.push(cur);
                s1.push(cur);
                cur = cur->right_child;
            }
            cur = s1.top();//压完了从栈顶开始往左压
            s1.pop();
            cur = cur->left_child;
        }
        //位置存完了
        while (!s2.empty()) {
            binaryTreeNode<T>* a = s2.top();
            cout << a->element << " ";
            s2.pop();
        }
    }

    void levelOrder(){
       Queue<binaryTreeNode<T>*> q(tree_size);//队列类层次遍历 
        binaryTreeNode<T>* t; 
        q.push(root);
        while (!q.empty()) {//层次遍历 
            t = q.front();
            cout << t->element << " ";
            q.pop();
            if (t->left_child != NULL) {
                q.push(t->left_child);
            }
            if (t->right_child != NULL) {
                q.push(t->right_child);
            }
        }
    }

    void numLevelOrder() {
        Queue<binaryTreeNode<T>*> q(tree_size);//队列类层次遍历 
        binaryTreeNode<T>* t;
        int b[tree_size + 1];//以各节点为根的节点数 
        q.push(root);
        while (!q.empty()) {//层次遍历 
            t = q.front();
            b[t->element]=treeNum(t);//将节点数，存在对应节点的数组位置上 
            q.pop();
            if (t->left_child != NULL) {
                q.push(t->left_child);
            }
            if (t->right_child != NULL) {
                q.push(t->right_child);
            }
        }
        for(int i=1;i<=tree_size;i++){
            cout<<b[i]<<" ";
        }
        cout<<endl;
    }


    void heightLevelOrder() {
        Queue<binaryTreeNode<T>*> q(tree_size);
        binaryTreeNode<T>* t;
        int b[tree_size + 1];//各节点高度 
        q.push(root);
        while (!q.empty()) {//层次遍历 
            t = q.front();
            b[t->element]=treeHeight(t);//将高度存到数组的对应位置 
            q.pop();
            if (t->left_child != NULL) {
                q.push(t->left_child);
            }
            if (t->right_child != NULL) {
                q.push(t->right_child);
            }
        }
        for(int i=1;i<=tree_size;i++){
            cout<<b[i]<<" ";
        }
        cout<<endl;
    }

    int treeNum(binaryTreeNode<T>* node){
      	if (node == NULL) return 0;
        int nl = treeNum(node->left_child);//左子树节点数 
        int nr = treeNum(node->right_child);//右子树节点数 
        return (nl++)+(nr++)+1;
    }

    int treeHeight(binaryTreeNode<T>* node){
        if (node == NULL) return 0;
        int hl = treeHeight(node->left_child);//左子树的高 
        int hr = treeHeight(node->right_child);//右子树的高 
        if (hl > hr)return ++hl;
        else return ++hr;
    }
};

int main() {
    int n;
    cin >> n;

    binaryTree<int> tree(n);

    // 输出各种遍历
    tree.preOrder(tree.root);
    cout << endl;
    tree.inOrder();
    cout << endl;
    tree.postOrder();
    cout << endl;
    tree.levelOrder();
    cout << endl;
    tree.numLevelOrder();
    tree.heightLevelOrder();
    
    return 0;
}