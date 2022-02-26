//****************************************************************************
// InOutUtil.cc                                         Input-Output utilities
//****************************************************************************

#include "InOutUtil.h"
#include <iostream.h>
#include <fstream.h>

ostream &operator<<(ostream &output, const Interval &I)
{
  if(I.origin)
    {
      for(integer j=I.beg; j<=I.end; j++) output << I.origin[j]
					       << "\n";
    }
  else cout << "Interval empty. No output produced." << endl;
  return output;                                      // enables cout << x << y
}

///////////////////////////////////////////////////////////////////////////////
istream &operator>>(istream &input, Interval &I)
{
  if(I.origin)
    {
      for(integer j=I.beg; j<=I.end; j++) 
	{
	  if(!input.eof()) input >> I.origin[j];
	  else 
	    {
	  cout << "Cannot read into Interval: not enough data available."
	       << "Exiting." << endl;
	  exit(1);
	    }
	}
    }
  else cout << "cannot input into empty interval." << endl;
  return input;
}                                                      // enables cin >> x >> y

///////////////////////////////////////////////////////////////////////////////
void PrintToFile(const Interval &I, const char *filename)
{
  ofstream glug(filename);
  if( !glug )
    {
      cout << "Can't open output file " << filename << ". Exiting." << endl;
      exit(1);
    }
  glug << I ;
}

///////////////////////////////////////////////////////////////////////////////
void ReadFromFile(Interval &I, const char *filename)
{
  ifstream glug(filename);
  if( !glug )
    {
      cout << "Can't open input file " << filename << ". Exiting." << endl;
      exit(1);
    }
  glug >> I;
}

///////////////////////////////////////////////////////////////////////////////
ostream &operator<<(ostream &output, const HedgePer &H)
{
  if(H.origin)
    {
      for(integer i=0; i < H.dim; i++)
	output << H.origin[i] << endl;
    }
  else cout << "HedgePer empty. No output produced." << endl;
  return output;
}

///////////////////////////////////////////////////////////////////////////////
istream &operator>>(istream &input, HedgePer &H)
{
  for(integer j=0; j < H.dim; j++) 
    {
      if(!input.eof()) input >> H.origin[j];
      else 
	{
	  cout << "Cannot read into HedgePer: not enough data available."
	       << "Exiting." << endl;
	  exit(1);
	}
    }
  return input;
}                                                      // enables cin >> x >> y

///////////////////////////////////////////////////////////////////////////////
void PrintToFile(const HedgePer &H, const char *filename)
{
  ofstream glug(filename);
  if( !glug )
    {
      cout << "Can't open output file " << filename << ". Exiting." << endl;
      exit(1);
    }
  glug << H ;
}

///////////////////////////////////////////////////////////////////////////////
void ReadFromFile(HedgePer &H, const char *filename)
{
  ifstream glug(filename);
  if( !glug )
    {
      cout << "Can't open input file " << filename << ". Exiting." << endl;
      exit(1);
    }
  glug >> H;
}

///////////////////////////////////////////////////////////////////////////////
void coutLevels(const HedgePer &H)
{
  if(H.levels) 
    for(integer i=0; i<H.num_of_levels; i++) cout << H.levels[i] << endl;
}

///////////////////////////////////////////////////////////////////////////////
void PrintLevelsToFile(const HedgePer &H, const char *filename)
{
  if(H.levels)
    {
       ofstream glug(filename);
       if( !glug )
	 {
	   cout << "Can't open input file " << filename << ".Exiting." << endl;
	   exit(1);
	 }
       for(integer i=0; i<H.num_of_levels; i++) glug << H.levels[i] << endl;
    }
  else cout << "Hedge levels empty. No output produced." << endl;
}

///////////////////////////////////////////////////////////////////////////////
ostream &operator<<(ostream &output, const HedgeAper &H)
{
  if(H.root)
    {
      for(integer i=0; i < H.num_of_levels; i++)
	output << H.root[i];
    }
  else cout << "HedgeAper empty. No output produced." << endl;
  return output;
}

