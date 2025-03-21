#include "minLoseTree.h"
#include <iostream>

int main() {
    std::cout << "程序开始执行..." << std::endl;
    
    std::vector<int> values = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    std::cout << "输入数组大小: " << values.size() << std::endl;
    
    std::cout << "创建败者树..." << std::endl;
    LoserTree<int> tree(values);
    
    std::cout << "打印树结构:" << std::endl;
    tree.printTree();
    
    std::cout << "Winner index: " << tree.WinnerIndex() << std::endl;
    std::cout << "Winner value: " << tree.WinnerValue() << std::endl;

    return 0;
}