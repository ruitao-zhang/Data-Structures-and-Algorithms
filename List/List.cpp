#include "List.h"
#include <iostream>

int main()
{
    List<int> lst;

    std::cout << "push_back:" << std::endl;
    for (int i = 0; i < 5; ++i)
    {
        lst.push_back(i);
        std::cout << "插入: " << i << std::endl;
    }

    std::cout << " ++ 迭代器:" << std::endl;
    for (List<int>::iterator it = lst.begin(); it != lst.end(); ++it)
    {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    std::cout << " -- 迭代器:" << std::endl;
    auto it = lst.end();
    --it; 
    for (; it != lst.begin(); --it)
    {
        std::cout << *it << " ";
    }
    std::cout << *it << std::endl; 

    std::cout << "front: " << lst.front() << std::endl;
    std::cout << "back: " << lst.back() << std::endl;

    std::cout << "push_front:" << std::endl;
    lst.push_front(10);
    lst.push_front(20);
    for (auto it : lst)
    {
        std::cout << it << " ";
    }
    std::cout << std::endl;

    std::cout << "pop_back 和 pop_front:" << std::endl;
    lst.pop_back();
    lst.pop_front();
    for (auto it : lst)
    {
        std::cout << it << " ";
    }
    std::cout << std::endl;

    std::cout << "区间删除:" << std::endl;
    auto start = ++lst.begin();  
    auto end = lst.end();       
    lst.erase(start, end);
    for (auto it : lst)
    {
        std::cout << it << " ";
    }
    std::cout << std::endl;

    std::cout << "清空链表:" << std::endl;
    lst.clear();
    std::cout << lst.size() << std::endl;
    
    return 0;
}
