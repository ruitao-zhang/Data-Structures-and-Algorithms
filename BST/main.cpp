#include "BinarySearchTree.h"
#include <algorithm>
#include <iostream>
#include <cassert>
#include <queue>

void testRemove() {
    BinarySearchTree<int> bst;

    bst.insert(50);
    bst.insert(30);
    bst.insert(70);
    bst.insert(20);
    bst.insert(40);
    bst.insert(60);
    bst.insert(80);

    std::cout << "Initial tree:" << std::endl;
    bst.printLevelOrder(); (按照层输出）

    // 测试删除树叶节点
    bst.remove(20);
    std::cout << "Remove 20:" << std::endl;
    bst.printLevelOrder();

    // 测试删除有一个子节点的节点
    bst.remove(30);
    std::cout << "Remove 30 (one child):" << std::endl;
    bst.printLevelOrder();

    // 测试删除有两个子节点的节点
    bst.remove(50);
    std::cout << "Remove 50 (two children):" << std::endl;
    bst.printLevelOrder();

    // 测试删除不存在的节点
    bst.remove(100); 
    std::cout << "Remove 100(non-existent):" << std::endl;
    bst.printLevelOrder();

    // 删除节点至只剩一个
    bst.remove(40);
    bst.remove(70);
    bst.remove(80);
    std::cout << "Remove 40 70 and 80:" << std::endl;
    bst.printLevelOrder();

    // 测试删除最后一个节点
    bst.remove(60); 
    std::cout << "Empty tree:" << std::endl;
    bst.printLevelOrder();

    assert(bst.isEmpty()); // 确认树已空

    // 测试空树删除
    bst.remove(10); 
    std::cout << "Remove from empty tree:" << std::endl;
    bst.printLevelOrder();
}

int main() {
    testRemove();
    return 0;
}
