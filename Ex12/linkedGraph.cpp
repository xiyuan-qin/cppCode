#include<iostream>
using namespace std;

template<class T>
class Queue{
public:
    T *queue;
    int queue_front;
    int queue_back;
    int size;
    int capacity;

public:
    Queue(int initial_capacity = 10){
        capacity = initial_capacity;
        queue = new T[capacity];
        size = 0;
        queue_front = 0;
        queue_back = 0;
    }

    ~Queue(){
        delete []queue;
    }

    bool empty() const {
        return size == 0;
    }

    int size_of_queue() const{
        return size;
    }

    T& front() const{
        return queue[queue_front];
    }

    void pop(){
        if(size > 0){
            queue_front = (queue_front + 1) % capacity;
            size--;
        }
    }

    void push(T ele) {
        if (size == capacity) { // 队列满了，需扩容
            T *newQueue = new T[capacity * 2];
            for(int i = 0; i < size; i++){
                newQueue[i] = queue[(queue_front + i) % capacity];
            }
            delete[] queue;
            queue = newQueue;
            capacity *= 2;
            queue_front = 0;
            queue_back = size;
        }
        queue[queue_back] = ele;
        queue_back = (queue_back + 1) % capacity;
        size++;
    }
};

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

public:
    chainNode *next;
public:
    T ele_value;
};

template<class T>
class chainList{//链表
public:

    chainList(){//不给值的构造，创建新的链表
        first_node = NULL;
        list_length = 0;
    }

    chainList(T ele_value){
        first_node = new chainNode<T>(ele_value);
        list_length = 1;
    }

    ~chainList(){
        chainNode<T> *current = first_node;
        while(current != NULL){
            chainNode<T> *temp = current;
            current = current->next;
            delete temp;
        }
    }

    // 按照升序插入节点
    void ordered_insert(const T& value) {
        chainNode<T>* new_node = new chainNode<T>(value);

        if (!first_node || value < first_node->ele_value) {
            new_node->next = first_node;
            first_node = new_node;
        } else {
            chainNode<T>* current = first_node;
            while (current->next && current->next->ele_value < value) {
                current = current->next;
            }
            if (current->ele_value == value) {
                delete new_node; // 避免重复插入相同的值
                return;
            }
            new_node->next = current->next;
            current->next = new_node;
        }
        list_length++;
    }

    void delete_val(const T& value) {
        chainNode<T> *temp = first_node;
        chainNode<T> *prev = NULL;

        while (temp != NULL && temp->ele_value != value) {
            prev = temp;
            temp = temp->next;
        }

        if (temp == NULL) {
            return; // 没有找到要删除的值
        }

        if (prev == NULL) { // 删除的是头节点
            first_node = temp->next;
        } else {
            prev->next = temp->next;
        }
        delete temp;
        list_length--;
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

public:
    int list_length;

public:
    chainNode<T> *first_node;//头指针

};

template<class T>
class linkedGraph {
public:
    int node_num; // 节点数目
    int edge_num; // 边数目 
    int connected_component_num; // 连通分量数目    
    int *connected_component_pos; // 连通分量的位置
    chainList<int> *adj_list; // 邻接表

public:
    // 构造方法
    linkedGraph(int node_num) {
        this->node_num = node_num;
        this->edge_num = 0;
        this->connected_component_num = 0;
        adj_list = new chainList<int>[node_num + 1](); // 下标从1开始
        connected_component_pos = NULL;
    }

    ~linkedGraph(){
        delete[] adj_list;
        if(connected_component_pos != NULL)
            delete[] connected_component_pos;
    }

    // 判断是否存在边
    bool exist_edge(int u, int v) {//寻找是否存在 u->v 的边
        if (u > node_num || v > node_num || u < 1 || v < 1) return false;
        return ( adj_list[u].find(v) != -1 );//第u个节点的邻接表中是否有v
    }

    // 添加边
    void insert_edge(int u, int v) {
        if (u > node_num || v > node_num || u < 1 || v < 1) return ;//节点不存在
        if (exist_edge(u, v)) return;//边已经存在
        adj_list[u].ordered_insert(v);
        adj_list[v].ordered_insert(u);
        edge_num++;
    }

    // 删除边
    void delete_edge(int u, int v) {
        if (u > node_num || v > node_num || u < 1 || v < 1) return;//节点不存在
        if (!exist_edge(u, v)) return;//边不存在
        adj_list[u].delete_val(v);
        adj_list[v].delete_val(u);
        edge_num--;
    }

