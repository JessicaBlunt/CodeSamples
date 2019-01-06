/**
 * Holds information about one eclipse
 */

#include <string>
#include <iostream>
#include "Eclipse.h"

/**
 * \brief copies a Eclipse
 * \return a reference to a Eclipse
 */
Eclipse& Eclipse::operator=(const Eclipse& toCopy)
{
	if (this == &toCopy)
	{
		return *this;
	}
	catNo = toCopy.catNo;
	plateNo = toCopy.plateNo;
	year = toCopy.year;
	month = toCopy.month;
	day = toCopy.day;
	time = toCopy.time;
	dt= toCopy.dt;
	lunaNo = toCopy.lunaNo;
	sarosNo = toCopy.sarosNo;
	eclType = toCopy.eclType;
	gamma = toCopy.gamma;
	eclMag = toCopy.eclMag;
	lat = toCopy.lat;
	lon = toCopy.lon;
	sunAlt = toCopy.sunAlt;
	sunAzm = toCopy.sunAzm;
	pathWidth = toCopy.pathWidth;
	centralDur = toCopy.centralDur;
	masterLine = toCopy.masterLine;
	r = toCopy.r;
	return *this;
}


/**
 * \brief copies a Eclipse
 * \return a Eclipse
 */
Eclipse* Eclipse::operator=(const Eclipse* toCopy)
{
	return this;
}

/**
 * \brief check if two Eclipses are equivalent
 * \return true or false
 */
bool Eclipse::operator==(const Eclipse other)
{
	if (catNo != other.catNo || plateNo != other.plateNo ||
			year != other.year || month != other.month ||
			day != other.day || time != other.time || dt != other.dt ||
			lunaNo != other.lunaNo || sarosNo != other.sarosNo ||
			eclType != other.eclType || gamma != other.gamma ||
			eclMag != other.eclMag || lat != other.lat ||
			lon != other.lon || sunAlt != other.sunAlt ||
			sunAzm != other.sunAzm || pathWidth != other.pathWidth ||
			centralDur != other.centralDur)
	{
		/*cout << catNo << " " << other->catNo << "\n" <<
			plateNo << " " << other->plateNo << "\n" <<
			year << " " << other->year << "\n" << month << " " << other->month << "\n" <<
			day  << " " << other->day << "\n" << time  << " " <<  other->time << "\n" << dt  << " " <<  other->dt << "\n" <<
			lunaNo  << " " <<  other->lunaNo << "\n" << sarosNo  << " " <<  other->sarosNo << "\n" <<
			eclType  << " " <<  other->eclType << "\n" << gamma  << " " <<  other->gamma << "\n" <<
			eclMag  << " " <<  other->eclMag << "\n" << lat  << " " <<  other->lat << "\n" <<
			lon << " " << other->lon << "\n" << sunAlt << " " << other->sunAlt << "\n" <<
			sunAzm << " " << other->sunAzm << "\n" << pathWidth  << " " << other->pathWidth << "\n" <<
			centralDur  << " " << other->centralDur << "\n";
			*/
		return false;
	}
	else
	{
		return true;
	}
}

/**
 * \brief The default destructor
 */
Eclipse::~Eclipse()
{
}



/**
 * \brief The default constructor
 */
Eclipse::Eclipse()
{
	catNo = 0;
	plateNo = 0;
	year = 0;
	month = "";
	day = 0;
	time = "";
	dt = 0;
	lunaNo = 0;
	sarosNo = 0;
	eclType = "";
	gamma = 0;
	eclMag = 0;
	lat = 0;
	lon = 0;
	sunAlt = 0;
	sunAzm = 0;
	pathWidth = 0;
	centralDur = "";
	masterLine = "";
	r = 0;
}

/**
 * \brief Eclipse constructor
 * \param splitLine[] the array of values
 * \param r the row of the data file
 *
 * This method creates an Eclipse object and checks that all
 * numerical values could be of the correct type (whole or decimal)
 */
