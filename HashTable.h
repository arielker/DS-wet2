#pragma once
#include <ostream>

// Hash table size 
#define TABLE_INITIAL_SIZE 17

// Used in second hash function. 
#define PRIME 7 

template <class T>
class HashTableElement
{
public:
	int _key;
	T* _value;
	bool _isMainHash;//this parameter determine if to delete _value (server) in the d'tor

	HashTableElement(int key, T* value, bool isMainHash) : _key(key), _value(value), _isMainHash(isMainHash) {};
	~HashTableElement()
	{
		if (_isMainHash)
		{
			if(_key != -1)
			{
				delete _value;
			}
		}
	};
};

template<class T>
class HashTable
{
	// Pointer to an array containing buckets 
	HashTableElement<T>** _hashTable;
	int _currSize;
	int _numOfExistingElements;
	bool _isMainHash;//this parameter determine if to delete _value (server) in the d'tor 

public:
	explicit HashTable(bool isMainHash)
	{
		_isMainHash = isMainHash;
		_hashTable = new HashTableElement<T> * [TABLE_INITIAL_SIZE];
		_currSize = TABLE_INITIAL_SIZE;
		_numOfExistingElements = 0;
		for (int i = 0; i < _currSize; i++)
			_hashTable[i] = NULL; // means that this cell hasn't been initialized (not deleted)
	}
	
	~HashTable()
	{
		for (int i = 0; i < _currSize; i++)
		{
			if (_hashTable[i] != NULL)
			{
				//if(_hashTable[i]->_key != -1)
				//{
				delete _hashTable[i];
				//}
			}
		}
		delete[] _hashTable;
	}
	
	// function to check if hash table is full 
	bool isFull()
	{
		// if hash size reaches maximum size 
		return (_currSize == _numOfExistingElements);
	}
	// function to check if hash table is quarter full 
	bool isFullQuarter()
	{
		//****************in delete dont shrink if size==8*****************
		
		// if hash size reaches fourth of the curent size 
		return (_currSize/4 >= _numOfExistingElements);
	}
	
	// function to calculate first hash 
	int hash1(int key)
	{
		return (key % _currSize);
	}

	// function to calculate second hash 
	int hash2(int key)
	{
		return (PRIME - (key % PRIME));
	}

	void doubleTable()
	{
		int oldSize = _currSize;
		_currSize = 2*_currSize;
		HashTableElement<T>** newhashTable = new HashTableElement<T> * [_currSize];
		HashTableElement<T>** oldhashTable = _hashTable;
		_hashTable = newhashTable;
		for (int i = 0; i < _currSize; i++)
		{
			_hashTable[i] = NULL;
		}
		for (int i = 0; i<oldSize; i++)
		{
			if (oldhashTable[i]!=NULL)
			{
				if (oldhashTable[i]->_key != -1)
				{
					insertHash(oldhashTable[i]->_key, oldhashTable[i]->_value);
				}
			}
		}
		//dtor needs to delete all "tuples" of HashTabelElement but not the servers, because "id tree" method
		for (int i = 0; i < oldSize; i++)
		{
			if (oldhashTable[i] != NULL)
			{
				oldhashTable[i]->_isMainHash = false;//****************************
				delete oldhashTable[i];
			}
		}
		delete[] oldhashTable;
	}

	void quarterTable()
	{

		int oldSize = _currSize;
		_currSize = _currSize/4;
		HashTableElement<T>** newhashTable = new HashTableElement<T> * [_currSize];
		HashTableElement<T>** oldhashTable = _hashTable;
		_hashTable = newhashTable;
		for (int i = 0; i < _currSize; i++)
		{
			_hashTable[i] = NULL;
		}
		for (int i = 0; i<oldSize; i++)
		{
			if (oldhashTable[i]!=NULL && oldhashTable[i]->_key !=-1)
			{
				insertHash(oldhashTable[i]->_key, oldhashTable[i]->_value);
			}
		}
		//dtor needs to delete all "tuples" of HashTabelElement but not the servers, because "id tree" method
		for (int i = 0; i < oldSize; i++)
		{
			if (oldhashTable[i] != NULL)
			{
				oldhashTable[i]->_isMainHash = false;//****************************
				delete oldhashTable[i];
			}
		}
		delete[] oldhashTable;
	}

