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


#ifndef Octree_h
#define Octree_h


#include "OctreeImplementation.hpp"




namespace hxa7241_graphics
{


/**
 * Agent abstract base, for client use with Octree.<br/><br/>
 *
 * Client of Octree must define a concrete derivative of
 * OctreeAgent<ItemType>.<br/><br/>
 *
 * This is similar to a proxy: it is an intermediary for an Octree to query
 * its typeless subject items, when inserting or removing.<br/><br/>
 *
 * The overlap methods are to determine an item's relation to a cell or cells,
 * for insertion or removal. The parameters supply the bounds of the cell.
 * <br/><br/>
 *
 * Return value of getSubcellOverlaps is 8 bits, each bit is a bool
 * corresponding to a subcell, the high bit for subcell 7, the low bit for
 * subcell 0.<br/><br/>
 *
 * Subcell numbering:
 * <pre>
 *    y z       6 7
 *    |/   2 3  4 5
 *     -x  0 1
 * </pre>
 * in binary:
 * <pre>
 *    y z           110 111
 *    |/   010 011  100 101
 *     -x  000 001
 * </pre>
 *
 * @implementation
 * The ___V methods simply apply a type-cast to void*s and forward to their
 * abstract counterparts.<br/><br/>
 *
 * An Octree requires its contained items to provide positional info. But
 * requiring the item classes to implement an OctreeItem interface would
 * impose a direct interface change on every prospective item type, and enlarge
 * their instances with a vptr.<br/><br/>
 *
 * Instead, this agent transfers the Octree-related interface/implementation
 * away from the item type into a separate class. The Octree can now hold void
 * pointers to items and call the agent to query them indirectly.<br/><br/>
 */
template<class TYPE>
class OctreeAgent
   : public OctreeAgentV
{
/// standard object services ---------------------------------------------------
protected:
            OctreeAgent() {}
public:
   virtual ~OctreeAgent() {}
private:
            OctreeAgent( const OctreeAgent& );
   OctreeAgent& operator=( const OctreeAgent& );


/// void-to-type forwarders
public:
/// queries --------------------------------------------------------------------
   virtual bool  isOverlappingCellV ( const void*     pItem,
                                      const Vector3r& lowerCorner,
                                      const Vector3r& upperCorner )       const;
   virtual dword getSubcellOverlapsV( const void*     pItem,
                                      const Vector3r& lower,
                                      const Vector3r& middle,
                                      const Vector3r& upper )             const;


/// abstract interface
protected:
/// queries --------------------------------------------------------------------
   /**
    * Called by Octree to get relation of item to cell.
    */
   virtual bool  isOverlappingCell ( const TYPE&     item,
                                     const Vector3r& lowerCorner,
                                     const Vector3r& upperCorner )     const =0;
   /**
    * Called by Octree to get relation of item to subcell octants.<br/><br/>
    * Override to make a more efficent calculation (boundary testing can be
    * shared).
    * @return
    * 8 bits, each a bool corresponding to a subcell, the high bit for subcell
    * 7, the low bit for subcell 0.<br/><br/>
    * Subcell numbering:
    * <pre>
    *    y z       6 7
    *    |/   2 3  4 5
    *     -x  0 1
    * </pre>
    * in binary:
    * <pre>
    *    y z           110 111
    *    |/   010 011  100 101
    *     -x  000 001
    * </pre>
    */
   virtual dword getSubcellOverlaps( const TYPE&     item,
                                     const Vector3r& lowerCorner,
                                     const Vector3r& middlePoint,
                                     const Vector3r& upperCorner )        const;
};




/// void-to-type forwarders
template<class TYPE>
inline
bool OctreeAgent<TYPE>::isOverlappingCellV
(
   const void*     pItem,
   const Vector3r& lowerCorner,
   const Vector3r& upperCorner
) const
{
   bool is = false;

   // null check unnecessary because Octree interface disallows nulls
   //if( pItem )
   {
      is = isOverlappingCell( *reinterpret_cast<const TYPE*>( pItem ),
         lowerCorner, upperCorner );
   }

   return is;
}


template<class TYPE>
inline
dword OctreeAgent<TYPE>::getSubcellOverlapsV
(
   const void*     pItem,
   const Vector3r& lower,
   const Vector3r& middle,
   const Vector3r& upper
) const
{
   dword ov = ALL_OUTSIDE;

   // null check unnecessary because Octree interface disallows nulls
   //if( pItem )
   {
      ov = getSubcellOverlaps( *reinterpret_cast<const TYPE*>( pItem ),
         lower, middle, upper );
   }

   return ov;
}


/// default implementation
template<class TYPE>
dword OctreeAgent<TYPE>::getSubcellOverlaps
(
   const TYPE&     item,
   const Vector3r& lowerCorner,
   const Vector3r& middlePoint,
   const Vector3r& upperCorner
) const
{
   dword flags = ALL_OUTSIDE;

   const Vector3r* lowMidPoints[]  = { &lowerCorner, &middlePoint };
   const Vector3r* midHighPoints[] = { &middlePoint, &upperCorner };

   // step through each subcell
   for( dword i = 8;  i-- > 0; )
   {
      const Vector3r cellLowerCorner( lowMidPoints[ i       & 1]->getX(),
                                      lowMidPoints[(i >> 1) & 1]->getY(),
                                      lowMidPoints[(i >> 2) & 1]->getZ() );
      const Vector3r cellUpperCorner( midHighPoints[ i       & 1]->getX(),
                                      midHighPoints[(i >> 1) & 1]->getY(),
                                      midHighPoints[(i >> 2) & 1]->getZ() );
      // delegate to single-cell test
      flags |= static_cast<dword>(isOverlappingCell( item, cellLowerCorner,
         cellUpperCorner )) << i;
   }

   return flags;
}




/**
 * Visitor abstract base, for client use with Octree.<br/><br/>
 *
 * Client of Octree must define a concrete derivative of
 * OctreeVisitor<ItemType>.<br/><br/>
 *
 * This is a reversal of the Visitor pattern: it allows an operation to be
 * performed with the Octree, except the Octree is merely read from and it is
 * the visitor that is modified.<br/><br/>
 *
 * The visit methods are called by the tree nodes during the visit operation.
 * The parameters supply the cell and boundary info. The implementation can
 * call visit on the supplied cell.<br/><br/>
 *
 * The implementation of visitBranch needs to make the OctreeData to be given
 * in each call of visit.
 *
 * Subcell numbering:
 * <pre>
 *    y z       6 7
 *    |/   2 3  4 5
 *     -x  0 1
 * </pre>
 * in binary:
 * <pre>
 *    y z           110 111
 *    |/   010 011  100 101
 *     -x  000 001
 * </pre>
 *
 * @implementation
 * The ___V methods simply apply a type-cast to void*s and forward to their
 * abstract counterparts.<br/><br/>
 */
template<class TYPE>
class OctreeVisitor
   : public OctreeVisitorV
{
/// standard object services ---------------------------------------------------
protected:
            OctreeVisitor() {}
public:
   virtual ~OctreeVisitor() {}
private:
            OctreeVisitor( const OctreeVisitor& );
   OctreeVisitor& operator=( const OctreeVisitor& );


/// void-to-type forwarders
public:
/// commands -------------------------------------------------------------------
   virtual void  visitRootV  ( const OctreeCell* pRootCell,
                               const OctreeData& octreeData );
   virtual void  visitBranchV( const OctreeCell* subCells[8],
                               const OctreeData& octreeData );
   virtual void  visitLeafV  ( const Array<const void*>& items,
                               const OctreeData&         octreeData );


/// abstract interface
protected:
/// commands -------------------------------------------------------------------
   /**
    * Called by Octree when visit traversal is at the root.<br/><br/>
    * To continue deeper, implementation calls OctreeRoot::continueVisit(
    * pRootCell, octreeData, *this ). pRootCell can be null.<br/><br/>
    * @see OctreeData
    */
   virtual void  visitRoot  ( const OctreeCell* pRootCell,
                              const OctreeData& octreeData )                 =0;
   /**
    * Called by Octree when visit traversal is at a branch.<br/><br/>
    * To continue deeper, implementation calls OctreeBranch::continueVisit(
    * subCells, octreeData, subCellIndex, *this ) for any/all subCellIndex
    * values. subCells elements can be null.<br/><br/>
    * @see OctreeData
    */
   virtual void  visitBranch( const OctreeCell* subCells[8],
                              const OctreeData& octreeData )                 =0;
   /**
    * Called by Octree when visit traversal is at a leaf.<br/><br/>
    * @see OctreeData
    */
   virtual void  visitLeaf  ( const Array<const TYPE*>& items,
                              const OctreeData&         octreeData )         =0;
};




/// void-to-type forwarders
template<class TYPE>
inline
void OctreeVisitor<TYPE>::visitRootV
(
   const OctreeCell* pRootCell,
   const OctreeData& octreeData
)
{
   visitRoot( pRootCell, octreeData );
}


template<class TYPE>
inline
void OctreeVisitor<TYPE>::visitBranchV
(
   const OctreeCell* subCells[8],
   const OctreeData& octreeData
)
{
   visitBranch( subCells, octreeData );
}


template<class TYPE>
inline
void OctreeVisitor<TYPE>::visitLeafV
(
   const Array<const void*>& items,
   const OctreeData&         octreeData
)
{
   visitLeaf( reinterpret_cast<const Array<const TYPE*>&>( items ),
      octreeData );
}








/**
 * Octree based spatial index.<br/><br/>
 *
 * Client must define concrete derivatives of OctreeAgent<ItemType> and
 * OctreeVisitor<ItemType>.<br/><br/>
 *
 * maxItemCountPerCell is ignored where maxLevelCount or minCellSize is reached.
 * <br/><br/>
 *
 * The octree is cubical and axis aligned, partitions are axis aligned,
 * partitions divide in half, each level partitions the previous level in all
 * three axiss.<br/><br/>
 *
 * Storage is contracted or expanded as needed by item insertion and removal.
 * <br/><br/>
 *
 * (Occupies, very approximately, 20 bytes per point item. maybe...)<br/><br/>
 *
 * Octree is only an index: it points to client items, it does not manage
 * storage of items themselves.<br/><br/>
 *
 * @see OctreeAgent
 * @see OctreeVisitor
 *
 * @implementation
 * The octree structure follows the Composite pattern.<br/><br/>
 *
 * This template wrapper ensures the items indexed by the octree, and the agents
 * and visitors used when accessing them are, of matching types. All algorithmic
 * work is delegated to OctreeRoot and OctreeCell derivatives in
 * OctreeImplementation, which work with abstract base interfaces and void
 * pointers.<br/><br/>
 *
 * For the insertion and removal commands, the agent provides an interface for
 * the octree to query the typeless item, and for the visit query, the visitor
 * provides callbacks to read tree nodes for carrying out the visit operation.
 */
template<class TYPE>
class Octree
{
/// standard object services ---------------------------------------------------
public:
   /**
    * Constructs a particular format of octree.<br/><br/>
    * @parameters
    * * sizeOfCube is desired length along a side<br/>
    * * maxItemCountPerCell is desired max item pointers per leaf<br/>
    * * maxLevelCount is desired max depth of tree<br/>
    * * minCellSize is desired min size of cells<br/>
    */
            Octree( const Vector3r& positionOfLowerCorner,
                    real            sizeOfCube,
                    dword           maxItemCountPerCell,
                    dword           maxLevelCount,
                    real            minCellSize );

