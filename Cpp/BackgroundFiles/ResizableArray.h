#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H
#include <string>
#include <iostream>

class RAAddError{};
class RAConstructError{};
class RACopyError{};
class RADestructError{};
class RARemoveError{};
class RAReplaceError{};
class RAGetError{};


template <typename T>
class ResizableArray
{
	/**
	 * \brief Outputs DataLines from last to first
	 */
	template<typename U>
	friend std::ostream& operator << (std::ostream& s, ResizableArray<U>* da);
	template<typename U>
	friend std::ostream& operator << (std::ostream& s, ResizableArray<U*>* da);

	public:

		/**
		 * \brief The default constructor
		 */
		ResizableArray();

		/**
		 * \brief A constructor
		 */
		ResizableArray(int len);

		/**
		 * \brief The copy constructor
		 */
		ResizableArray(const ResizableArray& toCopy);

		/**
		 * \brief The copy assignment operator
		 */
		ResizableArray& operator=(const ResizableArray& toCopy);

		/**
		 * \brief The destructor
		 */
		virtual ~ResizableArray();

		/**
		 * \brief Gets and removes the last element of the array
		 * \return a default DataLine if the array is empty
		 * \return the last DataLine otherwise
		 *
		 * Loops through the array to delete and return the last DataLine
		 */
		T remove();

		/**
		 * \brief Replace the item at the specified index
		 * \param toAdd The item to be added
		 * \param pos The index of the item to be replaced
		 */
		void replaceAt(T toAdd, int pos);

		/**
		 * \brief Returns the object at the specified index
		 * \param pos :: the index
		 */
		T getIndex(int pos) const;

		/**
		 * \brief Removes and deletes the object at the specified index
		 * \param pos :: the index
		 */
		void removeAt(int pos);

		/**
		 * \brief Adds a DataLine to a specified index the array
		 * \param toAdd :: the DataLine to add to the array
		 * \param pos :: the index at which the object should be added
		 *
		 * Adds a DataLine object to the end of the array and expands the array if needed
		 */
		void addAt(T toAdd, int pos);

		/**
		 * \brief Adds a DataLine to the end of the array
		 * \param toAdd :: the DataLine to add to the array
		 *
		 * Adds a DataLine object to the end of the array and expands the array if needed
		 */
		void add(T toAdd);

		/**
		 * \brief Outputs all elements of the array from index 0 to index len-1
		 */
		void displayForwards();

		/**
		 * \brief Outputs all elements of the array from index len-1 to index 0
		 */
		void displayBackwards();

		/**
		 *\brief gets the length of the array
		 *\return the length of the array as a copy
		 */
		int getSpacesFull();

		/**
		 * \brief finds the index of an element
		 * \return -1 if not found, otherwise index
		 * \param toFind the item whose index is to be found
		 */
		int find(T toFind);

		/**
		 * \brief swaps two elements
		 * \param a the index of one element
		 * \param b the index of the other element
		 */
		void swap(int a, int b);

	private:
		T* arr; /** a pointer to the array*/
		int len; /** saves the length of the array*/
		int spacesFull; /** saves the number of spaces filled */

		/**
		 * \brief Doubles or halves the size of the array if needed
		 */
		void resizeArr();
};

/**
 * An array which changes size - items can be added to the back and removed from the front or back.
 */
template<typename U>
std::ostream& operator << (std::ostream& s, ResizableArray<U>* da)
{
	for (int i = 0; i < da->spacesFull; ++i)
	{
		s << da->arr[i] << "\n";
	}
	return s;
}
template<typename U>
std::ostream& operator << (std::ostream& s, ResizableArray<U*>* da)
{
	for (int i = 0; i < da->spacesFull; ++i)
	{
		s << *da->arr[i] << "\n";
	}
	return s;
}
/**
 * \brief The default constructor
 *
 * Default initial length is 10
 */
template<typename T> ResizableArray<T>::ResizableArray()
{
	len = 10;
	spacesFull = 0;
	try
	{
		arr = new T[len];
		std::fill_n(arr, 10, *(new T()));
	}
	catch (std::bad_alloc &e)
	{
		throw new RAConstructError();
		return;
	}
}

