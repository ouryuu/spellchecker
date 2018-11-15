// HashSet.hpp
//
// ICS 46 Spring 2018
// Project #4: Set the Controls for the Heart of the Sun
//
// A HashSet is an implementation of a Set that is a separately-chained
// hash table, implemented as a dynamically-allocated array of linked
// lists.  At any given time, the HashSet has a "size" indicating
// how many elements are stored within it, along with a "capacity"
// indicating the size of the array.
//
// As elements are added to the HashSet and the proportion of the HashSet's
// size to its capacity exceeds 0.8 (i.e., there are more than 80% as many
// elements as there are array cells), the HashSet should be resized so
// that it is twice as large as it was before.
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to use a dynamically-
// allocated array and your own linked list implemenation; the linked list
// doesn't have to be its own class, though you can do that, if you'd like.

#ifndef HASHSET_HPP
#define HASHSET_HPP
#include <iostream>
#include <functional>
#include "Set.hpp"

template<typename ElementType>
struct HashNode
{
	ElementType value;
	HashNode* next;
};

template <typename ElementType>
class HashSet : public Set<ElementType>
{
public:
	// The default capacity of the HashSet before anything has been
	// added to it.
	static constexpr unsigned int DEFAULT_CAPACITY = 10;

	// A HashFunction is a function that takes a reference to a const
	// ElementType and returns an unsigned int.
	using HashFunction = std::function<unsigned int(const ElementType&)>;

public:
	// Initializes a HashSet to be empty, so that it will use the given
	// hash function whenever it needs to hash an element.
	explicit HashSet(HashFunction hashFunction);

	// Cleans up the HashSet so that it leaks no memory.
	virtual ~HashSet() noexcept;

	// Initializes a new HashSet to be a copy of an existing one.
	HashSet(const HashSet& s);

	// Initializes a new HashSet whose contents are moved from an
	// expiring one.
	HashSet(HashSet&& s) noexcept;

	// Assigns an existing HashSet into another.
	HashSet& operator=(const HashSet& s);

	// Assigns an expiring HashSet into another.
	HashSet& operator=(HashSet&& s) noexcept;


	// isImplemented() should be modified to return true if you've
	// decided to implement a HashSet, false otherwise.
	virtual bool isImplemented() const noexcept override;


	// add() adds an element to the set.  If the element is already in the set,
	// this function has no effect.  This function triggers a resizing of the
	// array when the ratio of size to capacity would exceed 0.8.  In the case
	// where the array is resized, this function runs in linear time (with
	// respect to the number of elements, assuming a good hash function);
	// otherwise, it runs in constant time (again, assuming a good hash
	// function).
	virtual void add(const ElementType& element) override;


	// contains() returns true if the given element is already in the set,
	// false otherwise.  This function runs in constant time (with respect
	// to the number of elements, assuming a good hash function).
	virtual bool contains(const ElementType& element) const override;


	// size() returns the number of elements in the set.
	virtual unsigned int size() const noexcept override;


	// elementsAtIndex() returns the number of elements that hashed to a
	// particular index in the array.  If the index is out of the boundaries
	// of the array, this function returns 0.
	unsigned int elementsAtIndex(unsigned int index) const;


	// isElementAtIndex() returns true if the given element hashed to a
	// particular index in the array, false otherwise.  If the index is
	// out of the boundaries of the array, this functions returns 0.
	bool isElementAtIndex(const ElementType& element, unsigned int index) const;


private:
	HashFunction hashFunction;
	unsigned int setsize;
	unsigned int capacity;
	double DEFAULT_LOADFACTOR = 0.8;
	HashNode<ElementType>** nodearray;

	HashNode<ElementType>* newNode(const ElementType& val, HashNode<ElementType>* next);
	HashNode<ElementType>* copyNode(HashNode<ElementType>* head);
	void deleteNode(HashNode<ElementType>* head);
	unsigned int generateIndex(const ElementType& val);
	void insertNode(const ElementType& val);
	void rehash();
	HashNode<ElementType>** copyArray(HashNode<ElementType>** other, unsigned int arraysize);
	void deleteArray(HashNode<ElementType>**& other, unsigned int arraysize);

	void print()
	{
		std::cout << "\nloadfactor: " << (double)setsize / capacity << '\n';
		for(int i = 0; i < capacity; i++)
		{
			std::cout << '[' << i << ']';
			HashNode<ElementType>* head = nodearray[i];
			while(head != nullptr)
			{
				std::cout << head->value << ' ';
				head = head->next; 
			}
			std::cout << '\n';
		}
	}
};



namespace impl_
{
	template <typename ElementType>
	unsigned int HashSet__undefinedHashFunction(const ElementType& element)
	{
		return 0;
	}
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashFunction hashFunction)
	: hashFunction{hashFunction}, setsize{0}, capacity{10000}
{
	nodearray = new HashNode<ElementType>*[capacity];
	for(int i = 0; i < capacity; i++)
		nodearray[i] = nullptr;
}


