#include<iostream>
using namespace std;

template<class T>
class hashTable{
public:
    T *table;
    //int size;
    int D;
public:
    hashTable(int initail_size = 10){
        D = initail_size;
        table = new T[D];
        for(int i = 0 ; i < D ; i++){
            table[i] = -1;
        } 
    }
    ~hashTable(){
        delete[] table;
    }

    void insert(T value){
        T index = value % D;
        int count = 0;
        while(table[index % D] != -1){//在散列表相应位置不为空的情况下
            if(count == D) return;
            if(table[index % D] == value){
                cout<<"Existed"<<endl;
                return;
            }
            index++;
            count++;
        }
        table[index % D] = value ;
        cout<<(index%D)<<endl;
    }

    void find(T value){
        for(int i = 0 ; i < D ; i++){
            if(table[i] == value){
                cout<<i<<endl;
                return ;
            }
        }
        cout<<-1<<endl;
    }

    void erase(T value) {
        int index = value % D;
        int start = index;
        bool found = false;

        // 找到需要删除的元素
        while (table[index] != -1) {  // 空位表示未找到
            if (table[index] == value) {
                found = true;
                break;
            }
            index = (index + 1) % D;
            if (index == start) break;
        }

        if (!found) {
            cout << "Not Found" << endl;
            return;
        }

        // 标记删除，并开始调整后续探测路径
        table[index] = -1;
        int count = 0;

        int next = (index + 1) % D;
        while (table[next] != -1) {  // 遇到空位停止,最差情况返回原点
            int original_position = table[next] % D;

            // 判断是否应该前移
            if ((next > index && (original_position <= index || original_position > next)) ||//没有环绕，当original_position位于index和next的中间位置时证明存放在正确位置上，不需要移动
                (next < index && (original_position <= index && original_position > next))) {//环绕了，只有当original_position位于两者中间时才需要移动
                table[index] = table[next];
                table[next] = -1;
                index = next;
                count++;
            }

            next = (next + 1) % D;//继续往后移动
        }

        cout << count << endl;
    }


};

int main(){
    int D , m;
    cin>>D>>m;
    hashTable<int> table(D);
    int opt , x;
    for(int k = 0 ; k < m ; k++){
        cin>>opt>>x;
        switch (opt)
        {
        case 0:
            table.insert(x);
            break;
        case 1:
            table.find(x);
            break;
        case 2:
            table.erase(x);
            break;
        default:
            break;
        }
        
    }
    return 0;
}