/**
 * \brief A constructor
 */
template<typename T> ResizableArray<T>::ResizableArray(int len)
{
	this->len = len;
	if (this->len == NULL)
	{
		std::cerr << "Error in ResizableArray overloaded constructor\n";
		return;
	}
	spacesFull = 0;
	try
	{
		arr = new T[len];
	}
	catch (std::bad_alloc &e)
	{
		throw new RAConstructError();
		return;
	}
	fill_n(arr, len, new T());
}

/**
 * \brief The copy constructor
 */
template<typename T> ResizableArray<T>::ResizableArray(const ResizableArray& toCopy)
{
	this->len = toCopy.len;
	this->len = len;
	if (this->len == NULL)
	{
		throw new RAConstructError();
		return;
	}
	this->spacesFull = toCopy.spacesFull;
	if (this->spacesFull == NULL)
	{
		throw new RACopyError();
		return;
	}
	try
	{
		this->arr = new T[len];
	}
	catch (std::bad_alloc &e)
	{
		throw new RACopyError();
		return;
	}

	//Fill the array
	for (int i = 0; i < spacesFull; ++i)
	{
		this->arr[i] = toCopy.arr[i];
	}
	for (int i = spacesFull; i < len; ++i)
	{
		this->arr[i] = *(new T());
	}

}

/**
 * \brief The copy assignment operator
 */
template<typename T> ResizableArray<T>& ResizableArray<T>::operator=(const ResizableArray& toCopy)
{
	if (&toCopy == this)
	{
		return *this;
	}

	try
	{
		delete[] &(this->arr);
	}
	catch (std::bad_alloc &e)
	{
		throw new RACopyError();
	}
	this->len = toCopy.len;
	if (this->len == NULL)
	{
		throw new RACopyError();
	}
	this->spacesFull = toCopy.spacesFull;
	if (this->spacesFull == NULL)
	{
		throw new RACopyError();
	}
	try
	{
		this->arr = new T[len];
	}
	catch(std::bad_alloc &e)
	{
		throw new RACopyError();
	}

	//Fill the array
	for (int i = 0; i < spacesFull; ++i)
	{
		this->arr[i] = toCopy.arr[i];
		if (this->arr[i] != toCopy.arr[i])
		{
			throw new RACopyError();
		}
	}
	for (int i = spacesFull; i < len; ++i)
	{
		this->arr[i] = *(new T());
		if (this->arr[i] != toCopy.arr[i])
		{
			throw new RACopyError();
		}
	}

	return *this;
}

/**
 * \brief The destructor
 */
template<typename T> ResizableArray<T>::~ResizableArray()
{
	//Delete all DataLines
	try
	{
		delete[] arr;
	}
	catch (std::bad_alloc &e)
	{
		throw new RADestructError();
		return;
	}

}

/**
 * \brief Gets and removes the last element of the array
 * \return a default DataLine if the array is empty
 * \return the last DataLine otherwise
 *
 * Loops through the array to delete and return the last DataLine
 */
template<typename T> T ResizableArray<T>::remove()
{
	try
	{
		T toReturn = getIndex(spacesFull-1);
		removeAt(spacesFull - 1); //This also adjusts spacesFull
		resizeArr();
		return toReturn;
	}
	catch(std::bad_alloc& e)
	{
		throw new RARemoveError();
		return -1;
	}
}

/**
 * \brief Returns the object at the specified index
 * \param pos :: the index
 */
template<typename T> T ResizableArray<T>::getIndex(int pos) const
{
	try
	{
		return arr[pos];
	}
	catch(std::bad_alloc& e)
	{
		throw new RAGetError();
 	}
}

/**
 * \brief Removes and deletes the object at the specified index
 * \param pos :: the index
 */
template<typename T> void ResizableArray<T>::removeAt(int pos)
{
	//delete arr[pos];

	for (int i = pos; i < spacesFull-1; ++i)
	{
		arr[i] = arr[i+1];
	}
	arr[spacesFull - 1] = *(new T());
	if (arr[spacesFull - 1] == NULL)
	{
		throw new RARemoveError;
	}
	spacesFull -= 1;
	resizeArr();
}

