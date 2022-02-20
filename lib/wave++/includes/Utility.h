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


typedef real_DWT(*costFun)(const real_DWT *block, const integer &blocklength,
		       const real_DWT &sigma, const integer &signaldim);
typedef real_DWT(*costFunAdv)(const real_DWT *block, const integer &blocklength,
				const real_DWT &sigma, const real_DWT &factor, const integer &signaldim); //advanced cost function type with param factor

/**************************** Periodic case **********************************/

void ExtractHedge(HedgePer &H, const ArrayTreePer &A);
        // Extract hedge H from an array bin tree A
        // on input H  defined by a consistent sequence of levels
        // and apropriate memory allocated   
        // on output H filled out by the appropriate elts of A

void SuperposeHedge(const HedgePer &H, ArrayTreePer &A);
        // superpose (add on)  hedge H onto array bin tree A,
        // assumption: both H and A previously completely defined.
           
void GetCosts(const ArrayTreePer &A, Tree &B, costFun F, const real_DWT &sigma);
        // using functional F  calculate costs from given tree A.
        // assumption: on input B is default empty tree
        // on output B filled up with costs and B.maxlevel=A.maxlevel

void GetCosts(const ArrayTreePer &A, Tree &T, costFunAdv F, const real_DWT &sigma, const real_DWT &factor);
			// using functional F  calculate costs from given tree A.
			// assumption: on input B is default empty tree
			// on output B filled up with costs and B.maxlevel=A.maxlevel
			// advanced function to integrate the cost function used in "demoWavPack.cpp" 
			// implemented by Johannes Troppacher

void GetCostsHelp(const ArrayTreePer &A,  Node<real_DWT> **ptr,
		  costFun F, const real_DWT &sigma,
		  const integer &L, const integer &B );              
        // utility

void GetCostsHelp(const ArrayTreePer &A,  Node<real_DWT> **ptr,
			costFunAdv F, const real_DWT &sigma, const real_DWT &factor,
			const integer &L, const integer &B  );
			// utility
			// advanced function to integrate the cost function used in "demoWavPack.cpp" 
			// implemented by Johannes Troppacher

void BestBasis(HedgePer &H, const Tree &B);
        // get cost minimizing  basis in bin tree B
        // assumption: on input H is default empty hedge
        // on output H.num_of_levels and H.levels are completely defined
        // but H.origin is still 0

real_DWT BestBasisHelp(integer *Levs, integer &n, Node<real_DWT> *ptr, 
		   const integer &L);   
         // utility

void ExtractBestBasis(const ArrayTreePer &A, HedgePer &H, costFun F, 
		      const double &sigma);
         // wrapping function: start with array bin tree A, calculate costs 
         // using F and sigma. Temporarily store costs in a bin tree and then
         // find best basis which minimizes cost. On input H is an empty hedge,
         // on output H completely filled up and contains the best basis

void ExtractBestBasis(const ArrayTreePer &A, HedgePer &H, const double &sigma, const double &factor);
			// wrapping function: start with array bin tree A, calculate costs 
			// using F and sigma. Temporarily store costs in a bin tree and then
			// find best basis which minimizes cost. On input H is an empty hedge,
			// on output H completely filled up and contains the best basis
			// advanced function to integrate the cost function used in "demoWavPack.cpp" 
			// implemented by Johannes Troppacher

real_DWT OracCostAdv(const real_DWT *data, const integer &n, 
			const real_DWT &sigma, const real_DWT &factor, const integer &k);
			// advanced function to integrate the cost function used in "demoWavPack.cpp" 
			// and former defined in "demoTools.cpp" 
			// implemented by Johannes Troppacher

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
	      const real_DWT &sigma);
          // using functional F  calculate costs from a given array bin tree A
          // assumption : on input B is empty tree
          // on output B filled up with costs
                          
void GetCostsHelp(const ArrayTreeAper &A,  Node<real_DWT> **ptr,
		  costFun F, const real_DWT &sigma,
		  const integer &L, const integer &B );              
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
