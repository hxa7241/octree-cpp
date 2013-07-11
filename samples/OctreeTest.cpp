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


#include <vector>
#include <set>
#include <utility>
#include <algorithm>
#include <memory>
#include <iostream>
#include <sstream>
#include <time.h>

#include "Octree.hpp"

#include "OctreeStreamOut.hpp"
#include "OctreeTest.hpp"


using namespace hxa7241_graphics;




std::ostream& operator<<( std::ostream&, const Vector3r& );

std::ostream& operator<<
(
   std::ostream&   out,
   const Vector3r& v3f
)
{
   out << "("  << v3f.getX() << ", " << v3f.getY() << ", " << v3f.getZ() << ")";

   return out;
}




/// OctreeItemTest /////////////////////////////////////////////////////////////

class OctreeItemTest
{
/// standard object services ---------------------------------------------------
public:
            OctreeItemTest( const Vector3r& position  = Vector3r::ZERO(),
                            const Vector3r& dimension = Vector3r::ZERO(),
                            dword           payload   = 0 );

   virtual ~OctreeItemTest();
            OctreeItemTest( const OctreeItemTest& );
   OctreeItemTest& operator=( const OctreeItemTest& );


/// queries --------------------------------------------------------------------
   virtual const Vector3r& getPosition()                                  const;
   virtual const Vector3r& getDimensions()                                const;

   virtual dword getPayload()                                             const;


/// fields ---------------------------------------------------------------------
private:
   Vector3r position_m;
   Vector3r dimensions_m;

   dword    payload_m;
};




/// standard object services ---------------------------------------------------
OctreeItemTest::OctreeItemTest
(
   const Vector3r& position,
   const Vector3r& dimensions,
   const dword     payload
)
 : position_m  ( position )
 , dimensions_m( dimensions )
 , payload_m   ( payload )
{
}


OctreeItemTest::~OctreeItemTest()
{
}


OctreeItemTest::OctreeItemTest
(
   const OctreeItemTest& other
)
 : position_m  ( other.position_m )
 , dimensions_m( other.dimensions_m )
 , payload_m   ( other.payload_m )
{
}


OctreeItemTest& OctreeItemTest::operator=
(
   const OctreeItemTest& other
)
{
   if( &other != this )
   {
      position_m   = other.position_m;
      dimensions_m = other.dimensions_m;
      payload_m    = other.payload_m;
   }

   return *this;
}


/// queries --------------------------------------------------------------------
const Vector3r& OctreeItemTest::getPosition() const
{
   return position_m;
}


const Vector3r& OctreeItemTest::getDimensions() const
{
   return dimensions_m;
}


dword OctreeItemTest::getPayload() const
{
   return payload_m;
}


std::ostream& operator<<( std::ostream&, const OctreeItemTest& );

std::ostream& operator<<
(
   std::ostream&         out,
   const OctreeItemTest& item
)
{
   const Vector3r& position( item.getPosition() );
   const Vector3r& dimensions( item.getDimensions() );

   out << "{" << position << " " << dimensions << "}";

   return out;
}




/// OctreeAgentTest ////////////////////////////////////////////////////////////

class OctreeAgentTest
   : public OctreeAgent<OctreeItemTest>
{
/// standard object services ---------------------------------------------------
public:
            OctreeAgentTest() {};

   virtual ~OctreeAgentTest() {};
private:
            OctreeAgentTest( const OctreeAgentTest& );
   OctreeAgentTest& operator=( const OctreeAgentTest& );


/// queries --------------------------------------------------------------------
/// octree agent overrides
protected:
   virtual bool  isOverlappingCell ( const OctreeItemTest& item,
                                     const Vector3r& lowerCorner,
                                     const Vector3r& upperCorner )        const;


/// implementation -------------------------------------------------------------
public:
   static  bool isOverlapping( const Vector3r& itemLower,
                               const Vector3r& itemUpper,
                               const Vector3r& cellLower,
                               const Vector3r& cellUpper );
};




/// queries --------------------------------------------------------------------
/// octree agent overrides
bool OctreeAgentTest::isOverlappingCell
(
   const OctreeItemTest& item,
   const Vector3r&       lowerCorner,
   const Vector3r&       upperCorner
) const
{
   return isOverlapping( item.getPosition(),
      item.getPosition() + item.getDimensions(), lowerCorner, upperCorner );
}


