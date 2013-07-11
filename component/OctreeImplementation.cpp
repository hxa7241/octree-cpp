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


#include "OctreeImplementation.hpp"


using namespace hxa7241_graphics;




/// OctreeRoot /////////////////////////////////////////////////////////////////


/// standard object services ---------------------------------------------------
OctreeRoot::OctreeRoot
(
   const Vector3r& position,
   const real      sizeOfCube,
   const dword     maxItemsPerCell,
   const dword     maxLevelCount,
   const real      minCellSize
)
 : dimensions_m( position, sizeOfCube, maxItemsPerCell, maxLevelCount,
      minCellSize )
 , pRootCell_m ( 0 )
{
}


OctreeRoot::~OctreeRoot()
{
   delete pRootCell_m;
}


OctreeRoot::OctreeRoot
(
   const OctreeRoot& other
)
 : dimensions_m( other.dimensions_m )
 , pRootCell_m ( OctreeCell::cloneNonZero( other.pRootCell_m ) )
{
}


OctreeRoot& OctreeRoot::operator=
(
   const OctreeRoot& other
)
{
   if( &other != this )
   {
      // make new data before deleting old
      OctreeCell* pRootCell = OctreeCell::cloneNonZero( other.pRootCell_m );
      delete pRootCell_m;
      pRootCell_m = pRootCell;

      dimensions_m = other.dimensions_m;
   }

   return *this;
}




/// commands -------------------------------------------------------------------
bool OctreeRoot::insert
(
   const void* const   pItem,
   const OctreeAgentV& agent
)
{
   bool isInserted = false;

   const OctreeData data( dimensions_m );

   // check if item overlaps root cell
   if( agent.isOverlappingCellV( pItem, data.getBound().getLowerCorner(),
      data.getBound().getUpperCorner() ) )
   {
      OctreeLeaf::insertMaybeCreate( data, pRootCell_m, pItem, agent );

      isInserted = true;
   }

   return isInserted;
}


bool OctreeRoot::remove
(
   const void* const   pItem,
   const OctreeAgentV& //agent
)
{
   bool isRemoved = false;

   if( pRootCell_m )
   {
      dword unusedBranchItemCount = 0;
      isRemoved = pRootCell_m->remove( pRootCell_m, pItem,
         dimensions_m.getMaxItemCountPerCell(), unusedBranchItemCount );
   }

   return isRemoved;
}




/// queries --------------------------------------------------------------------
void OctreeRoot::visit
(
   OctreeVisitorV& visitor
) const
{
   const OctreeData data( dimensions_m );

   visitor.visitRootV( pRootCell_m, data );
}


bool OctreeRoot::isEmpty() const
{
   return !pRootCell_m;
}


void OctreeRoot::getInfo
(
   const dword rootWrapperByteSize,
   dword&      byteSize,
   dword&      leafCount,
   dword&      itemCount,
   dword&      maxDepth
) const
{
   byteSize  = 0;
   leafCount = 0;
   itemCount = 0;
   maxDepth  = 0;

   if( pRootCell_m )
   {
      pRootCell_m->getInfo( byteSize, leafCount, itemCount, maxDepth );
   }

   byteSize += rootWrapperByteSize;
}


const Vector3r& OctreeRoot::getPosition() const
{
   return dimensions_m.getPosition();
}


real OctreeRoot::getSize() const
{
   return dimensions_m.getSize();
}


dword OctreeRoot::getMaxItemCountPerCell() const
{
   return dimensions_m.getMaxItemCountPerCell();
}


dword OctreeRoot::getMaxLevelCount() const
{
   return dimensions_m.getMaxLevelCount();
}


real OctreeRoot::getMinCellSize() const
{
   return dimensions_m.getMinCellSize();
}




/// statics --------------------------------------------------------------------
void OctreeRoot::continueVisit
(
   const OctreeCell* pRootCell,
   const OctreeData& octreeData,
   OctreeVisitorV&   visitor
)
{
   if( pRootCell )
   {
      pRootCell->visit( octreeData, visitor );
   }
}








/// OctreeCell /////////////////////////////////////////////////////////////////


/// statics --------------------------------------------------------------------
OctreeCell* OctreeCell::cloneNonZero
(
   const OctreeCell* pOriginal
)
{
   return pOriginal ? pOriginal->clone() : 0;
}








/// OctreeBranch ///////////////////////////////////////////////////////////////


/// standard object services ---------------------------------------------------
OctreeBranch::OctreeBranch()
{
   OctreeBranch::zeroSubCells();
}


OctreeBranch::OctreeBranch
(
   const OctreeData&         thisData,
   const Array<const void*>& items,
   const void* const         pItem,
   const OctreeAgentV&       agent
)
{
   OctreeBranch::zeroSubCells();

   try
   {
      OctreeCell* pNotUsed = 0;

      // insert items
      for( int j = items.getLength();  j-- > 0; )
      {
         OctreeBranch::insert( thisData, pNotUsed, items[j], agent );
      }

      // insert last item
      OctreeBranch::insert( thisData, pNotUsed, pItem, agent );
   }
   catch( ... )
   {
      // delete any allocated cells
      this->~OctreeBranch();

      throw;
   }
}


