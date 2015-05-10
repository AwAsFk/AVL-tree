#include <iostream>

using namespace std;

template<typename Key>
class BST{
private:
    class BSTNode{
        public:
        Key key;
        BSTNode * left;
        BSTNode * right;
        BSTNode(){
            left = NULL;
            right = NULL;
        };
        ~BSTNode(){};
        BSTNode(const Key & k, BSTNode * l, BSTNode * r){
            key = k;
            left = l;
            right = r;
        };
    };
    BSTNode * root;
    int numberOfElements;

    void display(BSTNode * rootOFNode){
        cout << rootOFNode->key << endl;
        if (rootOFNode->left)
            display(rootOFNode->left);
        else cout << "-" << endl;
        if (rootOFNode->right)
            display(rootOFNode->right);
        else cout << "-" << endl;
    };

    int height(BSTNode * rootOfNode){
        int x1 = 0 , x2 = 0;
        if (rootOfNode->left)
            x1 = height(rootOfNode->left) + 1;
        if (rootOfNode->right)
            x2 = height(rootOfNode->right) + 1;
        return (x1 > x2 ? x1 : x2);
    };

public:
    BST(){
        root = NULL;
        numberOfElements = 0;
    };
    ~BST(){
        //TODO removeAll
    };
    BST(const BST & bst){
        //TODO
    };
    BST & operator=(const BST & bst){
        //TODO
    };

    bool isEmpty() const{
        return (numberOfElements == 0);
    };

    bool insert(const Key & k){
        // empty
        if (root == NULL){
            root = new BSTNode(k, NULL, NULL);
            numberOfElements++;
            return true;
        }
        // not empty
        // if true - left, otherwise - right
        bool direction;
        BSTNode * ancestor = NULL;
        BSTNode * current = root;
        while (current != NULL && current->key != k){
            ancestor = current;
            if (current->key > k){
                current = current->left;
                direction = true;
            }
            else{
                current = current->right;
                direction = false;
            }
        }
        // if NULL
        if (!current){
            current = new BSTNode(k, NULL, NULL);
            direction ? ancestor->left = current : ancestor->right = current;
            numberOfElements++;
            return true;
        };
        return false;
    };

    void display(){
        display(root);
    };

    int height(){
        return height(root) + 1;
    };

    bool remove(const Key & k){
        BSTNode * nodeToDelete = root;
        BSTNode * parent = NULL;
        // searching for the element
        while (nodeToDelete != NULL && nodeToDelete->key != k){
            parent = nodeToDelete;
            if(nodeToDelete->key > k)
                nodeToDelete = nodeToDelete->left;
            else nodeToDelete = nodeToDelete->right;
        }
        // checking if element is found or not
        if (!nodeToDelete)
            return false;
        // if element is leaf or have only one child
        if (!nodeToDelete->left || !nodeToDelete->right){
            BSTNode * tmp = nodeToDelete->left;
            if (!tmp){
                //we are not sure if right exists, but we don't care
                tmp = nodeToDelete->right;
            }
            // if we deleting root element
            if (!parent){
                root = tmp;
            }
            else if (parent->left == nodeToDelete)
                parent->left = tmp;
            else parent->right = tmp;
            delete nodeToDelete;
            return true;
        }
        // hard case. we want to delete node which has 2 childs
        BSTNode *nodeToReplace = nodeToDelete->right;
        BSTNode *rparent = nodeToDelete;
        while(nodeToReplace->left){
            rparent = nodeToReplace;
            nodeToReplace = nodeToReplace->left;
        }
        if (rparent == nodeToDelete){
            nodeToDelete->right = nodeToReplace->right;
            //nodeToReplace->left = nodeToDelete->left;
        }
        else{
            rparent->left = nodeToReplace->right;
        }
        nodeToReplace->left = nodeToDelete->left;
        nodeToReplace->right = nodeToDelete->right;
        if (!parent){
            root = nodeToReplace;
        }
        else if (parent->left == nodeToDelete)
            parent->left = nodeToReplace;
        else parent->right = nodeToReplace;
        delete nodeToDelete;
        return true;
    }
};


int main(){
    BST<int> bst;
    bst.insert(4);
    bst.insert(2);
    bst.insert(1);
    bst.insert(3);
    bst.insert(10);
    bst.insert(7);
    bst.insert(8);
    bst.insert(17);
    bst.insert(16);
    //bst.insert(5);
    bst.display();
    //cout << bst.height() << endl;
    bst.remove(4);
    cout << "-----------------------------" << endl;
    bst.display();
    return 0;
}
