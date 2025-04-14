# C++ 数据结构与算法实现
![C++](https://img.shields.io/badge/C++-17/20-blue.svg)
![Platform](https://img.shields.io/badge/Platform-MacOS/Linux-green.svg)
![Status](https://img.shields.io/badge/Status-Completed-brightgreen.svg)

## 📚 项目概述
这是在山东大学计算机科学与技术专业学习期间完成的数据结构与算法实现集合。本项目严格遵循现代C++标准，通过实际编码巩固数据结构与算法理论知识，并针对关键问题进行算法优化与性能分析。

## 🧩 项目结构

### 数据结构实验 (data_structure_exp)
基础数据结构与算法的实现与测试：

| 实验 | 主题 | 文件 |
|-----|------|------|
| Ex1 | 基础算法测试 | test1.cpp, test2.cpp |
| Ex2 | 排序算法 | sorting.cpp |
| Ex3 | 通讯录管理 | adressBook.cpp |
| Ex4 | 链表操作 | chainList.cpp, sort_chainlist.cpp |
| Ex5 | 矩阵操作 | new_matrix.cpp |
| Ex6 | 栈实现 | stack.cpp |
| Ex7 | 队列实现 | queue.cpp |
| Ex8 | 哈希表 | hashTable.cpp, hashChain.cpp |
| Ex9 | 二叉树 | binaryTree.cpp, post_order_output.cpp |
| Ex10 | 哈夫曼编码 | huffmanCode.cpp, minHeap.cpp |
| Ex11 | 二叉搜索树 | binarySearchTree.cpp |
| Ex12 | 图表示 | linkedGraph.cpp |
| Ex13 | 最小生成树 | kTree.cpp, primTree.cpp |

### 数据结构课程设计 (data_structure_course_design)
高级数据结构与算法的深入实现与优化：

| 项目 | 主题 | 核心文件 |
|-----|------|---------|
| Ex1 | 跳表实现 | SkipList.h, skipList.cpp |
| Ex2 | 败者树与多路归并 | minLoseTree.h, minLoseTree.cpp |
| Ex3 | 目录树实现 | catalogTree.h, ex3_main.cpp |
| Ex4 | 网络优化算法 | WeightedDAG.cpp, AmplifierPlacement.cpp |

## 🔍 核心项目详解

### 跳表实现 (Ex1)
高效的随机化数据结构，兼具有序链表的简洁性和平衡树的高效查找性能。
- 平均时间复杂度：查找/插入/删除 - O(log n)
- 空间复杂度：O(n)
- 优化特性：随机化层次结构，快速定位元素

### 败者树与多路归并 (Ex2)
实现了高效的外部排序算法，适用于超大型数据集处理。
- 时间复杂度：O(n log k)，其中k为合并路数
- 优化特性：最小化比较次数，提高I/O效率

### 目录树实现 (Ex3)
模拟文件系统的目录结构管理，支持目录的创建、删除、导航和保存/恢复功能。
- 支持特性：多级目录创建、递归删除、状态保存与恢复
- 实现技术：树形数据结构，文件序列化/反序列化

### 网络优化算法 (Ex4)
解决汽油传送网络中的放大器放置问题，针对加权有向无环图实现了两种算法并进行了性能对比。
- 贪心算法与动态规划方法的实现与比较
- 性能分析：时间效率与解决方案质量对比
- 可视化工具：生成结果分析图表

## 💻 技术规范
- **编程规范**：遵循C++代码风格指南
- **C++标准**：C++17/C++20特性应用
- **代码组织**：模块化设计，接口与实现分离
- **内存管理**：合理的资源分配与释放策略
- **异常处理**：健壮的错误检测与处理机制

## 🛠️ 构建与运行
本项目在MacOS环境下开发，使用Clang++编译器：

```bash
# 创建输出目录
mkdir -p /path/to/project/output

# 编译单个文件
clang++ -std=c++17 -g source_file.cpp -o output/executable_name

# 运行可执行文件
./output/executable_name
```

## 📊 性能分析示例

以网络优化算法(Ex4)为例的性能分析：

| 节点数量 | 贪心算法(ms) | 动态规划(ms) | 性能比 |
|---------|------------|-----------|-------|
| 10      | 0.12       | 0.18      | 1.50  |
| 20      | 0.25       | 0.42      | 1.68  |
| 50      | 0.76       | 1.38      | 1.82  |
| 100     | 1.85       | 3.57      | 1.93  |

## 🎓 学术参考
- Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009). Introduction to Algorithms (3rd ed.). MIT Press.
- Sedgewick, R., & Wayne, K. (2011). Algorithms (4th ed.). Addison-Wesley Professional.
- Knuth, D. E. (1997). The Art of Computer Programming. Addison-Wesley Professional.

## ✨ 项目亮点
- **算法多样性**：从基础数据结构到高级算法的全面实现
- **优化思维**：针对特定问题的算法优化与改进
- **实用导向**：解决实际问题的应用场景设计
- **可视化分析**：直观展示算法性能与结果对比

---

**完成于2025年春季学期**  
山东大学计算机科学与技术学院