bool OctreeAgentTest::isOverlapping
(
   const Vector3r& itemLower,
   const Vector3r& itemUpper,
   const Vector3r& cellLower,
   const Vector3r& cellUpper
)
{
   // check the two ranges overlap in every dimension
   bool isOverlap = true;
   for( int i = 3;  i-- > 0; )
   {
      isOverlap &= (itemLower[i] < cellUpper[i]) &
                   (itemUpper[i] > cellLower[i]);
   }

   return isOverlap;
}




/// OctreeVisitorTest //////////////////////////////////////////////////////////

class OctreeVisitorTest
   : public OctreeVisitor<OctreeItemTest>
{
/// standard object services ---------------------------------------------------
public:
            OctreeVisitorTest( const Octree<OctreeItemTest>& );

   virtual ~OctreeVisitorTest();
private:
            OctreeVisitorTest( const OctreeVisitorTest& );
   OctreeVisitorTest& operator=( const OctreeVisitorTest& );


/// commands -------------------------------------------------------------------
protected:
   virtual void  visitRoot  ( const OctreeCell* pRootCell,
                              const OctreeData& octreeData );
   virtual void  visitBranch( const OctreeCell* subCells[8],
                              const OctreeData& octreeData );
   virtual void  visitLeaf  ( const Array<const OctreeItemTest*>& items,
                              const OctreeData& octreeData );


/// queries --------------------------------------------------------------------
public:
   typedef std::pair<OctreeData, Array<const OctreeItemTest*> > LeafData;

   const std::vector<const void*>& getIds()                               const;
   const std::vector<LeafData>&    getLeafs()                             const;


/// fields ---------------------------------------------------------------------
private:
   std::vector<const void*> ids_m;
   std::vector<LeafData>    leafs_m;
};




/// standard object services ---------------------------------------------------
OctreeVisitorTest::OctreeVisitorTest
(
   const Octree<OctreeItemTest>& octree
)
 : ids_m  ()
 , leafs_m()
{
   ids_m.push_back( static_cast<const void*>(&octree) );
}


OctreeVisitorTest::~OctreeVisitorTest()
{
}


OctreeVisitorTest::OctreeVisitorTest
(
   const OctreeVisitorTest& other
)
 : OctreeVisitor<OctreeItemTest>()
 , ids_m  ( other.ids_m )
 , leafs_m( other.leafs_m )
{
}


OctreeVisitorTest& OctreeVisitorTest::operator=
(
   const OctreeVisitorTest& other
)
{
   if( &other != this )
   {
      ids_m   = other.ids_m;
      leafs_m = other.leafs_m;
   }

   return *this;
}


/// commands -------------------------------------------------------------------
void OctreeVisitorTest::visitRoot
(
   const OctreeCell* pRootCell,
   const OctreeData& octreeData
)
{
   if( pRootCell )
   {
      ids_m.push_back( static_cast<const void*>(pRootCell) );

      pRootCell->visit( octreeData, *this );
   }
}


void OctreeVisitorTest::visitBranch
(
   const OctreeCell* subCells[8],
   const OctreeData& octreeData
)
{
   ids_m.push_back( subCells );

   // step through subcells
   for( dword i = 8;  i-- > 0; )
   {
      const OctreeCell* pSubCell = subCells[i];
      if( pSubCell )
      {
         ids_m.push_back( static_cast<const void*>(pSubCell) );

         // continue visit traversal
         OctreeBranch::continueVisit( subCells, octreeData, i, *this );
      }
   }
}


void OctreeVisitorTest::visitLeaf
(
   const Array<const OctreeItemTest*>& items,
   const OctreeData& octreeData
)
{
   ids_m.push_back( static_cast<const void*>(&items) );

   leafs_m.push_back( LeafData( octreeData, items ) );
}


/// queries --------------------------------------------------------------------
const std::vector<const void*>& OctreeVisitorTest::getIds() const
{
   return ids_m;
}


const std::vector<OctreeVisitorTest::LeafData>& OctreeVisitorTest::getLeafs()
const
{
   return leafs_m;
}








/// declarations ///////////////////////////////////////////////////////////////

static bool testConstruction
(
   std::ostream* out       = 0,
   const bool    isVerbose = false,
   const dword   seed      = 0
);
static bool testCommands
(
   std::ostream* out       = 0,
   const bool    isVerbose = false,
   const dword   seed      = 0
);
static bool testCommands1
(
   std::ostream* out       = 0,
   const bool    isVerbose = false,
   const dword   seed      = 0
);
static bool testCommands2
(
   std::ostream* out       = 0,
   const bool    isVerbose = false,
   const dword   seed      = 0
);
static bool testCommands3
(
   std::ostream* out       = 0,
   const bool    isVerbose = false,
   const dword   seed      = 0
);