OctreeBranch::~OctreeBranch()
{
   for( int i = 8;  i-- > 0; )
   {
      delete subCells_m[i];
   }
}


OctreeBranch::OctreeBranch
(
   const OctreeBranch& other
)
{
   OctreeBranch::zeroSubCells();

   try
   {
      for( int i = 8;  i-- > 0; )
      {
         subCells_m[i] = OctreeCell::cloneNonZero( other.subCells_m[i] );
      }
   }
   catch( ... )
   {
      // delete any allocated cells
      this->~OctreeBranch();

      throw;
   }
}


OctreeBranch& OctreeBranch::operator=
(
   const OctreeBranch& other
)
{
   if( &other != this )
   {
      OctreeBranch copy( other );

      // 'move' subCells copies to this (leaving copy empty)
      for( int i = 8;  i-- > 0; )
      {
         delete subCells_m[i];
         subCells_m[i] = copy.subCells_m[i];

         copy.subCells_m[i] = 0;
      }
   }

   return *this;
}




/// commands -------------------------------------------------------------------
void OctreeBranch::insert
(
   const OctreeData&   thisData,
   OctreeCell*&        ,//pThis,
   const void* const   pItem,
   const OctreeAgentV& agent
)
{
   // get subcell-item overlaps flags
   const OctreeBound& bound    = thisData.getBound();
   const dword        overlaps = agent.getSubcellOverlapsV( pItem,
      bound.getLowerCorner(), bound.getCenter(), bound.getUpperCorner() );

   // loop through sub cells
   for( int i = 8;  i-- > 0; )
   {
      // check if sub cell is overlapped by item
      if( (overlaps >> i) & 1 )
      {
         // make sub cell data
         const OctreeData subCellData( thisData, i );

         // add item to sub cell
         OctreeLeaf::insertMaybeCreate( subCellData, subCells_m[i],
            pItem, agent );
      }
   }
}


bool OctreeBranch::remove
(
   OctreeCell*&      pThis,
   const void* const pItem,
   const dword       maxItemsPerCell,
   dword&            itemCount
)
{
   bool  isRemoved       = false;
   dword branchItemCount = 0;

   // loop through sub cells
   for( int i = 8;  i-- > 0; )
   {
      // remove item from non-null sub cell
      OctreeCell*& pSubCell = subCells_m[i];
      if( pSubCell )
      {
         isRemoved |= pSubCell->remove( pSubCell, pItem, maxItemsPerCell,
            branchItemCount );
      }
   }

   itemCount += branchItemCount;

   // decide whether to collapse this branch
   if( branchItemCount > 0 )
   {
      // collapse to leaf
      if( branchItemCount <= maxItemsPerCell )
      {
         // all subcells *will* be leafs!
         // because:
         // a) if a branch has below it less item refs than the threshold,
         //    it collapses to a leaf (this function!)
         // b) the total of item refs below this branch in the tree is less
         //    than the threshold
         // c) therefore the total of item refs in any branch below this
         //    cell will be less than the threshold
         // d) branchs below this cell will be collapsed before this branch
         //    (because the recursive 'remove' call is before the
         //    collapsing code)
         // so: if this branch will collapse to a leaf, then all its sub
         // branchs (direct and indirect) will collapse to leafs, and that
         // will happen before this branch.
         OctreeCell*const pLeaf = new OctreeLeaf(
            reinterpret_cast<OctreeLeaf**>( subCells_m ) );

         delete pThis;
         pThis = pLeaf;
      }
   }
   else
   {
      // delete this branch
      delete pThis;
      pThis = 0;
   }

   return isRemoved;
}




/// queries --------------------------------------------------------------------
void OctreeBranch::visit
(
   const OctreeData& thisData,
   OctreeVisitorV&   visitor
) const
{
   visitor.visitBranchV( const_cast<const OctreeCell**>(subCells_m), thisData );
}


OctreeCell* OctreeBranch::clone() const
{
   return new OctreeBranch( *this );
}


void OctreeBranch::getInfo
(
   dword& byteSize,
   dword& leafCount,
   dword& itemCount,
   dword& maxDepth
) const
{
   byteSize += sizeof(*this);

   const dword thisDepth = maxDepth + 1;

   for( int i = 8;  i-- > 0; )
   {
      const OctreeCell*const pSubCell = subCells_m[i];
      if( pSubCell )
      {
         dword depth = thisDepth;
         pSubCell->getInfo( byteSize, leafCount, itemCount, depth );

         if( maxDepth < depth )
         {
            maxDepth = depth;
         }
      }
   }
}




/// statics --------------------------------------------------------------------
void OctreeBranch::continueVisit
(
   const OctreeCell* subCells[8],
   const OctreeData& octreeData,
   const dword       subCellIndex,
   OctreeVisitorV&   visitor
)
{
   const OctreeCell*const pSubCell = subCells[ subCellIndex & 0x07 ];
   if( pSubCell )
   {
      const OctreeData subCellData( octreeData, subCellIndex );
      pSubCell->visit( subCellData, visitor );
   }
}




