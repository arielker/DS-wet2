#ifndef HW1_234218_AVL_H
#define HW1_234218_AVL_H

const int EMPTY_TREE_SIZE = 0;
const int LEAF_NODE_HEIGHT = 0;
const int BALANCE_EQUAL_VALUE = 0;
const int MAXIMAL_POSITIVE_BALANCE = 1;
const int MINIMAL_NEGATIVE_BALANCE = -1;
const int HEIGHT_STEP_UNIT = 1;

template<typename T, typename K>
class AVL_Node {
    T data; ///is int actually
    K key; ///is Class
    int height;
    int size_of_subtree;
    AVL_Node* left;
    AVL_Node* right;

public:
    AVL_Node(T& data1, K& key1) :
            data(data1), key(key1), height(LEAF_NODE_HEIGHT),
            size_of_subtree(1), left(nullptr), right(nullptr){}


    ~AVL_Node() = default;

    AVL_Node* getRight() const{
        return this->right;
    }

    AVL_Node* getLeft() const{
        return this->left;
    }

    K getKey() const{
        return this->key;
    }

    int getSizeOfSubtree() const {
        return this->size_of_subtree;
    }

    void setRight(AVL_Node* node){
        this->right = node;
    }

    void setLeft(AVL_Node* node){
        this->left = node;
    }

    bool isLeaf() const{
        return (this->getLeft() == nullptr && this->getRight() == nullptr);
    }

    /**
     * comparison function between two nodes in the AVL with target node
     * containing target data and key
     * @param data1 - target data to be evaluated and compared
     * @param key1 - target key to be evaluated and compared
     * @return:
     *      @0 - if both nodes are equal
     *      @positive_integer - if current node is greater than target evaluated
     *      node with matching target data and key
     *      @negative_integer - else
     */
    int compFunc(T& data1, K key1){
        return *(this->key) == *key1 ? 0 : *(this->key) > *key1 ? 1 : -1;
    }

    void updateHeight(){
        if(this->isLeaf()) {
            this->height = LEAF_NODE_HEIGHT;
        } else if(nullptr == this->right) {
            this->height = this->left->height + HEIGHT_STEP_UNIT;
        } else if(nullptr == this->left) {
            this->height = this->right->height + HEIGHT_STEP_UNIT;
        } else if(this->right->height < this->left->height){
            this->height= this->left->height + HEIGHT_STEP_UNIT;
        } else {
            this->height= this->right->height + HEIGHT_STEP_UNIT;
        }
    }

    void updateSizeOfSubtree() {
        if(this->isLeaf()){
            this->size_of_subtree = 1;
        } else if (nullptr == this->left) {
            this ->size_of_subtree = 1 + this->right->size_of_subtree;
        } else if (nullptr == this->right) {
            this->size_of_subtree = 1 + this->left->size_of_subtree;
        } else {
            this->size_of_subtree = 1 + this->left->size_of_subtree + this->right->size_of_subtree;
        }
    }

    int getBalance() const{
        if(this->isLeaf()) {
            return 0;
        } else if (this->right == nullptr) {
            return this->left->height + 1;
        } else if (this->left == nullptr) {
            return -this->right->height - 1;
        }
        return ((this->left->height) - (this->right->height));
    }
};

template<typename T, typename K>
class AVL{
    int size;
    AVL_Node<T, K>* root;

    void RotateLeft(AVL_Node<T, K>** node) {
        AVL_Node<T, K>* temp = (*node)->getRight();
        (*node)->setRight((*node)->getRight()->getLeft());
        temp->setLeft((*node));
        temp->setLeft((*node));
        (*node) = temp;
        rotateEnd(*node);
    }

    void RotateRight(AVL_Node<T, K>** node) {
        AVL_Node<T, K>* temp = (*node)->getLeft();
        (*node)->setLeft((*node)->getLeft()->getRight());
        temp->setRight((*node));
        (*node) = temp;
        rotateEnd(*node);
    }

    /**
     * destruction function of the AVL tree
     * @param node - target node pointer of the AVL tree
     */
    void destroyAux(AVL_Node<T, K>* node) const {
        if(nullptr == node){
            return;
        }
        destroyAux(node->getLeft());
        destroyAux(node->getRight());
        delete node;
    }

