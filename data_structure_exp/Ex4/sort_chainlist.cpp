#include<iostream>
using namespace std;

template<class T>
class chainNode{//节点类
    template <class U> friend class chainList;
public:
    chainNode(T ele_value){
        this->ele_value = ele_value;
        next = NULL;//创建没有后续的节点next置为NULL
    }

    chainNode(T ele_value , chainNode *next){//带下一个位置的构造方法，用于插入
        this->ele_value = ele_value;
        this->next = next;
    }

   /* chainNode(chainNode& the_node){//拷贝构造，但是不会把the_node的下一个值也赋过去
        this->ele_value = the_node.ele_value;
    }*/
public:
    chainNode *next;
    T ele_value;
};

template<class T>
class chainList{//链表
    friend class iterator;
public:

    chainList(){//不给值的构造，创建新的链表
        first_node = NULL;
        list_length = 0;
    }

    chainList(T ele_value){
        first_node = new chainNode<T>(ele_value);
        list_length++;
    }

    /*chainList(chainList& the_list){//拷贝构造
        first_node = new chainNode<T>(the_list.first_node->ele_value);
        chainNode<T> *cur_node = first_node->next;
        chainNode<T> *the_cur_node = the_list.first_node->next;
        while(the_cur_node != NULL){
            cur_node = new chainNode<T>(the_cur_node);
            the_cur_node = the_cur_node->next;
            cur_node = cur_node->next;
        }
    }*/

    /*~chainList(){//meimu d wanyi sima d dongxi jiubu gai xie nizhege b xigou han shu sima dongxi 
        while(first_node != NULL){
            chainNode<T> *temp = first_node;
            first_node = first_node->next;
            delete temp;
        }
    }*/

    void insert(int index, const T& value) {
        if (index > list_length || index < 0) return;
        
        chainNode<T>* new_node = new chainNode<T>(value);
        
        if (index == 0) {  // 插入头部
            new_node->next = first_node;
            first_node = new_node;
        } else {
            chainNode<T>* temp = first_node;
            for (int i = 0; i < index - 1; i++) {
                temp = temp->next;
            }
            new_node->next = temp->next;
            temp->next = new_node;
        }
        list_length++;
    }

    void insert(T ele_value){
        if(first_node == NULL){
            first_node = new chainNode<T>(ele_value);
        }
        else{
            chainNode<T> *cur = first_node;
            while(cur->next != NULL){
                cur = cur->next;
            }
            cur->next = new chainNode<T>(ele_value);
        }
        list_length++;
    }
    
    void delete_val(const T& value) {
        chainNode<T> *temp = first_node;
        if (temp == NULL) return;
        
        if (temp->ele_value == value) {
            first_node = first_node->next;
            delete temp;
            list_length--;
            return;
        }
        
        while (temp->next != NULL && temp->next->ele_value != value) {//一定要先判断不为空，再找，顺序不能错！！
            temp = temp->next;
        }
        
        if (temp->next == NULL) {
            cout << -1 << endl;
            return;
        }
        
        chainNode<T> *temp2 = temp->next;
        temp->next = temp->next->next;
        delete temp2;
        list_length--;
    }

    void reverse(){
        /*chainNode<T> *new_head;//新表的表头
        chainNode<T> *new_cur_head = new_head;//新表可移动指针
        chainNode<T> *cur_node;//原先表可移动指针
        while(first_node->next != NULL){//当只剩下表头的时候退出循环
            cur_node = first_node;
            while(cur_node->next->next != NULL){//利用cur_node找到链表倒数第二个
                cur_node = cur_node->next;
            }
            new_head->next = cur_node->next;
            cur_node->next = NULL;
            new_head = new_head->next;
        }
        //对剩下的一个表头的处理
        new_cur_head->next = first_node;
        first_node = new_head;*/

        chainNode<T> *prev = NULL;
        chainNode<T> *current = first_node;
        chainNode<T> *next = NULL;
        
        while (current != NULL) {
            next = current->next;  // 暂存下一个节点
            current->next = prev;  // 反转指针
            prev = current;        // 前移 prev
            current = next;        // 前移 current
        }

        first_node = prev;  // 更新头节点
        
    }

    void print(){
        chainNode<T> *cur_node = first_node;
        while(cur_node != NULL){
            cout<<cur_node->ele_value<<" ";
            cur_node = cur_node->next;
        }
        cout<<endl;
    }

    int find(T val){
        int index = 0;
        chainNode<T> *cur_node = first_node;
        while(cur_node != NULL){
            if(cur_node->ele_value == val) return index;
            cur_node = cur_node->next;
            index++;
        }
        return -1 ; //未找到
    }

