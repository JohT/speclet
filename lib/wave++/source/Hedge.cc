//****************************************************************************
// Hedge.cc                                         Implementations of classes
//                                                      HedgePer and HedgeAper
//****************************************************************************

#include <iostream.h>
#include <fstream.h>
#include <assert.h>
#include "Hedge.h"

/********************** Hedge Periodic **************************************/

HedgePer::HedgePer(const integer &DIM, const integer &n, 
		   const integer *Levs) : dim(DIM), num_of_levels(n)
{
  assert(dim>0 && num_of_levels>0);
  levels=new integer [num_of_levels];
  assert(levels);
  for(int i=0; i<num_of_levels; i++) levels[i]=Levs[i];
  if (levels_consistent())
    {
      origin=new real [dim];
      assert(origin);
    }
  else
    {
      cout << "Attempt to declare a hedge with incosistent levels" 
	   << endl;
      exit(1);
    }
  integer p=maxlevel();
  assert( ((DIM>>p)<<p) == DIM );                       // dim divisible by 2^p ?
}

//////////////////////////////////////////////////////////////////////////////
HedgePer::HedgePer(const HedgePer &Rhs) : 
  dim(Rhs.dim), num_of_levels(Rhs.num_of_levels)
{
  if(Rhs.levels!=0 && Rhs.origin!=0)
    {
      levels=new integer [num_of_levels];
      assert(levels!=0);
      origin=new real [dim];
      assert(origin!=0);
      integer i;
      for(i=0; i<num_of_levels; i++)   levels[i]=Rhs.levels[i];
      for(i=0; i<dim; i++)  origin[i]=Rhs.origin[i];
    }
  else 
    {
      levels=0;
      origin=0;
    }
}

//////////////////////////////////////////////////////////////////////////////
void HedgePer::DestroyHedge()
{
  if (origin!=0) delete [] origin;
  if (levels!=0) delete [] levels;
  origin=0;
  levels=0;
  dim=0;
}

//////////////////////////////////////////////////////////////////////////////
const HedgePer &HedgePer::operator=(const HedgePer &Rhs)
{
  if(this != &Rhs)
    {
      DestroyHedge();
      dim=Rhs.dim;
      num_of_levels=Rhs.num_of_levels;
      if(Rhs.levels!=0 && Rhs.origin!=0)
	{
	  levels=new integer [num_of_levels];
	  assert(levels!=0);
	  origin=new real [dim];
	  assert(origin!=0);
	  integer i;
	  for(i=0; i<num_of_levels; i++)   levels[i]=Rhs.levels[i];
	  for(i=0; i<dim; i++)  origin[i]=Rhs.origin[i];
	}
      else 
	{
	  levels=0;
	  origin=0;
	}
    }
  return (*this);
}

//////////////////////////////////////////////////////////////////////////////
bool HedgePer::levels_consistent() const
{
  if (levels!=0)
    {
      integer p = maxlevel();
      integer i, k, sum = 1<<(p - levels[0]);
      for(i=1; i<num_of_levels; i++)
	{
	  k = p - levels[i];
	  if( (sum>>k)<<k != sum )  return false;       //sum divisible by 2^k?
	  sum += 1<<k;                                            // sum += 2^k
	  if( sum > (1<<p) ) return false;
	}
      if( sum != (1<<p) ) return false;
    }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
integer HedgePer::maxlevel() const
{
  if(levels)
    {
      integer p=0;
      for(integer j=0; j<num_of_levels; j++) 
	if ( p < levels[j] ) p = levels[j];
      return p;
    }
  else
    {
      cout << "Levels in hedge empty. Returning zero as maxlevel. " << endl;
      return 0;
    }
}

//////////////////////////////////////////////////////////////////////////////
integer HedgePer::block_length(const integer &i) const
{
  assert(0<=i && i<num_of_levels);
  return ( dim >> (levels[i]) );         // dim/number of blocks at i-th level 
}

//////////////////////////////////////////////////////////////////////////////
real *HedgePer::block_start(const integer &i)  const
{
  assert(origin);
  assert(0<=i && i<num_of_levels);
  integer len=0;
  for(integer j=0; j<i; j++)  len += dim >> (levels[j]);
  return (origin + len);
}

/********************** Hedge Aperiodic **************************************/

HedgeAper::HedgeAper(const integer &n, const integer *Levs) :
  num_of_levels(n)
{
  assert(num_of_levels>0);
  levels=new integer [num_of_levels];
  assert(levels);
  for(int i=0; i<num_of_levels; i++) levels[i]=Levs[i];
  if (levels_consistent())
    { 
      root = new Interval [num_of_levels];
      assert(root);
    }
  else
    {
      cout << "Attempt to declare a hedge with incosistent levels" 
	   << endl;
      exit(1);
    }
}

//////////////////////////////////////////////////////////////////////////////
HedgeAper::HedgeAper(const HedgeAper &Rhs) : num_of_levels(Rhs.num_of_levels)
{
  if(Rhs.levels!=0 && Rhs.root!=0)
    {
      levels=new integer [num_of_levels];
      assert(levels);
      root = new Interval [num_of_levels];
      assert(root);
      integer i;
      for(i=0; i<num_of_levels; i++)   
	{
	  levels[i]=Rhs.levels[i];
	  root[i]=Rhs.root[i];
	}
    }
  else 
    {
      levels=0;
      root=0;
    }
}

//////////////////////////////////////////////////////////////////////////////
void HedgeAper::DestroyHedge()
{
  if (root!=0) delete [] root;
  if (levels!=0) delete [] levels;
  root=0;
  levels=0;
}

//////////////////////////////////////////////////////////////////////////////
const HedgeAper &HedgeAper::operator=(const HedgeAper &Rhs)
{
  if(this != &Rhs)
    {
      DestroyHedge();
      num_of_levels=Rhs.num_of_levels;
      if(Rhs.levels!=0 && Rhs.root!=0)
	{
	  levels=new integer [num_of_levels];
	  assert(levels!=0);
	  root = new Interval [num_of_levels];
	  assert(root);
	  for(integer i=0; i<num_of_levels; i++)   
	    {
	      levels[i]=Rhs.levels[i];
	      root[i]=Rhs.root[i];
	    }
	}
      else 
	{
	  levels=0;
	  root=0;
	}
    }
  return (*this);
}

//////////////////////////////////////////////////////////////////////////////
bool HedgeAper::levels_consistent() const
{
  if (levels!=0)
    {
      integer p = maxlevel();
      integer i, k, sum = 1<<(p - levels[0]);
      for(i=1; i<num_of_levels; i++)
	{
	  k = p - levels[i];
	  if( (sum>>k)<<k != sum )  return false;       //sum divisible by 2^k?
	  sum += 1<<k;                                            // sum += 2^k
	  if( sum > (1<<p) ) return false;
	}
      if( sum != (1<<p) ) return false;
    }
  return true;
}

//////////////////////////////////////////////////////////////////////////////
integer HedgeAper::maxlevel() const
{
  if(levels)
    {
      integer p=0;
      for(integer j=0; j<num_of_levels; j++) 
	if ( p < levels[j] ) p = levels[j];
      return p;
    }
  else 
    {
      cout << "Levels in hedge empty. Returning 0 as maxlevel." << endl;
      return 0;
    }
}