///////////////////////////////////////////////////////////////////////////////
istream &operator>>(istream &input, HedgeAper &H)
{
  for(integer j=0; j < H.num_of_levels; j++) input >> H.root[j];
  return input;
}                                                      // enables cin >> x >> y

///////////////////////////////////////////////////////////////////////////////
void PrintToFile(const HedgeAper &H, const char *filename)
{
  ofstream glug(filename);
  if( !glug )
    {
      cout << "Can't open output file " << filename << ". Exiting." << endl;
      exit(1);
    }
  glug << H ;
}

///////////////////////////////////////////////////////////////////////////////
void ReadFromFile(HedgeAper &H, const char *filename)
{
  ifstream glug(filename);
  if( !glug )
    {
      cout << "Can't open input file " << filename << ". Exiting." << endl;
      exit(1);
    }
  glug >> H;
}

///////////////////////////////////////////////////////////////////////////////
void coutLevels(const HedgeAper &H)
{
  if(H.levels) 
    for(integer i=0; i<H.num_of_levels; i++) cout << H.levels[i] << endl;
}

///////////////////////////////////////////////////////////////////////////////
void PrintLevelsToFile(const HedgeAper &H, const char *filename)
{
  if(H.levels)
    {
       ofstream glug(filename);
       if( !glug )
	 {
	   cout << "Can't open input file " << filename << ".Exiting." << endl;
	   exit(1);
	 }
       for(integer i=0; i<H.num_of_levels; i++) glug << H.levels[i] << endl;
    }
  else cout << "Hedge levels empty. No output produced." << endl;
}

///////////////////////////////////////////////////////////////////////////////
ostream &operator<<(ostream &output, const ArrayTreePer &A)
{
  if(A.origin)
    {
      for(integer i=0; i < A.dim * (A.maxlevel+1); i++)
	output << A.origin[i] << endl;
    }
  else cout << "ArrayTreePer empty. No output produced." << endl;
  return output;
}

///////////////////////////////////////////////////////////////////////////////
istream &operator>>(istream &input, ArrayTreePer &A)
{
  for(integer j=0; j < A.dim * (A.maxlevel+1); j++) 
    {
      if(!input.eof()) input >> A.origin[j];
      else 
	{
	  cout << "Cannot read into ArrayTreePer: not enough data available."
	       << "Exiting." << endl;
	  exit(1);
	}
    }
  return input;
}                                                      // enables cin >> x >> y

///////////////////////////////////////////////////////////////////////////////
void PrintToFile(const ArrayTreePer &A, const char *filename)
{
  ofstream glug(filename);
  if( !glug )
    {
      cout << "Can't open output file " << filename << ". Exiting." << endl;
      exit(1);
    }
  glug << A ;
}

///////////////////////////////////////////////////////////////////////////////
void ReadFromFile(ArrayTreePer &A, const char *filename)
{
  ifstream glug(filename);
  if( !glug )
    {
      cout << "Can't open input file " << filename << ". Exiting." << endl;
      exit(1);
    }
  glug >> A;
}

///////////////////////////////////////////////////////////////////////////////
ostream &operator<<(ostream &output, const ArrayTreeAper &A)
{
  if(A.root)
    {
      for(integer i=0; i < A.size; i++)
	output << A.root[i];
    }
  else cout << "ArrayTreeAper empty. No output produced." << endl;
  return output;
}

///////////////////////////////////////////////////////////////////////////////
istream &operator>>(istream &input, ArrayTreeAper &A)
{
  for(integer j=0; j < A.size; j++) input >> A.root[j];
  return input;
}                                                      // enables cin >> x >> y

///////////////////////////////////////////////////////////////////////////////
void PrintToFile(const ArrayTreeAper &A, const char *filename)
{
  ofstream glug(filename);
  if( !glug )
    {
      cout << "Can't open output file " << filename << ". Exiting." << endl;
      exit(1);
    }
  glug << A ;
}

///////////////////////////////////////////////////////////////////////////////
void ReadFromFile(ArrayTreeAper &A, const char *filename)
{
  ifstream glug(filename);
  if( !glug )
    {
      cout << "Can't open input file " << filename << ". Exiting." << endl;
      exit(1);
    }
  glug >> A;
}