    int length(){
        return list_length;
    }

    /*void binSort(int range){
        chainNode<T> **bottom , **top;
        bottom = new chainNode<T>* [range + 1 ];
        top = new chainNode<T>* [range + 1 ];
        for(int b = 0 ; b <= range ; b++){
            bottom[b] = NULL;
        }

        //链表节点分配到箱子
        for(; first_node != NULL ; first_node = first_node->next){
            int theBin = first_node->ele_value;
            //箱子为空
            if(bottom[theBin] == NULL){
                bottom[theBin] = top[theBin] = first_node;
            }
            else{//箱子不空
                top[theBin]->next = first_node;
                top[theBin] = first_node;
            }
        }
        //收集到有序链表
        chainNode<T> *y = NULL;
        for(int theBin = 0 ; theBin <= range ; theBin++){
            if(bottom[theBin] != NULL){
                //箱子不空
                if(y == NULL)
                    first_node = bottom[theBin];
                else y->next = bottom[theBin];
                y = top[theBin];
            }
        }
        if(y != NULL){
            y->next = NULL;
        }
        //first_node = y;
        delete [] bottom;
        delete [] top;
    }*/
   void insertionSort() {
        if (!first_node || !first_node->next) return;//0 或者 1直接返回

        chainNode<T>* sorted = nullptr; // 用于存放排序好的链表
        chainNode<T>* current = first_node;

        while (current != nullptr) {//遍历链表
            chainNode<T>* next = current->next;
            if (!sorted || sorted->ele_value >= current->ele_value) {//sorted 为空或 current 的值小于等于 sorted 的头节点值
                current->next = sorted;//插入头部
                sorted = current;
            } else {//合适位置插入
                chainNode<T>* temp = sorted;
                while (temp->next && temp->next->ele_value < current->ele_value) {
                    temp = temp->next;
                }
                current->next = temp->next;
                temp->next = current;
            }
            current = next;
        }
            first_node = sorted;
    }

public:
    int list_length = 0;

public:
    chainNode<T> *first_node;//头指针

//迭代器
class iterator{
private: 
    chainNode<T> *cur_node;
    chainNode<T> *head;
public:
    iterator(const chainList<T> &the_list){
        cur_node = the_list.first_node;
        head = the_list.first_node;
    }

    iterator(chainNode<T> *the_node){
        cur_node = the_node;
    }

    T& operator *(){
        return cur_node->ele_value;
    }

    //前置++
    iterator& operator++(){
        cur_node = cur_node->next;
        return *this;
    }

    //后置++
    iterator operator++(int){
        iterator temp = *this;
        cur_node = cur_node->next;
        return temp;
    }

    bool operator!=(const iterator& other) const {
        return cur_node != other.cur_node;
    }

    bool operator==(const iterator& other) const {
        return cur_node == other.cur_node;
    }

   
};
    // 获取迭代器的起始位置
    iterator begin() const {
        return iterator(first_node);
    }

    // 获取迭代器的结束位置
    iterator end() const {
        return iterator(nullptr);
    }


};

template<class T>
int yihuohe(chainList<T> &list) {
    chainNode<T> *cur = list.first_node;
    int result = 0;
    int index = 0;
    while (cur != NULL) {
        result += index ^ cur->ele_value;
        cur = cur->next;
        index++;
    }
    return result;
}

template<class T>
chainList<T> merge(chainList<T> list1 , chainList<T> list2){
    chainList<T> list3;
    typename chainList<T>::iterator it1 = list1.begin();
    typename chainList<T>::iterator it2 = list2.begin();

    while (it1 != list1.end() && it2 != list2.end()) {
        if (*it1 <= *it2) {
            list3.insert(*it1);
            ++it1;
        } else {
            list3.insert(*it2);
            ++it2;
        }
    }

    // 插入剩余元素
    while (it1 != list1.end()) {
        list3.insert(*it1);
        ++it1;
    }
    while (it2 != list2.end()) {
        list3.insert(*it2);
        ++it2;
    }

    return list3;
}


int main(){
    int N,M;
    cin>>N>>M;
    chainList<int> list1, list2;
    int value;
    for(int i = 0 ; i < N ; i++){
        cin>>value;
        list1.insert(i , value);
    }
    for(int i = 0 ; i < M ; i++){
        cin>>value;
        list2.insert(i , value);
    }
    list1.insertionSort();
    list2.insertionSort();
    cout<<yihuohe(list1)<<endl<<yihuohe(list2)<<endl;
    chainList<int> list3 = merge(list1, list2);
    
    //list3.print();
    //cout<<yihuohe(list1)<<endl;
    cout<<yihuohe(list3)<<endl;
    return 0;
}