	// function to insert key into hash table 
	bool insertHash(int key, T* server)
	{
		// get index from first hash 
		int index = hash1(key);
		int firstIndex = index;
		int firstAvailableIndex = -1;
		
		int k = 1; 
		while (_hashTable[index] != NULL)
		{ 
			if (_hashTable[index]->_key == key)
			{
				//Failler
				return false;
			}
			// if ((_hashTable[index]->_key == -1 ) or (_hashTable[index]->_key != key  && _hashTable[index]->_key != -1))
			//first r.i.p 
			if (firstAvailableIndex == -1 && _hashTable[index]->_key == -1) 
			{
				firstAvailableIndex = index;
			}
			index = (firstIndex + k * hash2(key)) % _currSize; 
			k++;
		}
		
		//blank spot index
		if (firstAvailableIndex == -1) 
		{
			firstAvailableIndex = index;
		}
		
		// if we are in blank spot 
		HashTableElement<T>* element = new HashTableElement<T>(key, server, _isMainHash);
		delete _hashTable[firstAvailableIndex];
		_hashTable[firstAvailableIndex] = element;
		_numOfExistingElements++;

		// if hash table is full 
		if (isFull())
		{
			doubleTable();
		}
		return true;
	}

	// function to insert key into hash table 
	bool deleteFromHash(int key)
	{
		// get index from first hash 
		int firstIndex = hash1(key);
		int index = hash1(key);
		int k = 1; 
		while (_hashTable[index] != NULL)
		{ 
			if (_hashTable[index]->_key == key)
			{
				//value found, now delete him
				_hashTable[index]->_key = -1;
				if (_hashTable[index]->_isMainHash)
				{
					delete _hashTable[index]->_value;
				}
				_hashTable[index]->_value = NULL;
				_numOfExistingElements--;
				
				// if hash table is full 
				if (_currSize > TABLE_INITIAL_SIZE) //do not shrink the table below TABLE_INITIAL_SIZE
				{
					if (isFullQuarter())
					{
						quarterTable();
					}
				}	
				return true;
			}
			
			index = (firstIndex + k * hash2(key)) % _currSize; 
			k++;
		}
		// if we are in blank spot 
		return false;
	}

	void mergeHash(HashTable<T>* table2)
	{
		for (int i = 0; i < table2->_currSize; i++)
		{
			if (table2->_hashTable[i] != NULL)
			{
				if (table2->_hashTable[i]->_key != -1)
				{
					insertHash(table2->_hashTable[i]->_key, table2->_hashTable[i]->_value);
					table2->_hashTable[i]->_value = NULL;
				}
			}
		}
	}

	T* find(int key)
	{
		// get index from first hash 
		int first_index = hash1(key);
		int index = first_index, k = 1; 
		while (_hashTable[index] != NULL)
		{ 
			if (_hashTable[index]->_key == key)
			{
				//value found, return the value 
				return _hashTable[index]->_value;
			}
			index = (first_index + k * hash2(key)) % _currSize; 
			k++;
		}
		// if we are in blank spot 
		return NULL;
	}

	//// function to display the hash table 
	//void displayHash()
	//{
	//	for (int i = 0; i < _currSize; i++)
	//	{
	//		if (_hashTable[i] != NULL)
	//		{
	//			if (_hashTable[i]->_key != -1)
	//				std::cout << i << " --> " << _hashTable[i]->_key << std::endl;
	//		}
	//		else
	//			std::cout << i << std::endl;
	//	}
	//}

};
