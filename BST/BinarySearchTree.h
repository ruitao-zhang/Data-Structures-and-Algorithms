#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <algorithm>
#include <iostream>
#include <cassert>
#include<queue>
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
public:
    BinarySearchTree() : root{ nullptr } {}

    BinarySearchTree(const BinarySearchTree &rhs) : root{ nullptr }
    {
        root = clone(rhs.root);
    }

    BinarySearchTree(BinarySearchTree &&rhs) : root{ rhs.root }
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

private:
    struct BinaryNode
    {
        Comparable element;
        BinaryNode *left;
        BinaryNode *right;

        BinaryNode(const Comparable &theElement, BinaryNode *lt, BinaryNode *rt)
            : element{ theElement }, left{ lt }, right{ rt } {}

        BinaryNode(Comparable &&theElement, BinaryNode *lt, BinaryNode *rt)
            : element{ std::move(theElement) }, left{ lt }, right{ rt } {}
    };

    BinaryNode *root;

    void insert(const Comparable &x, BinaryNode *&t)
    {
        if (t == nullptr)
            t = new BinaryNode{x, nullptr, nullptr};
        else if (x < t->element)
            insert(x, t->left);
        else if (t->element < x)
            insert(x, t->right);
    }

    void insert(Comparable &&x, BinaryNode *&t)
    {
        if (t == nullptr)
            t = new BinaryNode{std::move(x), nullptr, nullptr};
        else if (x < t->element)
            insert(std::move(x), t->left);
        else if (t->element < x)
            insert(std::move(x), t->right);
    }

    void remove(const Comparable &x, BinaryNode *&t)
    {
        if (t == nullptr)
            return; // Item not found; do nothing

        if (x < t->element)
            remove(x, t->left);
        else if (t->element < x)
            remove(x, t->right);
        else if (t->left != nullptr && t->right != nullptr) // Two children
        {
            t->element = detachMin(t->right)->element;
        }
        else
        {
            BinaryNode *oldNode = t;
            t = (t->left != nullptr) ? t->left : t->right;
            delete oldNode;
        }
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
            return true; // Match
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

    BinaryNode *clone(BinaryNode *t) const
    {
        if (t == nullptr)
            return nullptr;
        else
            return new BinaryNode{t->element, clone(t->left), clone(t->right)};
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
};
#endif
