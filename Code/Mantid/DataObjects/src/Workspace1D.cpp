#include "MantidDataObjects/Workspace1D.h"
#include "MantidAPI/TripleRef.h"
#include "MantidAPI/TripleIterator.h"
#include "MantidAPI/TripleIteratorCode.h"
#include "MantidAPI/WorkspaceProperty.h"

DECLARE_WORKSPACE(Workspace1D)

namespace Mantid
{
namespace DataObjects
{

/// Constructor
Workspace1D::Workspace1D() : API::Workspace(), 
			     Histogram1D()
{ }

  /// Copy Constructor
Workspace1D::Workspace1D(const Workspace1D& A) :
  API::Workspace(A),Histogram1D(A)
{ }

/*!
    Assignment operator
    \param A :: Workspace  to copy
    \return *this
   */
Workspace1D& 
Workspace1D::operator=(const Workspace1D& A)
{
  if (this!=&A)
    {
      API::Workspace::operator=(A);
      Histogram1D::operator=(A);
    }
  return *this;
}

/// Destructor
Workspace1D::~Workspace1D()
{}

/** Returns the size of the workspace
 * \returns The number of items the workspace contains
 */
int Workspace1D::size() const
{
  return Histogram1D::size();
}

///get the size of each vector
int Workspace1D::blocksize() const
{
  int retVal = 1000000000;
  //if not empty
  if (size() > 0)
  {
    //set the reteurn value to the length of the first vector
    retVal = size();
  }
  return retVal; 
}

} // namespace DataObjects

} //NamespaceMantid

///\cond TEMPLATE
template DLLExport class Mantid::API::triple_iterator<Mantid::DataObjects::Workspace1D>;

template DLLExport class Mantid::API::WorkspaceProperty<Mantid::DataObjects::Workspace1D>;
///\endcond TEMPLATE
