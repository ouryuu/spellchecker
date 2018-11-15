// AVLSet.hpp
//
// ICS 46 Spring 2018
// Project #4: Set the Controls for the Heart of the Sun
//
// An AVLSet is an implementation of a Set that is an AVL tree, which uses
// the algorithms we discussed in lecture to maintain balance every time a
// new element is added to the set.  The balancing is actually optional,
// with a bool parameter able to be passed to the constructor to explicitly
// turn the balancing on or off (on is default).  If the balancing is off,
// the AVL tree acts like a binary search tree (e.g., it will become
// degenerate if elements are added in ascending order).
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to implement your AVL tree
// using your own dynamically-allocated nodes, with pointers connecting them,
// and with your own balancing algorithms used.

#ifndef AVLSET_HPP
#define AVLSET_HPP

#include <functional>
#include "Set.hpp"
#include <iostream>
#include <iomanip>
template <typename ElementType>
struct Node
{
	ElementType data;
	Node<ElementType>* left;
	Node<ElementType>* right;
	int height;
};

template <typename ElementType>
class AVLSet : public Set<ElementType>
{
public:
	// A VisitFunction is a function that takes a reference to a const
	// ElementType and returns no value.
	using VisitFunction = std::function<void(const ElementType&)>;

public:
	// Initializes an AVLSet to be empty, with or without balancing.
	explicit AVLSet(bool shouldBalance = true);

	// Cleans up the AVLSet so that it leaks no memory.
	virtual ~AVLSet() noexcept;

	// Initializes a new AVLSet to be a copy of an existing one.
	AVLSet(const AVLSet& s);

	// Initializes a new AVLSet whose contents are moved from an
	// expiring one.
	AVLSet(AVLSet&& s) noexcept;

	// Assigns an existing AVLSet into another.
	AVLSet& operator=(const AVLSet& s);

	// Assigns an expiring AVLSet into another.
	AVLSet& operator=(AVLSet&& s) noexcept;


	// isImplemented() should be modified to return true if you've
	// decided to implement an AVLSet, false otherwise.
	virtual bool isImplemented() const noexcept override;


	// add() adds an element to the set.  If the element is already in the set,
	// this function has no effect.  This function always runs in O(log n) time
	// when there are n elements in the AVL tree.
	virtual void add(const ElementType& element) override;


	// contains() returns true if the given element is already in the set,
	// false otherwise.  This function always runs in O(log n) time when
	// there are n elements in the AVL tree.
	virtual bool contains(const ElementType& element) const override;


	// size() returns the number of elements in the set.
	virtual unsigned int size() const noexcept override;


	// height() returns the height of the AVL tree.  Note that, by definition,
	// the height of an empty tree is -1.
	int height() const;


	// preorder() calls the given "visit" function for each of the elements
	// in the set, in the order determined by a preorder traversal of the AVL
	// tree.
	void preorder(VisitFunction visit) const;


	// inorder() calls the given "visit" function for each of the elements
	// in the set, in the order determined by an inorder traversal of the AVL
	// tree.
	void inorder(VisitFunction visit) const;


	// postorder() calls the given "visit" function for each of the elements
	// in the set, in the order determined by a postorder traversal of the AVL
	// tree.
	void postorder(VisitFunction visit) const;
/*
	void coutNode(Node<ElementType>* head, int indent = 0)
	{
		if(head != nullptr)
		{
			coutNode(head->right, indent + 4);
			std::cout << std::setw(indent) << ' ' << head->data << '\n';
			coutNode(head->left, indent + 4);
		}

	}
	void print()
	{
		std::cout << "==============\n";
		coutNode(head);
		std::cout << "==============\n";
	}
*/


private:
	// You'll no doubt want to add member variables and "helper" member
	// functions here.


	Node<ElementType>* head;
	int treesize;
	bool balance;

