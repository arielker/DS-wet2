#ifndef HW2_234218_W20_21_ONLINEHASHTABLE_H
#define HW2_234218_W20_21_ONLINEHASHTABLE_H

/**
 * This is some cool, sophisticated implementation of a generic Hash Table I found on web
 * never got to to try it tho but I guess it's not bad to have it here...
 */

const int TABLE_SIZE = 17;

template <typename K, typename V>
class HashNode {
public:
    HashNode(const K &key, const V &value) :
            key(key), value(value), next(nullptr) {
    }

    K getKey() const {
        return key;
    }

    V getValue() const {
        return value;
    }

    void setValue(V v) {
        HashNode::value = v;
    }

    HashNode *getNext() const {
        return next;
    }

    void setNext(HashNode *node) {
        HashNode::next = node;
    }

private:
    /// key-value pair
    K key;
    V value;
    /// next bucket with the same key
    HashNode *next;
};

/// Default hash function class
template <typename K>
struct KeyHash {
    unsigned long operator()(const K& key) const {
        return reinterpret_cast<unsigned long>(key) % TABLE_SIZE;
    }
};

/// Hash map class template
template <typename K, typename V, typename F = KeyHash<K>>
class HashMap {
public:
    HashMap() {
        /// construct zero initialized hash table of size
        table = new HashNode<K, V> *[TABLE_SIZE]();
    }

    ~HashMap() {
        /// destroy all buckets one by one
        for (int i = 0; i < TABLE_SIZE; ++i) {
            HashNode<K, V> *entry = table[i];
            while (entry != nullptr) {
                HashNode<K, V> *prev = entry;
                entry = entry->getNext();
                delete prev;
            }
            table[i] = nullptr;
        }
        /// destroy the hash table
        delete [] table;
    }

    bool get(const K &key, V &value) {
        unsigned long hashValue = hashFunc(key);
        HashNode<K, V> *entry = table[hashValue];

        while (entry != nullptr) {
            if (entry->getKey() == key) {
                value = entry->getValue();
                return true;
            }
            entry = entry->getNext();
        }
        return false;
    }

    void put(const K &key, const V &value) {
        unsigned long hashValue = hashFunc(key);
        HashNode<K, V> *prev = nullptr;
        HashNode<K, V> *entry = table[hashValue];

        while (entry != nullptr && entry->getKey() != key) {
            prev = entry;
            entry = entry->getNext();
        }

        if (entry == nullptr) {
            entry = new HashNode<K, V>(key, value);
            if (prev == nullptr) {
                /// insert as first bucket
                table[hashValue] = entry;
            } else {
                prev->setNext(entry);
            }
        } else {
            /// just update the value
            entry->setValue(value);
        }
    }

    void remove(const K &key) {
        unsigned long hashValue = hashFunc(key);
        HashNode<K, V> *prev = nullptr;
        HashNode<K, V> *entry = table[hashValue];

        while (entry != nullptr && entry->getKey() != key) {
            prev = entry;
            entry = entry->getNext();
        }

        if (entry == nullptr) {
            /// key not found
            return;
        }
        else {
            if (prev == nullptr) {
                /// remove first bucket of the list
                table[hashValue] = entry->getNext();
            } else {
                prev->setNext(entry->getNext());
            }
            delete entry;
        }
    }

private:
    /// hash table
    HashNode<K, V> **table;
    F hashFunc;
};

#endif //HW2_234218_W20_21_ONLINEHASHTABLE_H
