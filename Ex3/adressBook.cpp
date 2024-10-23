#include<iostream>
using namespace std;
class adressBook{
    friend class linearList;
public:
    adressBook(string name = "" , long long phone_num = 0, int class_num = 0, int dorm = 0){
        this->name = name;
        this->phone_num = phone_num;
        this->class_num = class_num;
        this->dorm = dorm;
    }
    void empty(){
        name = "";
        phone_num = 0;
        class_num = 0;
        dorm = 0;
    }
    
    
protected:
    string name;
    long long phone_num;
    int class_num;
    int dorm;
};

class linearList{
public:
    linearList(int array_length = 10000){
        this->array_length = array_length;
        element = new adressBook[array_length];
        list_size = 0;
    }
    void double_size(){
        adressBook *temp = element;
        element = new adressBook[array_length * 2];
        this->array_length *= 2;
        for(int i = 0; i < array_length / 2 ; i++){
            element[i]= temp[i];
        }
        delete[] temp;
    }

    void insert(string name , long long phone_num , int class_num , int dorm){
        if(list_size == array_length) double_size();//倍增
        element[list_size].name = name;
        element[list_size].phone_num = phone_num;
        element[list_size].class_num = class_num;
        element[list_size].dorm = dorm;
        list_size++;//维护数组现有长度
    }

   void delete_ele(string name) {
    int i = 0;
    for (i = 0; i < list_size; i++) {
        if (element[i].name == name)
            break;
    }
    if (i == list_size) return;  // 如果没有找到要删除的元素，则返回

    for (int j = i; j < list_size - 1; j++) {  // 使用 list_size - 1
        element[j] = element[j + 1];
    }
    element[list_size - 1].empty();
    list_size--;
}

    void edit(string name , int xiangmu , long long new_data){
        int i = 0;
        for(i = 0 ; i < list_size ; i++){
            if(element[i].name == name)
                break;
        }
        switch (xiangmu)
        {
        case 1:
            element[i].phone_num = new_data;
            break;

        case 2:
            element[i].class_num = (int)new_data;
            break;

        case 3:
            element[i].dorm = (int)new_data;
            break;
        }
    }

    int find(string name){
        bool flag = false;
        for(int i = 0 ; i < list_size ; i++){
            if(element[i].name == name){
                flag = true;
                break;
            }
        }
        if(flag) return 1;
        else return 0;
    }

    int dorm(int class_num){
        int result = 0;
        for(int i = 0 ; i < list_size ; i++){
            if(element[i].class_num == class_num)
                result = result ^ element[i].dorm;
        }
        return result;
    }
private:
    adressBook *element;
    int array_length;//数组真实容量
    int list_size;//数组现有长度
};


int main(){
    int n ;
    cin>>n;
    linearList a ;
    for(int index = 0 ;index < n ;index ++){
        int caozuo;
        cin>>caozuo;

        string name;
        long long phone_num;
        int class_num;
        int dorm;
        int xiangmu;
        long long new_data;
        switch (caozuo)
        {
        case 0:
            cin>>name>>phone_num>>class_num>>dorm;
            a.insert(name , phone_num , class_num, dorm);
            break;
        case 1:
            cin>>name;
            a.delete_ele(name);
            break;
        case 2:
            cin>>name>>xiangmu>>new_data;
            a.edit(name, xiangmu , new_data);
            break;
        case 3 :
            cin>>name;
            cout<<a.find(name)<<endl;
            break;
        case 4:
            cin>>dorm;
            cout<<a.dorm(dorm)<<endl;
            break;
        }
       
    }
    return 0;
}