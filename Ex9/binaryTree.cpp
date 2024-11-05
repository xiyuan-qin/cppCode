#include <iostream>
using namespace std;

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
            binaryTreeNode<T>* x = treenode[i];
            if (a != -1) {
                x->left_child = new binaryTreeNode(a);
                treenode[a] = x->left_child;
            }
            if (b != -1) {
                x->right_child = new binaryTreeNode(b);
                treenode[b] = x->right_child;
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

    void levelOrder() {//层次遍历
        Queue<binaryTreeNode<T>*> q(tree_size);
        binaryTreeNode<T>* t;
        q.push(root);
        while (!q.empty()) {
            t = q.front();
            cout << t->element << " ";
            q.pop();
            if (t->left_child != NULL) q.push(t->left_child);
            if (t->right_child != NULL) q.push(t->right_child);
        }
        cout << endl;
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


    void heightLevelOrder(binaryTreeNode<T>* node) {
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
    tree.inOrder(tree.root);
    cout << endl;
    tree.postOrder(tree.root);
    cout << endl;
    /*tree.levelOrder();
    cout << endl;
    tree.numLevelOrder();
    tree.heightLevelOrder(tree.root);*/
    
    return 0;
}