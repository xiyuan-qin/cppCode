#include<iostream>
using namespace std;
class binarySearchTreeNode {
public:
	int element;//元素 
	int left_size;//左子树节点数 
	binarySearchTreeNode* left_child;
	binarySearchTreeNode* right_child;
public:
	binarySearchTreeNode(int a) {
		element = a;
		left_size = 0;
		left_child = NULL;
		right_child = NULL;
	}
};

class binarySearchTree {
private:
	binarySearchTreeNode* root;
	int size;//树的大小 
public:
	binarySearchTree() {
		root = NULL;
		size = 0;
	}
	void insert(int b){
        binarySearchTreeNode* p = root;//搜索指针 
        binarySearchTreeNode* pp = NULL;//p的父节点的指针 
        int result = 0;
        while (p != NULL) {
            pp = p;//pp向下移动 
            result ^= p->element;
            if (b < p->element) {
                p = p->left_child;
            }
            else if (b > p->element) {
                p = p->right_child;
            }
            else {//已经存在输出0 
                cout << 0 << endl;
                return;
            }
        }
        binarySearchTreeNode* newNode = new binarySearchTreeNode(b);//为插入的元素建立新的节点 ，作为pp的子节点 
        if (root != NULL) {//树非空 
            if (b < pp->element)pp->left_child = newNode;
            else pp->right_child = newNode;
        }
        else {
            root = newNode;
        }
        size++;
        p = root;
        while (p->element!=b) {//更新左子树的节点个数 
            if (b < p->element) {//如果插入的元素比该节点元素小，说明其在该节点的左子树，left_size++ 
                p->left_size++;
                p = p->left_child;
            }
            else if (b > p->element) {
                p = p->right_child;
            }
        }
        cout << result << endl;
    }

	void find(int b){
        int result = 0;
        binarySearchTreeNode* p=root;//从根开始搜索 
        while (p != NULL) {
            result ^= p->element;
            if (b < p->element) {
                p = p->left_child;
            }
            else if (b > p->element) {
                p = p->right_child;
            }
            else {//找到则返回异或值 
                cout << result << endl;
                return;
            }
        }
        cout << 0 << endl;//没找到 
    }

	void erase(int b){
        int result = 0;
        binarySearchTreeNode* p = root;//搜索指针 
        binarySearchTreeNode* pp = NULL;//p的父节点的指针 
        while (p != NULL && p->element != b) {//找到元素b所在的位置 
            pp = p;
            if (b < p->element) {
                p = p->left_child;
            }
            else {
                p = p->right_child;
            }
        }
        if (p == NULL) {//不存在该元素 
            cout << 0 << endl;
            return;
        }
        p = root;
        while (p != NULL && p->element != b) {//更新左子树节点个数 
            result ^= p->element;
            if (b < p->element) {
                p->left_size--;
                p = p->left_child;
            }
            else {
                p = p->right_child;
            }
        }
        result ^= p->element;
        if (p->left_child != NULL && p->right_child != NULL) {//有两个孩子，转换成有0或1个孩子 
            binarySearchTreeNode* s = p->right_child;//找p的右子树的最小元素 
            binarySearchTreeNode* ps = p;//s的父节点 
            while (s->left_child != NULL) {//小元素在左孩子处 
                s->left_size--;//更新左子树节点个数 
                ps = s;
                s = s->left_child;
            }
            binarySearchTreeNode* q=new binarySearchTreeNode(s->element);//声明一个可以替代p指针的q指针 
            q->left_size = p->left_size;
            q->left_child = p->left_child;
            q->right_child = p->right_child;
            //让q指针作为pp的孩子 
            if (pp == NULL) {
                root = q;
            }
            else if (p == pp->left_child) {
                pp->left_child = q;
            }
            else {
                pp->right_child = q;
            }
            //让pp指向要删除节点(s)的父节点 
            if (ps == p) {
                pp = q;
            }
            else {
                pp = ps;
            }
            delete p;
            p = s;//删除节点替换为s
            //删除节点两个孩子的情况改为0个或1个孩子的情况 
        }
        binarySearchTreeNode* c;//保存删除节点的孩子指针
        if (p->left_child != NULL) {
            c = p->left_child;
        }
        else {
            c = p->right_child;
        }
        //将孩子指针连接上 
        if (p == root) {
            root = c;
        }
        else {
            if (p == pp->left_child) {
                pp->left_child = c;
            }
            else {
                pp->right_child = c;
            }
        }
        size--;//树的节点数减一 
        delete p;
        cout << result << endl;
    }