	Node<ElementType>* newNode(const ElementType& element, Node<ElementType>* left, Node<ElementType>* right, int height);
	int getHeight(Node<ElementType>* head) const;
	int compareHeight(Node<ElementType>* head);
	int branchHeight(Node<ElementType>* head);
	Node<ElementType>* leftRotate(Node<ElementType>* head);
	Node<ElementType>* rightRotate(Node<ElementType>* head);
	Node<ElementType>* insertNode(const ElementType& element, Node<ElementType>* head);
	Node<ElementType>* insertNodeBST(const ElementType& element, Node<ElementType>* head);
	Node<ElementType>* copyTree(Node<ElementType>* head);
	void deleteTree(Node<ElementType>* head);
	bool containsNode(const ElementType& element, Node<ElementType>* head) const;
	void previsitnode(VisitFunction& visit, Node<ElementType>* head) const;
	void invisitnode(VisitFunction& visit, Node<ElementType>* head) const;
	void postvisitnode(VisitFunction& visit, Node<ElementType>* head) const;






};


template <typename ElementType>
AVLSet<ElementType>::AVLSet(bool shouldBalance) : head{nullptr}, treesize{0}, balance{shouldBalance}
{
}


template <typename ElementType>
AVLSet<ElementType>::~AVLSet() noexcept
{
	deleteTree(head);
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(const AVLSet& s)
{
	head = copyTree(s.head);
	treesize = s.treesize;
	balance = s.balance;
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(AVLSet&& s) noexcept
{
	head = copyTree(s.head);
	treesize = s.treesize;
	balance = s.balance;
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(const AVLSet& s)
{
	deleteTree(head);
	head = copyTree(s.head);
	treesize = s.treesize;
	balance = s.balance;
	return *this;
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(AVLSet&& s) noexcept
{
	deleteTree(head);
	head = copyTree(s.head);
	treesize = s.treesize;
	balance = s.balance;
	return *this;
}


template <typename ElementType>
bool AVLSet<ElementType>::isImplemented() const noexcept
{
	return true;
}


template <typename ElementType>
void AVLSet<ElementType>::add(const ElementType& element)
{
	if(balance)
	{
		head = insertNode(element, head);
	}
	else
	{ 
		head = insertNodeBST(element, head);
	}
	treesize++;
}


template <typename ElementType>
bool AVLSet<ElementType>::contains(const ElementType& element) const
{
	return containsNode(element, head);
}


template <typename ElementType>
unsigned int AVLSet<ElementType>::size() const noexcept
{
	return treesize;
}


template <typename ElementType>
int AVLSet<ElementType>::height() const
{
	return getHeight(head) - 1;
}


template <typename ElementType>
void AVLSet<ElementType>::preorder(VisitFunction visit) const
{
	previsitnode(visit, head);
}


template <typename ElementType>
void AVLSet<ElementType>::inorder(VisitFunction visit) const
{
	invisitnode(visit, head);
}


template <typename ElementType>
void AVLSet<ElementType>::postorder(VisitFunction visit) const
{
	postvisitnode(visit, head);
}

template <typename ElementType>
Node<ElementType>* AVLSet<ElementType>::newNode(const ElementType& element, Node<ElementType>* left, Node<ElementType>* right, int height)
{
	Node<ElementType>* head = new Node<ElementType>;
	head->data = element;
	head->left = left;
	head->right = right;
	head->height = height;
	return head;
}

template <typename ElementType>
int AVLSet<ElementType>::getHeight(Node<ElementType>* head) const
{
	if(head == nullptr) return 0;
	return head->height;
}

template <typename ElementType>
int AVLSet<ElementType>::compareHeight(Node<ElementType>* head)
{
	return getHeight(head->left) - getHeight(head->right);
}

template <typename ElementType>
int AVLSet<ElementType>::branchHeight(Node<ElementType>* head)
{
	return getHeight(head->left) > getHeight(head->right) ? getHeight(head->left) : getHeight(head->right);
}

template <typename ElementType>
Node<ElementType>* AVLSet<ElementType>::leftRotate(Node<ElementType>* head)
{
	Node<ElementType>* main_right = head->right;
	Node<ElementType>* rightleft_sub = main_right->left;
	
	main_right->left = head;
	head->right = rightleft_sub;

	head->height = branchHeight(head) + 1;
	main_right->height = branchHeight(main_right) + 1;

	return main_right;
}

template <typename ElementType>
Node<ElementType>* AVLSet<ElementType>::rightRotate(Node<ElementType>* head)
{
	Node<ElementType>* main_left = head->left;
	Node<ElementType>* leftright_sub = main_left->right;

	head->left = leftright_sub;
	main_left->right = head;

	head->height = branchHeight(head) + 1;
	main_left->height = branchHeight(main_left) + 1;

	return main_left;
}

template <typename ElementType>
Node<ElementType>* AVLSet<ElementType>::insertNode(const ElementType& element, Node<ElementType>* head)
{
	//BST insertNode
	if(head == nullptr)
	{
		return newNode(element, nullptr, nullptr, 1);
	}
	if(head->data == element)
	{
		return head;
	} 
	else if (element > head->data)
	{
		head->right = insertNode(element, head->right);
	}
	else if (element < head->data)
	{
		head->left = insertNode(element, head->left);
	}
	//update height
	head->height = 1 + branchHeight(head);

	//compare height dirrerence of left and right
	int hei_diff = compareHeight(head);

	//perform rotation

	//leftleft rotation
	if(hei_diff > 1 && element < head->left->data)
	{
		return rightRotate(head);
	}

	//leftright rotation
	if(hei_diff > 1 && element > head->left->data)
	{
		head->left = leftRotate(head->left);
		return rightRotate(head);
	}

	//rightleft rotation
	if(hei_diff < -1 && element < head->right->data)
	{
		head->right = rightRotate(head->right);
		return leftRotate(head);
	}

	//rightright rotation
	if(hei_diff < -1 && element > head->right->data)
	{
		return leftRotate(head);
	}
	//difference no more than 2, return unmodified head
	return head;
}

template <typename ElementType>
Node<ElementType>* AVLSet<ElementType>::insertNodeBST(const ElementType& element, Node<ElementType>* head)
{
	if(head == nullptr)
	{
		return newNode(element, nullptr, nullptr, 1);
	}
	if(head->data == element)
	{
		return head;
	} 
	else if (element > head->data)
	{
		head->right = insertNodeBST(element, head->right);
	}
	else if (element < head->data)
	{
		head->left = insertNodeBST(element, head->left);
	}
	head->height = 1 + branchHeight(head);
	return head;
}

template<typename ElementType>
Node<ElementType>* AVLSet<ElementType>::copyTree(Node<ElementType>* head)
{
	if(head == nullptr) return nullptr;
	return newNode(head->data, copyTree(head->left), copyTree(head->right), head->height);
}

template<typename ElementType>
void AVLSet<ElementType>::deleteTree(Node<ElementType>* head)
{
	if(head != nullptr)
	{
		deleteTree(head->left);
		deleteTree(head->right);
		delete head;
	}
}

template<typename ElementType>
bool AVLSet<ElementType>::containsNode(const ElementType& element, Node<ElementType>* head) const
{
	if (head != nullptr) 
	{
		if(element == head->data)
			return true;
		else if(element < head->data)
			return containsNode(element, head->left);
		else if(element > head-> data)
			return containsNode(element, head->right);
	}
	return false;
}

template <typename ElementType>
void AVLSet<ElementType>::previsitnode(VisitFunction& visit, Node<ElementType>* head) const
{
	if(head == nullptr) return;
	visit(head->data);
	previsitnode(visit, head->left);
	previsitnode(visit, head->right);
}
template <typename ElementType>
void AVLSet<ElementType>::invisitnode(VisitFunction& visit, Node<ElementType>* head) const
{
	if(head == nullptr) return;
	invisitnode(visit, head->left);
	visit(head->data);
	invisitnode(visit, head->right);
	return;
}
template <typename ElementType>
void AVLSet<ElementType>::postvisitnode(VisitFunction& visit, Node<ElementType>* head) const
{
	if(head == nullptr) return;
	postvisitnode(visit, head->left);
	postvisitnode(visit, head->right);
	visit(head->data);
}

#endif // AVLSET_HPP