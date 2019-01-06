#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <string>
#include <iostream>
#include "LinkedListNode.h"

class LLAddError{};
class LLRemoveError{};

template <typename T>
class LinkedList
{
	/*
	 * \brief Outputs data with one element per line and no other additions
	 */
	template<typename U>
	friend std::ostream& operator << (std::ostream& s, LinkedList<U>* ll);
public:
	/**
	 * \brief The default constructor
	 */
	LinkedList();

	/**
	 * \brief The destructor
	 */
	~LinkedList();

	/**
	 * \brief The copy constructor
	 * \param ll the LinkedList to be copied
	 */
	LinkedList(const LinkedList& ll);

	/**
	 * \brief Adds a LinkedList node which holds data ordered based on indexer
	 * If an element with indexer already exists, replaces it
	 * \param indexer determines the item's position within the LinkedList
	 */
	void add(T data, int indexer);

	/**
	 * \brief Removes the item with indexer
	 * \param indexer serves as a unique item identifier
	 */
	void remove(int indexer);

	/**
	 * \brief removes all elements with indexers corresponding to the indexers in indexersToPurge
	 * \param indexersToPurge a LinkedList. The data field is ignored.
	 */
	void purge(LinkedList<T> *indexersToPurge);

	/**
	 * \brief adds all elements of toMerge. Note that add replaces an element if its indexer is already in use
	 * \param toMerge the items to add to the LinkedList if not already present
	 */
	void merge(LinkedList<T> *toMerge);

	/**
	 * \brief returns the head node
	 * \return the head node
	 */
	LinkedListNode<T> getHead();

	/**
	 * \brief returns the length on the LinkedList
	 * \return the length
	 */
	int getLen();
private:
	/**
	 * The head node
	 */
	LinkedListNode<T> *head;

	/**
	 * The tail node
	 */
	LinkedListNode<T> *tail;

	/**
	 * Various pointers to use throughout the class. They are not guarentteed to have any particular value at the
	 * start of a method.
	 */
	LinkedListNode<T> *temp1;
	LinkedListNode<T> *temp2;
	LinkedListNode<T> *temp3;
	LinkedListNode<T> *temp4;

	/**
	 * The length of the LinkedList, not including the head and tail nodes. Updates in add and remove.
	 */
	int len;
};
template<typename U>
std::ostream& operator << (std::ostream& s, LinkedList<U>* ll)
{
	LinkedListNode<U>* temp;
	temp = ll->head->next();
	while(temp->getIndexer() > -1)
	{
		s << temp;
		temp = temp->next();
	}
	return s;

}
template<typename T> LinkedList<T>::LinkedList()
{
	tail = new LinkedListNode<T>(*(new T()), -1, NULL);
	if(tail == NULL)
	{
		std::cerr << "tail DNE\n";
	}
	head = new LinkedListNode<T>(*(new T()), -1, tail);
	if(head == NULL)
	{
		std::cerr << "head DNE\n";
	}
	if(head->getIndexer() != -1)
	{
		std::cerr << "head default\n";
	}
	if(head->next() == NULL)
	{
		std::cerr << "tail of head DNE\n";
	}
	temp1 = head;
	temp2 = head;
	temp3 = head;
	temp4 = head;
	len = 0;
}
template<typename T> LinkedList<T>::LinkedList(const LinkedList& ll)
{
	this->tail = ll.tail;
	this->head = ll.head;
	if(tail == NULL)
	{
		std::cerr << "tail DNE\n";
	}
	if(head == NULL)
	{
		std::cerr << "head DNE\n";
	}
	if(head->getIndexer() != -1)
	{
		std::cerr << "head default\n";
	}
	if(head->next() == NULL)
	{
		std::cerr << "tail of head DNE\n";
	}
	//temp variables are arbitrary and therefore not copied normally
	this->temp1 = head;
	this->temp2 = head;
	this->temp3 = head;
	this->temp4 = head;
	this->len = ll.len;
}
template<typename T> LinkedList<T>::~LinkedList()
{
	// This must be blank because T could be a pointer type and the objects pointed to are
	// needed in other parts of the program. Individual objects will only be deleted
	// in the "purge" and "remove" methods
}
template<typename T> void LinkedList<T>::add(T data, int indexer)
{
	temp1 = head;
	temp2 = head->next();
	while(temp2->getIndexer() >=0 && temp2->getIndexer() < indexer)
	{
		temp1 = temp2;
		temp2 = temp1->next();
	}

	if(indexer == temp2->getIndexer())
	{
		//Replace
		temp3 = temp2->next();
		remove(indexer);
		temp1->newNext(new LinkedListNode<T>(data, indexer, temp3));
	}
	else
	{
		//The new data should be inserted just before temp2
		temp1->newNext(new LinkedListNode<T>(data, indexer, temp2));
		if (temp1->next()->getIndexer() == indexer)
		{
			len += 1;
		}
		else
		{
			throw new LLAddError();
		}
	}
}
template<typename T> void LinkedList<T>::remove(int indexer)
{
	try
	{
		temp1 = head;
		temp2 = head->next();
		while(temp2->getIndexer() <= indexer)
		{
			if(temp2->getIndexer() == indexer)
			{
				temp1->newNext(temp2->next());
				delete temp2;
				len -= 1;
				break;
			}
			temp1 = temp2;
			temp2 = temp1->next();
		}
	}
	catch (...)
	{
		throw new LLRemoveError();
	}

}
template<typename T> void LinkedList<T>::purge(LinkedList<T> *indexersToPurge)
{
	temp1 = head;
	temp2 = head->next();
	temp3 = indexersToPurge->head;
	temp4 = temp3->next();
	bool exit = false;
	while(temp4->getIndexer() >= 0)
	{
		temp2 = temp1->next();
		exit = false;
		while(temp2->getIndexer() >= 0
				&& temp2->getIndexer() <=
				temp4->getIndexer())
		{
			if(temp2->getIndexer() == temp4->getIndexer())
			{
				temp1->newNext(temp2->next());
				delete temp2;
				len -= 1;
				exit = true;
				break;
			}
			temp1 = temp2;
			temp2 = temp1->next();
		}
		if(exit)
		{
			temp3 = temp4;
			temp4 = temp3->next();
			continue;
		}
		if(temp4->getIndexer() != -1)
		{
			std::cout << "No entry with indexer " << temp4->getIndexer() << " was found.\n";
		}
		temp3 = temp4;
		temp4 = temp3->next();
	}
}
template<typename T> void LinkedList<T>::merge(LinkedList<T> *toMerge)
{
	temp1 = head;
	temp2 = head->next();
	temp3 = toMerge->head;
	temp4 = temp3->next();
	if(temp4->getIndexer() >= 0)
	{
		while(temp1->getIndexer() >= 0 && temp2->getIndexer() >= 0)
		{
			while(temp4->getIndexer() <= temp2->getIndexer())
			{
				temp3 = temp4;
				temp4 = temp3->next();
			}
			//At this point, temp3 needs to be inserted
			add(temp3->getData(), temp3->getIndexer());

			temp1 = temp2;
			temp2 = temp1->next();
		}
	}
	while(temp4->getIndexer() >= 0) //all remaining elements should be added
	{
		add(temp4->getData(), temp4->getIndexer());
		temp3 = temp4;
		temp4 = temp3->next();
	}
}
template<typename T> LinkedListNode<T> LinkedList<T>::getHead()
{
	return head;
}
template<typename T> int LinkedList<T>::getLen()
{
	return len;
}
#endif //LINKEDLIST_H