class RandomFast
{
public:
/// standard object services ---------------------------------------------------
   explicit RandomFast( const dword seed =0 )
    : random_m( seed )
   {
   }

   ~RandomFast()
   {
   }

   RandomFast( const RandomFast& other )
    : random_m( other.random_m )
   {
   }

   RandomFast& operator=( const RandomFast& other )
   {
      random_m = other.random_m;

      return *this;
   }


/// commands -------------------------------------------------------------------
   RandomFast& setSeed( const dword seed )
   {
      random_m = seed;

      return *this;
   }

   RandomFast& next()
   {
      random_m = static_cast<dword>(1664525) * random_m +
         static_cast<dword>(1013904223);

      return *this;
   }


/// queries --------------------------------------------------------------------
   dword getDword()                                                       const
   {
      return random_m;
   }

   udword getUdword()                                                     const
   {
      return static_cast<udword>(random_m);
   }

   float getFloat()                                                       const
   {
      dword itemp = static_cast<dword>(0x3F800000) |
         (static_cast<dword>(0x007FFFFF) & random_m);
      return *(reinterpret_cast<float*>(&itemp)) - 1.0f;
   }

   float getFloat( const float scale,
                   const float displace = 0.0f )                          const
   {
      return getFloat() * scale + displace;
   }


/// fields ---------------------------------------------------------------------
private:
   // current value of sequence and seed of the following part of the sequence
   dword random_m;
};


static void makeRandomFilledOctree
(
   RandomFast&                             rand,
   const dword                             howManyItems,
   std::auto_ptr<Octree<OctreeItemTest> >& pOctree,
   std::vector<OctreeItemTest>&            items
);
static void makeRandomOctree
(
   RandomFast&                             rand,
   std::auto_ptr<Octree<OctreeItemTest> >& pOctree
);
static void makeRandomItems
(
   RandomFast&                  rand,
   dword                        howMany,
   const Vector3r&              position,
   real                         size,
   std::vector<OctreeItemTest>& items
);








/// test functions /////////////////////////////////////////////////////////////

void hxa7241_graphics::preTest()
{
   // make items
   OctreeItemTest item1( Vector3r( 1, 1, 1 ) );
   OctreeItemTest item2( Vector3r( 3, 3, 3 ) );
   OctreeItemTest item3( Vector3r( 4, 4, 4 ) );

   // make octree
   Vector3r position( 0, 0, 0 );
   real     size      = 5.0f;
   dword    maxItems  = 1;
   dword    maxLevels = 4;

   Octree<OctreeItemTest> o1( position, size, maxItems, maxLevels, 0.001f );

   // insert items into octree
   OctreeAgentTest a;
   o1.insert( item1, a );
   o1.insert( item2, a );
   o1.insert( item3, a );

   // stream out octree
   OctreeStreamOut<OctreeItemTest> vso( std::cout, true );
   o1.visit( vso );
}




bool hxa7241_graphics::test_Octree
(
   std::ostream* pOut,
   const bool    isVerbose,
   dword         seed
)
{
   if( 0 == seed )
   {
      seed = static_cast<dword>(::time(0));
   }

   if( pOut )
   {
      pOut->flags( std::ios_base::boolalpha );// | std::ios_base::uppercase );
   }

   return testConstruction( pOut, isVerbose, seed ) &&
      testCommands( pOut, isVerbose, seed );
}




