#include <iostream>
#ifndef AVL_H
#define AVL_H

#include "ResizableArray.h"
#include "HashTable.h"

class AVLConstructError{};
class AVLDestructError{};
class AVLGetError{};
/**
 * This class is defined recursively.
 *
			//If head and tree empty
			AVL<T>(true, T(),-999);
 */
template <typename T>
class AVL
{
	template <typename U>
	friend std::ostream& operator << (std::ostream& s, AVL<U> *e);
public:
	/** true for the original root only */
	bool isRoot = false;
	/** used to position the node */
	int indexer = -999;
	/** the constructor */
	AVL(bool isRoot, T data, int indexer);
	/** the copy constructor */
	AVL(AVL<T>* toCopy);
	/** the destructor */
	~AVL();
	/** finds the node with the given indexer */
	AVL<T> find(int indexer);
	/** return a node's contents */
	T getData();
	/** add a node with data data and indexer indexer */
	bool add(T data, int indexer);
	/** remove the node with the given indexer */
	bool remove(int indexer);
	/** check if two nodes are equal*/
	bool operator==(const AVL<T>* other);
	/** get the number of nodes in the tree */
	int getSize();
	/** fill an array with the contents of the tree */
	void fillArr(ResizableArray<T>* arr);
	/** fill a hash table with the contents of the tree */
	void fillHT(HashTable<T>* ht);
	/** merge two trees */
	void merge(const AVL<T>* other);
	/** remove all elements of this tree with indexers found in other */
	void purge(const AVL<T>* other);
	/** print post order */
	void postOrderPrint();
	/** print pre order */
	void preOrderPrint();
	/** balance the tree <broken>*/
	int balance(bool isFirst = true);
private:
	int balanceFactor = 0;
	T data;
	AVL<T> *left = NULL;
	AVL<T> *right = NULL;
	void zig();
	void zag();
	void zigzag();
	void zagzig();
};
template <typename U>
std::ostream& operator << (std::ostream& s, AVL<U> *e)
{
	if (NULL != e->left)
	{
		s << e->left;
	}
	s << &e->data;
	if (NULL != e->right)
	{
		s << e->right;
	}
	return s;
}
template <typename T> AVL<T>::AVL(bool isRoot, T data, int indexer)
{
	if(data == *(new T()))
	{
		return;
	}
	try
	{

		this->isRoot = isRoot;
		this->data = data;
		this->indexer = indexer;
		left = NULL;
		right = NULL;
		balanceFactor = 0;
		//cout << "\n\ncreating node" <<indexer << "\nisRoot "<< this->isRoot << "\ncatNo " << this->data.getCatNo() << "\nplateNo" << this->data.getPlateNo() << "\nyear " << this->data.getYear() << "\nmonth " << this->data.getMonth() << "\nindexer " << this->indexer << endl;
		if(not (this->data == data))
		{
			cout << "no match\n";
		}
	}
	catch(...)
	{
		throw new AVLConstructError();
	}
}
template <typename T> AVL<T>::AVL(AVL<T>* toCopy)
{
	this->isRoot = toCopy->isRoot;
	this->data = toCopy->data;
	this->indexer = toCopy->indexer;
	this->balanceFactor = toCopy->balanceFactor;
	if(toCopy->left == NULL)
	{
		this->left = NULL;
	}
	else
	{
		this->left = new AVL<T>(toCopy->left); //used new here for deep copy
	}
	if(toCopy->right == NULL)
	{
		this->right = NULL;
	}
	else
	{
		this->right = new AVL<T>(toCopy->right); //used new here for deep copy
	}
	if(not (this->isRoot == toCopy->isRoot && this->data == toCopy->data && this->indexer == toCopy->indexer))
	{
		throw new AVLConstructError();
	}
}
template <typename T> AVL<T>::~AVL()
{

}
template <typename T> AVL<T> AVL<T>::find(int indexer)
{
	if(this->indexer == indexer)
	{
		return this;
	}
	else if(this->indexer > indexer)
	{
		if(left == NULL)
		{
			throw new AVLGetError();
		}
		else
		{
			return left->find(indexer);
		}
	}
	else if(this->indexer < indexer)
	{
		if(right == NULL)
		{
			throw new AVLGetError();
		}
		else
		{
			return right->find(indexer);
		}
	}
}
template <typename T> T AVL<T>::getData()
{
	return data;
}
/**
 * returns true if got deeper
 */