           ~Octree();
            Octree( const Octree& );
   /**
    * @exceptions
    * Can throw storage allocation exceptions. In such cases the octree is
    * unmodified.
    */
   Octree& operator=( const Octree& );


/// commands -------------------------------------------------------------------
   /**
    * Add pointer(s) to the item to the octree.<br/><br/>
    * (If an item has non-zero volume, it may have pointers in multiple
    * cells.)<br/><br/>
    * @return is the item inserted -- false if item not inside root bound
    * @exceptions
    * Can throw storage allocation exceptions. In such cases the octree remains
    * structurally ok, but the item will not be fully added, -- call this
    * method again or the remove method.
    * @see remove, OctreeAgent
    */
           bool  insert( const TYPE&              item,
                         const OctreeAgent<TYPE>& agent );
   /**
    * Removes pointer(s) to the item from the octree.<br/><br/>
    * (If an item has non-zero volume, it may have pointers in multiple
    * cells.)<br/><br/>
    * @return is the item removed -- false if item wasn't present
    * @exceptions
    * Can throw storage allocation exceptions. In such cases the octree remains
    * structurally ok, but the item will not be fully removed, -- call this
    * method again or the insert method.
    * @see insert, OctreeAgent
    */
           bool  remove( const TYPE&              item,
                         const OctreeAgent<TYPE>& agent );


/// queries --------------------------------------------------------------------
   /**
    * Execute a visit query operation.
    * @see OctreeVisitor
    */
           void  visit( OctreeVisitor<TYPE>& visitor )                    const;

