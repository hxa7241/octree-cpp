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


#ifndef OctreeAuxiliary_h
#define OctreeAuxiliary_h


#include "Array.hpp"
#include "Vector3r.hpp"




namespace hxa7241_graphics
{
   using namespace hxa7241;
   using hxa7241_general::Array;
   class OctreeCell;


/**
 * Global octree data -- one instance for whole octree.<br/><br/>
 *
 * Constant.
 *
 * @invariants
 * size_m >= 0<br/>
 * maxItemsPerCell_m >= 1<br/>
 * maxLevel_m >= 0 and <= MAX_LEVEL<br/>
 * minSize_m >= MIN_SIZE and <= size_m<br/>
 */
class OctreeDimensions
{
/// standard object services ---------------------------------------------------
public:
            OctreeDimensions( const Vector3r& positionOfLowerCorner,
                              real            size,
                              dword           maxItemCountPerCell,
                              dword           maxLevelCount,
                              real            minCellSize );

           ~OctreeDimensions();
            OctreeDimensions( const OctreeDimensions& );
   OctreeDimensions& operator=( const OctreeDimensions& );


/// queries --------------------------------------------------------------------
           const Vector3r& getPosition()                                  const;
           real            getSize()                                      const;
           dword           getMaxItemCountPerCell()                       const;
           dword           getMaxLevelCount()                             const;
           real            getMinCellSize()                               const;

           bool            isSubdivide( dword itemCount,
                                        dword level,
                                        real  size )                      const;


/// fields ---------------------------------------------------------------------
private:
   Vector3r positionOfLowerCorner_m;
   real     size_m;
   dword    maxItemsPerCell_m;
   dword    maxLevel_m;
   real     minSize_m;

   static const dword MAX_LEVEL;
   static const real  MIN_SIZE;
};




/**
 * Geometric data for the bound of an octree cell.<br/><br/>
 *
 * Constant.<br/><br/>
 *
 * Radius is that of the circumsphere.<br/><br/>
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
 */
class OctreeBound
{
/// standard object services ---------------------------------------------------
public:
            OctreeBound();
            OctreeBound( const Vector3r& positionOfLowerCorner,
                         real            size );
            OctreeBound( const OctreeBound& parentCellBound,
                         dword              subCellIndex );

           ~OctreeBound();
            OctreeBound( const OctreeBound& );
   OctreeBound& operator=( const OctreeBound& );


/// queries --------------------------------------------------------------------
           const Vector3r& getLowerCorner()                               const;
           const Vector3r& getUpperCorner()                               const;
           const Vector3r& getCenter()                                    const;
           real            getRadius()                                    const;
           real            getSize()                                      const;


/// fields ---------------------------------------------------------------------
private:
   Vector3r positionOfLowerCorner_m;
   Vector3r positionOfUpperCorner_m;
   Vector3r center_m;
   real     circumSphereRadius_m;
};




/**
 * Octree cell data during traversal.<br/><br/>
 *
 * Constant.<br/><br/>
 *
 * To be made during each level of tree descent, so storage is avoided, except
 * to hold one at the root.<br/><br/>
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
 * @see OctreeBound
 * @see OctreeDimensions
 */
class OctreeData
{
/// standard object services ---------------------------------------------------
public:
   explicit OctreeData( const OctreeDimensions& dimensions );
            OctreeData( const OctreeData& parentCellData,
                        dword             subCellIndex );
            OctreeData( const OctreeData&,
                        const OctreeDimensions& );

           ~OctreeData();
            OctreeData( const OctreeData& );
   OctreeData& operator=( const OctreeData& );


/// queries --------------------------------------------------------------------
           const OctreeBound&      getBound()                             const;
           dword                   getLevel()                             const;
           const OctreeDimensions& getDimensions()                        const;

           bool  isSubdivide( dword itemCount )                           const;


/// fields ---------------------------------------------------------------------
private:
   // local to cell
   OctreeBound bound_m;
   dword       level_m;