template <typename T> bool AVL<T>::add(T data, int indexer)
{
	bool toReturn = false;

	if(this->indexer == -999)
	{
		this->data = data;
		this->indexer = indexer;
	}
	if(indexer < this->indexer)
	{
		if(NULL == left)
		{
			left = new AVL(false, data, indexer);
			balanceFactor -= 1;
			if (right == NULL) //or if balance factor != 0
			{
				balance();
				toReturn = true;
			}
			else
			{
				balance();
				toReturn = false;
			}
		}
		else
		{
			if(left->add(data, indexer)) //if the left subtree got deeper
			{
				balanceFactor -= 1;
				if(0 == balanceFactor) //if the subtree was balanced, it did not get deeper
				{
					balance();
					toReturn = false;
				}
				else
				{
					balance();
					toReturn = true;
				}
			}
			else
			{
				//do nothing - the subtree did not change length
			}
		}
	}
	else if(indexer > this->indexer)
	{
		if(NULL == right)
		{
			right = new AVL(false, data, indexer);
			balanceFactor += 1;
			if (left == NULL) //or if balance factor != 0
			{
				balance();
				toReturn = true;
			}
			else
			{
				balance();
				toReturn = false;
			}
		}
		else
		{
			if(right->add(data, indexer)) //if the left subtree got deeper
			{
				balanceFactor += 1;
				if(0 == balanceFactor) //if the subtree was balanced, it did not get deeper
				{
					balance();
					toReturn = false;
				}
				else
				{
					balance();
					toReturn = true;
				}
			}
			else
			{
				//do nothing - the subtree did not change length
			}
		}
	}
	else if(indexer == this->indexer)
	{
		this->data = data;
		//return false;
	}
	balance();
	return toReturn;
}
template <typename T> void AVL<T>::merge(const AVL<T>* other)
{
	add(other->data, other->indexer);
	if(NULL != other->left)
	{
		merge(other->left);
	}
	if(NULL != other->right)
	{
		merge(other->right);
	}
}
template <typename T> void AVL<T>::purge(const AVL<T>* other)
{
	remove(other->indexer);
	if(NULL != other->left)
	{
		purge(other->left);
	}
	if(NULL != other->right)
	{
		purge(other->right);
	}
}
/**
 * returns true if got shallower
 */
