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
class binarySearchTreeNode {
public:
    int left_size;
    T key;
    binarySearchTreeNode<T>* left_child;
    binarySearchTreeNode<T>* right_child;

    binarySearchTreeNode() {
        left_child = right_child = NULL;
        left_size = 0;
    }

    binarySearchTreeNode(const T& ele) {
        key = ele;
        left_child = right_child = NULL;
        left_size = 0;
    }

    binarySearchTreeNode(const T& ele, binarySearchTreeNode<T>* left_child, binarySearchTreeNode<T>* right_child) {
        key = ele;
        this->left_child = left_child;
        this->right_child = right_child;
        left_size = 0;
    }
};

// 二叉搜索树类声明
template <class T>
class binarySearchTree {
public:
    binarySearchTreeNode<T>* root;
    binarySearchTreeNode<T>** treenode; // 节点的指针数组 
    int tree_size;

    binarySearchTree(int t) {
        root = new binarySearchTreeNode<T>(1); // 初始化根节点
        tree_size = t;
        treenode = new binarySearchTreeNode<T>*[tree_size + 1]; // 给指针数组分配空间 
        treenode[1] = root;

        for(int i = 1; i <= tree_size; i++) {
            int a, b;
            cin >> a >> b;

            binarySearchTreeNode<T>* x = treenode[i]; // 当前节点

            if (a != -1) {
                x->left_child = new binarySearchTreeNode<T>(a);
                treenode[a] = x->left_child; // 确保将左子节点存入正确位置
            }
            if (b != -1) {
                x->right_child = new binarySearchTreeNode<T>(b);
                treenode[b] = x->right_child; // 确保将右子节点存入正确位置
            }
        }
    }

    binarySearchTree(){
        tree_size = 0;
        root = NULL;
    }

    bool empty()const { return tree_size == 0; }

    void insert(T key){
        binarySearchTreeNode<T> *p = root,
                                *pp = NULL;

        int result = 0;
        
        //遍历寻找插入点
        while(p != NULL){
            pp =p;
            if(key < p->key){
                result ^= p->key;
                p = p->left_child;
            }
            else if(key > p->key){
                result ^= p->key;
                p = p->right_child;
            } 
            else{//已经存在
                cout<<0<<endl;
                return ;
            }
        }

        binarySearchTreeNode<T> *temp = new binarySearchTreeNode<T>(key);
        if(root != NULL){//树非空
            if(key < pp->key) pp->left_child = temp;
            else              pp->right_child = temp;
        }
        else root = temp;
        //插入完成后维护left_size;
        temp->left_size = treeNum(temp);
        tree_size++;
        cout<<result<<endl;

    }

    void find(T key){
        binarySearchTreeNode<T> *cur = root;
        int result = 0;
        while(cur != NULL){
            if(key < cur->key){//key小于当前节点值，往左走
                result ^= cur->key;
                cur = cur->left_child;
            }
            if(key > cur->key){//key大于当前节点值，往右走
                result ^= cur->key;
                cur = cur->right_child;
            }
            if(cur->key == key) {
                result ^= key;
                break;
            }
        }
        if(cur == NULL){
            cout<<0<<endl;
        }
        else{
            cout<<result<<endl;
        }
    }

    void erase(T key){
        binarySearchTreeNode<T> *p = root,//搜索指针
                                *pp = NULL;//p的父节点
        binarySearchTreeNode<T> *temp_q;                     
        int result = 0;

        //查找删除元素的位置
        while(p != NULL){
            pp = p;
            if(key < p->key){
                result ^= p->key;
                p = p->left_child;
            }
            else if(key > p->key){
                result ^= p->key;
                p = p->right_child;
            }
            if(p->key == key) {
                result ^= key;
                break;
            }
        }
        if(p == NULL){//没有该元素
            cout<<0<<endl;
            return ;
        }

        //如果p有两个孩子
        if(p->left_child != NULL && p->right_child != NULL){
            //转换成有1个或0个
            //在右子树中找最小的
            binarySearchTreeNode<T> *s = p->right_child,
                                    *ps = p;//s的父节点
            while (s->left_child != NULL)//移动到最小的元素
            {
                ps = s;
                s = s->left_child;
            }

            p->key = s->key;//移上去
            binarySearchTreeNode<T> *q = new binarySearchTreeNode<T>(s->key, p->left_child , p->right_child);
            temp_q = q;

            //与P关联的指针修改为与q关联
            if(pp == NULL) root = q;
            else if(p == pp->left_child) pp->left_child = q;
            else pp->right_child = q;

            //p指向新的删除节点s，pp为p的父节点
            if (ps==p) pp=q;
            else pp=ps;
            delete p;
            p=s;
        }
        // p 最多有一个孩子
        // 在c 中保存孩子指针
        binarySearchTreeNode<T> *c;
        if(p->left_child != NULL) c = p->left_child;
        else c = p->right_child;

        if(p == root)   root = c;
        else{
            if(p == pp->left_child) pp->left_child = c;
            else                    pp->right_child =c;
        }
        tree_size--;
        temp_q->left_size = treeNum(temp_q);
        delete p;
        cout<<result<<endl;
    }

    void find_by_rank(int rank){

        binarySearchTreeNode<T> *cur = root;
        int result = 0;
        while(cur != NULL){
            if(rank < cur->left_size){//rank小于当前节点值，往左走
                result ^= cur->key; 
                cur = cur->left_child;
            }
            if(rank > cur->left_size){//rank大于当前节点值，往右走
                result ^= cur->key;
                rank -= (cur->left_size + 1);
                cur = cur->right_child;
            }
            if(rank == cur->left_size) {
                result ^= cur->key;
                break;
            }
        }
        if(cur == NULL){
            cout<<0<<endl;
        }
        else{
            cout<<result<<endl;
        }
    }