   // global for octree
   const OctreeDimensions* pDimensions_m;
};








/**
 * Agent abstract base, for Octree implementation use.<br/><br/>
 *
 * Return value of getSubcellOverlapsV is 8 bits, each bit is a bool
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
 * @see OctreeCell
 * @see OctreeBranch
 * @see OctreeLeaf
 */
class OctreeAgentV
{
/// standard object services ---------------------------------------------------
protected:
            OctreeAgentV() {}
public:
   virtual ~OctreeAgentV() {}
private:
            OctreeAgentV( const OctreeAgentV& );
   OctreeAgentV& operator=( const OctreeAgentV& );
public:


/// queries --------------------------------------------------------------------
   virtual bool  isOverlappingCellV ( const void*     pItem,
                                      const Vector3r& lowerCorner,
                                      const Vector3r& upperCorner )    const =0;
   virtual dword getSubcellOverlapsV( const void*     pItem,
                                      const Vector3r& lower,
                                      const Vector3r& middle,
                                      const Vector3r& upper )          const =0;


/// constants ------------------------------------------------------------------
   static const dword ALL_INSIDE  = 0x0000FFFF;
   static const dword ALL_OUTSIDE = 0x00000000;
};




/**
 * Visitor abstract base, for Octree implementation use.<br/><br/>
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
 * @see OctreeCell
 * @see OctreeBranch
 * @see OctreeLeaf
 */
class OctreeVisitorV
{
/// standard object services ---------------------------------------------------
protected:
            OctreeVisitorV() {}
public:
   virtual ~OctreeVisitorV() {}
private:
            OctreeVisitorV( const OctreeVisitorV& );
   OctreeVisitorV& operator=( const OctreeVisitorV& );
public:


/// commands -------------------------------------------------------------------
   virtual void  visitRootV  ( const OctreeCell* pRootCell,
                               const OctreeData& octreeData )                =0;
   virtual void  visitBranchV( const OctreeCell* subCells[8],
                               const OctreeData& octreeData )                =0;
   virtual void  visitLeafV  ( const Array<const void*>& items,
                               const OctreeData&         octreeData )        =0;
};








/// inlines ///

/// OctreeDimensions -----------------------------------------------------------
inline
const Vector3r& OctreeDimensions::getPosition() const
{
   return positionOfLowerCorner_m;
}


inline
real OctreeDimensions::getSize() const
{
   return size_m;
}


inline
dword OctreeDimensions::getMaxItemCountPerCell() const
{
   return maxItemsPerCell_m;
}


inline
dword OctreeDimensions::getMaxLevelCount() const
{
   return maxLevel_m + 1;
}


inline
real OctreeDimensions::getMinCellSize() const
{
   return minSize_m;
}




/// OctreeBound ----------------------------------------------------------------
inline
const Vector3r& OctreeBound::getLowerCorner() const
{
   return positionOfLowerCorner_m;
}


inline
const Vector3r& OctreeBound::getUpperCorner() const
{
   return positionOfUpperCorner_m;
}


inline
const Vector3r& OctreeBound::getCenter() const
{
   return center_m;
}


inline
real OctreeBound::getRadius() const
{
   return circumSphereRadius_m;
}


inline
real OctreeBound::getSize() const
{
   return positionOfUpperCorner_m.getX() - positionOfLowerCorner_m.getX();
}




/// OctreeData -----------------------------------------------------------------
inline
const OctreeBound& OctreeData::getBound() const
{
   return bound_m;
}


inline
dword OctreeData::getLevel() const
{
   return level_m;
}


inline
const OctreeDimensions& OctreeData::getDimensions() const
{
   return *pDimensions_m;
}


inline
bool OctreeData::isSubdivide
(
   const dword itemCount
) const
{
   return pDimensions_m->isSubdivide( itemCount, level_m, bound_m.getSize() );
}


}//namespace




#endif//OctreeAuxiliary_h