template <typename T> bool AVL<T>::remove(int indexer)
{
	bool shortend = false;
	//handles deletion of root
	if(isRoot && this->indexer == indexer)
	{
		if(NULL == left && NULL == right)
		{
			//If head and tree empty
			new (this) AVL<T>(true, *(new T()), -999);
			balanceFactor = 0;
			shortend = true;
		}
		else if(NULL == right)
		{
			AVL<T> *temp = left; //deliberately a pointer so that left can be deleted after reassignment
			indexer = temp->indexer;
			data = temp->data;
			left = temp->left;
			right = temp->right;
			balanceFactor += 1;
			delete temp;
			if (balanceFactor == 0)//if balanced through removal, shortened
			{
				shortend = true;
			}
			else
			{
				shortend = false;
			}
		}
		else if(NULL == left)
		{
			AVL<T> *temp = right; //deliberately a pointer so that right can be deleted after reassignment
			indexer = temp->indexer;
			data = temp->data;
			left = temp->left;
			right = temp->right;
			balanceFactor -= 1;
			delete temp;
			if (balanceFactor == 0)//if balanced through removal, shortened
			{
				shortend = true;
			}
			else
			{
				shortend = false;
			}
		}
		else
		{
			AVL<T> *temp = left;
			while(temp->right != NULL)
			{
				if(NULL == temp->left)
				{
					shortend = true;
				}
				else
				{
					shortend = false;
				}
				temp = temp->right;
			}
			//make copy of temp
			AVL<T> temp2 = *temp;
			remove(temp->indexer); //handles setting to NULL and adjusting balance factors
			//Copy data from node being brought up to node being removed
			indexer = temp2.indexer;
			data = temp2.data;
			//left and right do not change
		}
	}
	else
	{
		if(NULL != left && left->indexer == indexer)
		{
			if(NULL != left->left && NULL != left->right)
			{
				//has 2 children - move up central node
				AVL<T> *temp = left->left;
				while(temp->right != NULL)
				{
					if(NULL == temp->left)
					{
						shortend = true;
					}
					else
					{
						shortend = false;
					}
					temp = temp->right;
				}
				//make copy of temp
				AVL<T> temp2 = *temp;
				remove(temp->indexer); //handles setting to NULL and adjusting balance factors
				//Copy data from node being brought up to node being removed
				left->indexer = temp2.indexer;
				left->data = temp2.data;
			}
			else if(NULL != left->left)
			{
				//has 1 child - move up left
				AVL<T> *temp = left->left; //deliberately a pointer so that left can be deleted after reassignment
				left->indexer = temp->indexer;
				left->data = temp->data;
				left->left = temp->left;
				left->right = temp->right;
				left->balanceFactor += 1;
				delete temp;
				if (left->balanceFactor == 0)//if balanced through removal, shortened
				{
					balanceFactor += 1;
					shortend = true;
				}
				else
				{
					shortend = false;
				}
			}
			else if(NULL != left->right)
			{
				AVL<T> *temp = left->right; //deliberately a pointer so that right can be deleted after reassignment
				left->indexer = temp->indexer;
				left->data = temp->data;
				left->left = temp->left;
				left->right = temp->right;
				left->balanceFactor -= 1;
				delete temp;
				if (left->balanceFactor == 0)//if balanced through removal, shortened
				{
					balanceFactor += 1;
					shortend = true;
				}
				else
				{
					shortend = false;
				}
			}
			else
			{
				//no children
				left = NULL;
				balanceFactor += 1;
			}
		}
		else if(NULL != right && right->indexer == indexer)
		{
			if(NULL != right->left && NULL != right->right)
			{
				//has 2 children - move up central node
				AVL<T> *temp = right->left;
				while(temp->right != NULL)
				{
					if(NULL == temp->left)
					{
						shortend = true;
					}
					else
					{
						shortend = false;
					}
					temp = temp->right;
				}
				//make copy of temp
				AVL<T> temp2 = *temp;
				remove(temp->indexer); //handles setting to NULL and adjusting balance factors
				//Copy data from node being brought up to node being removed
				right->indexer = temp2.indexer;
				right->data = temp2.data;
			}
			else if(NULL != right->left)
			{
				//has 1 child - move up left
				AVL<T> *temp = right->left; //deliberately a pointer so that left can be deleted after reassignment
				right->indexer = temp->indexer;
				right->data = temp->data;
				right->left = temp->left;
				right->right = temp->right;
				right->balanceFactor += 1;
				delete temp;
				if (right->balanceFactor == 0)//if balanced through removal, shortened
				{
					balanceFactor -= 1;
					shortend = true;
				}
				else
				{
					shortend = false;
				}
			}
			else if(NULL != right->right)
			{
				AVL<T> *temp = right->right; //deliberately a pointer so that right can be deleted after reassignment
				right->indexer = temp->indexer;
				right->data = temp->data;
				right->left = temp->left;
				right->right = temp->right;
				right->balanceFactor -= 1;
				delete temp;
				if (right->balanceFactor == 0)//if balanced through removal, shortened
				{

					balanceFactor -= 1;
					shortend = true;
				}
				else
				{
					shortend = false;
				}
			}
			else
			{
				//no children
				right = NULL;
				balanceFactor += 1;
			}
		}
		else
		{
			if(this->indexer > indexer)
			{
				if(left == NULL)
				{
					//do nothing - nothing to remove
				}
				else
				{
					if(left->remove(indexer))
					{
						balanceFactor += 1;
					}
				}
			}
			else if(this->indexer < indexer)
			{
				if(right == NULL)
				{
					//do nothing - nothing to remove
				}
				else
				{
					if(right->remove(indexer))
					{
						balanceFactor -= 1;
					}
				}
			}
		}
	}
	balance();
	return shortend;
}
template <typename T> bool AVL<T>::operator==(const AVL<T>* other)
{
	if(data == other->data && indexer ==other->indexer)
	{
		return true;
	}
	else
	{
		return false;
	}
}
template <typename T> int AVL<T>::getSize()
{
	int size = 1; //root
	if(NULL != left)
	{
		size += left->getSize();
	}
	if(NULL != right)
	{
		size += right->getSize();
	}
	return size;
}
template <typename T> void AVL<T>::fillArr(ResizableArray<T>* arr)
{
	if(NULL != left)
	{
		left->fillArr(arr);
	}
	arr->add(data);
	if(NULL != right)
	{
		right->fillArr(arr);
	}
}
template <typename T> void AVL<T>::fillHT(HashTable<T>* ht)
{
	if(NULL != left)
	{
		left->fillHT(ht);
	}
	ht->add(indexer, data);
	if(NULL != right)
	{
		right->fillHT(ht);
	}
}
template <typename T> int AVL<T>::balance(bool isFirst)
{
	int oldLeftBal = 0;
	int oldRightBal = 0;

	/*
	if((!isRoot) && (isFirst))
	{
		return 0;
	}
	*/

	if(NULL != left)
	{
		oldLeftBal = left->balanceFactor;
	}
	if(NULL != right)
	{
		oldRightBal = right->balanceFactor;
	}

	//Traverses down recursively, works up after returns
	if(NULL != left)
	{
		left->balance(false);
	}
	if(NULL != right)
	{
		right->balance(false);
	}

	if(NULL != left && left->balanceFactor != oldLeftBal)
	{
		balanceFactor += 1;
	}
	else if(NULL != right && right->balanceFactor != oldRightBal)
	{
		balanceFactor -= 1;
	}

	if(balanceFactor >= 2)
	{
		//right-heavy
		if(right->balanceFactor < 0)
		{
			zagzig();
		}
		else
		{
			zag();
		}
		return -1;
	}
	else if(balanceFactor <= -2)
	{
		//left-heavy
		if(left->balanceFactor > 0)
		{
			zigzag();
		}
		else
		{
			zig();
		}
		return 1;
	}
	return 0;
}
template <typename T> void AVL<T>::zag()
{
	/**
	 * left child becomes the new root.
	 * primary node takes ownership of right's left child as its right child, or in this case, null.
	 * right child takes ownership of primary node as its left child
	 * CITE: https://www.cise.ufl.edu/~nemo/cop3530/AVL-Tree-Rotations.pdf
	 * <comment only, not code>
	*/

	AVL<T> *temp = new AVL(this);

	cout << "PRE-ZAG\n" << temp;
	//move right child to root
	delete right;
	data = temp->right->data;
	indexer = temp->right->indexer;

	//position previous root as left child of new
	left = new AVL(false, temp->data, temp->indexer);

	//move right grandchild up
	right = temp->right->right;
	balanceFactor = 0;
	cout << "POST-ZAG\n" << this;

}
template <typename T> void AVL<T>::zig()
{
	AVL<T> *temp = new AVL(this);

	//move left child to root
	delete left;
	data = temp->left->data;
	indexer = temp->left->indexer;

	//position previous root as right child of new
	right = new AVL(false, temp->data, temp->indexer);
	if(NULL != temp->right)
	{
		right->right = temp->right;
	}

	//move left grandchild up
	left = temp->left->left;

	if(NULL != temp->left->right)
	{
		right->left = temp->left->right;
	}
	balanceFactor = 0;

}
template <typename T> void AVL<T>::zigzag()
{
	left->zag();
	zig();
}
template <typename T> void AVL<T>::zagzig()
{
	right->zig();
	zag();
}
template <typename T> void AVL<T>::postOrderPrint()
{
	if(NULL != left)
	{
		left->postOrderPrint();
	}
	if(NULL != right)
	{
		right->postOrderPrint();
	}
	if(indexer != -999)
	{
		std::cout << &data << std::endl;
	}
}
template <typename T> void AVL<T>::preOrderPrint()
{
	if(indexer != -999)
	{
		std::cout << &data << std::endl;
	}
	if(NULL != left)
	{
		left->preOrderPrint();
	}
	if(NULL != right)
	{
		right->preOrderPrint();
	}
}

#endif //AVL_H
