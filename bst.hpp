#ifndef BST_ASSIGNMENT_HPP
#define BST_ASSIGNMENT_HPP

#include <iostream>
#include <algorithm>

template <typename T>
class BST
{
public:
    class Node 
    {
    public:
        T key;
        int height = 0;
        Node* left = nullptr;
        Node* right = nullptr;
        Node* parent = nullptr;

        Node(){}

        Node(T k, Node* input_node = nullptr)
        {
            key = k;
            parent = input_node;
        }
    };

private:
    Node* root_ = nullptr;
    unsigned int size_ = 0;

public:
    // Default constructor.  No action required on this one.
    BST(); 

    // Destructor.
    ~BST();
    // insert
    // insert the key k into the BST while maintaining the BST property
    void insert(T k);

    // successor
    // Return a pointer to the node containing the smallest key larger 
    // than k
    Node* successor(T k);

    // delete the minimum
    // Erase the minimum key in the tree
    void delete_min();

    // erase
    // Locate the key k in the tree and remove it
    void erase(T k);

    // Implement a right rotation about the node pointed to by 
    // node
    void rotate_right(Node* node);

    // Returns the number of keys in the tree
    // we implement this for you, but it is up to you to correctly
    // update the size_ variable
    unsigned size();

    // Prints out the keys in the tree via an in-order traversal
    void print();

    // Returns a pointer to the node containing the key k
    Node* find(T k);

    // Creates a vector holding the keys in the tree by
    // doing an in-order traversal
    std::vector<T> make_vec();

    // The next two functions are to check your height values 
    std::vector<int> your_postorder_heights();

    std::vector<int> real_postorder_heights();

    // get_root_value returns the value stored at the root
    T get_root_value();

    // Return a pointer to the node containing the minimum key in the tree
    Node* min();

private: 
    // Assumes that the subtrees rooted at node's children have 
    // correct heights and then walks up the tree from node to the root 
    // correcting the heights.
    void fix_height(Node* node);

    // helper function for the destructor
    void delete_subtree(Node* node);

    // returns pointer to minimum node in subtree rooted by node
    // Assumes node is not nullptr
    Node* min(Node* node);

    // helper function for print
    void print(Node* node);

    // helper function for make_vec
    void make_vec(Node* node, std::vector<T>& vec);

    void your_postorder_heights(Node* node, std::vector<int>& vec);

    int real_postorder_heights(Node* node, std::vector<int>& vec);

};

// Default constructor
template <typename T>
BST<T>::BST()
{
}

// Destructor
template <typename T>
BST<T>::~BST()
{
    delete_subtree(root_);
}

// helper function for destructor
template <typename T>
void BST<T>::delete_subtree(Node* node)
{
    if(node==nullptr)
    {
        return;
    }
    delete_subtree(node->left);
    delete_subtree(node->right);
    delete node;
}

// Assumes that the subtrees rooted at node's children have correct heights 
// and then walks up the tree from node to the root correcting the heights.
template <typename T>
void BST<T>::fix_height(Node* node) {
    Node* curr = node;
    while (curr != nullptr) {
        if (curr->left == nullptr && curr->right == nullptr) {
            curr->height = 0;
        } else if(curr->left == nullptr) {
            curr->height = curr->right->height + 1;
        } else if(curr->right == nullptr) {
            curr->height = curr->left->height + 1;
        } else {        
            curr->height = std::max(curr->left->height, curr->right->height) + 1;
        }
        curr = curr->parent; 
    }
}

// Insert key "k" into the tree. If "k" already exists, no action is taken.
template <typename T>
void BST<T>::insert(T k)
{
    // Node will iterate down through the tree starting from the root
    Node* node = root_;
    // Prev_node will hold node's parent
    Node* prev_node = node;
    bool went_right;

    if(node == nullptr) {
        root_ = new Node(k);
        fix_height(root_);
        ++size_;
        return;
    }
    while(node != nullptr) {
        prev_node = node;
        if(k < node->key) {
            node = node->left;
            went_right = false;
        }
        else if (k > node->key) {
            node = node->right;
            went_right = true;
        }
        // Item already in set
        else {
            return;
        }
    }

    // New node is either left or right child of prev_node
    if(went_right) {
        prev_node->right = new Node(k, prev_node);
        fix_height(prev_node->right);
    }
    else {
        prev_node->left = new Node(k, prev_node);
        fix_height(prev_node->left);
    }
    ++size_;
}

