#include <iostream>

using namespace std;

template<typename Key, typename Info>
class Dictionary{
private:
    class DictionaryNode{
        public:
	    	Key key;
        	Info info;
        	// can be -1, 0, 1 - difference between left and right subtree
        	// if the value of this variable is not as above, then it will means
        	// that tree is unbalanced
        	char balanceFactor;
	        DictionaryNode * left;
	        DictionaryNode * right;
	        DictionaryNode(){
	            left = NULL;
	            right = NULL;
	        };
	        ~DictionaryNode(){};
	        DictionaryNode(const Key & k, const Info & i, char bf, DictionaryNode * l, DictionaryNode * r){
	            info = i;
	            key = k;
	            balanceFactor = bf;
	            left = l;
	            right = r;
	        };
    };

    DictionaryNode * root;
    int numberOfElements;

    void display(DictionaryNode * rootOfNode){
    	if (rootOfNode){
	        cout << "balanceFactor = " << (int)rootOfNode->balanceFactor << " key =  " << rootOfNode->key << endl;
	        if (rootOfNode->left)
	            display(rootOfNode->left);
	        else cout << "-" << endl;
	        if (rootOfNode->right)
	            display(rootOfNode->right);
	        else cout << "-" << endl;
    	}
    };

    int height(DictionaryNode * rootOfNode){
    	if (!rootOfNode)
    		return 0;
        int x1 = 0 , x2 = 0;
        // if (rootOfNode->left)
        x1 = height(rootOfNode->left) + 1;
        // if (rootOfNode->right)
        x2 = height(rootOfNode->right) + 1;
        return (x1 > x2 ? x1 : x2);
    };

    void removeSubTree(DictionaryNode * node){
    	if (node){
	    	if (node->left)
	    		removeSubTree(node->left);
	    	if (node->right)
	    		removeSubTree(node->right);
	    	if (node == root){
	    		delete node;
	    		root = NULL;
	    	}
	    	else
	    		delete node;
	    	numberOfElements--;
    	}
    };

    void insertSubTree(DictionaryNode * node){
    	if (node){
    		insert(node->key, node->info);
    		numberOfElements++;
    		// this.balanceFactor = node->balanceFactor;
    		if (node->left)
    			insertSubTree(node->left);
    		if (node->right)
    			insertSubTree(node->right);
    	}
    };

    /*void insert(const Key & k, const Info & i, DictionaryNode * node){

    }*/

    // only in copyconstructor and assigment operator
    void copyAll(const Dictionary & dictionary){
    	root = NULL;
    	numberOfElements = 0;
    	insertSubTree(dictionary.root);
    };

    void calculateBalanceFactor(DictionaryNode * node){
    	if (node){
    		int leftHeight = height(node->left);
    		int rightHeight = height(node->right);
    		node->balanceFactor = (rightHeight - leftHeight);
    		if (node->left)
    			calculateBalanceFactor(node->left);
    		if (node->right)
    			calculateBalanceFactor(node->right);
    	}
    }

