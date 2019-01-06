#ifndef ECLIPSE_H
#define ECLIPSE_H
#include <string>

using namespace std;

class Eclipse
{
	friend std::ostream& operator << (std::ostream& s, Eclipse *e);
	public:

		/**
		 * \brief The default constructor
		 */
		Eclipse();

		/**
		 * \brief The overwriten constructor
		 */
		Eclipse(string splitLine[], int r, string masterLine);

		/**
		 * \brief copies a Eclipse
		 * \return a reference to a Eclipse
		 */
		Eclipse& operator=(const Eclipse& objToCopy); //copy operator

		/**
		 * \brief copies a Eclipse
		 * \return a Eclipse
		 */
		Eclipse* operator=(const Eclipse* toCopy); //another copy operator

		/**
		 * \brief checks if two Eclipses are equivalent
		 */
		bool operator==(const Eclipse other); //equality operator

		/**
		 * \brief The default destructor
		 */
		~Eclipse();

		/**
		 * \brief the copy constructor
		 */
		Eclipse(const Eclipse& toCopy);

		/**
		 * \brief prints the data about the eclipse in CSV format
		 */
		void print();

		/**
		 * \brief checks if a string contains the '.' symbol
		 * \param check the string to check
		 * \return true if it does
		 * \return false otherwise
		 */
		bool isDecimal(string check);

		/**
		 * \brief "getter" methods for all 18 fields plus the original data line.
		 * \return the item
		 */
		int getCatNo();
		int getPlateNo();
		int getYear();
		string getMonth();
		int getDay();
		string getTime();
		int getDt();
		int getLunaNo();
		int getSarosNo();
		string getEclType();
		double getGamma();
		double getEclMag();
		double getLat();
		double getLon();
		int getSunAlt();
		int getSunAzm();
		int getPathWidth();
		string getCentralDur();
		string getMasterLine();
		int getR();

	private:
		/**
		 * These variables hold information about the eclipse
		 */
		int catNo;
		int plateNo;
		int year;
		string month;
		int day;
		string time;
		int dt;
		int lunaNo;
		int sarosNo;
		string eclType;
		double gamma;
		double eclMag;
		double lat;
		double lon;
		int sunAlt;
		int sunAzm;
		int pathWidth;
		string centralDur;
		string masterLine;
		int r;


};

#endif //Eclipse_H
