#include <string>
#include "ResizableArray.h"
#include "ParallelResizableArray.h"
#include "Eclipse.h"

std::ostream& operator << (std::ostream& s, ParallelResizableArray* e)
{
	s << &(e->master);
	return s;
}

ParallelResizableArray::ParallelResizableArray()
{
	sortField = 1;
}
ParallelResizableArray::ParallelResizableArray(const ParallelResizableArray& toCopy)
{
	this->f1 = toCopy.f1;
	this->f2 = toCopy.f2;
	this->f3 = toCopy.f3;
	this->f4 = toCopy.f4;
	this->f5 = toCopy.f5;
	this->f6 = toCopy.f6;
	this->f7 = toCopy.f7;
	this->f8 = toCopy.f8;
	this->f9 = toCopy.f9;
	this->f10 = toCopy.f10;
	this->f11 = toCopy.f11;
	this->f12 = toCopy.f12;
	this->f13 = toCopy.f13;
	this->f14 = toCopy.f14;
	this->f15 = toCopy.f15;
	this->f16 = toCopy.f16;
	this->f17 = toCopy.f17;
	this->f18 = toCopy.f18;
	this->sortField = toCopy.sortField;
	this->master = toCopy.master;

}
int ParallelResizableArray::getMonthInd(string month)
{
	if (month == "Jan")
	{
		return 1;
	}
	else if (month == "Feb")
	{
		return 2;
	}
	else if (month == "Mar")
	{
		return 3;
	}
	else if (month == "Apr")
	{
		return 4;
	}
	else if (month == "May")
	{
		return 5;
	}
	else if (month == "Jun")
	{
		return 6;
	}
	else if (month == "Jul")
	{
		return 7;
	}
	else if (month ==  "Aug")
	{
		return 8;
	}
	else if (month == "Sep")
	{
		return 9;
	}
	else if (month == "Oct")
	{
		return 10;
	}
	else if (month ==  "Nov")
	{
		return 11;
	}
	else if (month == "Dec")
	{
		return 12;
	}
	else
	{
		cout << "The month entered was not valid.\n";
		return -1;
	}
}
void ParallelResizableArray::swap(int a, int b)
{
	f1.swap(a, b);
	f2.swap(a, b);
	f3.swap(a, b);
	f4.swap(a, b);
	f5.swap(a, b);
	f6.swap(a, b);
	f7.swap(a, b);
	f8.swap(a, b);
	f9.swap(a, b);
	f10.swap(a, b);
	f11.swap(a, b);
	f12.swap(a, b);
	f13.swap(a, b);
	f14.swap(a, b);
	f15.swap(a, b);
	f16.swap(a, b);
	f17.swap(a, b);
	f18.swap(a, b);
	master.swap(a, b);
}
int ParallelResizableArray::getIndex(int& field, int& ind, int sample)
{
	try
	{
		switch(field)
		{
		case 1:
			return *f1.getIndex(ind);
			break;
		case 2:
			return *f2.getIndex(ind);
			break;
		case 3:
			return *f3.getIndex(ind);
			break;
		case 5:
			return *f5.getIndex(ind);
			break;
		case 7:
			return *f7.getIndex(ind);
			break;
		case 8:
			return *f8.getIndex(ind);
			break;
		case 9:
			return *f9.getIndex(ind);
			break;
		case 15:
			return *f15.getIndex(ind);
			break;
		case 16:
			return *f16.getIndex(ind);
			break;
		case 17:
			return *f17.getIndex(ind);
			break;
		default:
			cerr << "Trouble retrieving index " << ind << " of field " << field << ".\n";
			return -1;
			break;
		}
	}
	catch(RAGetError *e)
	{
		cerr << "Trouble retrieving index " << ind << " of field " << field << ".\n";
	}
	return -1;
}
double ParallelResizableArray::getIndex(int& field, int& ind, double sample)
{
	try
	{
		switch(field)
		{
		case 11:
			return *f11.getIndex(ind);
			break;
		case 12:
			return *f12.getIndex(ind);
			break;
		case 13:
			return *f13.getIndex(ind);
			break;
		case 14:
			return *f14.getIndex(ind);
			break;
		default:
			cerr << "Trouble retrieving index " << ind << " of field " << field << ".\n";
			return 0.0;
			break;
		}
	}
	catch(RAGetError *e)
	{
		cerr << "Trouble retrieving index " << ind << " of field " << field << ".\n";
	}
	return 0.0;
}
std::string ParallelResizableArray::getIndex(int& field, int& ind, std::string sample)
{
	try
	{
		switch(field)
		{
		case 4:
			return *f4.getIndex(ind);
			break;
		case 6:
			return *f6.getIndex(ind);
			break;
		case 10:
			return *f10.getIndex(ind);
			break;
		case 18:
			return *f18.getIndex(ind);
			break;
		default:
			cerr << "Trouble retrieving index " << ind << " of field " << field << ".\n";
			return "";
			break;
		}
	}
	catch(RAGetError *e)
	{
		cerr << "Trouble retrieving index " << ind << " of field " << field << ".\n";
	}
	return "";
}
void ParallelResizableArray::sort(int field, int front, int back, int sample)
{
	//cout << "In sort. Field = " << field << endl;
	///////////////////////////////////////////////////CITE////////////////////////////////////////////////////////////
	///// This code is an altered version of that posted by rolfl on 17 January 2015 on StackExchange /////////////////
	///// See https://codereview.stackexchange.com/questions/77782/quick-sort-implementation       ////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (front < back)
	{
		int mid = front + (back - front) / 2;
		const int pivot = getIndex(field, mid, sample);
		// move the mid point value to the front.
		swap(mid,front);

		int i = front + 1;
		int j = back;
		while (i <= j)
		{
			while(i <= j && getIndex(field, i, sample) <= pivot)
			{
				i++;
			}

			while(i <= j && getIndex(field, j, sample) > pivot)
			{
				j--;
			}

			if (i < j)
			{
				swap(i, j);
			}
		}
		swap(i - 1,front);
		int part = i - 1;

		if (front < part - 1)
		{
			sort(field, front, part - 1, sample);
		}
		if (part + 1 < back)
		{
			sort(field, part + 1, back, sample);
		}

	}
}
void ParallelResizableArray::sort(int field, int front, int back, double sample)
{
	///////////////////////////////////////////////////CITE////////////////////////////////////////////////////////////
	///// This code is an altered version of that posted by rolfl on 17 January 2015 on StackExchange /////////////////
	///// See https://codereview.stackexchange.com/questions/77782/quick-sort-implementation       ////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (front < back)
	{
		int mid = front + (back - front) / 2;
		const double pivot = getIndex(field, mid, sample);
		// move the mid point value to the front.
		swap(mid,front);

		int i = front + 1;
		int j = back;
		while (i <= j) {
			while(i <= j && getIndex(field, i, sample) <= pivot)
			{
				i++;
			}

			while(i <= j && getIndex(field, j, sample) > pivot)
			{
				j--;
			}

			if (i < j)
			{
				swap(i, j);
			}
		}
		swap(i - 1,front);
		int part = i - 1;

		if (front < part - 1)
		{
			sort(field, front, part - 1, sample);
		}
		if (part + 1 < back)
		{
			sort(field, part + 1, back, sample);
		}

	}
}
void ParallelResizableArray::sort(int field, int front, int back, string sample)
{
	///////////////////////////////////////////////////CITE////////////////////////////////////////////////////////////
	///// This code is an altered version of that posted by rolfl on 17 January 2015 on StackExchange /////////////////
	///// See https://codereview.stackexchange.com/questions/77782/quick-sort-implementation       ////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (front < back && field != 4)
	{
		int mid = front + (back - front) / 2;
		const string pivot = getIndex(field, mid, sample);
		// move the mid point value to the front.
		swap(mid,front);

		int i = front + 1;
		int j = back;
		while (i <= j) {
			while(i <= j && getIndex(field, i, sample) <= pivot)
			{
				i++;
			}

			while(i <= j && getIndex(field, j, sample) > pivot)
			{
				j--;
			}

			if (i < j)
			{
				swap(i, j);
			}
		}
		swap(i - 1,front);
		int part = i - 1;

		if (front < part - 1)
		{
			sort(field, front, part - 1, sample);
		}
		if (part + 1 < back)
		{
			sort(field, part + 1, back, sample);
		}

	}
	else if (front < back && field == 4)
	{
		int mid = front + (back - front) / 2;
		const int pivot = getMonthInd(getIndex(field, mid, sample));
		// move the mid point value to the front.
		swap(mid,front);

		int i = front + 1;
		int j = back;
		while (i <= j) {
			while(i <= j && getMonthInd(getIndex(field, i, sample)) <= pivot)
			{
				i++;
			}

			while(i <= j && getMonthInd(getIndex(field, j, sample)) > pivot)
			{
				j--;
			}

			if (i < j)
			{
				swap(i, j);
			}
		}
		swap(i - 1,front);
		int part = i - 1;

		if (front < part - 1)
		{
			sort(field, front, part - 1, sample);
		}
		if (part + 1 < back)
		{
			sort(field, part + 1, back, sample);
		}

	}
}
int ParallelResizableArray::addNew(Eclipse addNew)
{
	int *a = new int(addNew.getCatNo());
	int *b = new int(addNew.getPlateNo());
	int *c = new int(addNew.getYear());
	string *d = new string(addNew.getMonth());
	int *e = new int(addNew.getDay());
	string *f = new string(addNew.getTime());
	int *g = new int(addNew.getDt());
	int *h = new int(addNew.getLunaNo());
	int *i = new int(addNew.getSarosNo());
	string *j = new string(addNew.getEclType());
	double *k = new double(addNew.getGamma());
	double *l = new double(addNew.getEclMag());
	double *m = new double(addNew.getLat());
	double *n = new double(addNew.getLon());
	int *o = new int(addNew.getSunAlt());
	int *p = new int(addNew.getSunAzm());
	int *q = new int(addNew.getPathWidth());
	string *r = new string(addNew.getCentralDur());
	string *s = new string(addNew.getMasterLine());

	//In this version of EclipseR, the Eclipses will be added in order of CatNo and the ordering does not need
	//to be checked by ParallelResizableArray. It is also guarenteed by EclipseR.cpp that there are no duplicates
	try
	{
		f1.add(a);
		f2.add(b);
		f3.add(c);
		f4.add(d);
		f5.add(e);
		f6.add(f);
		f7.add(g);
		f8.add(h);
		f9.add(i);
		f10.add(j);
		f11.add(k);
		f12.add(l);
		f13.add(m);
		f14.add(n);
		f15.add(o);
		f16.add(p);
		f17.add(q);
		f18.add(r);
		master.add(s);
	}
	catch(RAAddError *e)
	{
		cerr << "Unable to add " << &addNew << ".\n";
		return 0;
	}
	catch(RAConstructError *e)
	{
		cerr << "Unable to resize array to add " << &addNew << ".\n";
	}
	return 1;
}

