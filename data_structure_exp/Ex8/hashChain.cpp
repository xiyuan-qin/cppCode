#include <iostream>
using namespace std;

template<class T>
class chainNode {
    template <class U> friend class chainList;
public:
    chainNode(T ele_value) {
        this->ele_value = ele_value;
        next = NULL;
    }

    chainNode(T ele_value, chainNode *next) {
        this->ele_value = ele_value;
        this->next = next;
    }

public:
    chainNode *next;
    T ele_value;
};

template<class T>
class chainList {
public:
    chainList() {
        first_node = NULL;
        list_length = 0;
    }

    chainList(T ele_value) {
        first_node = new chainNode<T>(ele_value);
        list_length++;
    }

    ~chainList() {
        while (first_node != NULL) {
            chainNode<T> *temp = first_node;
            first_node = first_node->next;
            delete temp;
        }
    }

    void insert(int index, const T& value) {
        if (index > list_length || index < 0) return;

        chainNode<T>* new_node = new chainNode<T>(value);

        if (index == 0) {
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

    void insert(const T& value) {
        insert(0, value);
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

        while (temp->next != NULL && temp->next->ele_value != value) {
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

    void reverse() {
        chainNode<T> *prev = NULL;
        chainNode<T> *current = first_node;
        chainNode<T> *next = NULL;

        while (current != NULL) {
            next = current->next;
            current->next = prev;
            prev = current;
            current = next;
        }

        first_node = prev;
    }

    void print() {
        chainNode<T> *cur_node = first_node;
        while (cur_node != NULL) {
            cout << cur_node->ele_value << " ";
            cur_node = cur_node->next;
        }
        cout << endl;
    }

    int find(T val) {
        int index = 0;
        chainNode<T> *cur_node = first_node;
        while (cur_node != NULL) {
            if (cur_node->ele_value == val) return index;
            cur_node = cur_node->next;
            index++;
        }
        return -1;
    }

    int length() {
        return list_length;
    }

public:
    int list_length = 0;
    chainNode<T> *first_node;
};

int main() {
    chainList<int> *hashTable;
    int D, m;
    int opt, x;
    cin >> D >> m;
    hashTable = new chainList<int>[D];
    for (int i = 0; i < m; i++) {
        cin >> opt >> x;
        int j = x % D;
        bool found = false;
        chainNode<int> *temp = hashTable[j].first_node;

        switch (opt) {
        case 0: // Insert
            while (temp != NULL) {
                if (temp->ele_value == x) {
                    cout << "Existed" << endl;
                    found = true;
                    break;
                }
                temp = temp->next;
            }
            if (!found) {
                hashTable[j].insert(x);
            }
            break;
        case 1: // Find
            while (temp != NULL) {
                if (temp->ele_value == x) {
                    found = true;
                    break;
                }
                temp = temp->next;
            }
            if (found) cout << hashTable[j].length() << endl;
            else cout << "Not Found" << endl;
            break;
        case 2: // Delete
            while (temp != NULL) {
                if (temp->ele_value == x) {
                    found = true;
                    break;
                }
                temp = temp->next;
            }
            if (found) {
                hashTable[j].delete_val(x);
                cout << hashTable[j].length() << endl;
            } else cout << "Delete Failed" << endl;
            break;
        default:
            cout << "Invalid operation" << endl;
            break;
        }
    }
    delete[] hashTable;
    return 0;
}