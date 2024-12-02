#ifndef HEAP_SORT_H
#define HEAP_SORT_H

#include <vector>
#include <algorithm>
#include <random>
#include <iostream>
#include <chrono>

using namespace std;

// 验证排序结果是否正确
bool check(const vector<int>& vec) {
    for (size_t i = 1; i < vec.size(); ++i) {
        if (vec[i - 1] > vec[i]) return false;
    }
    return true;
}


// 实现堆调整
void heapify(vector<int>& vec, size_t n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && vec[left] > vec[largest]) {
        largest = left;
    }

    if (right < n && vec[right] > vec[largest]) {
        largest = right;
    }

    if (largest != i) {
        swap(vec[i], vec[largest]);
        heapify(vec, n, largest);
    }
}

// 堆构建
void makeHeap(vector<int>& vec) {
    size_t n = vec.size();
    for (int i = n / 2 - 1; i >= 0; --i) {
        heapify(vec, n, i);
    }
}

// 实现pop_heap
void popheap(vector<int>& vec, size_t heapSize) {
    if (heapSize > 1) {
        swap(vec[0], vec[heapSize - 1]);
        heapify(vec, heapSize - 1, 0);
    }
}

// 实现堆排序
void heapSort(vector<int>& vec) {
    makeHeap(vec); // 构建堆
    for (size_t i = vec.size(); i > 1; --i) {
        popheap(vec, i); // 移出最大值
    }
}


// 用 STL的sort_heap 排序
void SortHeap(vector<int>& vec) {
    make_heap(vec.begin(), vec.end()); // 转换为堆
    sort_heap(vec.begin(), vec.end()); // 排序
}

// 随机序列生成函数
vector<int> Rdnumber(size_t n, int minVal = 0, int maxVal = 1000000) {
    vector<int> vec(n);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(minVal, maxVal);
    for (auto& val : vec) {
        val = dist(gen);
    }
    return vec;
}

// 有序序列生成函数
vector<int> Odnumber(size_t n) {
    vector<int> vec(n);
    for (size_t i = 0; i < n; ++i) {
        vec[i] = i;
    }
    return vec;
}

// 逆序序列生成函数
vector<int> Rvnumber(size_t n) {
    vector<int> vec(n);
    for (size_t i = 0; i < n; ++i) {
        vec[i] = n - i;
    }
    return vec;
}

// 部分重复序列生成函数
vector<int> Rpnumber(size_t n, int uniqueCount) {
    vector<int> vec(n);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, uniqueCount - 1);
    for (auto& val : vec) {
        val = dist(gen);
    }
    return vec;
}

// 打印向量的内容
void printVector(const vector<int>& vec) {
    for (const auto& val : vec) {
        cout << val << " ";
    }
    cout << endl;
}

#endif 
