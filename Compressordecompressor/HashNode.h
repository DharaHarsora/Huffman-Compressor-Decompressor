#pragma once
template<typename K, typename V>
class HashNode {
public:
    K key;
    V value;
    HashNode<K, V>* next;
public:
    HashNode(const K& key, const V& val) : key(key), value(val), next(NULL) {
        
    }
    K getKey() const {
        return key;
    }

    V getValue() const {
        return value;
    }

    void setValue(V value) {
        this->value = value;
    }

    HashNode<K, V>* getNext() const {
        return next;
    }

    void setNext(HashNode<K, V>* next) {
        this->next = next;
    }
};