   /**
    * Reports if the octree is empty.
    */
           bool  isEmpty()                                                const;
   /**
    * Provides stats on the octree.<br/><br/>
    * @parameters
    * * byteSize is size in bytes<br/>
    * * leafCount is number of leafs<br/>
    * * itemRefCount is total number of item pointers in all leafs<br/>
    * * maxDepth is deepest depth of tree<br/>
    */
           void  getInfo( dword& byteSize,
                          dword& leafCount,
                          dword& itemRefCount,
                          dword& maxDepth )                               const;

   /**
    * Gives the position supplied at construction.
    */
           const Vector3r& getPosition()                                  const;
   /**
    * Gives the size supplied at construction.
    */
           real            getSize()                                      const;
   /**
    * Gives the leaf pointer limit supplied at construction.
    */
           dword           getMaxItemCountPerCell()                       const;
   /**
    * Gives the depth limit supplied at construction.
    */
           dword           getMaxLevelCount()                             const;
   /**
    * Gives the size limit supplied at construction.
    */
           real            getMinCellSize()                               const;


/// fields ---------------------------------------------------------------------
private:
   OctreeRoot root_m;
};




/// templates ///

/// standard object services ---------------------------------------------------
template<class TYPE>
inline
Octree<TYPE>::Octree
(
   const Vector3r& position,
   const real      sizeOfCube,
   const dword     maxItemCountPerCell,
   const dword     maxLevelCount,
   const real      minCellSize
)
 : root_m( position, sizeOfCube, maxItemCountPerCell, maxLevelCount,
      minCellSize )
{
}


template<class TYPE>
inline
Octree<TYPE>::~Octree()
{
}


template<class TYPE>
inline
Octree<TYPE>::Octree
(
   const Octree& other
)
 : root_m( other.root_m )
{
}


template<class TYPE>
inline
Octree<TYPE>& Octree<TYPE>::operator=
(
   const Octree& other
)
{
   root_m = other.root_m;

   return *this;
}




/// commands -------------------------------------------------------------------
template<class TYPE>
inline
bool Octree<TYPE>::insert
(
   const TYPE&              item,
   const OctreeAgent<TYPE>& agent
)
{
   return root_m.insert( &item, agent );
}


template<class TYPE>
inline
bool Octree<TYPE>::remove
(
   const TYPE&              item,
   const OctreeAgent<TYPE>& agent
)
{
   return root_m.remove( &item, agent );
}




/// queries --------------------------------------------------------------------
template<class TYPE>
inline
void Octree<TYPE>::visit
(
   OctreeVisitor<TYPE>& visitor
) const
{
   root_m.visit( visitor );
}


template<class TYPE>
inline
bool Octree<TYPE>::isEmpty() const
{
   return root_m.isEmpty();
}


template<class TYPE>
inline
void Octree<TYPE>::getInfo
(
   dword& byteSize,
   dword& leafCount,
   dword& itemRefCount,
   dword& maxDepth
) const
{
   root_m.getInfo( sizeof(*this), byteSize, leafCount, itemRefCount, maxDepth );
}


template<class TYPE>
inline
const Vector3r& Octree<TYPE>::getPosition() const
{
   return root_m.getPosition();
}


template<class TYPE>
inline
real Octree<TYPE>::getSize() const
{
   return root_m.getSize();
}


template<class TYPE>
inline
dword Octree<TYPE>::getMaxItemCountPerCell() const
{
   return root_m.getMaxItemCountPerCell();
}


template<class TYPE>
inline
dword Octree<TYPE>::getMaxLevelCount() const
{
   return root_m.getMaxLevelCount();
}


template<class TYPE>
inline
real Octree<TYPE>::getMinCellSize() const
{
   return root_m.getMinCellSize();
}


}//namespace




#endif//Octree_h