int ParallelResizableArray::find(int field, int elem, int front, int back, bool toPrint)
{
	int eclipsesFound = 0;
	if (sortField == field) //this does a binary search
	{
		if (field == 1)
		{
			if(elem == *f1.getIndex((front + back)/2))
			{
				int place = (front + back)/2;
				if (toPrint == true)
				{
					cout << *master.getIndex(place);
				}
				while(place > 1 && *f1.getIndex(place - 1) == elem) //search before
				{
					place -= 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				place = (front + back)/2;
				while(place < (f1.getSpacesFull() - 2) && *f1.getIndex(place + 1) == elem) //search after
				{
					place += 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				return place;
			}
			else if (back - front <= 0 && elem != *f1.getIndex(front))
			{
				return -1;
			}
			else if(elem < *f1.getIndex((front + back)/2))
			{
				return find(1, elem, front, (front + back)/2, toPrint);
			}
			else if(elem > *f1.getIndex((front + back)/2))
			{
				return find(1, elem, (front + back)/2 + 1, back, toPrint);
			}
		}

		if (field == 2)
		{
			if (back - front <= 0 && elem != *f2.getIndex(front))
			{
				return -1;
			}
			else if(elem == *f2.getIndex((front + back)/2))
			{
				int place = (front + back)/2;
				if (toPrint == true)
				{
					cout << *master.getIndex(place);
				}
				eclipsesFound++;
				while(*f2.getIndex(place - 1) == elem && place > 1) //search before
				{
					place -= 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				place = (front + back)/2;
				while(*f2.getIndex(place + 1) == elem && place < (f1.getSpacesFull() - 2)) //search after
				{
					place += 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				return place;
			}
			else if(elem < *f2.getIndex((front + back)/2))
			{
				return find(2, elem, front, (front + back)/2, toPrint);
			}
			else if(elem > *f2.getIndex((front + back)/2))
			{
				return find(2, elem, (front + back)/2, back, toPrint);
			}
		}
		else if (field == 3)
		{
			if (back - front <= 0 && elem != *f3.getIndex(front))
			{
				return -1;
			}
			else if(elem == *f3.getIndex((front + back)/2))
			{
				int place = (front + back)/2;
				if (toPrint == true)
				{
					cout << *master.getIndex(place);
				}
				eclipsesFound++;
				while(*f3.getIndex(place - 1) == elem && place > 1) //search before
				{
					place -= 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				place = (front + back)/2;
				while(*f3.getIndex(place + 1) == elem && place < (f1.getSpacesFull() - 2)) //search after
				{
					place += 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				return place;
			}
			else if(elem < *f3.getIndex((front + back)/2))
			{
				return find(3, elem, front, (front + back)/2, toPrint);
			}
			else if(elem > *f3.getIndex((front + back)/2))
			{
				return find(3, elem, (front + back)/2, back, toPrint);
			}
		}
		else if (field == 5)
		{
			if (back - front <= 0 && elem != *f5.getIndex(front))
			{
				return -1;
			}
			else if(elem == *f5.getIndex((front + back)/2))
			{
				int place = (front + back)/2;
				if (toPrint == true)
				{
					cout << *master.getIndex(place);
				}
				eclipsesFound++;
				while(*f5.getIndex(place - 1) == elem && place > 1) //search before
				{
					place -= 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				place = (front + back)/2;
				while(*f5.getIndex(place + 1) == elem && place < (f1.getSpacesFull() - 2)) //search after
				{
					place += 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				return place;
			}
			else if(elem < *f5.getIndex((front + back)/2))
			{
				return find(5, elem, front, (front + back)/2, toPrint);
			}
			else if(elem > *f5.getIndex((front + back)/2))
			{
				return find(5, elem, (front + back)/2, back, toPrint);
			}
		}
		else if (field == 7)
		{
			if (back - front <= 0 && elem != *f7.getIndex(front))
			{
				return -1;
			}
			else if(elem == *f7.getIndex((front + back)/2))
			{
				int place = (front + back)/2;
				if (toPrint == true)
				{
					cout << *master.getIndex(place);
				}
				eclipsesFound++;
				while(*f7.getIndex(place - 1) == elem && place > 1) //search before
				{
					place -= 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				place = (front + back)/2;
				while(*f7.getIndex(place + 1) == elem && place < (f1.getSpacesFull() - 2)) //search after
				{
					place += 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				return place;
			}
			else if(elem < *f7.getIndex((front + back)/2))
			{
				return find(7, elem, front, (front + back)/2, toPrint);
			}
			else if(elem > *f7.getIndex((front + back)/2))
			{
				return find(7, elem, (front + back)/2, back, toPrint);
			}
		}
		else if (field == 8)
		{
			if (back - front <= 0 && elem != *f8.getIndex(front))
			{
				return -1;
			}
			else if(elem == *f8.getIndex((front + back)/2))
			{
				int place = (front + back)/2;
				if (toPrint == true)
				{
					cout << *master.getIndex(place);
				}
				eclipsesFound++;
				while(*f8.getIndex(place - 1) == elem && place > 1) //search before
				{
					place -= 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				place = (front + back)/2;
				while(*f8.getIndex(place + 1) == elem && place < (f1.getSpacesFull() - 2)) //search after
				{
					place += 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				return place;
			}
			else if(elem < *f8.getIndex((front + back)/2))
			{
				return find(8, elem, front, (front + back)/2, toPrint);
			}
			else if(elem > *f8.getIndex((front + back)/2))
			{
				return find(8, elem, (front + back)/2, back, toPrint);
			}
		}
		else if (field == 9)
		{
			if (back - front <= 0 && elem != *f9.getIndex(front))
			{
				return -1;
			}
			else if(elem == *f9.getIndex((front + back)/2))
			{
				int place = (front + back)/2;
				if (toPrint == true)
				{
					cout << *master.getIndex(place);
				}
				eclipsesFound++;
				while(*f9.getIndex(place - 1) == elem && place > 1) //search before
				{
					place -= 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				place = (front + back)/2;
				while(*f9.getIndex(place + 1) == elem && place < (f1.getSpacesFull() - 2)) //search after
				{
					place += 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				return place;
			}
			else if(elem < *f9.getIndex((front + back)/2))
			{
				return find(9, elem, front, (front + back)/2, toPrint);
			}
			else if(elem > *f9.getIndex((front + back)/2))
			{
				return find(9, elem, (front + back)/2, back, toPrint);
			}
		}
		else if (field == 15)
		{
			if (back - front <= 0 && elem != *f15.getIndex(front))
			{
				return -1;
			}
			else if(elem == *f15.getIndex((front + back)/2))
			{
				int place = (front + back)/2;
				if (toPrint == true)
				{
					cout << *master.getIndex(place);
				}
				eclipsesFound++;
				while(*f15.getIndex(place - 1) == elem && place > 1) //search before
				{
					place -= 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				place = (front + back)/2;
				while(*f15.getIndex(place + 1) == elem && place < (f1.getSpacesFull() - 2)) //search after
				{
					place += 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				return place;
			}
			else if(elem < *f15.getIndex((front + back)/2))
			{
				return find(15, elem, front, (front + back)/2, toPrint);
			}
			else if(elem > *f15.getIndex((front + back)/2))
			{
				return find(15, elem, (front + back)/2, back, toPrint);
			}
		}
		else if (field == 16)
		{
			if (back - front <= 0 && elem != *f16.getIndex(front))
			{
				return -1;
			}
			else if(elem == *f16.getIndex((front + back)/2))
			{
				int place = (front + back)/2;
				if (toPrint == true)
				{
					cout << *master.getIndex(place);
				}
				eclipsesFound++;
				while(*f16.getIndex(place - 1) == elem && place > 1) //search before
				{
					place -= 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				place = (front + back)/2;
				while(*f16.getIndex(place + 1) == elem && place < (f1.getSpacesFull() - 2)) //search after
				{
					place += 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				return place;
			}
			else if(elem < *f16.getIndex((front + back)/2))
			{
				return find(16, elem, front, (front + back)/2, toPrint);
			}
			else if(elem > *f16.getIndex((front + back)/2))
			{
				return find(16, elem, (front + back)/2, back, toPrint);
			}
		}
		else if (field == 17)
		{
			if (back - front <= 0 && elem != *f17.getIndex(front))
			{
				return -1;
			}
			else if(elem == *f17.getIndex((front + back)/2))
			{
				int place = (front + back)/2;
				if (toPrint == true)
				{
					cout << *master.getIndex(place);
				}
				eclipsesFound++;
				while(*f17.getIndex(place - 1) == elem && place > 1) //search before
				{
					place -= 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				place = (front + back)/2;
				while(*f17.getIndex(place + 1) == elem && place < (f1.getSpacesFull() - 2)) //search after
				{
					place += 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				return place;
			}
			else if(elem < *f17.getIndex((front + back)/2))
			{
				return find(17, elem, front, (front + back)/2, toPrint);
			}
			else if(elem > *f17.getIndex((front + back)/2))
			{
				return find(17, elem, (front + back)/2, back, toPrint);
			}
		}
	}
	else //linear search
	{
		ResizableArray<std::string> temp;
		for (int i = 0; i < master.getSpacesFull(); ++i)
		{
			if((field == 1 && *f1.getIndex(i) == elem)
			|| (field == 2 && *f2.getIndex(i) == elem)
			|| (field == 3 && *f3.getIndex(i) == elem)
			|| (field == 5 && *f5.getIndex(i) == elem)
			|| (field == 7 && *f7.getIndex(i) == elem)
			|| (field == 8 && *f8.getIndex(i) == elem)
			|| (field == 9 && *f9.getIndex(i) == elem)
			|| (field == 15 && *f15.getIndex(i) == elem)
			|| (field == 16 && *f16.getIndex(i) == elem)
			|| (field == 17 && (*f17.getIndex(i) == elem)))
			{
				temp.add(*master.getIndex(i));
				eclipsesFound++;
			}
		}
		if (toPrint == true)
		{
			cout << &temp;
		}
	}
	if (toPrint == true)
	{
		cout << "Eclipses found: " << eclipsesFound << endl;
	}
	return -1; //nonsense val; everything is already printed
}
int ParallelResizableArray::find(int field, double elem, int front, int back, bool toPrint)
{
	int eclipsesFound = 0;
	if (sortField == field) //this does a binary search
	{
		if (field == 11)
		{
			if (back - front <= 0 && elem != *f11.getIndex(front))
			{
				return -1;
			}
			else if(elem == *f11.getIndex((front + back)/2))
			{
				int place = (front + back)/2;
				if (toPrint == true)
				{
					cout << *master.getIndex(place);
				}
				eclipsesFound++;
				while(*f11.getIndex(place - 1) == elem && place > 1) //search before
				{
					place -= 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				place = (front + back)/2;
				while(*f11.getIndex(place + 1) == elem && place < (f1.getSpacesFull() - 2)) //search after
				{
					place += 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				return place;
			}
			else if(elem < *f11.getIndex((front + back)/2))
			{
				return find(11, elem, front, (front + back)/2, toPrint);
			}
			else if(elem > *f11.getIndex((front + back)/2))
			{
				return find(11, elem, (front + back)/2, back, toPrint);
			}
		}
		else if (field == 12)
		{
			if (back - front <= 0 && elem != *f12.getIndex(front))
			{
				return -1;
			}
			else if(elem == *f12.getIndex((front + back)/2))
			{
				int place = (front + back)/2;
				if (toPrint == true)
				{
					cout << *master.getIndex(place);
				}
				eclipsesFound++;
				while(*f12.getIndex(place - 1) == elem && place > 1) //search before
				{
					place -= 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				place = (front + back)/2;
				while(*f12.getIndex(place + 1) == elem && place < (f1.getSpacesFull() - 2)) //search after
				{
					place += 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				return place;
			}
			else if(elem < *f12.getIndex((front + back)/2))
			{
				return find(12, elem, front, (front + back)/2, toPrint);
			}
			else if(elem > *f12.getIndex((front + back)/2))
			{
				return find(12, elem, (front + back)/2, back, toPrint);
			}
		}
		else if (field == 13)
		{
			if (back - front <= 0 && elem != *f13.getIndex(front))
			{
				return -1;
			}
			else if(elem == *f13.getIndex((front + back)/2))
			{
				int place = (front + back)/2;
				if (toPrint == true)
				{
					cout << *master.getIndex(place);
				}
				eclipsesFound++;
				while(*f13.getIndex(place - 1) == elem && place > 1) //search before
				{
					place -= 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				place = (front + back)/2;
				while(*f13.getIndex(place + 1) == elem && place < (f1.getSpacesFull() - 2)) //search after
				{
					place += 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				return place;
			}
			else if(elem < *f13.getIndex((front + back)/2))
			{
				return find(13, elem, front, (front + back)/2, 	toPrint);
			}
			else if(elem > *f13.getIndex((front + back)/2))
			{
				return find(13, elem, (front + back)/2, back, toPrint);
			}
		}
		else if (field == 14)
		{
			if (back - front <= 0 && elem != *f14.getIndex(front))
			{
				return -1;
			}
			else if(elem == *f14.getIndex((front + back)/2))
			{
				int place = (front + back)/2;
				if (toPrint == true)
				{
					cout << *master.getIndex(place);
				}
				eclipsesFound++;
				while(*f14.getIndex(place - 1) == elem && place > 1) //search before
				{
					place -= 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				place = (front + back)/2;
				while(*f14.getIndex(place + 1) == elem && place < (f1.getSpacesFull() - 2)) //search after
				{
					place += 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				if (toPrint == true)
				{
					cout << "Eclipses found: " << eclipsesFound << endl;
				}
				return place;
			}
			else if(elem < *f14.getIndex((front + back)/2))
			{
				return find(14, elem, front, (front + back)/2, toPrint);
			}
			else if(elem > *f14.getIndex((front + back)/2))
			{
				return find(14, elem, (front + back)/2, back, toPrint);
			}
		}
		else //linear search
		{
			ResizableArray<std::string> temp;
			for (int i = 0; i < master.getSpacesFull(); ++i)
			{
				if((field == 11 && areClose(*f11.getIndex(i), elem))
				|| (field == 12 && areClose(*f12.getIndex(i), elem))
				|| (field == 13 && areClose(*f13.getIndex(i), elem))
				|| (field == 14 && areClose(*f14.getIndex(i), elem)))
				{
					temp.add(*master.getIndex(i));
					eclipsesFound++;
				}
			}
			if (toPrint == true)
			{
				cout << "Eclipses found: " << eclipsesFound << endl;
				cout << &temp;
			}
			return -1;
		}
	}
	return -1;
}

int ParallelResizableArray::find(int field, string elem, int front, int back, bool toPrint)
{
	int eclipsesFound = 0;
	if (elem == "")
	{
		cout << "That search key is invalid\n";
		return -1;
	}
	if (sortField == field) //this does a binary search
	{
		if (field == 4)
		{
			if (back - front <= 0 && elem != *f4.getIndex(front))
			{
				return -1;
			}
			else if(getMonthInd(elem) < 0)
			{
				return -1;
			}
			else if(elem == *f4.getIndex((front + back)/2))
			{
				int place = (front + back)/2;
				if (toPrint == true)
				{
					cout << *master.getIndex(place);
				}
				eclipsesFound++;
				while(*f4.getIndex(place - 1) == elem && place > 1) //search before
				{
					place -= 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				place = (front + back)/2;
				while(*f4.getIndex(place + 1) == elem && place < (f1.getSpacesFull() - 2)) //search after
				{
					place += 1;
					if (toPrint == true)
					{
					cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				if (toPrint == true)
				{
					cout << "Eclipses found: " << eclipsesFound << endl;
				}
				return place;
			}
			else if(getMonthInd(elem) < getMonthInd(*f4.getIndex((front + back)/2)))
			{
				return find(4, elem, front, (front + back)/2, toPrint);
			}
			else if(getMonthInd(elem) > getMonthInd(*f4.getIndex((front + back)/2)))
			{
				return find(4, elem, (front + back)/2, back, toPrint);
			}
		}

		else if (field == 6)
		{
			if (back - front <= 0 && elem != *f6.getIndex(front))
			{
				return -1;
			}
			else if(elem == *f6.getIndex((front + back)/2))
			{
				int place = (front + back)/2;
				if (toPrint == true)
				{
					cout << *master.getIndex(place);
				}
				eclipsesFound++;
				while(*f6.getIndex(place - 1) == elem && place > 1) //search before
				{
					place -= 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				place = (front + back)/2;
				while(*f6.getIndex(place + 1) == elem && place < (f1.getSpacesFull() - 2)) //search after
				{
					place += 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				if (toPrint == true)
				{
					cout << "Eclipses found: " << eclipsesFound << endl;
				}
				return place;
			}
			else if(elem < *f6.getIndex((front + back)/2))
			{
				return find(6, elem, front, (front + back)/2, toPrint);
			}
			else if(elem > *f6.getIndex((front + back)/2))
			{
				return find(6, elem, (front + back)/2, back, toPrint);
			}
		}

		else if (field == 10)
		{
			if (back - front <= 0 && elem != *f10.getIndex(front))
			{
				return -1;
			}
			else if(elem == *f10.getIndex((front + back)/2))
			{
				int place = (front + back)/2;
				if (toPrint == true)
				{
					cout << master.getIndex(place);
				}
				eclipsesFound++;
				while(*f10.getIndex(place - 1) == elem && place > 1) //search before
				{
					place -= 1;
					if (toPrint == true)
					{
						cout << master.getIndex(place);
					}
					eclipsesFound++;
				}
				place = (front + back)/2;
				while(*f10.getIndex(place + 1) == elem && place < (f1.getSpacesFull() - 2)) //search after
				{
					place += 1;
					if (toPrint == true)
					{
						cout << master.getIndex(place);
					}
					eclipsesFound++;
				}
				if (toPrint == true)
				{
					cout << "Eclipses found: " << eclipsesFound << endl;
				}
				return place;
			}
			else if(elem < *f10.getIndex((front + back)/2))
			{
				return find(10, elem, front, (front + back)/2, toPrint);
			}
			else if(elem > *f10.getIndex((front + back)/2))
			{
				return find(10, elem, (front + back)/2, back, toPrint);
			}
		}


		else if (field == 18)
		{
			if (elem == "")
			{
				elem = "zzzzzzzzzzzzz";
			}
			if (back - front <= 0 && elem != *f18.getIndex(front))
			{
				return -1;
			}
			else if(elem == *f18.getIndex((front + back)/2))
			{
				int place = (front + back)/2;
				if (toPrint == true)
				{
					cout << *master.getIndex(place);
				}
				eclipsesFound++;
				while(*f18.getIndex(place - 1) == elem && place > 1) //search before
				{
					place -= 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				place = (front + back)/2;
				while(*f18.getIndex(place + 1) == elem && place < (f1.getSpacesFull() - 2)) //search after
				{
					place += 1;
					if (toPrint == true)
					{
						cout << *master.getIndex(place);
					}
					eclipsesFound++;
				}
				if (toPrint == true)
				{
					cout << "Eclipses found: " << eclipsesFound << endl;
				}
				return place;
			}
			else if(elem < *f18.getIndex((front + back)/2))
			{
				return find(18, elem, front, (front + back)/2, toPrint);
			}
			else if(elem > *f18.getIndex((front + back)/2))
			{
				return find(18, elem, (front + back)/2, back, toPrint);
			}
		}
	}
	else
	{
		ResizableArray<std::string> temp;
		for (int i = 0; i < master.getSpacesFull(); ++i)
		{
			if((field == 4 && *f4.getIndex(i) == elem)
			|| (field == 6 && *f6.getIndex(i) == elem)
			|| (field == 10 && *f10.getIndex(i) == elem)
			|| (field == 18 && ((*f18.getIndex(i) == elem) || (elem == "" && (*f18.getIndex(i)) == "zzzzzzzzzzzzz"))))
			{
				temp.add(*master.getIndex(i));
				eclipsesFound++;
			}
		}
		if (toPrint == true)
		{
			cout << &temp;
			cout << "Eclipses found: " << eclipsesFound << endl;
		}
	}
	return -1; //nonsense val; everything is already printed
}
int ParallelResizableArray::findSpot(int elem)
{
	int i = getLen()-1;

	while(i >= 0 && *f1.getIndex(i) > elem)
	{
		i--;
	}
	return i;
}
int ParallelResizableArray::getLen()
{
	return master.getSpacesFull();
}
bool ParallelResizableArray::areClose(double a, double b)
{
	if((a-b) < 0.000000001 && (b-a) < 0.00000001)
	{
		return true;
	}
	return false;
}
