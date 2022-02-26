//****************************************************************************
// InOutUtil.cc                                         Input-Output utilities
//****************************************************************************

#include "InOutUtil.h"
#include <iostream>
#include <fstream>

std::ostream &operator<<(std::ostream &output, const Interval &I)
{
  if(I.origin)
    {
      for(integer_number j=I.beg; j<=I.end; j++) output << I.origin[j]
					       << "\n";
    }
  else std::cout << "Interval empty. No output produced." << std::endl;
  return output;                                      // enables cout << x << y
}

///////////////////////////////////////////////////////////////////////////////
std::istream &operator>>(std::istream &input, Interval &I)
{
  if(I.origin)
    {
      for(integer_number j=I.beg; j<=I.end; j++) 
	{
	  if(!input.eof()) input >> I.origin[j];
	  else 
	    {
	  std::cout << "Cannot read into Interval: not enough data available."
	       << "Exiting." << std::endl;
	  exit(1);
	    }
	}
    }
  else std::cout << "cannot input into empty interval." << std::endl;
  return input;
}                                                      // enables cin >> x >> y

///////////////////////////////////////////////////////////////////////////////
void PrintToFile(const Interval &I, const char *filename)
{
  std::ofstream glug(filename);
  if( !glug )
    {
      std::cout << "Can't open output file " << filename << ". Exiting." << std::endl;
      exit(1);
    }
  glug << I ;
}

///////////////////////////////////////////////////////////////////////////////
void ReadFromFile(Interval &I, const char *filename)
{
  std::ifstream glug(filename);
  if( !glug )
    {
      std::cout << "Can't open input file " << filename << ". Exiting." << std::endl;
      exit(1);
    }
  glug >> I;
}

///////////////////////////////////////////////////////////////////////////////
std::ostream &operator<<(std::ostream &output, const HedgePer &H)
{
  if(H.origin)
    {
      for(integer_number i=0; i < H.dim; i++)
	output << H.origin[i] << std::endl;
    }
  else std::cout << "HedgePer empty. No output produced." << std::endl;
  return output;
}

///////////////////////////////////////////////////////////////////////////////
std::istream &operator>>(std::istream &input, HedgePer &H)
{
  for(integer_number j=0; j < H.dim; j++) 
    {
      if(!input.eof()) input >> H.origin[j];
      else 
	{
	  std::cout << "Cannot read into HedgePer: not enough data available."
	       << "Exiting." << std::endl;
	  exit(1);
	}
    }
  return input;
}                                                      // enables cin >> x >> y

///////////////////////////////////////////////////////////////////////////////
void PrintToFile(const HedgePer &H, const char *filename)
{
  std::ofstream glug(filename);
  if( !glug )
    {
      std::cout << "Can't open output file " << filename << ". Exiting." << std::endl;
      exit(1);
    }
  glug << H ;
}

///////////////////////////////////////////////////////////////////////////////
void ReadFromFile(HedgePer &H, const char *filename)
{
  std::ifstream glug(filename);
  if( !glug )
    {
      std::cout << "Can't open input file " << filename << ". Exiting." << std::endl;
      exit(1);
    }
  glug >> H;
}

