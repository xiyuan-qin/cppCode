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

    void erase(T value){
        int i = 0;
        int count = 0;
        bool find = false;

        for(i = 0 ; i < D ; i++){
            if(table[i] == value){
                find = true;
                break;
            }
        }
        if(!find){
            cout<<"Not Found"<<endl;
            return ;
        }

        int ori_i = i;
        if(table[i + 1] == -1){//该元素位置后面为空
                table[i] = -1;
                count = 0;
                cout<<count<<endl;
                return ;
        }
        while(true){//该元素位置后面不为空
            table[i % D] = table[(i + 1) % D];
            i++;
            
            if (table[i % D] == -1) {  // 找到空位结束
                table[(i - 1) % D] = -1;
                break;
            }
            
            if(i % D == ori_i){
                table[(i-1) % D] = -1;
                break;
            }
            count++;
        }
        cout<<count<<endl;
        
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