bool testConstruction
(
   std::ostream* pOut,
   const bool    isVerbose,
   const dword   seed
)
{
   // Copying:
   //
   // Generate some random octrees. Copy each and check it is identical to
   // its original in value, and check all octree object ids are different.

   bool isOk = true;

   RandomFast rand( seed );
   if( pOut )
   {
      *pOut << "\nseed= " << rand.getDword() << "\n";
   }

   // loop
   for( dword i = 0;  i < 10;  ++i )
   {
      std::auto_ptr<Octree<OctreeItemTest> > po1;
      std::vector<OctreeItemTest>            items;
      makeRandomFilledOctree( rand, 30, po1, items );

      if( (*pOut != 0) && isVerbose )
      {
         OctreeStreamOut<OctreeItemTest> so( *pOut, true );
         po1->visit( so );
         *pOut << "\n";
      }

      // make copy octree
      Octree<OctreeItemTest> o2( *po1 );

      {
         // stream out each to string
         std::ostringstream ss1;
         OctreeStreamOut<OctreeItemTest> so1( ss1, false );
         po1->visit( so1 );
         std::ostringstream ss2;
         OctreeStreamOut<OctreeItemTest> so2( ss2, false );
         o2.visit( so2 );

         // compare strings
         isOk &= (ss1.str() == ss2.str());
      }

      {
         // make arrays of octree object ids
         OctreeVisitorTest v1( *po1 );
         po1->visit( v1 );
         OctreeVisitorTest v2( o2 );
         o2.visit( v2 );

         isOk &= (v1.getIds().size() == v2.getIds().size());

         // check set intersection of ids is empty
         std::vector<const void*> ids1( v1.getIds() );
         std::vector<const void*> ids2( v2.getIds() );
         std::sort( ids1.begin(), ids1.end() );
         std::sort( ids2.begin(), ids2.end() );
         std::vector<const void*> intersect;
         std::set_intersection( ids1.begin(), ids1.end(),
                           ids2.begin(), ids2.end(),
                           intersect.begin() );
         isOk &= intersect.empty();
      }

      if( pOut )
      {
         *pOut << i << " " << isOk << "\n";
      }
   }

   if( pOut )
   {
      *pOut << "\n--- testConstruction: " << isOk << "\n";
   }

   return isOk;
}


bool testCommands
(
   std::ostream* pOut,
   const bool    isVerbose,
   const dword   seed
)
{
   return testCommands1( pOut, isVerbose, seed ) &&
          testCommands2( pOut, isVerbose, seed ) &&
          testCommands3( pOut, isVerbose, seed );
}


bool testCommands1
(
   std::ostream* pOut,
   const bool    isVerbose,
   const dword   //seed
)
{
   // simple structural test:
   //
   // 1) make octree with four item per leaf limit
   // 2) add an item in each octant
   // 3) check each item is in the correct octant

   bool isOk = true;

   if( pOut )
   {
      *pOut << "\n\n";
   }


   // make octree
   Octree<OctreeItemTest> o1( Vector3r::ZERO(), 2, 4, 4, 0.0001f );

   // make items
   std::vector<OctreeItemTest> i1(8);
   OctreeAgentTest a;
   for( dword i = 0;  i < 8;  ++i )
   {
      // add item in each octant
      const Vector3r position( static_cast<real>(i & 1)        + 0.25f,
                               static_cast<real>((i >> 1) & 1) + 0.25f,
                               static_cast<real>((i >> 2) & 1) + 0.25f );
      i1[i] = OctreeItemTest( position, Vector3r::HALF(), i );
      o1.insert( i1[i], a );

      if( pOut && isVerbose )
      {
         *pOut << "item " << i << " :  " << i1[i] << "\n";
      }
   }
   if( pOut && isVerbose )
   {
      *pOut << "\n";
   }

   if( pOut && isVerbose )
   {
      OctreeStreamOut<OctreeItemTest> so( *pOut, true );
      o1.visit( so );
      *pOut << "\n";
   }

   // make info from octree
   OctreeVisitorTest v1( o1 );
   o1.visit( v1 );

   // step through leafs
   const std::vector<OctreeVisitorTest::LeafData>& leafs( v1.getLeafs() );
   for( udword j = leafs.size();  j-- > 0; )
   {
      const OctreeVisitorTest::LeafData& leaf( leafs[j] );

      const Vector3r& cellLower( leaf.first.getBound().getLowerCorner() );
      const Vector3r& cellUpper( leaf.first.getBound().getUpperCorner() );

      // step through items
      const Array<const OctreeItemTest*>& items( leaf.second );
      for( int i = 0;  i < items.getLength();  ++i )
      {
         const OctreeItemTest* pItem = items[i];

         const Vector3r itemLower( pItem->getPosition() );
         const Vector3r itemUpper( itemLower + pItem->getDimensions() );

         // check item is overlapping its leaf cell
         const bool is =
            OctreeAgentTest::isOverlapping( itemLower, itemUpper,
                                             cellLower, cellUpper );
         isOk &= is;
         if( pOut && isVerbose )
         {
            *pOut << "overlap " << is << " :  " <<
                     itemLower << "  " << itemUpper << "  " <<
                     cellLower << "  " << cellUpper << "\n";
         }

         // check item is in correct leaf cell
         const dword octantIndex = pItem->getPayload();
         const Vector3r octantLower( static_cast<real>(octantIndex & 1),
                                    static_cast<real>((octantIndex >> 1) & 1),
                                    static_cast<real>((octantIndex >> 2) & 1) );
         const Vector3r octantUpper( octantLower + Vector3r::ONE() );
         isOk &= (cellLower == octantLower);
         isOk &= (cellUpper == octantUpper);

         if( pOut )
         {
            if( isVerbose )
            {
               *pOut << "\toctant match " << (cellLower == octantLower) <<
                        " :  " << cellLower << "  " << octantLower << "\n";
               *pOut << "\toctant match " << (cellUpper == octantUpper) <<
                        " :  " << cellUpper << "  " << octantUpper << "\n";
            }
            else
            {
               *pOut << isOk << "\n";
            }
         }
      }
   }

   if( pOut )
   {
      *pOut << "\n--- testCommands1: " << isOk << "\n";
   }

   return isOk;
}


