/*!
 *  \brief     Manages eclipse data
 *  \details   Original data must be in format provided on the class webpage.
 *  \author    Jessica Wiedemeier
 *  \date      11 Sept 2017
 *  \version   0.2
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include "EclipseR.h"
#include "ParallelResizableArray.h"
#include "ResizableArray.h"
#include "Eclipse.h"
#include "HashTable.h"
#include "AVL.h"
using namespace std;

int r = 0; //This is the the number an eclipse would have if all lines were correct and ordered - used for error messages
ParallelResizableArray *eclipses = new ParallelResizableArray();
ResizableArray<Eclipse> *arr;
AVL<Eclipse> avl = new AVL<Eclipse>(true, *(new Eclipse()), -999);
Eclipse *tempEclipse = new Eclipse();
string line; // Used to hold the current line of the original data file
string fileName = ""; //holds the name of the file from user input
fstream *myFstream = new fstream();
bool hasFirstFile = false;
bool exitAfterProcess = false;
int linesAttempted = 0;
int linesRead = 0;
//This is a placeholder; the actual tally is eclipses->getLen().
char option; //holds user's answers to multi-choice questions temporarially
string* header = new string[10];
int field = 0; //holds user input
string strHolder = "";
int sampleInt = 0;
double sampleDouble = 0.0;
string sampleString = "";

void refreshArray()
{
	delete eclipses;
	eclipses = new ParallelResizableArray();
	AVL<Eclipse> avlNode = avl;
	arr = new ResizableArray<Eclipse>();
	avl.fillArr(arr);
	for(int i = 0; i < avl.getSize(); ++i)
	{
		eclipses->addNew(arr->getIndex(i));
	}
	delete arr;
}

AVL<Eclipse> getFile(bool isFirst)
{
	/* Get the file */
	cout << "Please enter the name of a file.\nFile Name: "; //prompt user for input

	AVL<Eclipse> fileData = new AVL<Eclipse>(true, *(new Eclipse()), -999);
	//if cin didn't work, exit
	if (cin.fail())
	{
		return fileData;
	}

	getline(cin, fileName);

	myFstream->open(fileName, ios::in);

	if(myFstream->fail() && isFirst)
	{
		cout << "File is not available.\n";
		return getFile(isFirst); //Asks for file again
	}
	else if(myFstream->fail())
	{
		return fileData;
	}

	/* Read the first 10 lines of the file and discard */
	for (int i = 0; i < 10; ++i)
	{
		getline(*myFstream, header[i]);
	}

	/*
	 * Here's the data processing loop! Note that it is with the file input loop for simplicity.
	 */
	while (true)
	{
		if (exitAfterProcess == true)
		{
			break;
		}
		///////////////////////////////////////////////////CITE//////////////////////////////////////////////////////////
		///// This code is an altered version of that posted by KTC on 25 October 2008 on StackExchange /////////////////
		///// See https://stackoverflow.com/questions/236129/most-elegant-way-to-split-a-string         /////////////////
		///// The original code did not include comments, arrays, or a check for empty tokens.          /////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		string splitLine[19]; //This is larger than should ever be used to enable checking for lines with too many cols
		fill_n(splitLine, 19, ""); //Fills the array with empty strings - enables checks for empty cols later
		std::getline(*myFstream, line); //Puts the next line in variable line
		int indOfSplitLine = 0; //Keeps track of the next index to fill in splitLine - not every iteration will add

		string::size_type prev_pos = 0, pos = 0; //I honestly don't understand the syntax here
		while( (pos = line.find(' ', pos)) != string::npos ) //While a space remains
		{
			string substring( line.substr(prev_pos, pos-prev_pos) ); //Take a substring from the last space used to the next space
			if (substring != "" && indOfSplitLine < 19)//If the token is non-empty and there is room in the array...
			{
				splitLine[indOfSplitLine] = substring; //...add to splitLine
				indOfSplitLine++;
			}
			prev_pos = ++pos; //I think this is the pointer arithmetic mentioned in class.
		}
		string substring( line.substr(prev_pos, pos-prev_pos) ); // Last word
		if (substring != "" && indOfSplitLine < 19)//If the token is non-empty and there is room in the array...
		{
			splitLine[indOfSplitLine] = substring; //...add to the array
			indOfSplitLine++;

		}
		++r;

		////////////////////////////////////////////END CITE////////////////////////////////////////////////////////////
		/*
		 * Now that the code block based on KTC's code has executed, splitLine contains the contents of up to 19
		 * columns of a line. Based on the contents at index 9, it can be determined if more rows than are correct
		 * were included. If the row is good, it will be added to eclipses as a DataLine
		 */
		//Check if number of cols is correct and, if so, save the line
		if (splitLine[9].empty())
		{
			if (splitLine[0].empty())
			{
				break; //This should be the end of the input file.
			}
			else
			{
				linesAttempted += 1;
				cerr << "Error in data row " + to_string(r) + ": 0 columns found. Should be at least 16.\n";
			}
		}
		else if (splitLine[9].find('P') != string::npos)
		{
			linesAttempted += 1;
			//check for input errors and output the line
			if (!splitLine[15].empty() && splitLine[16].empty())
			{
				try
				{
					tempEclipse = new Eclipse(splitLine, r, line);
					fileData.add(*tempEclipse, std::stoi(splitLine[0]));
					fileData.balance();
					linesRead += 1;
				}
				catch(const std::invalid_argument& a)
				{
					cerr << "Error in data row " + to_string(r) + ": Something in this row was not of the expected data type.\n";
				}
			}
			else
			{
				int cols;
				for (cols = 0; cols < 18; ++cols) //Get the number of filled columns
				{
					if (splitLine[cols] == "")
					{
						break;
					}
				}
				cerr << "Error in data row " + to_string(r) + ": " + to_string(cols) + " columns found. Should be 16.\n";
			}
		}
		else if (splitLine[9].find('P') == string::npos)
		{
			linesAttempted += 1;
			//check for input errors and output the line
			if (!splitLine[17].empty() && splitLine[18].empty())
			{
				try
				{
					tempEclipse = new Eclipse(splitLine, r, line);
					fileData.add(*tempEclipse, std::stoi(splitLine[0]));
					linesRead += 1;
				}
				catch(const std::invalid_argument& a)
				{
					cerr << "Error in data row " + to_string(r) + ": Something in this row was not of the expected data type.\n";
				}

			}
			else
			{
				int cols;
				for (cols = 0; cols < 18; ++cols) //Get the number of filled columns
				{
					if (splitLine[cols] == "")
					{
						break;
					}
				}
				cerr << "Error in data row " + to_string(r) + ": " + to_string(cols) + " columns found. Should be 18.\n";
			}
		}
		else
		{
			linesAttempted += 1;
			if (splitLine[0].empty())
			{
				cerr << "Trouble reading line.\n";
			}
			else
			{
				cerr << "Invalid eclipse type (eclipse number " << splitLine[0] << ")\n";
			}
		}
		//repeat loop to read next line
	}
	hasFirstFile = true;
	myFstream->close();
	return fileData;
}
/**
 * \brief This is the main function.
 *
 * This function...
 * \return 0
 */
