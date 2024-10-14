#include <iostream>
#include <stdexcept>
#include <iomanip>  
#include <cmath>    

template <typename T>
class SingleLinkedList {
private:
    struct Node {
        T data;
        Node* next;
        Node(T _data) : data(_data), next(nullptr) {}
    };

    Node* head;       // 链表头指针
    Node* currentPos; // 当前指针

public:
    // 空链表构造函数
    SingleLinkedList() : head(nullptr), currentPos(nullptr) {}

    // 使用 initializer_list 初始化链表
    SingleLinkedList(std::initializer_list<T> initList) : head(nullptr), currentPos(nullptr) {
        for (const T& val : initList) {
            insertAtEnd(val); 
        }
        currentPos = getTail();  
    }

    // 深拷贝
    SingleLinkedList(const SingleLinkedList& other) : head(nullptr), currentPos(nullptr) {
        Node* temp = other.head;
        while (temp != nullptr) {
            insertAtEnd(temp->data);
            temp = temp->next;
        }
        currentPos = getTail();  
    }

    // 深拷贝
    SingleLinkedList& operator=(const SingleLinkedList& other) {
        if (this != &other) {  // 防止自我赋值
            clear();  
            Node* temp = other.head;
            while (temp != nullptr) {
                insertAtEnd(temp->data);
                temp = temp->next;
            }
            currentPos = getTail();  
        }
        return *this;
    }

    // 判断链表是否为空
    bool isEmpty() const {
        return head == nullptr;
    }

    // 返回当前位置的值
    T getCurrentVal() const {
        if (currentPos == nullptr) {
            throw std::runtime_error("Empty current position! Can't get value!");
        }
        return currentPos->data;
    }

    // 设置当前位置的值
    void setCurrentVal(const T &_val) {
        if (currentPos == nullptr) {
            throw std::runtime_error("Empty current position! Can't set value!");
        }
        currentPos->data = _val;
    }

    // 在 currentPos 后插入一个新元素
    void insert(T _val) {
        if (isEmpty()) {
            head = new Node(_val);
            currentPos = head;
        } else {
            Node* newNode = new Node(_val);
            newNode->next = currentPos->next;
            currentPos->next = newNode;
            currentPos = newNode;  // 更新 currentPos 为新插入的节点
        }
    }

    // 在链表尾部插入元素，确保顺序插入
    void insertAtEnd(T _val) {
        Node* newNode = new Node(_val);
        if (isEmpty()) {
            head = newNode;
            currentPos = head;
        } else {
            Node* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }

    // 获取链表的尾节点
    Node* getTail() const {
        if (isEmpty()) return nullptr;
        Node* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        return temp;
    }

    // 删除 currentPos 后面的元素
    void remove() {
        if (isEmpty()) {
            return; // 空链表，直接返回
        }
        if (head == currentPos && currentPos->next == nullptr) {
            // 链表只有一个节点
            delete currentPos;
            head = nullptr;
            currentPos = nullptr;
        } else if (currentPos != nullptr && currentPos->next != nullptr) {
            // 删除 currentPos 后的节点
            Node* temp = currentPos->next;
            currentPos->next = temp->next;
            delete temp;
        }
    }

    // 打印链表所有元素
    void printList() const {
        Node* temp = head;
        while (temp != nullptr) {
            // 检查是否为浮点数，并根据小数部分是否存在决定输出格式
            if (std::is_floating_point<T>::value) {
                if (std::floor(temp->data) == temp->data) {
                    std::cout << std::fixed << std::setprecision(1) << temp->data << " ";
                } else {
                    std::cout << temp->data << " ";
                }
            } else {
                std::cout << temp->data << " ";
            }
            temp = temp->next;
        }
        std::cout << std::endl;
    }

    // 查找链表中的元素
    bool find(const T& val) {
        Node* temp = head;
        while (temp != nullptr) {
            if (temp->data == val) {
                currentPos = temp; // 找到值，更新 currentPos
                return true;
            }
            temp = temp->next;
        }
        return false;
    }

    // 清空链表
    void clear() {
        Node* temp = head;
        while (temp != nullptr) {
            Node* nextNode = temp->next;
            delete temp;
            temp = nextNode;
        }
        head = nullptr;
        currentPos = nullptr;
    }

    // 释放链表内存
    ~SingleLinkedList() {
        clear();
    }
};
int main(){
//SingleLinkedList<int> a;
SingleLinkedList<int> a{1,2,3,4,5};
int a_get = a.getCurrentVal();
std::cout << a_get << std::endl; //测试【取值】操作是否正确，包括可能的空链表测试(//部分)，每种情况9分
//SingleLinkedList<char> b;
SingleLinkedList<char> b{'a','c','e'};
b.setCurrentVal('g');
b.printList(); //测试【定义值】操作是否正确，包括可能的空链表测试(//部分)，每种情况9分
SingleLinkedList<char> e;
std::cout << b.isEmpty() << " "
<< e.isEmpty() << std::endl; //测试【空链表判断】操作是否正确，18分
SingleLinkedList<double> c1;
SingleLinkedList<double> c2{1.5,2.0,9.0,4.7};
SingleLinkedList<double> c3;
c3 = c2;
c1.insert(3.0);
c2.insert(8.8);
bool f_c3 = c3.find(2.0);
c3.insert(8.8);
c1.printList();
c2.printList();
c3.printList(); //测试【插入】操作是否正确，每种情况6分
e.remove();
c1.remove();
bool f = c2.find(9.0);
c2.remove();
e.printList();
c1.printList();
c2.printList(); //测试【删除】操作是否正确，每种情况6分
return 0;
}
