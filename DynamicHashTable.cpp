#include "DynamicHashTable.h"

DynamicHashTable::DynamicHashTable(int size) :
        empty_cell_value(EMPTY_CELL_ID),
        deleted_node_value(DELETED_NODE_ID),
        array_size(size), servers_count(NO_SERVERS_COUNT),
        nodes(new ServerNode*[array_size]),
        step_size(PRIME_NUMBERS[INITIAL_PRIME_INDEX]),
        delete_count(INITIAL_COUNT){
    initializeArray();
}

DynamicHashTable::~DynamicHashTable() {
    for (int i = FIRST_ARRAY_INDEX; i < array_size; i++) {
        if (nodes[i] && (nodes[i] != &deleted_node_value) &&
            (nodes[i] != &empty_cell_value)) {
            delete nodes[i];
        }
    }
    delete[] nodes;
}

void DynamicHashTable::insert(int server_id, ServerNode *parent) {
    if(servers_count == array_size){
        increaseSize();
    }
    ServerNode* added_node = insertNode(nullptr, server_id);
    added_node->setParent(parent);
}

ServerNode * DynamicHashTable::findNode(int server_id) {
    for(int i=0; i < array_size; i++){
        int current_index = hashFunction(server_id,i);
        if(nodes[current_index]->getId() == server_id){
            return nodes[current_index];
        } else if(nodes[current_index] == &empty_cell_value){
            return nullptr;
        }
    }
    return nullptr; //the array is full and the element wasn't found
}

bool DynamicHashTable::remove(int server_id) {
    for(int i=0; i<array_size; i++){
        int current_index = hashFunction(server_id,i);
        if(nodes[current_index]->getId() == server_id) {
            ServerNode* server_to_delete = nodes[current_index];
            delete server_to_delete;
            nodes[current_index] = &deleted_node_value;
            delete_count += ELEMENT_WEIGHT_IN_COUNT;
            servers_count -= ELEMENT_WEIGHT_IN_COUNT;
            //checks if the removal causes an array resizing
            if(servers_count <= (array_size/REDUCE_CHECK_FACTOR)){
                reduceSize();
            }
            if(delete_count * CLEANSE_DELETED_FACTOR > array_size){
                changeSize(array_size); //cleanse deleted cells' mark
            }
            return true;
        } else if(nodes[current_index] == &empty_cell_value){
            return false;
        }
    }
    return false;
}