int main()
{
	//Now that the data is loaded, populate the array
	avl = getFile(true);

	try
	{
		int htSize = avl.getSize() * 2;
		HashTable<Eclipse> *ht = new HashTable<Eclipse>(htSize); // 2 is large enough to reduce the number of collisions but small enough to take minimal memory
		delete arr;
		arr = new ResizableArray<Eclipse>;
		avl.fillArr(arr);
		for(int i = 0; i < avl.getSize(); ++i)
		{
			try
			{
				ht->add(arr->getIndex(i).getCatNo(), arr->getIndex(i));
			}
			catch (AddError* e)
			{
				cerr << "Failed to add item. Exiting.\n";
				return 0;
			}
		}
		refreshArray();

		/*
		 * Here's the data manipulation loop!
		 */
		while(true)
		{
			//Prompt user
			cout <<  "Please enter\n‘O’ for output,\n‘S’ for sort,\n‘F’ for find,\n'M' for merge,\n'P' for purge,\n'C' to output by catalog number,\n'H' to output by order stored in hash table,\n'L' to output by order added to hash table, or\n‘Q’ for quit.\n";
			//read input
			try
			{
				cin >> option;
			}
			catch(...)
			{
				cerr << "Trouble reading user input\n";
				return 1;
			}
			if (option == 'O')
			{

				cout << "Please enter the name of the file to be created:\n";
				try
				{
					getline(cin, fileName);
					getline(cin, fileName);
				}
				catch(...)
				{
					cerr << "Trouble reading user input\n";
					return 1;
				}

				if (fileName == "")
				{
					for(int i = 0; i < 10; ++i)
					{
						cout << header[i] << endl;
					}
					cout << eclipses;
					cout << "Data lines read: " << linesAttempted << "; Valid eclipses read: " << linesRead << "; Eclipses in memory: " << eclipses->getLen() << endl;
				}
				else
				{
					try
					{
						myFstream->open(fileName, ios::out);
					}
					catch (...)
					{
						cout << "File is not available.";
						continue;
					}

					for(int i = 0; i < 10; ++i)
					{
						*myFstream << header[i] << "\n";
					}
					*myFstream << eclipses;
					*myFstream << "Data lines read: " << linesAttempted << "; Valid eclipses read: " << linesRead << "; Eclipses in memory: " << eclipses->getLen() << endl;
					myFstream->close();
				}
			}
			else if (option == 'S')
			{
				cout << "Which field would you like to sort by? Please enter an integer between 1 and 18.\n";
				try
				{
					cin >> field;
				}
				catch(...)
				{
					cerr << "Trouble reading user input\n";
					return 1;
				}
				switch(field)
					{
					case 1:
						eclipses->sort(field, 0, eclipses->getLen()-1, sampleInt);
						break;
					case 2:
						eclipses->sort(field, 0, eclipses->getLen()-1, sampleInt);
						break;
					case 3:
						eclipses->sort(field, 0, eclipses->getLen()-1, sampleInt);
						break;
					case 4:
						eclipses->sort(field, 0, eclipses->getLen()-1, sampleString);
						break;
					case 5:
						eclipses->sort(field, 0, eclipses->getLen()-1, sampleInt);
						break;
					case 6:
						eclipses->sort(field, 0, eclipses->getLen()-1, sampleString);
						break;
					case 7:
						eclipses->sort(field, 0, eclipses->getLen()-1, sampleInt);
						break;
					case 8:
						eclipses->sort(field, 0, eclipses->getLen()-1, sampleInt);
						break;
					case 9:
						eclipses->sort(field, 0, eclipses->getLen()-1, sampleInt);
						break;
					case 10:
						eclipses->sort(field, 0, eclipses->getLen()-1, sampleString);
						break;
					case 11:
						eclipses->sort(field, 0, eclipses->getLen()-1, sampleDouble);
						break;
					case 12:
						eclipses->sort(field, 0, eclipses->getLen()-1, sampleDouble);
						break;
					case 13:
						eclipses->sort(field, 0, eclipses->getLen()-1, sampleDouble);
						break;
					case 14:
						eclipses->sort(field, 0, eclipses->getLen()-1, sampleDouble);
						break;
					case 15:
						eclipses->sort(field, 0, eclipses->getLen()-1, sampleInt);
						break;
					case 16:
						eclipses->sort(field, 0, eclipses->getLen()-1, sampleInt);
						break;
					case 17:
						eclipses->sort(field, 0, eclipses->getLen()-1, sampleInt);
						break;
					case 18:
						eclipses->sort(field, 0, eclipses->getLen()-1, sampleString);
						break;
					default:
						break;
					}

			}
			else if (option == 'F')
			{
				cout << "Which field would you like to search for? Please enter an integer between 1 and 18.\n";
				try
				{
					cin >> field;
				}
				catch(...)
				{
					cerr << "Trouble reading user input\n";
					return 1;
				}
				if (field == 1)
				{
					cout << "Please enter an integer value to search for.\n";
					try
					{
						cin >> strHolder;
					}
					catch(...)
					{
						cerr << "Trouble reading user input\n";
						return 1;
					}
					Eclipse a = ht->get(stoi(strHolder));
					cout << &a << endl;
				}
				else if (field == 2 || field == 3 || field == 5 || field == 7 || field ==  8
						|| field == 9 || field == 15 || field == 16 || field == 17)
				{
					cout << "Please enter an integer value to search for.\n";
					try
					{
						cin >> strHolder;
					}
					catch(...)
					{
						cerr << "Trouble reading user input\n";
						return 1;
					}
					if (strHolder == "" && field != 17)
					{
						continue;
					}
					else if (strHolder == "" && field == 17)
					{
						strHolder = "99999999";
					}
					try
					{
						for(int i = 0; i < 10; ++i)
						{
							cout << header[i] << endl;
						}
						eclipses->find(field, stoi(strHolder), 0, eclipses->getLen()-1, true);
					}
					catch(...)
					{
						continue;
					}
				}
				else if (field == 11 || field == 12 || field == 13 || field == 14)
				{
					cout << "Please enter a decimal value to search for.\n";

					try
					{
						cin >> strHolder;
					}
					catch(...)
					{
						cerr << "Trouble reading user input\n";
						return 1;
					}
					if (strHolder == "")
					{
						continue;
					}
					try
					{
						for(int i = 0; i < 10; ++i)
						{
							cout << header[i] << endl;
						}
						eclipses->find(field, stod(strHolder), 0, eclipses->getLen()-1, true);
					}
					catch(...)
					{
						continue;
					}
				}
				else if (field == 4)
				{
					cout << "Please enter a 3-letter month code to search for (ex. January = Jan).\n";
					try
					{
						cin >> strHolder;
					}
					catch(...)
					{
						cerr << "Trouble reading user input\n";
						return 1;
					}
					if (strHolder == "")
					{
						continue;
					}
					try
					{
						for(int i = 0; i < 10; ++i)
						{
							cout << header[i] << endl;
						}
						eclipses->find(field, strHolder, 0, eclipses->getLen()-1, true);
					}
					catch(...)
					{
						continue;
					}
				}
				else if (field == 10)
				{
					cout << "Please enter the eclipse type to search for.\n";
					try
					{
						cin >> strHolder;
					}
					catch(...)
					{
						cerr << "Trouble reading user input\n";
						return 1;
					}
					if (strHolder == "")
					{
						continue;
					}
					try
					{
						for(int i = 0; i < 10; ++i)
						{
							cout << header[i] << endl;
						}
						eclipses->find(field, strHolder, 0, eclipses->getLen()-1, true);
					}
					catch(...)
					{
						continue;
					}
				}
				else if (field == 18)
				{
					cout << "Please enter a central duration to search for in the form ##m##s.\n";
					try
					{
						cin >> strHolder;
					}
					catch(...)
					{
						cerr << "Trouble reading user input\n";
						return 1;
					}
					if (strHolder == "")
					{
						continue;
					}
					try
					{
						for(int i = 0; i < 10; ++i)
						{
							cout << header[i] << endl;
						}
						eclipses->find(field, strHolder, 0, eclipses->getLen()-1, true);
					}
					catch(...)
					{
						continue;
					}
				}

			}
			else if (option == 'M')
			{
				AVL<Eclipse> toMerge = new AVL<Eclipse>(NULL, *(new Eclipse()), -999);
				getline(cin, fileName);
				toMerge = getFile(false);
				if(toMerge.getSize() >= 1)
				{
					avl.merge(&toMerge);
					refreshArray();
					ht->clear();
					avl.fillHT(ht);
				}
				else
				{
					cerr << "File is not available.\n";
				}
			}
			else if (option == 'P')
			{
				AVL<Eclipse> toPurge = new AVL<Eclipse>(NULL, *(new Eclipse()), -999);
				getline(cin, fileName);
				toPurge = getFile(false);
				if(toPurge.getSize() >= 1)
				{
					avl.purge(&toPurge);
					refreshArray();
					ht->clear();
					avl.fillHT(ht);

				}
				else
				{
					cerr << "File is not available.\n";
				}
			}
			else if (option == 'C')
			{
				for(int i = 0; i < 10; ++i)
				{
					cout << header[i] << endl;
				}
				cout << &avl;
				cout << "Data lines read: " << linesAttempted << "; Valid eclipses read: " <<
						linesRead << "; Eclipses in memory: " << eclipses->getLen() << endl;
			}
			else if (option == 'H')
			{
				ht->printAsStored();
			}
			else if (option == 'L')
			{
				ht->printOrdered();
			}
			else if (option == 'R')
			{
				avl.preOrderPrint();
			}
			else if (option == 'T')
			{
				avl.postOrderPrint();
			}
			else if (option == 'Q')
			{
				cout << "Thank you for using EclipseR!\n";
				return 0;
			}


		}
	}
	catch (ConstructionError* e)
	{
		cerr<<"Failed to create hash table. Exiting.\n";
		return 0;
	}
	catch (ResizeError* e)
	{
		cerr<< "Failed to resize hash table. Exiting.\n";
		return 0;
	}

	return 0;
}
