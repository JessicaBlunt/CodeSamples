#ifndef PARALLELRESIZABLEARRAY_H
#define PARALLELRESIZABLEARRAY_H
#include <string>
#include "ResizableArray.h"
#include "Eclipse.h"

/**
 * This class manages several parallel ResizableArrays of eclipse data.
 */
class ParallelResizableArray
{
	/**
	 * \brief outputs the array with one element per line
	 */
	friend std::ostream& operator << (std::ostream& s, ParallelResizableArray* e);
public:

	/**
	 * \brief the constructor
	 */
	ParallelResizableArray();

	/**
	 * \brief the copy constructor
	 * \param toCopy the existing ParallelResizableArray to be copied
	 */
	ParallelResizableArray(const ParallelResizableArray& toCopy);

	/**
	 * \brief sorts the requested field and moves all other fields in parallel
	 * \param field the number of the field (1-18) to be sorted
	 * \param front the index of the front of the segment to be sorted
	 * \param back the index of the end of the segment to be sorted
	 * \param sample any object of the same type as the ResizableArray to be sorted
	 */
	void sort(int field, int front, int back, int sample);

	/**
	 * \brief sorts the requested field and moves all other fields in parallel
	 * \param field the number of the field (1-18) to be sorted
	 * \param front the index of the front of the segment to be sorted
	 * \param back the index of the end of the segment to be sorted
	 * \param sample any object of the same type as the ResizableArray to be sorted
	 */
	void sort(int field, int front, int back, double sample);

	/**
	 * \brief sorts the requested field and moves all other fields in parallel
	 * \param field the number of the field (1-18) to be sorted
	 * \param front the index of the front of the segment to be sorted
	 * \param back the index of the end of the segment to be sorted
	 * \param sample any object of the same type as the ResizableArray to be sorted
	 */
	void sort(int field, int front, int back, string sample);

	/**
	 * \brief Disassembles an Eclipse and adds each component to the appropriate array
	 * \param addNew the Eclipse to be added
	 * \return 1 if a new set of elements was created, 2 if a pre-existing element was replaced, or 0 if failled
	 */
	int addNew(Eclipse addNew);

	/**
	 * \brief prints the original data line for each eclipse matching the given criteria
	 * \param field the field to be searched (1-18)
	 * \param elem the element to be searched for
	 * \param front the first index of the array segment to be searched
	 * \param back the last index of the array segment to be searched
	 * \return the index of the last matching element or -1 if none were found
	 */
	int find(int field, int elem, int front, int back, bool toPrint);

	/**
	 * \brief prints the original data line for each eclipse matching the given criteria
	 * \param field the field to be searched (1-18)
	 * \param elem the element to be searched for
	 * \param front the first index of the array segment to be searched
	 * \param back the last index of the array segment to be searched
	 * \return the index of the last matching element or -1 if none were found
	 */
	int find(int field, double elem, int front, int back, bool toPrint);

	/**
	 * \brief prints the original data line for each eclipse matching the given criteria
	 * \param field the field to be searched (1-18)
	 * \param elem the element to be searched for
	 * \param front the first index of the array segment to be searched
	 * \param back the last index of the array segment to be searched
	 * \return the index of the last matching element or -1 if none were found
	 */
	int find(int field, string elem, int front, int back, bool toPrint);

	/**
	 * \finds the first index at which a catNo-sorted ParallelResizableArray is greater than elem (linear)
	 * \param elem the element to be searched for
	 * \return the index or -1 if none were found
	 */
	int findSpot(int elem);

	/**
	 * \brief gets the effective length of the component arrays
	 * \return the number of spaces filled in each ResizableArray
	 */
	int getLen();

	/**
	 * \brief swaps the elements at the given indices in all component ResizableArrays
	 * \param a one index
	 * \param b the other index
	 */
	void swap(int a, int b);

	/**
	 * \brief gets the item at a given index of a given field
	 * \param field the field from which the item should be retrieved (1-18)
	 * \param ind the index the item should be retrieved from
	 * \param sample any object of the same data type as the object to be returned
	 * \return the object at index ind of field field
	 */
	int getIndex(int& field, int& ind, int sample);

	/**
	 * \brief gets the item at a given index of a given field
	 * \param field the field from which the item should be retrieved (1-18)
	 * \param ind the index the item should be retrieved from
	 * \param sample any object of the same data type as the object to be returned
	 * \return the object at index ind of field field
	 */
	double getIndex(int& field, int& ind, double sample);

	/**
	 * \brief gets the item at a given index of a given field
	 * \param field the field from which the item should be retrieved (1-18)
	 * \param ind the index the item should be retrieved from
	 * \param sample any object of the same data type as the object to be returned
	 * \return the object at index ind of field field
	 */
	string getIndex(int& field, int& ind, string sample);

private:
	ResizableArray<int*> f1;
	ResizableArray<int*> f2;
	ResizableArray<int*> f3;
	ResizableArray<std::string*> f4;
	ResizableArray<int*> f5;
	ResizableArray<std::string*> f6;
	ResizableArray<int*> f7;
	ResizableArray<int*> f8;
	ResizableArray<int*> f9;
	ResizableArray<std::string*> f10;
	ResizableArray<double*> f11;
	ResizableArray<double*> f12;
	ResizableArray<double*> f13;
	ResizableArray<double*> f14;
	ResizableArray<int*> f15;
	ResizableArray<int*> f16;
	ResizableArray<int*> f17;
	ResizableArray<std::string*> f18;
	ResizableArray<std::string*> master; //holds original input for each eclipse
	bool areClose(double a, double b);
	int getMonthInd(string month);
	int sortField;
};

#endif //PARALLELRESIZABLEARRAY_H

