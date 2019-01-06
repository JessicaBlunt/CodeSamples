/*!
 *  \brief     Converts eclipse data from .txt to .csv
 *  \details   Original data must be in format provided on the class webpage.
 *  \author    Jessica Wiedemeier
 *  \date      11 Sept 2017
 *  \version   0.2
 */


#ifndef ECLIPSER_H
#define ECLIPSER_H
#include <string>
#include "ResizableArray.h"
#include "Eclipse.h"
#include "AVL.h"


void refreshArray();
AVL<Eclipse> getFile();

/**
 * \brief This is the main method.
 *
 * This method reads the eclipse archive from NASA and outputs the file, without
 * the 10 header lines, in CSV format.Returns 0.
 */
int main();

#endif
