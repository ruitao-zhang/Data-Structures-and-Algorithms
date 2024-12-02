#include "HeapSort.h" 

void test(const string& description, vector<int> data) {
    cout << "测试: " << description << endl;

    vector<int> hand_data = data;
    vector<int> stl_data = data;

    // 手动堆排序测试
    auto start_hand = chrono::high_resolution_clock::now();
    heapSort(hand_data);
    auto end_hand = chrono::high_resolution_clock::now();

    // 检查手动堆排序是否正确
    if (!check(hand_data)) {
        cout << "手动失败" << endl;
        return;
    }
  
    // 计算用时
    auto time_hand = chrono::duration_cast<chrono::milliseconds>(end_hand - start_hand).count();
    cout << "手动用时: " << time_hand << " ms" << endl;

    // STL 堆排序测试
    auto start_stl = chrono::high_resolution_clock::now();
    SortHeap(stl_data);
    auto end_stl = chrono::high_resolution_clock::now();

    // 检查 STL 堆排序是否正确
    if (!check(stl_data)) {
        cout << "STL失败" << endl;
        retur
          n;
    }
  
    //计算用时
    auto time_stl = chrono::duration_cast<chrono::milliseconds>(end_stl - start_stl).count();
    cout << "STL sort_heap用时: " << time_stl << " ms" << endl;

    // 比较用时
    cout << "比较: ";
    if (time_hand < time_stl) {
        cout << "手动的比sort_heap快" << (time_stl - time_hand) << " ms" << endl;
    } else {
        cout << "sort_heap比手动的快" << (time_hand - time_stl) << " ms" << endl;
    } 

    cout << endl;
}

int main() {
    const size_t n = 1000000; // 测试序列长度

    // 测试随机序列
    vector<int> a = Rdnumber(n);
    test("随机序列", a);

    // 测试有序序列
    vector<int> b = Odnumber(n);
    test("有序序列", b);

    // 测试逆序序列
    vector<int> c = Rvnumber(n);
    test("逆序序列", c);

    // 测试部分重复序列
    vector<int> d = Rpnumber(n, 1000); // 设有1000个不同值
    test("部分元素重复序列", d);

    return 0;
}
