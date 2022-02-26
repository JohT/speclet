//*****************************************************************************
// Utility.h                                       Prototypes utility functions
//*****************************************************************************

#ifndef UTILITY_H
#define UTILITY_H

#include "common.h"
#include "ArrayTree.h"
#include "BinTree.h"
#include "Hedge.h"
#include "Interval.h"


typedef real_number(*costFun)(const real_number *block, const integer_number &blocklength,
		       const real_number &sigma, const integer_number &signaldim);

/**************************** Periodic case **********************************/

void ExtractHedge(HedgePer &H, const ArrayTreePer &A);
        // Extract hedge H from an array bin tree A
        // on input H  defined by a consistent sequence of levels
        // and apropriate memory allocated   
        // on output H filled out by the appropriate elts of A

void SuperposeHedge(const HedgePer &H, ArrayTreePer &A);
        // superpose (add on)  hedge H onto array bin tree A,
        // assumption: both H and A previously completely defined.
           
void GetCosts(const ArrayTreePer &A, Tree &B, costFun F, const real_number &sigma);
        // using functional F  calculate costs from given tree A.
        // assumption: on input B is default empty tree
        // on output B filled up with costs and B.maxlevel=A.maxlevel
  
void GetCostsHelp(const ArrayTreePer &A,  Node<real_number> **ptr,
		  costFun F, const real_number &sigma,
		  const integer_number &L, const integer_number &B );              
        // utility

void BestBasis(HedgePer &H, const Tree &B);
        // get cost minimizing  basis in bin tree B
        // assumption: on input H is default empty hedge
        // on output H.num_of_levels and H.levels are completely defined
        // but H.origin is still 0

real_number BestBasisHelp(integer_number *Levs, integer_number &n, Node<real_number> *ptr, 
		   const integer_number &L);   
         // utility

void ExtractBestBasis(const ArrayTreePer &A, HedgePer &H, costFun F, 
		      const double &sigma);
         // wrapping function: start with array bin tree A, calculate costs 
         // using F and sigma. Temporarily store costs in a bin tree and then
         // find best basis which minimizes cost. On input H is an empty hedge,
         // on output H completely filled up and contains the best basis

/*************************** Aperiodic case **********************************/

void ExtractHedge(HedgeAper &H, const ArrayTreeAper &A); 
         // Extract hedge H from an array bin tree A
         // assumption: on input H  defined by a consistent sequence of levels
         // and apropriate number of empty intervals allocated
         // on output intervals of H are filled  by the corresp. elts of A

void SuperposeHedge(const HedgeAper &H, ArrayTreeAper &A); 
         // superpose (add on) hedge H onto array bin tree A,
         // assumption: both H and A previously completely defined.

void GetCosts(const ArrayTreeAper &A, Tree &B, costFun F, 
	      const real_number &sigma);
          // using functional F  calculate costs from a given array bin tree A
          // assumption : on input B is empty tree
          // on output B filled up with costs
                          
void GetCostsHelp(const ArrayTreeAper &A,  Node<real_number> **ptr,
		  costFun F, const real_number &sigma,
		  const integer_number &L, const integer_number &B );              
          // utility

void BestBasis(HedgeAper &H, const Tree &B);
          // get cost minimizing  basis in bin tree B
          // on input H is empty hedge, on output H.num_of_levels and H.levels
          // completely defined, but H.root is still 0

void ExtractBestBasis(const ArrayTreeAper &A, HedgeAper &H, costFun F, 
		      const double &sigma);
         // wrapping function: start with array bin tree A, calculate costs 
         // using F and sigma. Temporarily store costs in a bin tree and then
         // find best basis which minimizes cost. On input H is an empty hedge,
         // on output H completely filled up and contains the best basis
#endif
