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


#ifndef OctreeImplementation_h
#define OctreeImplementation_h


#include "OctreeAuxiliary.hpp"




namespace hxa7241_graphics
{


/**
 * Implementation class for the Octree template.
 *
 * @invariants
 * pRootCell_m can be null, or point to an OctreeCell instance.<br/><br/>
 *
 * At construction, pRootCell_m is set to a legal value.<br/>
 * At destruction, pRootCell_m is deleted.<br/>
 * Whenever pRootCell_m is modified, it must be deleted then set to a legal
 * value.<br/>
 * A legal value is: either 0, or the value from invocation of 'new'.
 */
class OctreeRoot
{
/// standard object services ---------------------------------------------------
public:
            OctreeRoot( const Vector3r& position,
                        real            sizeOfCube,
                        dword           maxItemsPerCell,
                        dword           maxLevelCount,
                        real            minCellSize );

           ~OctreeRoot();
            OctreeRoot( const OctreeRoot& );
   OctreeRoot& operator=( const OctreeRoot& );


/// commands -------------------------------------------------------------------
           bool  insert( const void*         pItem,
                         const OctreeAgentV& agent );
           bool  remove( const void*         pItem,
                         const OctreeAgentV& agent );


/// queries --------------------------------------------------------------------
           void  visit( OctreeVisitorV& visitor )                         const;

           bool  isEmpty()                                                const;
           void  getInfo( dword  rootWrapperByteSize,
                          dword& byteSize,
                          dword& leafCount,
                          dword& itemCount,
                          dword& maxDepth )                               const;

           const Vector3r& getPosition()                                  const;
           real            getSize()                                      const;
           dword           getMaxItemCountPerCell()                       const;
           dword           getMaxLevelCount()                             const;
           real            getMinCellSize()                               const;


/// statics --------------------------------------------------------------------
   static  void  continueVisit( const OctreeCell* pRootCell,
                                const OctreeData& octreeData,
                                OctreeVisitorV&   visitor );


/// fields ---------------------------------------------------------------------
private:
   OctreeDimensions dimensions_m;
   OctreeCell*      pRootCell_m;
};




/**
 * Abstract base for Composite types, for implementing Octree nodes.
 *
 * @implementation
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
class OctreeCell
{
/// standard object services ---------------------------------------------------
protected:
            OctreeCell() {}
public:
   virtual ~OctreeCell() {}
private:
            OctreeCell( const OctreeCell& );
   OctreeCell& operator=( const OctreeCell& );
public:


/// commands -------------------------------------------------------------------
   virtual void  insert( const OctreeData&   thisData,
                         OctreeCell*&        pThis,
                         const void*         pItem,
                         const OctreeAgentV& agent )                         =0;
   virtual bool  remove( OctreeCell*&        pThis,
                         const void*         pItem,
                         const dword         maxItemsPerCell,
                         dword&              itemCount )                     =0;


/// queries --------------------------------------------------------------------
   virtual void  visit( const OctreeData& thisData,
                        OctreeVisitorV&   visitor )                    const =0;

   virtual OctreeCell* clone()                                         const =0;

   virtual void  getInfo( dword& byteSize,
                          dword& leafCount,
                          dword& itemCount,
                          dword& maxDepth )                            const =0;


/// statics --------------------------------------------------------------------
   static  OctreeCell* cloneNonZero( const OctreeCell* );
};




/**
 * Inner node implementation of an octree cell.<br/><br/>
 *
 * Stores pointers to eight (at most) child cells.
 *
 * @invariants
 * subCells_m elements can be null, or point to an OctreeCell instance.
 */
class OctreeBranch
   : public OctreeCell
{
/// standard object services ---------------------------------------------------
public:
            OctreeBranch();
            OctreeBranch( const OctreeData&         thisData,
                          const Array<const void*>& items,
                          const void* const         pItem,
                          const OctreeAgentV&       agent );

   virtual ~OctreeBranch();
            OctreeBranch( const OctreeBranch& );
   OctreeBranch& operator=( const OctreeBranch& );


/// commands -------------------------------------------------------------------
   virtual void  insert( const OctreeData&   thisData,
                         OctreeCell*&        pThis,
                         const void*         pItem,
                         const OctreeAgentV& agent );
   virtual bool  remove( OctreeCell*&        pThis,
                         const void*         pItem,
                         const dword         maxItemsPerCell,
                         dword&              itemCount );


/// queries --------------------------------------------------------------------
   virtual void  visit( const OctreeData& thisData,
                        OctreeVisitorV&   visitor )                       const;

   virtual OctreeCell* clone()                                            const;

   virtual void  getInfo( dword& byteSize,
                          dword& leafCount,
                          dword& itemCount,
                          dword& maxDepth )                               const;


/// statics --------------------------------------------------------------------
   static  void  continueVisit( const OctreeCell* subCells[8],
                                const OctreeData& octreeData,
                                dword             subCellIndex,
                                OctreeVisitorV&   visitor );


/// implementation -------------------------------------------------------------
protected:
   virtual void  zeroSubCells();


/// fields ---------------------------------------------------------------------
private:
   OctreeCell* subCells_m[ 8 ];
};




/**
 * Outer node implementation of an octree cell.<br/><br/>
 *
 * Stores pointers to items.
 */
class OctreeLeaf
   : public OctreeCell
{
/// standard object services ---------------------------------------------------
public:
            OctreeLeaf();
            OctreeLeaf( const OctreeLeaf*const leafs[8] );
private:
   explicit OctreeLeaf( const void* pItem );

public:
   virtual ~OctreeLeaf();
            OctreeLeaf( const OctreeLeaf& );
   OctreeLeaf& operator=( const OctreeLeaf& );


/// commands -------------------------------------------------------------------
   virtual void  insert( const OctreeData&   thisData,
                         OctreeCell*&        pThis,
                         const void*         pItem,
                         const OctreeAgentV& agent );
   virtual bool  remove( OctreeCell*&        pThis,
                         const void*         pItem,
                         const dword         maxItemsPerCell,
                         dword&              itemCount );


/// queries --------------------------------------------------------------------
   virtual void  visit( const OctreeData& thisData,
                        OctreeVisitorV&   visitor )                       const;

   virtual OctreeCell* clone()                                            const;

   virtual void  getInfo( dword& byteSize,
                          dword& leafCount,
                          dword& itemCount,
                          dword& maxDepth )                               const;


/// statics --------------------------------------------------------------------
   static  void  insertMaybeCreate( const OctreeData&   cellData,
                                    OctreeCell*&        pCell,
                                    const void*         pItem,
                                    const OctreeAgentV& agent );


/// fields ---------------------------------------------------------------------
private:
   Array<const void*> items_m;
};


}//namespace




#endif//OctreeImplementation_h