    /**
     * searching recursively inside the AVL tree for the node with the target
     * key.
     * @param node - target pointer to the AVL tree
     * @param key - target key that is searched in the AVL tree
     * @return -
     *      @nullptr - in case there is no node with matching target key
     *      @node_pointer - pointer to target node with matching key
     */
    AVL_Node<T, K>* findAux(AVL_Node<T, K>* node, K& key, T& data) const{
        if (node->compFunc(data, key) == 0) {
            return node;
        }
        if (nullptr != node->getRight() && node->compFunc(data, key) < 0) {
            AVL_Node<T, K> *result = this->findAux(node->getRight(), key, data);
            if (nullptr != result) {
                return result;
            }
        } else if (nullptr != node->getLeft() && node->compFunc(data, key) > 0) {
            AVL_Node<T, K> *result = this->findAux(node->getLeft(), key, data);
            if (nullptr != result) {
                return result;
            }
        }
        return nullptr;
    }

    /**
     * check of balance factor in the tree for target pointer node in case of
     * deletion or insertion of a node in the tree.
     * @param node - target node pointer of the tree to which balance factor is
     *               checked.
     */
    void balanceCheckAndUpdate(AVL_Node<T, K>*& node){
        node->updateHeight();
        node->updateSizeOfSubtree();
        int balance = node->getBalance();
        if(balance < MINIMAL_NEGATIVE_BALANCE) {
            if(node->getRight()->getBalance() == MAXIMAL_POSITIVE_BALANCE) {
                AVL_Node<T, K> *temp = node->getRight();
                this->RotateRight(&temp);
                node->setRight(temp);
                this->RotateLeft(&node);
            } else if (node->getRight()->getBalance() <= BALANCE_EQUAL_VALUE) {
                this->RotateLeft(&node);
            }
        } else if(balance > MAXIMAL_POSITIVE_BALANCE){
            if(node->getLeft()->getBalance() == MINIMAL_NEGATIVE_BALANCE) {
                AVL_Node<T, K> *temp = node->getLeft();
                this->RotateLeft(&temp);
                node->setLeft(temp);
                this->RotateRight(&node);
            } else if (node->getLeft()->getBalance() >= BALANCE_EQUAL_VALUE) {
                this->RotateRight(&node);
            }
        }
    }

    static void changeNodes(AVL_Node<T, K>*& node, AVL_Node<T, K>*& current){
        AVL_Node<T, K>* tempRight = current->getRight();
        AVL_Node<T, K>* tempLeft = current->getLeft();
        current->setRight(node->getRight());
        current->setLeft(node->getLeft());
        node->setRight(tempLeft);
        node->setLeft(tempRight);
    }

    void rotateEnd(AVL_Node<T, K>* node){
        if(nullptr != node->getRight()){
            node->getRight()->updateHeight();
            node->getRight()->updateSizeOfSubtree();
        }
        if(nullptr != node->getLeft()){
            node->getLeft()->updateHeight();
            node->getLeft()->updateSizeOfSubtree();
        }
        node->updateHeight();
        node->updateSizeOfSubtree();
    }

    /**
     * inserts a new node with target data and key to the AVL tree.
     * in case the tree has already a node with an existing target key then
     * we return without inserting.
     * in case of insertion, we check ballance factor and rotate accordingly
     * @param node_pointer - pointer to target AVL tree
     * @param data - target data to be inserted ti the AVL tree
     * @param key - target key to be inserted
     */
    void insertAux(AVL_Node<T, K>** node_pointer, T data, K& key){
        AVL_Node<T, K> *node = *node_pointer;
        if(node->compFunc(data, key) == 0) {
            return;
        } else if(node->compFunc(data, key) > 0){
            if(nullptr != node->getLeft()){
                AVL_Node<T, K>* temp = node->getLeft();
                insertAux(&temp, data, key);
                node->setLeft(temp);
            }
            else {
                node->setLeft(new AVL_Node<T, K>(data,key));
            }
        } else {
            if(nullptr != node->getRight()){
                AVL_Node<T, K> *temp = node->getRight();
                insertAux(&temp,data,key);
                node->setRight(temp);
            } else {
                node->setRight(new AVL_Node<T, K>(data,key));
            }
        }
        balanceCheckAndUpdate(node);
        *node_pointer = node;
    }

    /**
     * helping function of deletion of a node with two sons in a tree.
     * algorithm of deletion according to the studied in the lecture
     * @param node_to_delete - target node to be deleted
     * @param node - right son of target node to be deleted
     */
    void removeAux2(AVL_Node<T, K>*& node_to_delete, AVL_Node<T, K>*& node){
        if(nullptr == node->getLeft()) {
            AVL_Node<T, K>* temp = node_to_delete;
            node_to_delete = node;
            node = temp;
            changeNodes(node, node_to_delete);
            removeNodeAux1(node);
        } else {
            AVL_Node<T, K>* temp = node->getLeft();
            removeAux2(node_to_delete,temp);
            node->setLeft(temp);
        }
        if (node) {
            balanceCheckAndUpdate(node);
        }
    }

