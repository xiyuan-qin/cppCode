#include <iostream>
#include <iomanip>


using namespace std;

template<class T>
class Stack {
private:
    T* stack;
    int stackTop;//栈顶
    int size;//栈的大小
public:
    Stack(int initial_size = 10) {//初始设定栈的大小为10
        stack = new T[initial_size];
        size = initial_size;
        stackTop = -1;//栈顶从-1开始
    }

    ~Stack() {
        delete[] stack;
    }

    bool empty() const {//判断是否为空
        return (stackTop == -1);
    }

    int Size() const {//拿取栈的大小
        return stackTop + 1;
    }

    T& top() {//返回栈顶的值
        return stack[stackTop];
    }

    void pop() {//删除栈顶的值
        if (stackTop == -1) {
            return ;
        }
        stackTop--;
    }

    void push(const T& the_ele) {//压入
        if (stackTop == size - 1) {//扩容
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

bool is_operator(char c) {//判断是否为操作符
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

bool is_digit(char c) {//判断是否为数字
    return c >= '0' && c <= '9';
}

int precedence(char op) {//判断操作符的优先级
    if (op == '+' || op == '-') return 1; // 低优先级
    if (op == '*' || op == '/') return 2; // 高优先级
    return 0;
}

string postFix(const string& str) {//转换为后置表达式
    Stack<char> operators;
    string postfix;//存放后置表达式的字符串

    for (char c : str) {
        if (is_operator(c)) {
            while (!operators.empty() && operators.top() != '(' &&
                   precedence(operators.top()) >= precedence(c)) {//栈顶的操作符优先级高就弹
                postfix += operators.top();//栈顶的操作符加入字符串里
                operators.pop();//删除栈顶
            }
            operators.push(c);//往里压
        }
        else if (c == '(') {//左括号就压
            operators.push(c);
        }
        else if (c == ')') {//右括号弹出直到左括号的
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

    while (!operators.empty()) {//弹出剩下的
        postfix += operators.top();
        operators.pop();
    }
    return postfix;
}

template<class T>
T process(const string& str) {//拿出两个再计算，算完了压回去
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