    void balance(DictionaryNode * node, DictionaryNode * ancestor){
    	// DictionaryNode * node = root;
    	if (node){
    		// if unbalanced
    		DictionaryNode * subtree = node;
	    	if (node->balanceFactor > 1 || node->balanceFactor < -1){
	    		// if tree is right heavy
	    		if (node->balanceFactor > 1){
	    			subtree = node->right;
	    			// doble-left rotation
	    			if (subtree->balanceFactor == -1){
	    				// rigth rotation
	    				node->right = subtree->left;
	    				subtree->left = subtree->left->right;
	    				node->right->right = subtree;
	    				subtree = node->right;
	    			}
	    			// left rotation
	    			node->right = subtree->left;
	    			subtree->left = node;
	    			if (node == root)
	    				root = subtree;
	    			else if (ancestor->left == node)
	    				ancestor->left = subtree;
	    			else if (ancestor->right == node)
	    				ancestor->right = subtree;
	    		}
	    		// if tree is left heavy
	    		else if (node->balanceFactor < -1){
	    			subtree = node->left;
	    			// double-right rotation
	    			if (subtree->balanceFactor == 1){
	    				// left rotation
	    				node->left = subtree->right;
	    				subtree->right = subtree->right->left;
	    				subtree->right->left = subtree;
	    				subtree = node->left;
	    			}
	    			// right rotation
	    			node->left = subtree->right;
	    			subtree->right = node;
	    			if (node == root)
	    				root = subtree;
	    			else if (ancestor->left == node)
	    				ancestor->left = subtree;
	    			else if (ancestor->right == node)
	    				ancestor->right = subtree;
	    		}
	    		calculateBalanceFactor(root);
	    	}
	    	if (subtree->left)
	    		balance(subtree->left, subtree);
	    	if (subtree->right)
	    		balance(subtree->right, subtree);
    	}
    }

public:
    Dictionary(){
        root = NULL;
        numberOfElements = 0;
    };

    ~Dictionary(){
        removeAll();
    };

    Dictionary(const Dictionary & dictionary){
        copyAll(dictionary);
    };

    Dictionary & operator=(const Dictionary & dictionary){
        if (this == &dictionary)
        	return *this;
        removeAll();
        copyAll(dictionary);
        return *this;
    };

    bool isEmpty() const{
        return (numberOfElements == 0);
    };

    bool insert(const Key & k, const Info & i){
        // empty
        if (root == NULL){
            root = new DictionaryNode(k, i, 0, NULL, NULL);
            numberOfElements++;
            return true;
        }
        // not empty
        // if true - left, otherwise - right
        bool direction;
        DictionaryNode * ancestor = NULL;
        DictionaryNode * current = root;
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
            current = new DictionaryNode(k, i, 0, NULL, NULL);
            direction ? ancestor->left = current : ancestor->right = current;
            numberOfElements++;
            calculateBalanceFactor(root);
            balance(root, NULL);
            return true;
        };
        return false;
    };

    void removeAll(){
    	removeSubTree(root);
    };

    int size(){
    	return numberOfElements;
    }

    void display(){
    	cout << "Start printing info about tree: " << endl;
        display(root);
        cout << "End printing info about tree: " << endl;
    };

    int height(){
        return height(root);
    };

    bool remove(const Key & k){
        DictionaryNode * nodeToDelete = root;
        DictionaryNode * parent = NULL;
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
            DictionaryNode * tmp = nodeToDelete->left;
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
            numberOfElements--;
            calculateBalanceFactor(root);
            balance(root, NULL);
            return true;
        }
        // hard case. we want to delete node which has 2 childs
        DictionaryNode *nodeToReplace = nodeToDelete->right;
        DictionaryNode *rparent = nodeToDelete;
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
        calculateBalanceFactor(root);
        balance(root, NULL);
        numberOfElements--;
        return true;
    };
};

int main(){
    Dictionary<int, int> dict1;
    dict1.insert(5, 1);
    //dict1.insert(5, 1);
    dict1.insert(4, 1);
    dict1.insert(9, 1);
    dict1.insert(1, 1);
    dict1.insert(0, 1);
    dict1.insert(11, 1);
    dict1.insert(10, 1);
    dict1.insert(7, 1);
    dict1.insert(6, 1);
    dict1.insert(8, 1);
    dict1.insert(13, 1);
    dict1.insert(15, 1);
    dict1.insert(21, 1);
    dict1.insert(3, 1);
    dict1.remove(0);
    dict1.display();
    cout << "Height = " << dict1.height() << " size = " << dict1.size() << endl;

    /*Dictionary<int, int> dict2(dict1);
    dict2.display();
    cout << "Height = " << dict2.height() << endl;
    dict1.removeAll();
    dict1.display();
    cout << "Height = " << dict1.height() << endl;
    dict2.remove(9);
    dict2.display();*/
    return 0;
}