    /**
     * function that separates 4 cases of deletion of a node from target AVL
     * tree: has 2 sons, is a leaf or has one of right/left sons.
     * @param node - target node to be deleted from the tree
     */
    void removeNodeAux1(AVL_Node<T, K>*& node){
        if(node->isLeaf()){
            delete(node);
            node = nullptr;
        } else if(nullptr != node->getRight() && nullptr != node->getLeft()) {
            AVL_Node<T, K>* temp = node->getRight();
            removeAux2(node,temp);
            node->setRight(temp);
        } else if(nullptr == node->getLeft()) {
            AVL_Node<T, K> *temp = node;
            node = node->getRight();
            delete(temp);
        } else if(nullptr == node->getRight()){
            AVL_Node<T, K> *temp = node;
            node = node->getLeft();
            delete(temp);
        }
    }

    /**
     * deletes a node with target key from the AVL tree
     * @param node - node pointer to target AVL tree
     * @param key - target key to be deleted from the tree
     * @param was_deleted - flag that indicates if a node was deleted or not.
     */
    void deleteAux(AVL_Node<T, K>*& node, K& key, T& data1, bool* was_deleted){
        if(nullptr == node){
            return;
        }
        if(node->compFunc(data1, key) == 0){
            removeNodeAux1(node);
            *was_deleted = true;
        } else if(node->compFunc(data1, key) > 0) {
            AVL_Node<T, K>* temp_node = node->getLeft();
            deleteAux(temp_node,key, data1, was_deleted);
            node->setLeft(temp_node);
        } else if(node->compFunc(data1, key) < 0){
            AVL_Node<T, K>* temp_node = node->getRight();
            deleteAux(temp_node,key, data1, was_deleted);
            node->setRight(temp_node);
        }
        if(nullptr != node) {
            balanceCheckAndUpdate(node);
        }
    }

    K GetIthWatchedClass_aux(int i, AVL_Node<T, K>* node) {
        int rightSonSubtreeSize = 0;
        if(nullptr != node->getRight()){
            rightSonSubtreeSize = node->getRight()->getSizeOfSubtree();
        }
        if(i == node->getSizeOfSubtree()){
            while (nullptr != node->getLeft()){
                node = node->getLeft();
            }
            return node->getKey();
        } else if (i > rightSonSubtreeSize){
            return i - 1 == rightSonSubtreeSize
                ? node->getKey()
                : GetIthWatchedClass_aux(i - rightSonSubtreeSize - 1, node->getLeft());
        } else {
            return GetIthWatchedClass_aux(i, node->getRight());
        }
    }

public:
    AVL(): size(EMPTY_TREE_SIZE), root(nullptr){}

    ~AVL(){
        destroyAux(this->root);
    }

    AVL_Node<T, K>* find(K key, T data1) const{
        return nullptr == this->root ? nullptr : findAux(this->root, key, data1);
    }

    AVL_Node<T, K>* insert(const T& data, K& key){
        T data1 = const_cast<const T&>(data);
        if(nullptr == this->root){
            this->root = new AVL_Node<T, K>(data1, key);
            ++(this->size);
            return this->root;
        } else if(!(this->isExist(key, data1))){
            ++(this->size);
            insertAux(&(this->root),data1,key);
            return findAux(this->root, key, data1);
        }
        return nullptr;
    }

    AVL_Node<T, K>* insert(T& data, K& key){
        if(nullptr == this->root){
            this->root = new AVL_Node<T, K>(data, key);
            ++(this->size);
            return this->root;
        } else if(!(this->isExist(key, data))){
            ++(this->size);
            insertAux(&(this->root),data,key);
            return findAux(this->root, key, data);
        }
        return nullptr;
    }

    bool isExist(K& key, T& data1) const{
        return this->find(key, data1) != nullptr;
    }

    void deleteNode(K& key, const T& data){
        T data1 = const_cast<const T&>(data);
        bool was_deleted = false;
        deleteAux(this->root, key, data1, &was_deleted);
        if(was_deleted){
            --(this->size);
        }
    }

    void deleteNode(K& key, T& data){
        bool was_deleted = false;
        deleteAux(this->root, key, data, &was_deleted);
        if(was_deleted){
            --(this->size);
        }
    }

    int getSize() const{
        return this->size;
    }

    K GetIthWatchedClass(int i){
        return GetIthWatchedClass_aux(i, this->root);
    }
};

#endif //HW1_234218_AVL_H