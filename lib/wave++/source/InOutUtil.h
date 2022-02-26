//****************************************************************************
// InOutUtil.h                                          Input-Output utilities
//****************************************************************************
#ifndef INOUTUTIL_H
#define INOUTUTIL_H

#include "common.h"
#include "Interval.h"
#include "Hedge.h"
#include "ArrayTree.h"
#include <iostream>

/************************** Interval *****************************************/

std::ostream &operator<<(std::ostream &, const Interval &);
     // output interval as a column

std::istream &operator>>(std::istream &, Interval &);
     // read into I from a stream

void PrintToFile(const Interval &I, const char *filename);
     // print to file as column

void ReadFromFile(Interval &I, const char *filename);
     // read data from file into I
     // assumption: I.beg and I.end set, I.origin allocated 
                     
/***************************** HedgePer *************************************/

std::ostream &operator<<(std::ostream &output, const HedgePer &H);
     // output hedge's data as one long column

std::istream &operator>>(std::istream &input, HedgePer &H);
     // read into H from input stream
     // assumptions: Hedge completely allocated 

void PrintToFile(const HedgePer &H, const char *filename);
     // print to file as one long column

void coutLevels(const HedgePer &H);
     // print levels of H to screen

void ReadFromFile(HedgePer &H, const char *filename);
     // read data from file into H
     // assumption: Hedge completely allocated 

void PrintLevelsToFile(const HedgePer &H, const char *filename);
     // print levels of H as one column

/***************************** HedgeAper *************************************/

std::ostream &operator<<(std::ostream &output, const HedgeAper &H);
        // output hedge's data as one long column

std::istream &operator>>(std::istream &input, HedgeAper &H);
     // read into H from input stream
     // assumptions: H completely allocated 

void PrintToFile(const HedgeAper &H, const char *filename);
     // print to file as one long column

void ReadFromFile(HedgeAper &H, const char *filename);
     // read data from file into H
     // assumptions: H completely allocated 

void coutLevels(const HedgeAper &H);
     // print levels of H to screen

void PrintLevelsToFile(const HedgeAper &H, const char *filename);
     // print levels of H as one column

/***************************** ArrayTreePer **********************************/

std::ostream &operator<<(std::ostream &output, const ArrayTreePer &A);
     // output A as one long column, level after level

std::istream &operator>>(std::istream &input, ArrayTreePer &A);
     // read into A from input stream, level after level
     // assumption: A completely allocated 

void PrintToFile(const ArrayTreePer &A, const char *filename);
     // print to file as one long column, level after level

void ReadFromFile(ArrayTreePer &A, const char *filename);
     // read data from file into A
     // assumption: A.maxlevel and A.dim set, A.origin allocated 

/***************************** ArrayTreeAper *********************************/

std::ostream &operator<<(std::ostream &output, const ArrayTreeAper &A);
     // output A as one long column, level after level

std::istream &operator>>(std::istream &input, ArrayTreeAper &A);
     // read into A from input stream, level after level
     // assumptions: A completely allocated 

void PrintToFile(const ArrayTreeAper &A, const char *filename);
     // print to file as one long column, level after level

void ReadFromFile(ArrayTreeAper &A, const char *filename);
     // read data from file into A
     // assumption: A completely allocated 
     // and all intervals have  beg and end defined and origins allocated
     // assumption: file has enough data to fill out A 
#endif