bool testCommands2
(
   std::ostream* pOut,
   const bool    ,//isVerbose,
   const dword   seed
)
{
   // basic insert/remove test:
   //
   // insert a lot, visit to collect all items, check they are all there
   // remove them all, get counts to check they all came out

   bool isOk = true;


   if( pOut )
   {
      *pOut << "\n\n";
   }

   RandomFast rand( seed );
   if( pOut )
   {
      *pOut << "\nseed= " << rand.getDword() << "\n\n";
   }

   for( dword j = 0;  j < 10;  ++j )
   {
      // make octree with items
      std::auto_ptr<Octree<OctreeItemTest> > po1;
      std::vector<OctreeItemTest>            i1;
      makeRandomFilledOctree( rand, 100, po1, i1 );

//    // only for 10 or so items (otherwise > 14MB of output)
//    if( pOut && isVerbose )
//    {
//       OctreeStreamOut<OctreeItemTest> so( *pOut, false );
//       po1->visit( so );
//       *pOut << "\n";
//       for( udword i = 0;  i < i1.size();  ++i )
//       {
//          *pOut << i1[i] << "\n";
//       }
//    }

      // collect leafs items
      OctreeVisitorTest v1( *po1 );
      po1->visit( v1 );

      dword byteSize, leafCount, itemRefCount, maxDepth;
      po1->getInfo( byteSize, leafCount, itemRefCount, maxDepth );

      // check collection length equals info leaf count
      isOk &= (v1.getLeafs().size() == static_cast<udword>(leafCount));

      // compare items list to leafs collection
      {
         const std::vector<OctreeVisitorTest::LeafData>& leafs( v1.getLeafs() );

         std::set<const void*> i2s;
         for( udword k = 0;  k < leafs.size();  ++k )
         {
            const Array<const OctreeItemTest*>& items( leafs[k].second );
            for( dword m = 0;  m < items.getLength();  ++m )
            {
               i2s.insert( items[m] );
            }
         }
         std::set<const void*> i1s;
         for( udword k = 0;  k < i1.size();  ++k )
         {
            i1s.insert( &(i1[k]) );
         }

         isOk &= (i1s == i2s);
      }

      // remove items
      OctreeAgentTest a;
      for( udword i = 0;  i < i1.size();  ++i )
      {
         po1->remove( i1[i], a );

//       // only for 10 or so items (otherwise > 14MB of output)
//       if( pOut && isVerbose )
//       {
//          OctreeStreamOut<OctreeItemTest> so( *pOut, false );
//          po1->visit( so );
//       }
      }

      // check octree info data is all 'zero'
      //dword byteSize, leafCount, itemRefCount, maxDepth;
      po1->getInfo( byteSize, leafCount, itemRefCount, maxDepth );
      isOk &= (leafCount == 0);
      isOk &= (itemRefCount == 0);
      isOk &= (maxDepth == 0);
      isOk &= po1->isEmpty();

      if( pOut )
      {
         *pOut << j << " " << isOk << "\n";
      }
   }

   if( pOut )
   {
      *pOut << "\n--- testCommands2: " << isOk << "\n";
   }


   return isOk;
}


