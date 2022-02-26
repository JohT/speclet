//****************************************************************************
// BinTree.h                                     Header file for BinTree class
//***************************************************************************
#ifndef BINTREE_H
#define BINTREE_H

#include "common.h"
#include <assert.h>

template <class CONTENTYPE> 
class Node                      // node of BinTree
{
public:
  Node(const CONTENTYPE &c, Node<CONTENTYPE> *l, Node<CONTENTYPE> *r) :
    content(c), left(l), right(r) { }
                                // constructor. sets content, left and right

  CONTENTYPE content;           // what is stored in the node
  Node<CONTENTYPE> *left;       // pointer to left child
  Node<CONTENTYPE> *right;      // pointer to right child
};

///////////////////////////////////////////////////////////////////////////////
template <class CONTENTYPE>
class BinTree
{
 public:
  BinTree():  root(0) { }                 // default constructor, empty tree

  BinTree(const BinTree<CONTENTYPE> &);   // copy constructor

  virtual ~BinTree();                     // destructor

  virtual BinTree<CONTENTYPE>& operator=(const BinTree<CONTENTYPE>& Rhs); 
          // overloaded assignment

  void CopyTree(const Node<CONTENTYPE>* From, Node<CONTENTYPE>* &To);
          // copies tree rooted at From into a tree rooted at To

  void DestroyTree();                            // deallocates all memeory

  void DestroyTreeHelp(Node<CONTENTYPE>* &ptr);  // utility
 
  Node<CONTENTYPE> *root;
};
// end of BinTree class definition

//////////////////////////////////////////////////////////////////////////////
template <class CONTENTYPE>
class BinTreeComplete : public BinTree<CONTENTYPE>
{
 public:
  BinTreeComplete() : maxlevel(0) { }

  BinTreeComplete(integer_number MAXLEVEL, const CONTENTYPE &c);        
       // set maxlevel=MAXLEVEL and construct tree with contents of all
       // nodes equal c. 

  BinTreeComplete(const BinTreeComplete<CONTENTYPE> &);     // copy constructor

  BinTreeComplete<CONTENTYPE>& operator=(const BinTreeComplete<CONTENTYPE> &); 
                                       
  Node<CONTENTYPE> *block(const integer_number &L, const integer_number &B);
       // return pointer to B-th node at level L

 Node<CONTENTYPE> *blockHelp(Node<CONTENTYPE> *pos,
			     const integer_number &L, const integer_number &B) const;   

 void SetConst(Node<CONTENTYPE> **ptr, const integer_number &L, const CONTENTYPE &c);
       // attach to *ptr a tree with levels 0...L, filled up with constant c

 integer_number maxlevel;     // levels numbered from 0 to maxlevel                               
};
// end of BinTreeComplete class definition

typedef BinTreeComplete<real_number> Tree;

///////////////////////////////////////////////////////////////////////////////
/*********************** Implementation **************************************/

template <class CONTENTYPE>
BinTree<CONTENTYPE>::BinTree(const BinTree<CONTENTYPE> &Rhs)
{
  CopyTree(Rhs.root, root);
}

///////////////////////////////////////////////////////////////////////////////
template <class CONTENTYPE>
 BinTree<CONTENTYPE>::~BinTree()
{ 
    DestroyTree();
}

///////////////////////////////////////////////////////////////////////////////
template <class CONTENTYPE>
BinTree<CONTENTYPE> &BinTree<CONTENTYPE>::operator=(const BinTree<CONTENTYPE>& Rhs)  
{
  if(this != &Rhs)
    {
      DestroyTreeHelp(root);
      CopyTree(Rhs.root, root);
    }
  return *this;
}

///////////////////////////////////////////////////////////////////////////////
template <class CONTENTYPE>
void BinTree<CONTENTYPE>::CopyTree(const Node<CONTENTYPE>* From,
				   Node<CONTENTYPE>* &To)
{
  // use preorder
  if( From != 0 )
    {  // copy node
      To = new Node<CONTENTYPE>(From->content, 0, 0);
      assert( To != 0);   
      CopyTree(From->left, To->left);
      CopyTree(From->right, To->right);
    }
  else  To = 0;   // copy empty tree
} 

///////////////////////////////////////////////////////////////////////////////
template <class CONTENTYPE>
void BinTree<CONTENTYPE>::DestroyTree()
{
  DestroyTreeHelp( root );
}

///////////////////////////////////////////////////////////////////////////////
template <class CONTENTYPE>
void BinTree<CONTENTYPE>::DestroyTreeHelp(Node<CONTENTYPE>* &ptr)
{
  // use postorder
  if( ptr != 0 )
    { 
      DestroyTreeHelp(ptr->left);
      DestroyTreeHelp(ptr->right);  
      delete ptr;
      ptr=0;
    }
}

///////////////////////////////////////////////////////////////////////////////
template <class CONTENTYPE>
BinTreeComplete<CONTENTYPE>::BinTreeComplete(integer_number MAXLEVEL, 
					     const CONTENTYPE &c) :
  maxlevel(MAXLEVEL)
{
  assert(maxlevel>0);
  SetConst(&BinTree<CONTENTYPE>::root, maxlevel, c);
}

///////////////////////////////////////////////////////////////////////////////
template <class CONTENTYPE>
BinTreeComplete<CONTENTYPE>::BinTreeComplete(
				  const BinTreeComplete<CONTENTYPE> &Rhs)
{
  BinTree<CONTENTYPE>::CopyTree(Rhs.root, BinTree<CONTENTYPE>::root);
}

///////////////////////////////////////////////////////////////////////////////
template <class CONTENTYPE>
BinTreeComplete<CONTENTYPE>& BinTreeComplete<CONTENTYPE>::operator=
                                       (const BinTreeComplete<CONTENTYPE> &Rhs)
{
    maxlevel = Rhs.maxlevel;
    BinTree<CONTENTYPE>::operator=(Rhs);
}

///////////////////////////////////////////////////////////////////////////////
template <class CONTENTYPE>
Node<CONTENTYPE> *BinTreeComplete<CONTENTYPE>::block(const integer_number &L,
						    const integer_number &B)
{
  assert( 0<=L && L<=maxlevel && 0<=B && B<=(1<<L) );
  return blockHelp(BinTree<CONTENTYPE>::root, L, B);
}

///////////////////////////////////////////////////////////////////////////////
template <class CONTENTYPE>
Node<CONTENTYPE> *BinTreeComplete<CONTENTYPE>::blockHelp(Node<CONTENTYPE> *pos,
				     const  integer_number &L, const integer_number &B) const
{
  Node<CONTENTYPE> *ptr;
  if( L && pos)
    {
      if(B&(1<<(L-1)))                  // if L-bit of B is odd => turn right
	ptr=blockHelp(pos->right, L-1, B^(1<<(L-1)) );
      else
	ptr=blockHelp(pos->left, L-1, B);
    }
  else ptr=pos;
  return ptr;
}

///////////////////////////////////////////////////////////////////////////////
template <class CONTENTYPE>
void BinTreeComplete<CONTENTYPE>::SetConst(Node<CONTENTYPE> **ptr, 
				   const integer_number &L, const CONTENTYPE &c)
{
  if( L > 0 )
    {
      if( *ptr==0 )
	{       
	  (*ptr) = new Node<CONTENTYPE>(c, 0, 0);
	  assert( *ptr != 0 );
	}
      else (*ptr)->content=c;  
      SetConst(&((*ptr)->left), L-1, c);
      SetConst(&((*ptr)->right), L-1, c);
    }
}

#endif