template <typename ElementType>
HashSet<ElementType>::~HashSet() noexcept
{
	deleteArray(nodearray, capacity);
}


template <typename ElementType>
HashSet<ElementType>::HashSet(const HashSet& s)
	: hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}
{
	setsize = s.setsize;
	capacity = s.capacity;
	nodearray = copyArray(s.nodearray, s.capacity);
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashSet&& s) noexcept
	: hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}
{
	setsize = s.setsize;
	capacity = s.capacity;
	nodearray = copyArray(s.nodearray, s.capacity);
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(const HashSet& s)
{
	deleteArray(nodearray, capacity);
	setsize = s.setsize;
	capacity = s.capacity;
	hashFunction = s.hashFunction;
	nodearray = copyArray(s.nodearray, s.capacity);
	return *this;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(HashSet&& s) noexcept
{
	deleteArray(nodearray, capacity);
	setsize = s.setsize;
	capacity = s.capacity;
	hashFunction = s.hashFunction;
	nodearray = copyArray(s.nodearray, s.capacity);
	return *this;
}


template <typename ElementType>
bool HashSet<ElementType>::isImplemented() const noexcept
{
	return true;
}


template <typename ElementType>
void HashSet<ElementType>::add(const ElementType& element)
{
	insertNode(element);
}


template <typename ElementType>
bool HashSet<ElementType>::contains(const ElementType& element) const
{
	unsigned int hash_index = hashFunction(element) % capacity;
	return isElementAtIndex(element, hash_index);
}


template <typename ElementType>
unsigned int HashSet<ElementType>::size() const noexcept
{
	return setsize;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::elementsAtIndex(unsigned int index) const
{
	unsigned int node_num = 0;
	HashNode<ElementType>* head = nodearray[index];
	while (head != nullptr)
	{
		node_num++;
		head = head->next;
	}
	return node_num;
}


template <typename ElementType>
bool HashSet<ElementType>::isElementAtIndex(const ElementType& element, unsigned int index) const
{
	if(index >= capacity) return false;
	HashNode<ElementType>* head = nodearray[index];
	while(head != nullptr)
	{
		if(head->value == element) return true;
		head = head->next;
	}
	return false;
}
template <typename ElementType>
HashNode<ElementType>* HashSet<ElementType>::newNode(const ElementType& val, HashNode<ElementType>* next)
{
	HashNode<ElementType>* head = new HashNode<ElementType>;
	head->value = val;
	head->next = next;
	return head;
}

template <typename ElementType>
void HashSet<ElementType>::deleteNode(HashNode<ElementType>* head)
{
	if(head != nullptr)
	{
		deleteNode(head->next);
		delete head;
	}
	return;
}

template <typename ElementType>
unsigned int HashSet<ElementType>::generateIndex(const ElementType& val)
{
	return hashFunction(val) % capacity;
}

template <typename ElementType>
void HashSet<ElementType>::rehash()
{
	HashNode<ElementType>** old_array = nodearray;
	int old_capacity = capacity;
	nodearray = new HashNode<ElementType>*[capacity * 2];
	setsize = 0;
	capacity = 2 * capacity;
	for(int i = 0; i < capacity; i++)
		nodearray[i] = nullptr;
	for(int i = 0; i < old_capacity; i++)
	{
		HashNode<ElementType>* head = old_array[i];
		while(head != nullptr)
		{
			insertNode(head->value);
			head = head->next;
		}
	}
	deleteArray(old_array, old_capacity);
}

template <typename ElementType>
void HashSet<ElementType>::insertNode(const ElementType& val)
{
	int array_index = generateIndex(val);
	HashNode<ElementType>* index_head = nodearray[array_index];
	if(index_head == nullptr)
	{
		nodearray[array_index] = newNode(val, nullptr);
	}
	else
	{

		while(index_head->next != nullptr)
		{
			if(index_head->value == val)
				return;
			index_head = index_head->next;
		}
		if(index_head->value != val)
			index_head->next = newNode(val, nullptr);
	}

	setsize++;
	double loadfactor = (double)setsize / capacity;
	if(loadfactor > DEFAULT_LOADFACTOR)
		rehash();
}

template <typename ElementType>
HashNode<ElementType>* HashSet<ElementType>::copyNode(HashNode<ElementType>* head)
{
	if(head == nullptr) return nullptr;
	return newNode(head->value, copyNode(head->next));
}

template <typename ElementType>
HashNode<ElementType>** HashSet<ElementType>::copyArray(HashNode<ElementType>** other, unsigned int arraysize)
{
	HashNode<ElementType>** temp = new HashNode<ElementType>*[arraysize];
	for(int i = 0; i < arraysize; i++)
		temp[i] = copyNode(other[i]);
	return temp;
}

template <typename ElementType>
void HashSet<ElementType>::deleteArray(HashNode<ElementType>**& other, unsigned int arraysize)
{
	for(int i = 0; i < arraysize; i++)
	{
		deleteNode(other[i]);
	}
	delete[] other;
}
#endif // HASHSET_HPP