/// implementation -------------------------------------------------------------
void OctreeBranch::zeroSubCells()
{
   for( int i = 8;  i-- > 0; )
   {
      subCells_m[i] = 0;
   }
}








/// OctreeLeaf /////////////////////////////////////////////////////////////////


/// standard object services ---------------------------------------------------
OctreeLeaf::OctreeLeaf()
 : items_m()
{
}


OctreeLeaf::OctreeLeaf
(
   const void* pItem
)
 : items_m()
{
   items_m.append( pItem );
}


OctreeLeaf::OctreeLeaf
(
   const OctreeLeaf*const leafs[8]
)
 : items_m()
{
   // sum all items lengths
   dword totalLength = 0;
   for( int i = 8;  i-- > 0; )
   {
      const OctreeLeaf*const pLeaf = leafs[i];
      if( pLeaf )
      {
         totalLength += pLeaf->items_m.getLength();
      }
   }

   // prepare items array to hold all other items
   items_m.setLength( totalLength );

   // copy items arrays
   const void** pElement = items_m.getStorage();
   for( int i = 0;  i < 8;  ++i )
   {
      const OctreeLeaf*const pLeaf = leafs[i];
      if( pLeaf )
      {
         const void* const* pOtherElement = pLeaf->items_m.getStorage();
         const void* const* pOtherEnd     = pOtherElement +
            pLeaf->items_m.getLength();
         for( ;  pOtherElement < pOtherEnd;  ++pOtherElement, ++pElement )
         {
            *pElement = *pOtherElement;
         }
      }
   }
}


OctreeLeaf::~OctreeLeaf()
{
}


OctreeLeaf::OctreeLeaf
(
   const OctreeLeaf& other
)
 : items_m( other.items_m )
{
}


OctreeLeaf& OctreeLeaf::operator=
(
   const OctreeLeaf& other
)
{
   items_m = other.items_m;

   return *this;
}




/// commands -------------------------------------------------------------------
void OctreeLeaf::insert
(
   const OctreeData&   thisData,
   OctreeCell*&        pThis,
   const void* const   pItem,
   const OctreeAgentV& agent
)
{
   // check if item already present
   bool isAlreadyPresent = false;
   for( int i = items_m.getLength();  (i-- > 0) & !isAlreadyPresent; )
   {
      isAlreadyPresent |= (pItem == items_m[i]);
   }

   // only insert if item not already present
   if( !isAlreadyPresent )
   {
      // check if leaf should be subdivided
      if( !thisData.isSubdivide( items_m.getLength() + 1 ) )
      {
         // append item to collection
         items_m.append( pItem );
      }
      else
      {
         // subdivide by making branch and adding items to it
         OctreeCell*const pBranch = new OctreeBranch( thisData, items_m, pItem,
            agent );

         // replace this with branch
         delete pThis;
         pThis = pBranch;
      }
   }
}


bool OctreeLeaf::remove
(
   OctreeCell*&      pThis,
   const void* const pItem,
   const dword       ,//maxItemsPerCell,
   dword&            itemCount
)
{
   bool isRemoved = false;

   // loop through items
   for( int i = 0;  i < items_m.getLength(); )
   {
      // check if item is present
      if( items_m[i] == pItem )
      {
         // remove item
         items_m.remove( i );
         isRemoved = true;
      }
      else
      {
         ++i;
      }
   }

   itemCount += items_m.getLength();

   // check if leaf is now empty
   if( items_m.isEmpty() )
   {
      // remove this leaf
      delete pThis;
      pThis = 0;
   }

   return isRemoved;
}




/// queries --------------------------------------------------------------------
void OctreeLeaf::visit
(
   const OctreeData& thisData,
   OctreeVisitorV&   visitor
) const
{
   visitor.visitLeafV( items_m, thisData );
}


OctreeCell* OctreeLeaf::clone() const
{
   return new OctreeLeaf( *this );
}


void OctreeLeaf::getInfo
(
   dword& byteSize,
   dword& leafCount,
   dword& itemCount,
   dword& maxDepth
) const
{
   byteSize  += sizeof(*this) + (items_m.getLength() * sizeof(void*));
   ++leafCount;
   itemCount += items_m.getLength();
   ++maxDepth;
}




/// statics --------------------------------------------------------------------
void OctreeLeaf::insertMaybeCreate
(
   const OctreeData&   cellData,
   OctreeCell*&        pCell,
   const void* const   pItem,
   const OctreeAgentV& agent
)
{
   // check cell exists
   if( !pCell )
   {
      // make leaf, adding item
      OctreeCell*const pLeaf = new OctreeLeaf( pItem );

      // replace cell with leaf
      delete pCell;
      pCell = pLeaf;
   }
   else
   {
      // forward to existing cell
      pCell->insert( cellData, pCell, pItem, agent );
   }
}
