#ifndef LINKEDLISTNODE_H
#define LINKEDLISTNODE_H
#include <iostream>

template <typename T>
class LinkedListNode
{
	/**
	 * \brief outputs the data field
	 */
	template<typename U>
	friend std::ostream& operator << (std::ostream& s, LinkedListNode<U>* lln);
public:

	/**
	 * \brief the constructor
	 */
	LinkedListNode();

	/**
	 * \brief the overwritten constructor
	 * \param data the data
	 * \param indexer used to order the LinkedList
	 * \param next a pointer to another node
	 */
	LinkedListNode(T data, int indexer, LinkedListNode<T>* next);

	/**
	 * \brief the destructor
	 */
	~LinkedListNode();

	/**
	 * \brief the copy constructor
	 */
	LinkedListNode(LinkedListNode<T>* lln);

	/**
	 * \brief gets the data field
	 */
	T getData();

	/**
	 * \brief gets the indexer field
	 */
	int getIndexer();

	/**
	 * \brief gets the next field
	 */
	LinkedListNode* next();

	/**
	 * \brief sets the next field
	 * \param lln the new value of next
	 */
	void newNext(LinkedListNode<T>* lln);

	/**
	 * \brief sets the data
	 * \param data the new value of data
	 */
	void newData(T data);

private:
	LinkedListNode* nextNode;
	T data;
	int indexer;
};

template<typename U>
std::ostream& operator << (std::ostream& s, LinkedListNode<U>* lln)
{
	Eclipse a = lln->getData();
	s << &a << " ";
	return s;
}
template<typename T> LinkedListNode<T>::LinkedListNode(T data, int indexer, LinkedListNode<T>* next)
{
	this->nextNode = next;
	this->data = data;
	this->indexer = indexer;
}
template<typename T> LinkedListNode<T>::LinkedListNode()
{
	this->nextNode = NULL;
	T* a = new T();
	this->data = *a;
	this->indexer = -1;
}
template<typename T> LinkedListNode<T>::~LinkedListNode()
{
	//don't do anything because T could be a primitive type and other pointers are referenced in other code
}
template<typename T> LinkedListNode<T>::LinkedListNode(LinkedListNode<T>* lln)
{
	this->nextNode = lln->next();
	this->data = lln->getData();
	this->indexer = lln->getIndexer();
}
template<typename T> T LinkedListNode<T>::getData()
{
	return data;
}
template<typename T> int LinkedListNode<T>::getIndexer()
{
	return indexer;
}
template<typename T> LinkedListNode<T>* LinkedListNode<T>::next()
{
	return nextNode;
}
template<typename T> void LinkedListNode<T>::newNext(LinkedListNode<T>* next)
{
	this->nextNode = next;
}
template<typename T> void LinkedListNode<T>::newData(T data)
{
	this->data = data;
}
#endif //LINKEDLISTNODE_H
