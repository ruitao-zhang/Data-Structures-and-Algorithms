#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <algorithm>
#include <iostream>
#include <cassert>
#include <queue>
using namespace std;

class UnderflowException { };
class IllegalArgumentException { };
class ArrayIndexOutOfBoundsException { };
class IteratorOutOfBoundsException { };
class IteratorMismatchException { };
class IteratorUninitializedException { };

template <typename Comparable>
class BinarySearchTree
{
protected:
    struct BinaryNode
    {
        Comparable element;
        BinaryNode *left;
        BinaryNode *right;
        int height;  // 记录节点高度

        BinaryNode(const Comparable &theElement, BinaryNode *lt, BinaryNode *rt, int h = 1)
            : element{theElement}, left{lt}, right{rt}, height{h} {}

        BinaryNode(Comparable &&theElement, BinaryNode *lt, BinaryNode *rt, int h = 1)
            : element{std::move(theElement)}, left{lt}, right{rt}, height{h} {}
    };

public:
    BinarySearchTree() : root{nullptr} {}

    BinarySearchTree(const BinarySearchTree &rhs) : root{nullptr}
    {
        root = clone(rhs.root);
    }

    BinarySearchTree(BinarySearchTree &&rhs) : root{rhs.root}
    {
        rhs.root = nullptr;
    }

    ~BinarySearchTree()
    {
        makeEmpty();
    }

    BinarySearchTree &operator=(const BinarySearchTree &rhs)
    {
        BinarySearchTree copy = rhs;
        std::swap(*this, copy);
        return *this;
    }

    BinarySearchTree &operator=(BinarySearchTree &&rhs)
    {
        std::swap(root, rhs.root);
        return *this;
    }

    const Comparable &findMin() const
    {
        if (isEmpty())
            throw UnderflowException{};
        return findMin(root)->element;
    }

    const Comparable &findMax() const
    {
        if (isEmpty())
            throw UnderflowException{};
        return findMax(root)->element;
    }

    bool contains(const Comparable &x) const
    {
        return contains(x, root);
    }

    bool isEmpty() const
    {
        return root == nullptr;
    }

    void printTree( ostream & out = cout ) const
    {
        if( isEmpty( ) )
            out << "Empty tree" << endl;
        else
            printTree( root, out );
    }

    void printLevelOrder(ostream &out = cout) const
    {
        if (isEmpty())
        {
            out << "Empty tree" << endl;
            return;
        }

        std::queue<BinaryNode*> q;
        q.push(root);

        while (!q.empty())
        {
            BinaryNode* node = q.front();
            q.pop();
            out << node->element << " ";

            if (node->left != nullptr)
                q.push(node->left);
            if (node->right != nullptr)
                q.push(node->right);
        }
        out << endl; 
    }

    void makeEmpty()
    {
        makeEmpty(root);
    }

    void insert(const Comparable &x)
    {
        insert(x, root);
    }

    void insert(Comparable &&x)
    {
        insert(std::move(x), root);
    }

    void remove(const Comparable &x)
    {
        remove(x, root);
    }

protected:
    BinaryNode *root;

    void insert(const Comparable &x, BinaryNode *&t)
    {
        if (t == nullptr)
            t = new BinaryNode{x, nullptr, nullptr};
        else if (x < t->element)
            insert(x, t->left);
        else if (t->element < x)
            insert(x, t->right);

        updateHeight(t);
    }

    void insert(Comparable &&x, BinaryNode *&t)
    {
        if (t == nullptr)
            t = new BinaryNode{std::move(x), nullptr, nullptr};
        else if (x < t->element)
            insert(std::move(x), t->left);
        else if (t->element < x)
            insert(std::move(x), t->right);

        updateHeight(t);
    }

    void remove(const Comparable &x, BinaryNode *&t)
    {
        if (t == nullptr)
            return; 

        if (x < t->element)
            remove(x, t->left);
        else if (t->element < x)
            remove(x, t->right);
        else if (t->left != nullptr && t->right != nullptr) 
        {
            t->element = detachMin(t->right)->element;
        }
        else
        {
            BinaryNode *oldNode = t;
            t = (t->left != nullptr) ? t->left : t->right;
            delete oldNode;
        }

        if (t != nullptr)
        {
            updateHeight(t);
            int balance = balanceFactor(t);

            if (balance > 1)
            {
                if (balanceFactor(t->left) < 0)
                    t->left = rotateLeft(t->left);
                t = rotateRight(t);
            }
            else if (balance < -1)
            {
                if (balanceFactor(t->right) > 0)
                    t->right = rotateRight(t->right);
                t = rotateLeft(t);
            }
        }
    }

    BinaryNode *detachMin(BinaryNode *&t)
    {
        if (t == nullptr)
            return nullptr;

        if (t->left == nullptr)
        {
            BinaryNode *minNode = t;
            t = t->right;
            return minNode;
        }
        return detachMin(t->left);
    }

    BinaryNode *findMin(BinaryNode *t) const
    {
        if (t == nullptr)
            return nullptr;
        if (t->left == nullptr)
            return t;
        return findMin(t->left);
    }

    BinaryNode *findMax(BinaryNode *t) const
    {
        if (t != nullptr)
            while (t->right != nullptr)
                t = t->right;
        return t;
    }

    bool contains(const Comparable &x, BinaryNode *t) const
    {
        if (t == nullptr)
            return false;
        else if (x < t->element)
            return contains(x, t->left);
        else if (t->element < x)
            return contains(x, t->right);
        else
            return true; 
    }

    void makeEmpty(BinaryNode *&t)
    {
        if (t != nullptr)
        {
            makeEmpty(t->left);
            makeEmpty(t->right);
            delete t;
        }
        t = nullptr;
    }

    void printTree( BinaryNode *t, ostream & out ) const
    {
        if( t != nullptr )
        {
            printTree( t->left, out );
            out << t->element << endl;
            printTree( t->right, out );
        }
    }

    BinaryNode *clone(BinaryNode *t) const
    {
        if (t == nullptr)
            return nullptr;
        else
            return new BinaryNode{t->element, clone(t->left), clone(t->right), t->height};
    }

    // 计算高度
    int height(BinaryNode *t) const
    {
        return (t == nullptr) ? 0 : t->height;
    }

    // 计算平衡因子
    int balanceFactor(BinaryNode *t) const
    {
        return (t == nullptr) ? 0 : height(t->left) - height(t->right);
    }

    // 更新节点高度
    void updateHeight(BinaryNode *&t)
    {
        if (t != nullptr)
            t->height = std::max(height(t->left), height(t->right)) + 1;
    }

    // 右旋
    BinaryNode* rotateRight(BinaryNode *t)
    {
        BinaryNode *newRoot = t->left;
        t->left = newRoot->right;
        newRoot->right = t;
        updateHeight(t);
        updateHeight(newRoot);
        return newRoot;
    }

    // 左旋
    BinaryNode* rotateLeft(BinaryNode *t)
    {
        BinaryNode *newRoot = t->right;
        t->right = newRoot->left;
        newRoot->left = t;
        updateHeight(t);
        updateHeight(newRoot);
        return newRoot;
    }
};

#endif