///////////////////////////////////////////////////////////////////////////////
void coutLevels(const HedgePer &H)
{
  if(H.levels) 
    for(integer_number i=0; i<H.num_of_levels; i++) std::cout << H.levels[i] << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
void PrintLevelsToFile(const HedgePer &H, const char *filename)
{
  if(H.levels)
    {
       std::ofstream glug(filename);
       if( !glug )
	 {
	   std::cout << "Can't open input file " << filename << ".Exiting." << std::endl;
	   exit(1);
	 }
       for(integer_number i=0; i<H.num_of_levels; i++) glug << H.levels[i] << std::endl;
    }
  else std::cout << "Hedge levels empty. No output produced." << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
std::ostream &operator<<(std::ostream &output, const HedgeAper &H)
{
  if(H.root)
    {
      for(integer_number i=0; i < H.num_of_levels; i++)
	output << H.root[i];
    }
  else std::cout << "HedgeAper empty. No output produced." << std::endl;
  return output;
}

///////////////////////////////////////////////////////////////////////////////
std::istream &operator>>(std::istream &input, HedgeAper &H)
{
  for(integer_number j=0; j < H.num_of_levels; j++) input >> H.root[j];
  return input;
}                                                      // enables cin >> x >> y

///////////////////////////////////////////////////////////////////////////////
void PrintToFile(const HedgeAper &H, const char *filename)
{
  std::ofstream glug(filename);
  if( !glug )
    {
      std::cout << "Can't open output file " << filename << ". Exiting." << std::endl;
      exit(1);
    }
  glug << H ;
}

///////////////////////////////////////////////////////////////////////////////
void ReadFromFile(HedgeAper &H, const char *filename)
{
  std::ifstream glug(filename);
  if( !glug )
    {
      std::cout << "Can't open input file " << filename << ". Exiting." << std::endl;
      exit(1);
    }
  glug >> H;
}

///////////////////////////////////////////////////////////////////////////////
void coutLevels(const HedgeAper &H)
{
  if(H.levels) 
    for(integer_number i=0; i<H.num_of_levels; i++) std::cout << H.levels[i] << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
void PrintLevelsToFile(const HedgeAper &H, const char *filename)
{
  if(H.levels)
    {
       std::ofstream glug(filename);
       if( !glug )
	 {
	   std::cout << "Can't open input file " << filename << ".Exiting." << std::endl;
	   exit(1);
	 }
       for(integer_number i=0; i<H.num_of_levels; i++) glug << H.levels[i] << std::endl;
    }
  else std::cout << "Hedge levels empty. No output produced." << std::endl;
}

///////////////////////////////////////////////////////////////////////////////
std::ostream &operator<<(std::ostream &output, const ArrayTreePer &A)
{
  if(A.origin)
    {
      for(integer_number i=0; i < A.dim * (A.maxlevel+1); i++)
	output << A.origin[i] << std::endl;
    }
  else std::cout << "ArrayTreePer empty. No output produced." << std::endl;
  return output;
}

///////////////////////////////////////////////////////////////////////////////
std::istream &operator>>(std::istream &input, ArrayTreePer &A)
{
  for(integer_number j=0; j < A.dim * (A.maxlevel+1); j++) 
    {
      if(!input.eof()) input >> A.origin[j];
      else 
	{
	  std::cout << "Cannot read into ArrayTreePer: not enough data available."
	       << "Exiting." << std::endl;
	  exit(1);
	}
    }
  return input;
}                                                      // enables cin >> x >> y

///////////////////////////////////////////////////////////////////////////////
void PrintToFile(const ArrayTreePer &A, const char *filename)
{
  std::ofstream glug(filename);
  if( !glug )
    {
      std::cout << "Can't open output file " << filename << ". Exiting." << std::endl;
      exit(1);
    }
  glug << A ;
}

///////////////////////////////////////////////////////////////////////////////
void ReadFromFile(ArrayTreePer &A, const char *filename)
{
  std::ifstream glug(filename);
  if( !glug )
    {
      std::cout << "Can't open input file " << filename << ". Exiting." << std::endl;
      exit(1);
    }
  glug >> A;
}

///////////////////////////////////////////////////////////////////////////////
std::ostream &operator<<(std::ostream &output, const ArrayTreeAper &A)
{
  if(A.root)
    {
      for(integer_number i=0; i < A.size; i++)
	output << A.root[i];
    }
  else std::cout << "ArrayTreeAper empty. No output produced." << std::endl;
  return output;
}

///////////////////////////////////////////////////////////////////////////////
std::istream &operator>>(std::istream &input, ArrayTreeAper &A)
{
  for(integer_number j=0; j < A.size; j++) input >> A.root[j];
  return input;
}                                                      // enables cin >> x >> y

///////////////////////////////////////////////////////////////////////////////
void PrintToFile(const ArrayTreeAper &A, const char *filename)
{
  std::ofstream glug(filename);
  if( !glug )
    {
      std::cout << "Can't open output file " << filename << ". Exiting." << std::endl;
      exit(1);
    }
  glug << A ;
}

///////////////////////////////////////////////////////////////////////////////
void ReadFromFile(ArrayTreeAper &A, const char *filename)
{
  std::ifstream glug(filename);
  if( !glug )
    {
      std::cout << "Can't open input file " << filename << ". Exiting." << std::endl;
      exit(1);
    }
  glug >> A;
}