/**
 * \brief Replace the item at the specified index
 * \param toAdd The item to be added
 * \param pos The index of the item to be replaced
 */
template<typename T> void ResizableArray<T>::replaceAt(T toAdd, int pos)
{
	try
	{
		arr[pos] = toAdd;
		if (arr[pos] != toAdd)
		{
			throw new RAReplaceError();
		}
	}
	catch (std::bad_alloc& e)
	{
		throw new RAReplaceError();
	}
}

/**
 * \brief Adds a DataLine to a specified index the array
 * \param toAdd :: the DataLine to add to the array
 * \param pos :: the index at which the object should be added
 *
 * Adds a DataLine object to the end of the array and expands the array if needed
 */
template<typename T> void ResizableArray<T>::addAt(T toAdd, int pos)
{
	try
	{
		T *tempArr = new T[len];
		int curInd;
		for (curInd = 0; curInd < pos; ++curInd)
		{
			tempArr[curInd] = arr[curInd];
		}
		tempArr[pos] = toAdd;
		for (; curInd < spacesFull; ++curInd)
		{
			tempArr[curInd + 1] = arr[curInd];
		}
		spacesFull += 1;
		//delete[] arr;
		arr = tempArr;
		tempArr = 0;
	}
	catch (std::bad_alloc &e)
	{
		throw new RAAddError();
	}
	resizeArr(); //as always after changing the number of elements to ensure at least one more can fit
}

/**
 * \brief Adds a DataLine to the end of the array
 * \param toAdd :: the DataLine to add to the array
 *
 * Adds a DataLine object to the end of the array and expands the array if needed
 */
template<typename T> void ResizableArray<T>::add(T toAdd)
{
	addAt(toAdd, spacesFull); //insert just after the last filled index
}

/**
 * \brief Outputs all elements of the array from index 0 to index len-1
 */
template<typename T> void ResizableArray<T>::displayForwards()
{
	for (int i = 0; i < spacesFull; ++i)
	{
		std::cout << arr[i];
	}
}

/**
 * \brief Outputs all elements of the array from index len-1 to index 0
 */
template<typename T> void ResizableArray<T>::displayBackwards()
{
	for (int i = spacesFull-1; i >= 0; --i)
	{
		std::cout << arr[i];
	}
}

/**
 * \brief Gets the size of the array - note that some spaces included will be empty
 */
template<typename T> int ResizableArray<T>::getSpacesFull()
{
	return spacesFull;
}

/**
 * \brief Doubles or halves the size of the array if needed
 */
template<typename T> void ResizableArray<T>::resizeArr()
{
	//if not room for one more...
	if (len - spacesFull < 1)
	{
		//Double the size
		try
		{
			T *tempArr = new T[len * 2];
			for (int i = 0; i < len; ++i) //Copy the old array into the new
			{
				tempArr[i] = arr[i];
			}
			for (int i = len; i < len * 2; ++i)
			{
				tempArr[i] = *(new T());
			}
			len *= 2;
			//delete[] arr;
			arr = tempArr;
			tempArr = 0;
		}
		catch (std::bad_alloc &e)
		{
			throw new RAConstructError();
		}
	}
	//if room for at least one more after cutting size in half...
	if ((len/2) - spacesFull >= 1)
	{
		try
		{
		T *tempArr = new T[len / 2];
		for (int i = 0; i < len / 2; ++i)
		{
			tempArr[i] = arr[i];
		}
		//delete[] arr;
		arr = tempArr;
		tempArr = 0;
		}
		catch (std::bad_alloc &e)
		{
			throw new RAConstructError();
		}
	}
}

/**
 * \brief finds the index of an element
 * \return 0 if not found, otherwise index
 */
template<typename T> int ResizableArray<T>::find(T toFind)
{
	for(int i = 0; i < spacesFull; ++i)
	{
		if (arr[i] == toFind)
		{
			return i;
		}
	}
	return -1;
}


template<typename T> void ResizableArray<T>::swap(int a, int b)
{
	T temp = arr[a];
	arr[a] = arr[b];
	arr[b] = temp;
}



#endif