	void find_by_rank(int index){
        index--;//索引减一才表示左子树的节点个数 
        int result = 0;
        binarySearchTreeNode* p = root;
        while (p != NULL && p->left_size != index) {
            result ^= p->element;
            if (index < p->left_size) {//索引小于左子树节点个数时，说明该节点在左子树 
                p = p->left_child;
            }
            else {//大于的时候，在右子树 
                index -= (p->left_size + 1);//在右子树，更新索引，原索引减去左子树个数和根 
                p = p->right_child;
            }
        }
        if (p == NULL) {//没找到 
            cout << 0 << endl;
            return;
        }
        else {
            result ^= p->element;
            cout << result << endl;
        }
    }
    
	void erase_by_rank(int index){
        index--;//索引减一才表示左子树的节点个数 
        int result = 0;
        binarySearchTreeNode* p = root;//搜索指针 
        binarySearchTreeNode* pp = NULL;//p的父节点 
        while (p != NULL && p->left_size != index) {
            pp = p;
            result ^= p->element;
            if (index < p->left_size) {//索引小于左子树节点个数时，说明该节点在左子树 
                p = p->left_child;
            }
            else {//大于的时候，在右子树 
                index -= (p->left_size + 1);//在右子树，更新索引，原索引减去左子树个数和根 
                p = p->right_child;
            }
        }
        if (p == NULL) {//没找到 
            cout << 0 << endl;
            return;
        }
        result ^= p->element;
        int b = p->element;//索引的位置所对应的元素，剩下的步骤和erase函数类似 
        p = root;
        while (p != NULL && p->element != b) {//更新left_size 
            if (b < p->element) {
                p->left_size--;
                p = p->left_child;
            }
            else {
                p = p->right_child;
            }
        }
        if (p->left_child != NULL && p->right_child != NULL) {//有两个孩子，转换成有0或1个孩子
            binarySearchTreeNode* s = p->right_child;//找p的右子树的最小元素 
            binarySearchTreeNode* ps = p;//s的父节点
            while (s->left_child != NULL) {//小元素在左孩子处 
                s->left_size--;//更新左子树节点个数 
                ps = s;
                s = s->left_child;
            }
            binarySearchTreeNode* q = new binarySearchTreeNode(s->element);//声明一个可以替代p指针的q指针 
            q->left_size = p->left_size;
            q->left_child = p->left_child;
            q->right_child = p->right_child;
            //让q指针作为pp的孩子 
            if (pp == NULL) {
                root = q;
            }
            else if (p == pp->left_child) {
                pp->left_child = q;
            }
            else {
                pp->right_child = q;
            }
            //让pp指向要删除节点(s)的父节点 
            if (ps == p) {
                pp = q;
            }
            else {
                pp = ps;
            }
            delete p;
            p = s;//删除节点替换为s
            //删除节点两个孩子的情况改为0个或1个孩子的情况 
        }
        binarySearchTreeNode* c;//保存删除节点的孩子指针
        if (p->left_child != NULL) {
            c = p->left_child;
        }
        else {
            c = p->right_child;
        }
        //将孩子指针连接上 
        if (p == root) {
            root = c;
        }
        else {
            if (p == pp->left_child) {
                pp->left_child = c;
            }
            else {
                pp->right_child = c;
            }
        }
        size--;//树节点树减一 
        delete p;
        cout << result << endl;
    }
};



int main() {
	int m;
	cin>>m;
	binarySearchTree x;
	for (int i = 0; i < m; i++) {
		int a,b;
		cin>>a>>b;
		if (a == 0) {
			x.insert(b);
		}
		else if (a == 1) {
			x.find(b);
		}
		else if (a == 2) {
			x.erase(b);
		}
		else if (a == 3) {
			x.find_by_rank(b);
		}
		else if (a == 4) {
			x.erase_by_rank(b);
		}
	}
	return 0;
}