/*------------------------------------------------------------------------------

   Octree Component, version 2.1
   Copyright (c) 2004-2007,  Harrison Ainsworth / HXA7241.

   http://www.hxa7241.org/

------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------

Copyright (c) 2004-2007, Harrison Ainsworth / HXA7241.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.
* Redistributions in binary form must reproduce the above copyright notice, this
  list of conditions and the following disclaimer in the documentation and/or
  other materials provided with the distribution.
* The name of the author may not be used to endorse or promote products derived
  from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.

------------------------------------------------------------------------------*/


#include <iostream>

#include "OctreeStreamOut.hpp"


using namespace hxa7241_graphics;




/// other ----------------------------------------------------------------------
static std::ostream& operator<<( std::ostream&, const Vector3r& );

std::ostream& operator<<
(
   std::ostream&   out,
   const Vector3r& v
)
{
   return out << "(" << v.getX() << ", " << v.getY() << ", " << v.getZ() << ")";
}




/// standard object services ---------------------------------------------------
OctreeStreamOutImplementation::OctreeStreamOutImplementation
(
   std::ostream& outStream,
   const bool    isLongFormat
)
 : pOutStream_m  ( &outStream )
 , isLongFormat_m( isLongFormat )
 , pCurrentCell_m( 0 )
 , indent_m      ( 0 )
{
}


OctreeStreamOutImplementation::~OctreeStreamOutImplementation()
{
}


OctreeStreamOutImplementation::OctreeStreamOutImplementation
(
   const OctreeStreamOutImplementation& other
)
 : pOutStream_m  ( other.pOutStream_m )
 , isLongFormat_m( other.isLongFormat_m )
 , pCurrentCell_m( 0 )
 , indent_m      ( 0 )
{
}


OctreeStreamOutImplementation& OctreeStreamOutImplementation::operator=
(
   const OctreeStreamOutImplementation& other
)
{
   if( &other != this )
   {
      pOutStream_m   = other.pOutStream_m;
      isLongFormat_m = other.isLongFormat_m;

      pCurrentCell_m = 0;
      indent_m       = 0;
   }

   return *this;
}




/// commands -------------------------------------------------------------------
void OctreeStreamOutImplementation::visitRoot
(
   const OctreeCell* pRootCell,
   const OctreeData& octreeData,
   OctreeVisitorV&   visitor
)
{
   dword byteSize  = 0;
   dword leafCount = 0;
   dword itemCount = 0;
   dword maxDepth  = 0;

   if( pRootCell )
   {
      pRootCell->getInfo( byteSize, leafCount, itemCount, maxDepth );
   }

   byteSize += sizeof(OctreeRoot);

   const OctreeDimensions& dimensions = octreeData.getDimensions();
   *pOutStream_m << "\nOctree " <<
      "\nposition: "        << dimensions.getPosition()            <<
      "  size: "            << dimensions.getSize()                <<
      "  maxItemsPerCell: " << dimensions.getMaxItemCountPerCell() <<
      "  maxLevelCount: "   << dimensions.getMaxLevelCount()       <<
      "\n(bytes: "          << byteSize                            <<
      ", leafs: "           << leafCount                           <<
      ", items: "           << itemCount                           <<
      ", depth: "           << maxDepth                            <<
      ")\n{ ";

   if( pRootCell )
   {
      pCurrentCell_m = pRootCell;
      indent_m       = 1;

      // continue visit traversal
      OctreeRoot::continueVisit( pRootCell, octreeData, visitor );
   }
   else
   {
      *pOutStream_m << "empty ";
   }

   *pOutStream_m << "}\n";
}


void OctreeStreamOutImplementation::visitBranch
(
   const OctreeCell* subCells[8],
   const OctreeData& octreeData,
   OctreeVisitorV&   visitor
)
{
   writeDataAndInfo( *pCurrentCell_m, octreeData, *pOutStream_m );

   // step through subcells
   for( int i = 0;  i < 8;  ++i )
   {
      *pOutStream_m << "\n";
      for( dword j = indent_m;  j-- > 0; )
      {
         *pOutStream_m << "\t";
      }
      *pOutStream_m << "[" << i << " ";

      const OctreeCell*const pSubCell = subCells[i];
      if( pSubCell )
      {
         pCurrentCell_m = pSubCell;
         indent_m      += 1;

         // continue visit traversal
         OctreeBranch::continueVisit( subCells, octreeData, i, visitor );

         indent_m -= 1;
      }
      else
      {
         *pOutStream_m << "empty ";
      }

      *pOutStream_m << "] ";
   }

   *pOutStream_m << "\n";
   for( dword i = indent_m - 1;  i-- > 0; )
   {
      *pOutStream_m << "\t";
   }
}


void OctreeStreamOutImplementation::visitLeafBefore
(
   const OctreeData& octreeData
)
{
   writeDataAndInfo( *pCurrentCell_m, octreeData, *pOutStream_m );

   indentNewline( 0 );

   if( isLongFormat_m )
   {
      *pOutStream_m << "<";
   }
}


void OctreeStreamOutImplementation::visitLeafAfter()
{
   if( isLongFormat_m )
   {
      indentNewline( 0 );
      *pOutStream_m << ">";
      indentNewline( -1 );
   }
}


bool OctreeStreamOutImplementation::isLongFormat() const
{
   return isLongFormat_m;
}


std::ostream& OctreeStreamOutImplementation::getOutStream() const
{
   return *pOutStream_m;
}


std::ostream& OctreeStreamOutImplementation::indentNewline
(
   const dword extraIndent
) const
{
   *pOutStream_m << "\n";
   for( dword i = indent_m + extraIndent;  i-- > 0; )
   {
      *pOutStream_m << "\t";
   }

   return *pOutStream_m;
}




/// implementation -------------------------------------------------------------

void OctreeStreamOutImplementation::writeDataAndInfo
(
   const OctreeCell& octreeCell,
   const OctreeData& octreeData,
   std::ostream&     outStream
)
{
   // do octreeData
   outStream << "level(" <<
      octreeData.getLevel()                  << ") lo"  <<
      octreeData.getBound().getLowerCorner() << " hi"   <<
      octreeData.getBound().getUpperCorner() << " ctr"  <<
      octreeData.getBound().getCenter()      << " sph(" <<
      octreeData.getBound().getRadius()      << ")  ";

   // do info
   dword byteSize  = 0;
   dword leafCount = 0;
   dword itemCount = 0;
   dword depthLargest  = 0;
   octreeCell.getInfo( byteSize, leafCount, itemCount, depthLargest );

   outStream <<
      "(bytes: "  << byteSize     <<
      ", leafs: " << leafCount    <<
      ", items: " << itemCount    <<
      ", depth: " << depthLargest << ")  ";
}