bool testCommands3
(
   std::ostream* ,//pOut,
   const bool    ,//isVerbose,
   const dword   //seed
)
{
   // Structural conformance postconditions:
   //
   // Randomly insert and remove many random points, one at a time.
   // After each command, check structure of octree:
   // * after insertion, the octree is (one of):
   //    * same, or
   //    * one leaf holding one more item, or
   //    * one leaf has changed to a branch containing 1-8 leafs and 0-7 nils,
   //      or
   //    * one leaf has changed to 0-n levels of branchs of one subcell, and
   //      either 2-8 leafs, or one leaf at max level
   // * after removal, the octree is (one of):
   //    * same, or
   //    * one leaf holding one less item, or
   //    * one leaf has changed to nil, or
   //    * eight leafs have disappeared, and their direct parent branch has
   //      changed to a leaf, or
   //    * one leaf has disappeared, one of its parent branchs has changed to
   //      nil, and any branchs between have disappeared

   bool isOk = true;
/*
   if( pOut )
   {
      *pOut << "\n\n";
   }

   RandomFast rand( seed );
   if( pOut )
   {
      *pOut << "\nseed= " << rand.getDword() << "\n\n";
   }


   if( pOut )
   {
      *pOut << "\n--- testCommands3: " << isOk << "\n";
   }
*/
   return isOk;
}




///-----------------------------------------------------------------------------

void makeRandomFilledOctree
(
   RandomFast&                             rand,
   const dword                             howManyItems,
   std::auto_ptr<Octree<OctreeItemTest> >& pOctree,
   std::vector<OctreeItemTest>&            items
)
{
   makeRandomOctree( rand, pOctree );
   makeRandomItems( rand, howManyItems,
                    pOctree->getPosition(), pOctree->getSize(),
                    items );

   OctreeAgentTest a;
   for( int i = 0, end = items.size();  i < end;  ++i )
   {
      pOctree->insert( items[i], a );
   }
}


void makeRandomOctree
(
   RandomFast&                             rand,
   std::auto_ptr<Octree<OctreeItemTest> >& pOctree
)
{
   // position: random between -/+100
   // (making sure x,y,z are defined in order)
   const real x = rand.next().getFloat(200.0f, -100.0f);
   const real y = rand.next().getFloat(200.0f, -100.0f);
   const real z = rand.next().getFloat(200.0f, -100.0f);
   const Vector3r position( x, y, z );

   // size: random between 0.5 and 50
   const real size = rand.next().getFloat(49.5f, 0.5f);

   // max items: random between 1 and 8
   const dword maxItems = (rand.next().getUdword() >> 29) + 1;

   // max levels: random between bounds
   const dword maxLevels = (rand.next().getUdword() >> 30) + 3;

   // construct
   pOctree.reset( new Octree<OctreeItemTest>( position, size, maxItems,
      maxLevels, 0.0001f ) );
}


void makeRandomItems
(
   RandomFast&                  rand,
   const dword                  howMany,
   const Vector3r&              position,
   const real                   size,
   std::vector<OctreeItemTest>& items
)
{
   items.resize( howMany );

   for( dword i = 0;  i < howMany;  ++i )
   {
      // make random item
      const real x = rand.next().getFloat();
      const real y = rand.next().getFloat();
      const real z = rand.next().getFloat();
      Vector3r itemPosition( x, y, z );
      itemPosition *= (size * 0.99f);
      itemPosition += (Vector3r::ONE() * (size * 0.005f));
      itemPosition += position;

      // add item
      if( rand.next().getDword() >= 0 )
      {
         items[i] = OctreeItemTest( itemPosition, Vector3r::ZERO() );
      }
      else
      {
         const real x = rand.next().getFloat();
         const real y = rand.next().getFloat();
         const real z = rand.next().getFloat();
         Vector3r dimensions( x, y, z );
         dimensions *= 0.175f;
         dimensions += Vector3r(0.025f, 0.025f, 0.025f);
         dimensions *= (size / 3.0f);
         items[i] = OctreeItemTest( itemPosition, dimensions );
      }
   }
}








/// main ///////////////////////////////////////////////////////////////////////

int main
(
   int   argc,
   char* argv[]
)
{
   bool  isVerbose = false;
   dword seed      = 0;

   if( argc >= 2 )
   {
      const std::string a1( argv[1] );
      isVerbose = (a1 == std::string("true"));
   }
   if( argc >= 3 )
   {
      std::istringstream iss( argv[2] );
      iss >> seed;
   }

   const bool isOk = hxa7241_graphics::test_Octree( &std::cout, isVerbose, seed
      );

   std::cout << "\n\n" << (isOk ? "---" : "***") << " test_Octree: " << isOk <<
      "\n";


   return isOk ? EXIT_SUCCESS : EXIT_FAILURE;
}