    void erase_by_rank(int rank){
        binarySearchTreeNode<T> *p = root,//搜索指针
                                *pp = NULL;//p的父节点
        binarySearchTreeNode<T> *temp_q;
        int result = 0;

        //查找删除元素的位置
        while(p != NULL){
            if(rank < p->left_size){//rank小于当前节点值，往左走
                result ^= p->key; 
                p = p->left_child;
            }
            if(rank > p->left_size){//rank大于当前节点值，往右走
                result ^= p->key;
                rank -= (p->left_size + 1);
                p = p->right_child;
            }
            if(rank == p->left_size) {
                result ^= p->key;
                break;
            }
        }
        if(p == NULL){//没有该元素
            cout<<0<<endl;
            return ;
        }

        //如果p有两个孩子
        if(p->left_child != NULL && p->right_child != NULL){
            //转换成有1个或0个
            //在右子树中找最小的
            binarySearchTreeNode<T> *s = p->right_child,
                                    *ps = p;//s的父节点
            while (s->left_child != NULL)//移动到最小的元素
            {
                ps = s;
                s = s->left_child;
            }
            p->key = s->key;//移上去

            binarySearchTreeNode<T> *q = new binarySearchTreeNode<T>(s->key, p->left_child , p->right_child);
            temp_q = q;

            //与P关联的指针修改为与q关联
            if(pp = NULL) root = q;
            else if(p == pp->left_child) pp->left_child = q;
            else pp->right_child = q;

            //p指向新的删除节点s，pp为p的父节点
            if (ps==p) pp=q;
            else pp=ps;
            delete p;
            p=s;
        }
        // p 最多有一个孩子
        // 在c 中保存孩子指针
        binarySearchTreeNode<T> *c;
        if(p->left_child != NULL) c = p->left_child;
        else c = p->right_child;

        if(p == root)   root = c;
        else{
            if(p == pp->left_child) pp->left_child = c;
            else                    pp->right_child =c;
        }
        tree_size--;
        temp_q->left_size = treeNum(temp_q);
        delete p;
        cout<<result<<endl;
    }

    //前序遍历
    void preOrder(binarySearchTreeNode<T>* node) {
        if (node) {//node非空
            cout << node->key << " ";
            preOrder(node->left_child);
            preOrder(node->right_child);
        }
    }

    //中序遍历
    void inOrder() {
       Stack<binarySearchTreeNode<T>*> s(tree_size);//存放节点的栈
       binarySearchTreeNode<T>* cur = root;
       while (cur != NULL || !s.empty())
       {
            while (cur != NULL)
            {
                s.push(cur);
                cur = cur->left_child;//一直往左压直到压到空
            }
            cur = s.top();//回溯
            s.pop();
            cout<< cur->key<<" ";
            cur = cur->right_child;
       }
    }

    //后序遍历
    void postOrder(){
        Stack<binarySearchTreeNode<T>*> s1(tree_size);//辅助栈，记录节点位置
        Stack<binarySearchTreeNode<T>*> s2(tree_size);//存放节点顺序
        binarySearchTreeNode<T>* cur = root;
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
            binarySearchTreeNode<T>* a = s2.top();
            cout << a->key << " ";
            s2.pop();
        }
    }

    void levelOrder(){
       Queue<binarySearchTreeNode<T>*> q(tree_size);//队列类层次遍历 
        binarySearchTreeNode<T>* t; 
        q.push(root);
        while (!q.empty()) {//层次遍历 
            t = q.front();
            cout << t->key << " ";
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
        Queue<binarySearchTreeNode<T>*> q(tree_size);//队列类层次遍历 
        binarySearchTreeNode<T>* t;
        int b[tree_size + 1];//以各节点为根的节点数 
        q.push(root);
        while (!q.empty()) {//层次遍历 
            t = q.front();
            b[t->key]=treeNum(t);//将节点数，存在对应节点的数组位置上 
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
        Queue<binarySearchTreeNode<T>*> q(tree_size);
        binarySearchTreeNode<T>* t;
        int b[tree_size + 1];//各节点高度 
        q.push(root);
        while (!q.empty()) {//层次遍历 
            t = q.front();
            b[t->key]=treeHeight(t);//将高度存到数组的对应位置 
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

    int treeNum(binarySearchTreeNode<T>* node){
      	if (node == NULL) return 0;
        int nl = treeNum(node->left_child);//左子树节点数 
        int nr = treeNum(node->right_child);//右子树节点数 
        return (nl++)+(nr++)+1;
    }

    int treeHeight(binarySearchTreeNode<T>* node){
        if (node == NULL) return 0;
        int hl = treeHeight(node->left_child);//左子树的高 
        int hr = treeHeight(node->right_child);//右子树的高 
        if (hl > hr)return ++hl;
        else return ++hr;
    }
};

int main(){
    binarySearchTree<int> bstree;
    int m , a , b;
    cin>>m;
    for(int i = 0 ; i < m ; i++){
        cin>>a>>b;
        switch (a)
        {
        case 0:
            bstree.insert(b);
            break;
        case 1:
            bstree.find(b);
            break;
        case 2:
            bstree.erase(b);
            break;
        case 3:
            bstree.find_by_rank(b);
            break;
        case 4:
            bstree.erase_by_rank(b);
            break;
        default:
            break;
        }
    }
    return 0;
}