// Return a pointer tothe node containing the minimum key in the tree larger than "k"
template <typename T>
typename BST<T>::Node* BST<T>::successor(T k)
{
    Node* k_node = find(k);
    Node* curr = root_;
    // If "k" does not exist in BST.
    if (k_node == nullptr) {
        return nullptr;
    }

    // Case 1: k_node has right child.
    if (k_node->right != nullptr) {
        return min(k_node->right);
    }

    // Case 2: k_node has no right child.
    Node* p = k_node->parent;
    while (p != nullptr && k_node == p->right) {
        k_node = p;
        p = p->parent;
    }
    return p;
}

// Remove the minimum key from the tree.
template <typename T>
void BST<T>::delete_min()
{
    if (root_ != nullptr) {
        // Store minimum key into min_node.
        Node* min_node = min();
        // If min_node is the root.
        if (min_node == root_) {
            // Check for right subtree.
            if (min_node->right != nullptr) {
                // Make right child of min_node the new root and update pointers.
                root_ = min_node->right;
                root_->parent = nullptr;
                --size_;
                return;
            } else {
                return;
            }
        }
        Node* pn = min_node->parent;
        Node* rn = min_node->right;
        // If min_node has a right child.
        if (min_node->right != nullptr) {
            // Update pointers to parent of min_node.
            rn->parent = min_node->parent;
            min_node->parent->left = rn;
        // If min_node has no right child.
        } else {
            min_node->parent = nullptr;
            if (pn->left == min_node) {
                pn->left = nullptr;
            }
        }
        --size_;
        fix_height(pn);
    }
}

// Locate the node containing key "k" and remove it.
template <typename T>
void BST<T>::erase(T k)
{
    // If node_to_remove does not exist.
    Node* node_to_remove = find(k);
    if (node_to_remove == nullptr) {
        return;
    }

    // Case 1: node_to_remove has no children (leaf node).
    if (node_to_remove->left == nullptr && node_to_remove->right == nullptr) {
        if (node_to_remove != root_) {
            if (node_to_remove->parent->left == node_to_remove) {
                node_to_remove->parent->left = nullptr;
            } else {
                node_to_remove->parent->right = nullptr;
            }
        } else {
            root_ = nullptr;
        }
        fix_height(node_to_remove);
        free(node_to_remove);
        --size_;
    }

    // Case 2: node_to_remove has two children
    else if (node_to_remove->left && node_to_remove->right) {
        Node* p = nullptr;
        Node* temp;
        // Compute inorder successor
        temp = node_to_remove->right;
        while (temp->left != nullptr) {
            p = temp;
            temp = temp->left;
        }
        // Check if inorder successor = node_to_remove.
        if (p != nullptr) {
            p->left = temp->right;
        // If inorder successor = node_to_remove...
        } else {
            node_to_remove->right = temp->right;
        }
        node_to_remove->key = temp->key;
        fix_height(temp);
        free(temp);
        --size_;
    } 
    
    // Case 3: node_to_remove only has one child
    else {
        Node* n = (node_to_remove->left)? node_to_remove->left: node_to_remove->right;
        // If the node to be deleted is not a root node, set parent to n (it's child).
        if  (node_to_remove != root_) { 
            if (node_to_remove == node_to_remove->parent->left) {
                node_to_remove->parent->left = n;
            } else {
                node_to_remove->parent->right = n;
            }
        //If the node to be deleted is a root node, set the root to the child.
        } else {
            root_ = n;
        }
        fix_height(node_to_remove);
        free(node_to_remove);
        --size_;
    }
}

