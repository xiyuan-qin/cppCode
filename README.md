# cppCode
C++ Data Structures and Algorithms Implementation

## 项目简介
这是在山东大学计算机科学与技术专业学习期间开发的高性能C++数据结构与算法实现库。本项目严格遵循现代C++标准(C++17/20)，专注于时间/空间复杂度优化和内存管理效率。

## 技术规范
- **编程规范**: 遵循Google C++代码风格指南
- **C++标准**: C++17/C++20
- **内存管理**: 智能指针、RAII原则实现
- **异常处理**: 完整的异常体系设计
- **单元测试**: 使用GoogleTest框架

## 项目结构
```
.
├── data_structure/                # 数据结构实现
│   ├── linear/                   # 线性结构
│   │   ├── vector/              # 动态数组
│   │   ├── linked_list/         # 链表家族
│   │   └── stack_queue/         # 栈与队列
│   ├── tree/                     # 树形结构
│   │   ├── binary_tree/         # 二叉树
│   │   ├── balanced_tree/       # 平衡树
│   │   └── multi_way_tree/      # 多路树
│   ├── graph/                    # 图论
│   │   ├── representation/      # 图的表示
│   │   └── algorithms/          # 图算法
│   └── advanced/                 # 高级数据结构
│       ├── hash/                # 哈希结构
│       └── disjoint_set/        # 并查集
├── algorithm/                     # 算法实现
│   ├── sorting/                  # 排序算法
│   ├── searching/                # 搜索算法
│   └── dynamic_programming/      # 动态规划
├── utils/                         # 工具类
│   ├── timer/                    # 性能计时器
│   └── memory_tracker/           # 内存分析
├── tests/                         # 单元测试
└── benchmark/                     # 性能基准测试
```

## 主要内容

### 数据结构实现与复杂度分析

| 数据结构 | 时间复杂度(平均) | 空间复杂度 | 优化特性 |
|---------|----------------|-----------|---------|
| 动态数组 | 访问: O(1), 插入: O(n) | O(n) | 缓存友好, SSO优化 |
| 链表    | 访问: O(n), 插入: O(1) | O(n) | 内存池分配 |
| 红黑树  | 查找/插入/删除: O(log n) | O(n) | 自平衡优化 |
| 哈希表  | 查找/插入/删除: O(1)* | O(n) | 开放寻址/链式解决冲突 |

\* 平均情况下，最坏情况为O(n)

### 算法实现

- **排序算法**
  - 快速排序 (含内省排序优化)
  - 归并排序 (自适应变体)
  - 堆排序
  - 基数排序

- **图算法**
  - Dijkstra最短路径 (斐波那契堆优化)
  - A*搜索算法
  - Kruskal/Prim最小生成树
  - 强连通分量算法

## 性能基准测试

使用Google Benchmark框架进行的性能测试结果:

```
BM_VectorPushBack/1000    14 ns         14 ns    50104050
BM_ListInsert/1000        56 ns         56 ns    12504032
BM_RBTreeInsert/1000      78 ns         78 ns     8944102
BM_HashTableLookup/1000    7 ns          7 ns    96231541
```

## 代码示例与模式

### 模板元编程示例
```cpp
// 编译期斐波那契计算
template<int N>
struct Fibonacci {
    static constexpr int value = Fibonacci<N-1>::value + Fibonacci<N-2>::value;
};

template<> struct Fibonacci<0> { static constexpr int value = 0; };
template<> struct Fibonacci<1> { static constexpr int value = 1; };
```

### CRTP设计模式
```cpp
template <typename Derived>
class TreeTraversal {
public:
    void traverseInOrder() {
        static_cast<Derived*>(this)->traverseImpl();
    }
};

class BinarySearchTree : public TreeTraversal<BinarySearchTree> {
    friend class TreeTraversal<BinarySearchTree>;
private:
    void traverseImpl() {
        // 具体实现
    }
};
```

## 环境配置

### 编译要求
- C++17/20兼容编译器 (GCC 9+, Clang 10+, MSVC 19.20+)
- CMake 3.15+
- 支持的平台: Linux, macOS, Windows

### 构建指令
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
```

## 学术参考
- Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009). Introduction to Algorithms (3rd ed.). MIT Press.
- Meyers, S. (2014). Effective Modern C++: 42 Specific Ways to Improve Your Use of C++11 and C++14. O'Reilly Media.
- Knuth, D. E. (1997). The Art of Computer Programming, Volume 1: Fundamental Algorithms (3rd ed.). Addison-Wesley Professional.

## 贡献指南

欢迎贡献代码，提交前请确保:
1. 代码通过所有单元测试
2. 维持或改进算法复杂度
3. 遵循项目代码规范
4. 提供完整文档和单元测试

## 持续集成

本项目使用GitHub Actions进行CI/CD:
- 自动构建与测试
- 代码质量检查 (clang-tidy, cppcheck)
- 代码覆盖率报告 (lcov)
- 性能回归测试

## 持续更新
本项目持续维护中，下一阶段开发计划:
- 协程与异步算法支持
- SIMD优化关键算法
- GPU加速计算支持
- 分布式算法实现
