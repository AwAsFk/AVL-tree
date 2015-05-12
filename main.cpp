#include <iostream>
#include <iomanip>
#include <math.h>

using namespace std;

// int gl_var = 10;

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
	            balanceFactor = 0;
	        };
	        ~DictionaryNode(){};
	        DictionaryNode(const Key & k, const Info & i, char bf = 0, DictionaryNode * l = NULL, DictionaryNode * r = NULL){
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
        return max(height(rootOfNode->left), height(rootOfNode->right)) + 1;
    };

    void postorder(DictionaryNode * node, int indent = 0)
	{
	    if(node != NULL) {
	        if(node->right) {
	            postorder(node->right, indent+4);
	        }
	        if (indent) {
	            std::cout << std::setw(indent) << ' ';
	        }
	        if (node->right) {
	            std::cout<<" /\n" << std::setw(indent) << ' ';
	        }
	        std::cout<< node->key << "\n ";
	        if(node->left) {
	            std::cout << std::setw(indent) << ' ' <<" \\\n";
	            postorder(node->left, indent+4);
	        }
	    }
	}

    void removeSubTree(DictionaryNode *& node){
    	if (node){
	    	if (node->left)
	    		removeSubTree(node->left);
	    	if (node->right)
	    		removeSubTree(node->right);
	    	delete node;
	    	node = NULL;
	    	numberOfElements--;
    	}
    };

    void insertSubTree(DictionaryNode *& node){
    	if (node){
    		root = insert(root, node->key, node->info);
    		if (node->left)
    			insertSubTree(node->left);
    		if (node->right)
    			insertSubTree(node->right);
    	}
    };

    // only in copyconstructor and assigment operator
    void copyAll(const Dictionary & dictionary){
    	root = NULL;
    	numberOfElements = 0;
    	DictionaryNode * node = dictionary.root;
    	insertSubTree(node);
    };

    void calculateBalanceFactor(DictionaryNode *& node){
    	if (node){
    		int leftHeight = height(node->left);
    		int rightHeight = height(node->right);
    		node->balanceFactor = (rightHeight - leftHeight);
    		/*if (node->left)
    			calculateBalanceFactor(node->left);
    		if (node->right)
    			calculateBalanceFactor(node->right);*/
    	}
    }

    DictionaryNode * leftRotation(DictionaryNode * node){
    	DictionaryNode * subtree = node->right;
 		// doble-left rotation
 		/*if (subtree->balanceFactor == -1){
 			// rigth rotation
 			node->right = subtree->left;
 			subtree->left = subtree->left->right;
 			node->right->right = subtree;
 			subtree = node->right;
 			/*node->right = rightRotation(subtree);
 			subtree = node->right;
 		}*/
 		// left rotation
 		node->right = subtree->left;
 		subtree->left = node;
 		calculateBalanceFactor(node);
 		calculateBalanceFactor(subtree);
 		return subtree;
    }

    DictionaryNode * rightRotation(DictionaryNode * node){
    	DictionaryNode * subtree = node->left;
 		// double-right rotation
 		/*if (subtree->balanceFactor == 1){
 			// left rotation
 			node->left = subtree->right;
 			subtree->right = subtree->right->left;
 			node->left->left = subtree;
 			subtree = node->left;
 			/*node->left = leftRotation(subtree);
 			subtree = node->left;
 		}*/
 		// right rotation
 		node->left = subtree->right;
 		subtree->right = node;
        calculateBalanceFactor(node);
 		calculateBalanceFactor(subtree);
 		return subtree;
    }

	DictionaryNode * insert(DictionaryNode * node, const Key & k, const Info & i){
		if (!node) {
			numberOfElements++;
			return new DictionaryNode(k, i);
		}
		if (node->key > k){
			node->left = insert(node->left, k, i);
		}
		else if (node->key < k){
			node->right = insert(node->right, k, i);
		}
		return balance(node);
	}

	DictionaryNode * findMin(DictionaryNode * node, DictionaryNode *& ancestor){
		if (node){
			if (node->left){
				ancestor = node;
				return findMin(node->left, ancestor);
			}
			else return node;
		}
		return NULL;
	}

	DictionaryNode * remove(DictionaryNode * node, const Key & k){
		if (!node) return NULL;
		if (node->key > k)
			node->left = remove(node->left, k);
		else if (node->key < k)
			node->right = remove(node->right, k);
		else{
			// easy way
			if (!node->left || !node->right){
            	DictionaryNode * tmp = node->left;
            	if (!tmp){
                //we are not sure if right exists, but we don't care
                	tmp = node->right;
            	}
            	if (node == root)
                    root = tmp;
	            delete node;
	            node = NULL;
	            numberOfElements--;
	            return balance(tmp);
	        }
	        // hard case
	        else{
	        	DictionaryNode * ancestor = node;
	        	DictionaryNode *nodeToReplace = findMin(node->right, ancestor);
		        if (node->right == nodeToReplace){
		            node->right = nodeToReplace->right;
		        }
		        else{
		            ancestor->left = nodeToReplace->right;
		        }
		        nodeToReplace->left = node->left;
		        nodeToReplace->right = node->right;
                if (node == root)
                    root = nodeToReplace;
		        delete node;
		        node = NULL;
		        numberOfElements--;
		        // calculateBalanceFactor(root);
		        return balance(nodeToReplace);
	        }
        }
        return balance(node);
	}



	DictionaryNode * balance(DictionaryNode * node){
    	// DictionaryNode * node = root;
    	calculateBalanceFactor(node);
    	if (node){
    		// if unbalanced
	    	if (node->balanceFactor > 1 || node->balanceFactor < -1){
	    		// if tree is right heavy
	    		if (node->balanceFactor > 1){
	    			if (node->right->balanceFactor == -1){
	    				node->right = rightRotation(node->right);
	    			}
	    			return leftRotation(node);
	    		}
	    		// if tree is left heavy
	    		else if (node->balanceFactor < -1){
	    			if (node->left->balanceFactor == 1){
	    				node->left = leftRotation(node->left);
	    			}
	    			return rightRotation(node);
                }
            }
        }
        return node;
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
    	int n = size();
        root = insert(root, k, i);
        if (size() > n)
        	return true;
        else return false;
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

    void print(){
    	cout << "Start" << endl;
    	postorder(root);
    	cout << "End" << endl;
    }

    bool remove(const Key & k){
        // int n = size();
        DictionaryNode * node = remove(root, k);
        if (height(node) > height(root))
            root = node;
        if (!node && size() > 0){
        	return false;
        }
        else {
        	return true;
        }
    };
};

int main(){
    Dictionary<int, int> dict1;
    dict1.removeAll();
    dict1.remove(5);
    dict1.insert(10, 1);
    dict1.insert(15, 1);
    dict1.insert(7, 1);
    dict1.insert(21, 1);
    dict1.insert(13, 1);
    dict1.insert(14, 1);
    dict1.display();
    dict1.insert(22, 1);
    dict1.insert(20, 1);
    dict1.insert(25, 1);
    // dict1.display();
    //dict1.remove(20);
    dict1.display();
    cout << "IsEmpty: " << dict1.isEmpty()
        << " Height = " << dict1.height() << " Size = " << dict1.size() << endl;
    dict1.print();
    return 0;
}
