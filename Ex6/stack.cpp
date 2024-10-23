#include <iostream>
#include <iomanip>


using namespace std;

template<class T>
class Stack {
private:
    T* stack;
    int stackTop;
    int size;
public:
    Stack(int initial_size = 10) {
        stack = new T[initial_size];
        size = initial_size;
        stackTop = -1;
    }

    ~Stack() {
        delete[] stack;
    }

    bool empty() const {
        return (stackTop == -1);
    }

    int Size() const {
        return stackTop + 1;
    }

    T& top() {
        return stack[stackTop];
    }

    void pop() {
        if (stackTop == -1) {
            return ;
        }
        stackTop--;
    }

    void push(const T& the_ele) {
        if (stackTop == size - 1) {
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

bool is_operator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

int precedence(char op) {
    if (op == '+' || op == '-') return 1; // 低优先级
    if (op == '*' || op == '/') return 2; // 高优先级
    return 0;
}

string postFix(const string& str) {
    Stack<char> operators;
    string postfix;

    for (char c : str) {
        if (is_operator(c)) {
            while (!operators.empty() && operators.top() != '(' &&
                   precedence(operators.top()) >= precedence(c)) {//栈顶的操作符优先级高就弹
                postfix += operators.top();
                operators.pop();
            }
            operators.push(c);
        }
        else if (c == '(') {
            operators.push(c);
        }
        else if (c == ')') {
            while (operators.top() != '(') {
                postfix += operators.top();
                operators.pop();
            }
            operators.pop(); //弹左括号
        }
        else {//数字
            postfix += c;
        }
    }

    while (!operators.empty()) {
        postfix += operators.top();
        operators.pop();
    }
    return postfix;
}

template<class T>
T process(const string& str) {
    string postfix = postFix(str);
    Stack<T> nums;

    for (char c : postfix) {
        if (is_digit(c)) {
            nums.push(c - '0');
        }
        else {
            T a = nums.top();
            nums.pop();
            T b = nums.top();
            nums.pop();
            if (c == '+') nums.push(b + a);
            else if (c == '-') nums.push(b - a);
            else if (c == '*') nums.push(b * a);
            else nums.push(b / a);
        }
    }
    return nums.top();
}

int main() {
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        string str;
        cin >> str;
        double result = process<double>(str);
        cout << fixed << setprecision(2) << result << endl;
    }
    return 0;
}
