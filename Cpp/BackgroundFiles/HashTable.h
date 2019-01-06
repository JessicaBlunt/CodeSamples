#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <string>
#include <iostream>
#include "LinkedList.h"

class ConstructionError{};
class CopyError{};
class AddError{};
class ResizeError{};
/**
 * \brief A simple Hash Table class
 *
 * 	Collision resolution: Linear probing
 * 	Hash function: Variant on multiplicative string hash
 *
 */
template <typename T>
class HashTable
{
public:
	HashTable(int len);
	HashTable(const HashTable<T>& toCopy);
	virtual ~HashTable();
	void add(int key, T toAdd);
	void remove(int key);
	T get(int key);
	void clear();
	void printOrdered();
	void printAsStored();
private:
	struct entry
	{
		T item;
		int key;
	};
	int length;
	LinkedList<T> ll;
	entry* array;
	int* spacesOccupied; //0 if empty since start, 1 if empty after removal, 2 if full
	int spacesFull;
	int hash(int key);
	void expandArr();
	int factorial(int num);

};
template <typename T> HashTable<T>::HashTable(int len)
{
	//Do not need to initialize LinkedList
	length = len;
	spacesFull = 0;
	try
	{
		array = new entry[length];
		for (int i = 0; i < length; ++i)
		{
			array[i] = *(new entry());
			array[i].item = *(new T());
			array[i].key = 0;
		}

		spacesOccupied = new int[length];
		std::fill_n(spacesOccupied, length, 0);

		//std::cout << "     SPACES OCCUPIED " << *spacesOccupied << endl;
	}
	catch (std::bad_alloc &e)
	{
		throw new ConstructionError();
	}
}
template <typename T> HashTable<T>::HashTable(const HashTable<T>& toCopy)
{
	length = toCopy.length;
	ll = toCopy.ll;
	array = toCopy.ll;
	spacesOccupied = toCopy.spacesOccupied;
	spacesFull = toCopy.spacesFull;
	if (this->length != toCopy.length || this->array != toCopy.array || this->spacesFull != toCopy.spacesFull)
	{
		throw new CopyError();
	}
}
template <typename T> HashTable<T>::~HashTable()
{
	delete [] array;
}
template <typename T> void HashTable<T>::add(int key, T toAdd)
{
	int pos = hash(key)-1;//-1 is to handle loop and end condition
	int itr = 0;
	do
	{
		itr+=1;
		pos+=1;
		//std::cout <<  "POS " << pos << std::endl;
		//std::cout << "KEY " << key << std::endl;
		//if ran off end of array
		if(pos >= length)
		{
			pos = 0;
		}
		if(spacesOccupied[pos] <= 1)
		{
			array[pos].item = toAdd;
			array[pos].key = key;
			spacesOccupied[pos] = 2;
			spacesFull+=1;
			try
			{
				ll.add(toAdd, key);
			}
			catch (LLAddError* e)
			{
				cerr << "Failed to add the following element.\n" << &toAdd << endl;
			}
			break;
		}
	}while(itr <= length);

	if(not (array[pos].item == toAdd))
	{
		//cout << *array[pos].item << "\n" << *toAdd << "\n";
		throw new AddError();
	}
	expandArr();
}
template <typename T> void HashTable<T>::remove(int key)
{
	int pos = hash(key);
	int origPos = pos;
	do
	{
		//if ran off end of array
		if(pos >= length)
		{
			pos = 0;
		}

		if(array[pos].key == key)
		{
			array[pos].item = *(new T());
			array[pos].key = 0;
			spacesOccupied[pos] = 1;
			spacesFull-=1;
			try
			{
				ll.remove(key);
			}
			catch (LLRemoveError* e)
			{
				cerr << "Failed to remove element " << key << ".\n";
			}
		}
	}while(pos != origPos && spacesOccupied[pos] != 0);
}
template <typename T> T HashTable<T>::get(int key)
{
	int pos = hash(key);
	int origPos = pos;
	do
	{
		//if ran off end of array
		if(pos >= length)
		{
			pos = 0;
		}

		if(array[pos].key == key)
		{
			return array[pos].item;
		}
	}while(pos != origPos && spacesOccupied[pos] != 0);
	return *(new T());
}
template <typename T> void HashTable<T>::clear()
{
	spacesFull = 0;
	fill_n(spacesOccupied, length, 0);
	ll = *(new LinkedList<Eclipse>());
}
template <typename T> void HashTable<T>::printOrdered()
{
	std::cout << &ll <<std::endl;
}
template <typename T> void HashTable<T>::printAsStored()
{
	for (int i = 0; i < length; ++i)
	{
		if(spacesOccupied[i] == 2)
		{
			std::cout << i << " " << &array[i].item << std::endl;
		}
		else
		{
			std::cout << i << " " << "NULL\n";
		}
	}
}
template <typename T> int HashTable<T>::hash(int key)
{
	int lengthKey = 0;
	int hash = 0;
	int keyCopy = key;
	int pow = 1;
	while (key > 0)
	{
		key = key / 10;
		pow = pow * 10;
		lengthKey++;
	}
	pow = pow/10;

	lengthKey -= 1;
	while (lengthKey >= 0)
	{
		if(hash == 0)
		{
			hash = length - factorial(keyCopy/pow);
			lengthKey -= 1;
		}
		else
		{
			if(keyCopy/pow > 0)
			{
				hash = hash * factorial((keyCopy/pow));
			}
			else
			{
				hash = hash + keyCopy/pow;
			}
			lengthKey -= 1;
		}
	}
	//Now hash is a large, randomized number
	if(((hash+keyCopy*7)%length) >= 0)
	{
		return ((hash+keyCopy*7) % length);
	}
	else
	{
		return ((hash+keyCopy*7) % length) * (-1);
	}
}
template <typename T> int HashTable<T>::factorial(int num)
{
	if (num <= 1)
	{
		return num;
	}
	else
	{
		return num * factorial(num-1);
	}
}

/**
 * \brief Doubles or halves the size of the array if needed
 */
template<typename T> void HashTable<T>::expandArr()
{
	//if load factor > 0.5
	if (spacesFull > 1 && length / spacesFull <= 3)
	{
		//Double the size
		try
		{
			entry* tempArr = new entry[length * 2];
			int *occTempArr = new int[length * 2];
			for (int i = 0; i < length * 2; ++i)
			{
				tempArr[i] = *(new entry());
				tempArr[i].key = 0;
				tempArr[i].item = *(new T());
				occTempArr[i] = 0;
			}
			//add old elems
			for (int i = 0; i < length; ++i)
			{
				if (spacesOccupied[i] == 2)
				{
					int pos = hash(array[i].key)-1;//-1 is to handle loop and end condition
					int itr = 0;
					do
					{
						itr+=1;
						pos+=1;
						//if ran off end of array
						if(pos >= length*2)
						{
							pos = 0;
						}
						if(occTempArr[pos] <= 1)
						{
							tempArr[pos].item = array[i].item;
							tempArr[pos].key = array[i].key;
							occTempArr[pos] = 2;
							break;
						}
						}while(itr <= length);

						if(not (array[i].item == tempArr[pos].item))
						{
							//cout << *array[pos].item << "\n" << *toAdd << "\n";
							throw new AddError();
						}
				}
			}
			length *= 2;
			//delete[] arr;
			array = tempArr;
			spacesOccupied = occTempArr;
			tempArr = 0;
		}
		catch (std::bad_alloc &e)
		{
			throw new ResizeError();
		}
	}
}

#endif //HASHTABLE_H