// Implement a right rotation about the node pointed to by "node."
template <typename T>
void BST<T>::rotate_right(Node* node)
{
    Node * move_up_node = node->left;
    if(node == nullptr || move_up_node == nullptr) {
        return;
    }

    else {
        // Set edge between node and its current inner grandchild
        node->left = move_up_node->right;
        if (node->left != nullptr) {
            node->left->parent = node;
            fix_height(node->left->parent);
        }
        // Set edge between parent of node and the node that moves up
        move_up_node->parent = node->parent;
        if(node == root_) {
            root_ = move_up_node;
            fix_height(root_);
        }
        else if(node == node->parent->right) {
            node->parent->right = move_up_node;
            fix_height(node->parent->right);
        }
        else {
            node->parent->left = move_up_node;
            fix_height(node->parent->left);
        }
        // Set edge between node and move_up_node
        node->parent = move_up_node;
        move_up_node->right = node;
        fix_height(move_up_node->right);
    }   
}

// returns a pointer to the minimum node
template <typename T>
typename BST<T>::Node* BST<T>::min()
{
    if(root_ == nullptr)
    {
        return root_;
    }
    else
    {
        return min(root_);
    }
}

// returns pointer to minimum node in the subtree rooted by node
// Assumes node is not nullptr
template <typename T>
typename BST<T>::Node* BST<T>::min(Node* node)
{
    while(node->left != nullptr)
    {
        node = node->left;
    } 
    return node;
}

// returns a pointer to node with key k
template <typename T>
typename BST<T>::Node* BST<T>::find(T k)
{
    Node* node = root_;  
    while(node != nullptr && node->key != k)
    {
        node = k < node->key ?  node->left : node->right;
    }
    return node;  
}

template <typename T>
unsigned BST<T>::size()
{
    return size_;
}

// prints out the keys in the tree using in-order traversal
template <typename T>
void BST<T>::print()
{
    print(root_);
}

// you can modify what is printed out to suit your needs
template <typename T>
void BST<T>::print(Node* node)
{
    if(node == nullptr)
    {
        return;
    }
    print(node->left);
    std::cout << node->key << " height " << node->height << '\n';
    print(node->right);
}

// This is used in our testing, please do not modify
template <typename T>
typename std::vector<T> BST<T>::make_vec()
{
    std::vector<T> vec;
    vec.reserve(size_);
    make_vec(root_, vec);
    return vec;
}

// This is used for our testing, please do not modify
template <typename T>
void BST<T>::make_vec(Node* node, std::vector<T>& vec)
{
    if(node == nullptr)
    {
        return;
    }
    make_vec(node->left, vec);
    vec.push_back(node->key);
    make_vec(node->right, vec);
    
}

// This is used for our testing, please do not modify
template <typename T>
void BST<T>::your_postorder_heights(Node* node, std::vector<int>& vec)
{
    if(node == nullptr)
    {
        return;
    }
    your_postorder_heights(node->left, vec);
    your_postorder_heights(node->right, vec);
    vec.push_back(node->height);
}

// This is used for our testing, please do not modify
template <typename T>
int BST<T>::real_postorder_heights(Node* node, std::vector<int>& vec)
{
    if(node == nullptr)
    {
        return -1;
    }
    int left_height = real_postorder_heights(node->left, vec);
    int right_height = real_postorder_heights(node->right, vec);
    int node_height = 1 + std::max(left_height, right_height);
    vec.push_back(node_height);
    return node_height;
}

// This is used for our testing, please do not modify
template <typename T>
typename std::vector<int> BST<T>::your_postorder_heights()
{
    std::vector<int> vec;
    vec.reserve(size_);
    your_postorder_heights(root_, vec);
    return vec;
}

// This is used for our testing, please do not modify
template <typename T>
typename std::vector<int> BST<T>::real_postorder_heights()
{
    std::vector<int> vec;
    vec.reserve(size_);
    real_postorder_heights(root_, vec);
    return vec;
}

// This is used for our testing, please do not modify
template <typename T>
T BST<T>::get_root_value()
{
    if(root_ == nullptr)
    {
        //std::cout << "Calling get_root_value on empty tree\n";
        T dummy {};
        return dummy;
    }
    else
    {
        return root_->key;
    }
}

#endif