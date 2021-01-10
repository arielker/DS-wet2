#ifndef HW2_WET_234218_DYNAMICHASHTABLE_H
#define HW2_WET_234218_DYNAMICHASHTABLE_H

#include "ServerNode.h"

const int FIRST_ARRAY_INDEX = 0;
const int ELEMENT_WEIGHT_IN_COUNT = 1;
const int PREFIX_CELLS_NUM = 1;
const int NO_SERVERS_COUNT =  0;
const int EMPTY_CELL_ID = -101;
const int DELETED_NODE_ID = -100;
const int INITIAL_ARRAY_SIZE = 17;
const int ARRAY_SIZE_FACTOR = 2;
const int REDUCE_CHECK_FACTOR  = ARRAY_SIZE_FACTOR * 2;
const int CLEANSE_DELETED_FACTOR = 4;
const int INITIAL_COUNT = 0;
const int INITIAL_PRIME_INDEX = 0;
const int PRIME_FACTOR = 40;
const int PRIME_NUMBER_COUNT = 11;
const int PRIME_NUMBERS[PRIME_NUMBER_COUNT] ={7,43,229,1303,8807,62563,377581,2222503,13316273,79897189,479382931};
const int MINIMAL_ARRAY_SIZE = INITIAL_ARRAY_SIZE;

class DynamicHashTable {
    ServerNode empty_cell_value;
    ServerNode deleted_node_value;
    int array_size;
    int servers_count;
    ServerNode** nodes;
    int step_size;
    int delete_count;

    void initializeArray() {
        for (int i = FIRST_ARRAY_INDEX; i < array_size; i++){
            nodes[i] = &empty_cell_value;
        }
    }

    void updateStepSize(){
        int index = FIRST_ARRAY_INDEX;
        while((index < PRIME_NUMBER_COUNT - 1) && (PRIME_NUMBERS[index] * PRIME_FACTOR < array_size)){
            index++;
        }
        step_size = PRIME_NUMBERS[index];
    }

    void changeSize(int new_size){
        int old_array_size= array_size;
        ServerNode** old_array = nodes;
        nodes = new ServerNode*[new_size];
        array_size = new_size;
        servers_count = NO_SERVERS_COUNT;
        updateStepSize();  //updates the hash function
        delete_count = INITIAL_COUNT;
        initializeArray();
        for(int i=FIRST_ARRAY_INDEX; i<old_array_size; i++){
            if((old_array[i]!= &empty_cell_value) &&
               (old_array[i]!= &deleted_node_value)){
                insertNode(old_array[i]);
            }
        }
        delete[] old_array;
    }

    void increaseSize(){
        changeSize(array_size * ARRAY_SIZE_FACTOR);
    }

    void reduceSize(){
        if(array_size <= MINIMAL_ARRAY_SIZE){
            return;
        }
        changeSize(array_size/ARRAY_SIZE_FACTOR);
    }

    int hashFunction(int server_id, int iteration_num){
        return ((server_id % array_size) + (iteration_num * step_size)) % array_size;
    }

    ServerNode* insertNode(ServerNode* node, int server_id = EMPTY_CELL_ID){
        if((node && (server_id != EMPTY_CELL_ID)) ||
           (!node && (server_id == EMPTY_CELL_ID))){
            return nullptr;
        }
        if (node) {
            server_id = node->getId();
        }
        for(int i = 0; i < array_size; i++){
            int current_index = hashFunction(server_id,i);
            if(nodes[current_index]->getId() == server_id){
                return nullptr;
            } else if((nodes[current_index] == &empty_cell_value) ||
                      (nodes[current_index]) == &deleted_node_value){
                if(!node){
                    node = new ServerNode(server_id);
                }
                if(nodes[current_index] == &deleted_node_value){
                    delete_count -= ELEMENT_WEIGHT_IN_COUNT;
                }
                nodes[current_index] = node;
                servers_count += ELEMENT_WEIGHT_IN_COUNT;
                return node;
            }
        }
        return nullptr;
    }

public:
    explicit DynamicHashTable(int size);
    void insert(int server_id, ServerNode* parent = nullptr);
    ServerNode* findNode(int server_id);
    bool remove(int server_id);
    ~DynamicHashTable();
};


#endif //HW2_WET_234218_DYNAMICHASHTABLE_H
