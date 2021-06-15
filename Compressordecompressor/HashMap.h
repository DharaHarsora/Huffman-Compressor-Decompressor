#pragma once

#ifndef TABLE_SIZE
#define TABLE_SIZE 7
#endif // TABLE_SIZE
#include "HashNode.h"
using namespace std;

template<typename K, typename V>
class HashMap {
public:
	HashNode<K, V>** table;
	int _size;
public:
	HashMap() {
		table = new HashNode<K, V> * [TABLE_SIZE]();
	}

	void put(const std::string& key, const V& value) {
		_size++;
		unsigned long hashValue = (hash(key) % TABLE_SIZE);
		HashNode<string, V>* prev = NULL;
		HashNode<string, V>* entry = table[hashValue];

		while (entry != NULL && entry->getKey() != key) {
			prev = entry;
			entry = entry->getNext();
		}

		if (entry == NULL) {
			entry = new HashNode<string, V>(key, value);
			if (prev == NULL) {
				table[hashValue] = entry;
			}
			else {
				prev->setNext(entry);
			}
		}
		else {
			entry->setValue(value);
		}
	}

	void put(const unsigned char& key, const V& value) {
		unsigned long hashValue = ((unsigned long)key % TABLE_SIZE);
		HashNode<unsigned char, V>* prev = NULL;
		HashNode<unsigned char, V>* entry = table[hashValue];

		while (entry != NULL && entry->getKey() != key) {
			prev = entry;
			entry = entry->getNext();
		}

		if (entry == NULL) {
			_size++;
			entry = new HashNode<unsigned char,V>(key, value);
			if (prev == NULL) {
				table[hashValue] = entry;
			}
			else {
				prev->setNext(entry);
			}
		}
		else {
			entry->setValue(value);
		}
	}

	int size() {
		return _size;
	}
	
	unsigned long hash(const std::string& s)
	{
		unsigned long hash = 5381;
		for (char ch : s) {
			hash = ((hash << 5) + hash) + (unsigned char)ch;
		}

		return hash;
	}

	V search(const unsigned char key)
	{
		unsigned long hashValue = ((key) % TABLE_SIZE);
		HashNode<K, V>* entry = table[hashValue];
		int flag = 0;

		while (entry != NULL && entry->getKey() != key) {
			entry = entry->getNext();
		}

		if (entry == NULL) {
			return V();
		}
		else {
			return entry->getValue();
		}
	}

	~HashMap() {
		for (int i = 0; i < TABLE_SIZE; ++i) {
			HashNode<K, V>* entry = table[i];
			while (entry != NULL) {
				HashNode<K, V>* prev = entry;
				entry = entry->getNext();
				delete prev;
			}
			table[i] = NULL;
		}
		delete[] table;
	}

};