    // 广度优先搜索，用于连通分量统计
    void bfs(int u, bool *visited) {
        Queue<int> q(node_num + 1);
        q.push(u);
        visited[u] = true;

        while (!q.empty()) {
            int w = q.front(); q.pop();
            chainNode<int> *temp = adj_list[w].first_node;
            while(temp != NULL){
                int v = temp->ele_value;
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
                temp = temp->next;
            }
        }
    }

    // 深度优先搜索,同时统计搜索到的数量和搜索路径
    void dfs(int u, bool *visited, int &num, int *array) {
        visited[u] = true;
        array[num++] = u;

        chainNode<int> *temp = adj_list[u].first_node;
        while(temp != NULL){
            int v = temp->ele_value;
            if (!visited[v]) {
                dfs(v, visited, num, array);
            }
            temp = temp->next;
        }
    }

    // 输出连通分量的个数
    int connected_component() { 
        int result = 0;
        bool *visited = new bool[node_num + 1]{false};

        if(connected_component_pos != NULL)
            delete[] connected_component_pos;
        connected_component_pos = new int[node_num + 1];

        for(int i = 1; i <= node_num; i++) {
            if(!visited[i]) {
                connected_component_pos[result++] = i;
                bfs(i, visited);
            }
        }
        connected_component_num = result;//保存连通分量的个数
        delete[] visited;
        return result;
    }

    //输出所有连通子图中最小点的编号
    void min_node_in_connected_component(){
        // 由于连通分量的最小节点已经是按顺序添加的，可以直接输出
        for(int i = 0; i < connected_component_num; i++) {
            cout << connected_component_pos[i] << " ";
        }
        cout << endl;
    }

    void new_dfs(int start){
        int num = 0;
        int *array = new int[node_num + 1];
        bool *visited = new bool[node_num + 1]{false};

        // 从start开始深度优先搜索，统计num和路径
        dfs(start, visited, num, array);

        cout << num << endl;
        for(int i = 0; i < num; i++) {
            cout << array[i] << " ";
        }
        cout << endl;

        delete[] array;
        delete[] visited;
    }

    void new_bfs(int t){
        int num = 0;
        int *array = new int[node_num + 1];
        bool *visited = new bool[node_num + 1]{false};

        Queue<int> q(node_num + 1);
        q.push(t);
        visited[t] = true;
        
        while (!q.empty()) {
            int w = q.front(); q.pop();

            array[num++] = w;
            chainNode<int> *temp = adj_list[w].first_node;
            while(temp != NULL){
                int v = temp->ele_value;
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
                temp = temp->next;
            }
        }

        cout << num << endl;

        for(int i = 0; i < num; i++) {
            cout << array[i] << " ";
        }
        cout << endl;

        delete[] array;
        delete[] visited;
    }

    void search(int s , int t){
        if(s == t) {
            cout << 0 << endl;
            return;
        }

        bool *visited = new bool[node_num + 1]{false};
        int *distance = new int[node_num + 1]{0};//每个节点到s的距离

        Queue<int> q(node_num + 1);
        q.push(s);
        visited[s] = true;

        while (!q.empty()) {
            int w = q.front(); q.pop();
            chainNode<int> *temp = adj_list[w].first_node;

            while(temp != NULL){
                int v = temp->ele_value;
                if (!visited[v]) {
                    visited[v] = true;
                    distance[v] = distance[w] + 1;//把v到s的距离更新为上一个节点w到s的距离+1
                    
                    if(v == t){//找到t
                        cout << distance[v] << endl;
                        delete[] visited;
                        delete[] distance;
                        return;
                    }
                    q.push(v);
                }
                temp = temp->next;
            }
        }

        cout << -1 << endl;
        delete[] visited;
        delete[] distance;
    }

};

int main(){
    int n , m , s ,t ;
    cin>>n>>m>>s>>t;
    linkedGraph<int> graph(n);
    for(int i = 0 ; i < m ; i++){
        int opt , u , v;
        cin>>opt>>u>>v;
        switch (opt)
        {
        case 0:
            graph.insert_edge(u, v);
            break;
        case 1:
            graph.delete_edge(u, v);
            break;
        }
    }
    
    cout<<graph.connected_component()<<endl;
    graph.min_node_in_connected_component();
    graph.new_dfs(s);
    graph.new_bfs(t);
    graph.search(s, t);
    
    return 0;
}
