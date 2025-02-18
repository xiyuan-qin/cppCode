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
protected:
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

    ~chainList(){
        while(first_node != NULL){
            chainNode<T> *temp = first_node;
            first_node = first_node->next;
            delete temp;
        }
    }

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
        prev = NULL;
        
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

};


//template<class T>


int main(){
    int num_of_node , num_of_act;
    cin>>num_of_node>>num_of_act;
    chainList<int> list;
    for(int i = 0 ; i < num_of_node ; i++){
        int node;
        cin>>node;
        list.insert(i , node);
    }
   // list.print();
    for(int i = 0 ; i < num_of_act ; i ++){
        int act ;
        cin>>act;
        switch (act )
        {
            int index ,value;
        case 1:
            cin>>index>>value;
            list.insert(index , value);
            break;
        case 2:
            cin>>value;
            list.delete_val(value);
            break;
        case 3:
            list.reverse();
            break;
        case 4:
            cin>>value;
            cout<<list.find(value)<<endl;
            break;
        case 5:{
            chainList<int>:: iterator it(list);
            int result = 0;
            for(int i = 0 ; i < list.length() ; i++){
                result += i ^ *it  ;
                ++it;
            }
            cout<<result<<endl;
        }
            break;

        }
    }
    return 0;
}