Eclipse::Eclipse(string splitLine[], int r, string masterLine)
{

	catNo = stoi(splitLine[0]);
	plateNo = stoi(splitLine[1]);
	year = stoi(splitLine[2]);
	month = splitLine[3];
	day = stoi(splitLine[4]);
	this->time = splitLine[5];
	dt = stoi(splitLine[6]);
	lunaNo = stoi(splitLine[7]);
	sarosNo = stoi(splitLine[8]);
	eclType = splitLine[9];
	gamma = stod(splitLine[10]);
	eclMag = stod(splitLine[11]);
	lat = stod(splitLine[12]);
	lon = stod(splitLine[13]);
	sunAlt = stoi(splitLine[14]);
	sunAzm = stoi(splitLine[15]);
	this->r = r;

	if (splitLine[16] != "" and splitLine[16] != "-")
	{
		pathWidth = stoi(splitLine[16]);
		centralDur = splitLine[17];
	}
	else
	{
		pathWidth = 99999999;
		centralDur = "zzzzzzzzzzzzz";
	}
	this->masterLine = masterLine;
	for (int i = 1; i <= 18; ++i)
	{
		if ((i >= 1 && i <= 3) || i == 5 || (i >= 7 && i <= 9) || (i >= 15 && i != 18))
		{
			if (string(splitLine[i-1]) == "-" and i != 17)
			{
				cerr << "Error in data row " + to_string(r) + ": Column " + to_string(i) + " is a dash. Set to 0.\n";
				splitLine[i-1] = "0";
			}
			else if (isDecimal(string(splitLine[i-1])))
			{
				cerr << "Error in data row " + to_string(r) + ": Column " + to_string(i) + " is not a whole number.\n";
			}
		}
		else if (i == 11 || i == 12)
		{
			if (string(splitLine[i-1]) == "-")
			{
				cerr << "Error in data row " + to_string(r) + ": Column " + to_string(i) + " is a dash. Set to 0.\n";
				splitLine[i-1] = "0";
			}
			if (!isDecimal(string(splitLine[i-1])))
			{
				cerr << "Error in data row " + to_string(r) + ": Column " + to_string(i) + " is not a decimal number.\n";
			}
		}

	}
}

/**
 * \brief the copy constructor
 */
Eclipse::Eclipse(const Eclipse& toCopy)
{
    catNo = toCopy.catNo;
    plateNo = toCopy.plateNo;
    year = toCopy.year;
    month = toCopy.month;
    day = toCopy.day;
    time = toCopy.time;
    dt= toCopy.dt;
    lunaNo = toCopy.lunaNo;
    sarosNo = toCopy.sarosNo;
    eclType = toCopy.eclType;
    gamma = toCopy.gamma;
    eclMag = toCopy.eclMag;
    lat = toCopy.lat;
    lon = toCopy.lon;
    sunAlt = toCopy.sunAlt;
   	sunAzm = toCopy.sunAzm;
   	pathWidth = toCopy.pathWidth;
   	centralDur = toCopy.centralDur;
   	masterLine = toCopy.masterLine;
   	r = toCopy.r;

   	//cout << " in cc " << toCopy.catNo << " and " << catNo << endl;
   	//return this;
}

/**
 * \brief prints the data about the eclipse in CSV format
 */
ostream& operator << (ostream& s, Eclipse *e)
{
	s << e->masterLine << endl;
	return s;
}

/**
 * \brief checks if a string contains the '.' symbol
 * \param check the string to check
 * \return true if it does
 * \return false otherwise
 */
bool Eclipse::isDecimal(string check)
{
	for(unsigned int i = 0; i < check.length(); ++i)
	{
		if (check[i] == '.')
		{
			return true;
		}
	}
	return false;
}

int Eclipse::getCatNo()
{
	return catNo;
}
int Eclipse::getPlateNo()
{
	return plateNo;
}
int Eclipse::getYear()
{
	return year;
}
string Eclipse::getMonth()
{
	return month;
}
int Eclipse::getDay()
{
	return day;
}
string Eclipse::getTime()
{
	return time;
}
int Eclipse::getDt()
{
	return dt;
}
int Eclipse::getLunaNo()
{
	return lunaNo;
}
int Eclipse::getSarosNo()
{
	return sarosNo;
}
string Eclipse::getEclType()
{
	return eclType;
}
double Eclipse::getGamma()
{
	return gamma;
}
double Eclipse::getEclMag()
{
	return eclMag;
}
double Eclipse::getLat()
{
	return lat;
}
double Eclipse::getLon()
{
	return lon;
}
int Eclipse::getSunAlt()
{
	return sunAlt;
}
int Eclipse::getSunAzm()
{
	return sunAzm;
}
int Eclipse::getPathWidth()
{
	return pathWidth;
}
string Eclipse::getCentralDur()
{
	return centralDur;
}
string Eclipse::getMasterLine()
{
	return masterLine;
}
int Eclipse::getR()
{
	return r;
}
