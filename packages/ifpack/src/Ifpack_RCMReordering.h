#ifndef IFPACK_RCMREORDERING_H
#define IFPACK_RCMREORDERING_H

#include "Ifpack_ConfigDefs.h"
#include "Ifpack_Reordering.h"
#include <vector>

namespace Teuchos {
  class ParameterList;
}
class Ifpack_Graph;
class Epetra_MultiVector;

//! Ifpack_RCMReordering: reverse Cuthill-McKee reordering.
/*!
Ifpack_RCMReordering defined reverse Cuthill-McKee reordering
for a given Ifpack_Graph. Note that the graph is used \e only
by method Compute().

A typical code reads as follows:
\code
#include "Ifpack_RCMReordering.h"
#include "Ifpack_Graph.h"
// A is an Epetra_RowMatrix pointer.
// Need to create a graph (which is a simple wrapper)
// This required include file Ifpack_Graph_Epetra_RowMatrix.h
Ifpack_Graph_Epetra_RowMatrix Graph(A);

// Construct the object
Ifpack_RCMReordering Reorder(Graph);
// Compute the reordering.
IFPACK_CHK_ERR(Reorder.Compute());
// Prints out some information
cout << Reorder;
\endcode

\note TODO: In P() and Pinv(), support the case of equal vectors.

\author Marzio Sala, SNL 9214

\date Oct-04
*/

class Ifpack_RCMReordering : public Ifpack_Reordering {

public:

  //! Constructor for Ifpack_Graph's.
  Ifpack_RCMReordering();

  //! Copy Constructor.
  Ifpack_RCMReordering(const Ifpack_RCMReordering& RHS);

  //! Assignment operator.
  Ifpack_RCMReordering& operator=(const Ifpack_RCMReordering& RHS);

  //! Destructor.
  virtual ~Ifpack_RCMReordering();
  
  //! Sets integer parameters `Name'.
  virtual int SetParameter(const string Name, const int Value);

  //! Sets double parameters `Name'.
  virtual int SetParameter(const string Name, const double Value);
  
#ifdef HAVE_IFPACK_TEUCHOS  
  //! Sets all parameters.
  virtual int SetParameters(Teuchos::ParameterList& List);
#endif

  //! Computes all it is necessary to initialize the reordering object.
  virtual int Compute(const Ifpack_Graph& Graph);

  //! Computes all it is necessary to initialize the reordering object.
  virtual int Compute(const Epetra_RowMatrix& Matrix);

  //! Returns \c true is the reordering object has been successfully initialized, false otherwise.
  virtual bool IsComputed() const
  {
    return(IsComputed_);
  }

  //! Returns the reordered index of row \c i.
  virtual int Reorder(const int i) const;

  //! Returns the inverse reordered index of row \c i.
  virtual int InvReorder(const int i) const;

  //! Applies reordering to multivector X, whose local length equals the number of local rows.
  virtual int P(const Epetra_MultiVector& Xorig,
		Epetra_MultiVector& Xreord) const;

  //! Applies inverse reordering to multivector X, whose local length equals the number of local rows.
  virtual int Pinv(const Epetra_MultiVector& Xorig,
		   Epetra_MultiVector& Xinvreord) const;

  
  //! Prints basic information on iostream. This function is used by operator<<.
  virtual ostream& Print(std::ostream& os) const;

  //! Returns the number of local rows.
  virtual int NumMyRows() const 
  {
    return(NumMyRows_);
  }

  //! Returns the root node.
  virtual int RootNode() const 
  {
    return(RootNode_);
  }

private:
  //! Defines the root node (defaulted to 0).
  int RootNode_;
  //! Number of local rows in the graph.
  int NumMyRows_;
  //! If \c true, the reordering has been successfully computed.
  bool IsComputed_;
  //! Contains the reordering.
  vector<int> Reorder_;
  //! Contains the inverse reordering.
  vector<int> InvReorder_;
}; 

#endif
