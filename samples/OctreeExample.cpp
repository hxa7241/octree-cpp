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
#include <iostream>

#include "Octree.hpp"


using namespace hxa7241_graphics;




/**
 * Step 1:<br/>
 * Check your item class provides access to its 3D properties.<br/><br/>
 *
 * Step 2:<br/>
 * Implement a derivative of OctreeAgent<YourItem>, to calculate if an item
 * overlaps a cell, or subcells.<br/><br/>
 *
 * Step 3:<br/>
 * Implement derivatives of OctreeVisitor<YourItem>, to perform operations
 * on an octree.<br/><br/>
 *
 * Step 4:<br/>
 * Write code to create items, octrees, and visitors, add/remove items
 * to octrees, and execute operations on octrees.
 */


/**
 * File contents:<br/><br/>

 * Classes<br/>
 *   * Block class: an example item<br/>
 *   * OctreeAgentBlock class: an agent for the example item<br/>
 *   * OctreeVisitorExample class: an example visitor using the item<br/>
 * Functions<br/>
 *   * main function: code to make some items, make an octree, and use the
 *     visitor.<br/>
 */




/// Vector3r stream out

std::ostream& operator<<( std::ostream&, const Vector3r& );

std::ostream& operator<<
(
   std::ostream&   out,
   const Vector3r& obj
)
{
   return out << '(' << obj[0] << ' ' << obj[1] << ' ' << obj[2] << ')';
}








/// Block //////////////////////////////////////////////////////////////////////

/**
 * An example item: a minimal axis-aligned 3D block.<br/><br/>
 *
 * You write this class, however you want. Though it will have some primitive
 * access to its 3D properties.
 */
class Block
{
/// standard object services ---------------------------------------------------
public:
            Block();
            Block( const Vector3r& position,
                   const Vector3r& dimension );

           ~Block();
            Block( const Block& );
   Block& operator=( const Block& );


/// queries --------------------------------------------------------------------
           const Vector3r& getPosition()                                  const;
           const Vector3r& getDimensions()                                const;


/// fields ---------------------------------------------------------------------
private:
   Vector3r position_m;
   Vector3r dimensions_m;
};




/// standard object services ---------------------------------------------------
Block::Block()
 : position_m  ()
 , dimensions_m()
{
}


Block::Block
(
   const Vector3r& position,
   const Vector3r& dimensions
)
 : position_m  ( position )
 , dimensions_m( dimensions )
{
}


Block::~Block()
{
}


Block::Block
(
   const Block& other
)
 : position_m  ( other.position_m )
 , dimensions_m( other.dimensions_m )
{
}


Block& Block::operator=
(
   const Block& other
)
{
   if( &other != this )
   {
      position_m   = other.position_m;
      dimensions_m = other.dimensions_m;
   }

   return *this;
}


/// queries --------------------------------------------------------------------
const Vector3r& Block::getPosition() const
{
   return position_m;
}


const Vector3r& Block::getDimensions() const
{
   return dimensions_m;
}








/// OctreeAgentBlock ///////////////////////////////////////////////////////////

/**
 * An example Octree agent, for Block item class.<br/><br/>
 *
 * You must write this class, as a concrete derivative of OctreeAgent<>. It
 * provides the Octree with a way to interact with your item class.
 */
class OctreeAgentBlock
   : public OctreeAgent<Block>
{
/// standard object services ---------------------------------------------------
public:
            OctreeAgentBlock() {};

   virtual ~OctreeAgentBlock() {};
private:
            OctreeAgentBlock( const OctreeAgentBlock& );
   OctreeAgentBlock& operator=( const OctreeAgentBlock& );


/// queries --------------------------------------------------------------------
/// octree agent overrides
protected:
   virtual bool  isOverlappingCell ( const Block&    item,
                                     const Vector3r& lowerCorner,
                                     const Vector3r& upperCorner )        const;

   // could also override getSubcellOverlaps to provide more efficent
   // calculation (boundary testing can be shared).
};




/// queries --------------------------------------------------------------------
bool OctreeAgentBlock::isOverlappingCell
(
   const Block&    item,
   const Vector3r& lowerCorner,
   const Vector3r& upperCorner
) const
{
   const Vector3r& itemLower( item.getPosition() );
   const Vector3r  itemUpper = item.getPosition() + item.getDimensions();

   // check the two ranges overlap in every dimension
   bool isOverlap = true;
   for( int i = 3;  i-- > 0; )
   {
      isOverlap &= (itemLower[i] < upperCorner[i]) &
                   (itemUpper[i] > lowerCorner[i]);
   }

   return isOverlap;
}








/// OctreeVisitorExample ///////////////////////////////////////////////////////

/**
 * An example Octree visitor, for writing out leaf content.<br/><br/>
 *
 * You must write this class, as a concrete derivative of OctreeVisitor<>. It
 * defines an search/inspection operation on your items in an octree. Any number
 * of different ones can be written and used on the same octree.
 */
class OctreeVisitorExample
   : public OctreeVisitor<Block>
{
/// standard object services ---------------------------------------------------
public:
            OctreeVisitorExample();

   virtual ~OctreeVisitorExample();
private:
            OctreeVisitorExample( const OctreeVisitorExample& );
   OctreeVisitorExample& operator=( const OctreeVisitorExample& );


/// commands -------------------------------------------------------------------
/// octree visitor overrides
protected:
   virtual void  visitRoot  ( const OctreeCell* pRootCell,
                              const OctreeData& octreeData );
   virtual void  visitBranch( const OctreeCell* subCells[8],
                              const OctreeData& octreeData );
   virtual void  visitLeaf  ( const Array<const Block*>& items,
                              const OctreeData& octreeData );

   // any other commands ...


/// queries --------------------------------------------------------------------
   // any queries ...


/// fields ---------------------------------------------------------------------
private:
   // any fields ...
};




/// standard object services ---------------------------------------------------
OctreeVisitorExample::OctreeVisitorExample()
{
}


OctreeVisitorExample::~OctreeVisitorExample()
{
}


/// commands -------------------------------------------------------------------
void OctreeVisitorExample::visitRoot
(
   const OctreeCell* pRootCell,
   const OctreeData& octreeData
)
{
   // continue visit traversal
   OctreeRoot::continueVisit( pRootCell, octreeData, *this );
}


void OctreeVisitorExample::visitBranch
(
   const OctreeCell* subCells[8],
   const OctreeData& octreeData
)
{
   // subcell numbering:
   //    y z       6 7
   //    |/   2 3  4 5
   //     -x  0 1
   //
   // (in binary:)
   //    y z           110 111
   //    |/   010 011  100 101
   //     -x  000 001
   //

   // step through subcells (can be in any order...)
   for( dword i = 0;  i < 8;  ++i )
   {
      // maybe do something here...

      // continue visit traversal
      OctreeBranch::continueVisit( subCells, octreeData, i, *this );

      // maybe do something here...
   }
}


void OctreeVisitorExample::visitLeaf
(
   const Array<const Block*>& items,
   const OctreeData&          octreeData
)
{
   // write out cell and items data:

   // make some short aliases
   const Vector3r& lower( octreeData.getBound().getLowerCorner() );
   const Vector3r& upper( octreeData.getBound().getUpperCorner() );

   // write leaf bound
   std::cout << "leaf: level(" << octreeData.getLevel() << ") ";
   std::cout << "lower" << lower << " " << "upper" << upper << "\n";

   // write items
   for( dword i = 0, end = items.getLength();  i < end;  ++i )
   {
      // make some short aliases
      const Vector3r& lower( items[i]->getPosition() );
      const Vector3r  upper( items[i]->getPosition() + items[i]->getDimensions()
         );

      // write item
      std::cout << "   item: ";
      std::cout << "lower" << lower << " " << "upper" << upper << "\n";
   }

   std::cout << "\n";
}








/// main ///////////////////////////////////////////////////////////////////////

/**
 * An example Octree usage, with the previously written classes.<br/><br/>
 *
 * You must write some code to instantiate the items, octree, agent, and
 * visitors; and then add/remove items and visit the octree.
 */
int main
(
   int    ,//argc,
   char*[] //argv[]
)
{
   // write banner
   std::cout << "\n  HXA Octree Component v2.1 C++  *example*\n" <<
      "  Copyright (c) 2004-2007, Harrison Ainsworth / HXA7241.\n"
      "  http://www.hxa7241.org/\n\n";


   // make some items
   std::vector<Block> blocks;
   for( int i = 0;  i < 8;  ++i )
   {
      // one in the middle of each octant
      blocks.push_back( Block( Vector3r( real(i & 1)        + 0.25f,
                                         real((i >> 1) & 1) + 0.25f,
                                         real((i >> 2) & 1) + 0.25f ),
                               Vector3r(0.5f, 0.5f, 0.5f) ) );
   }

   // make an octree:
   // position (0,0,0), size 2m, max items per leaf 4, max depth 4,
   // min cell size 1mm
   Octree<Block> octree( Vector3r::ZERO(), 2.0f, 4, 4, 0.001f );
   std::cout << "Octree: position" << octree.getPosition() <<
      " size(" << octree.getSize() << ")" << "\n\n";


   // make an agent (probably a singleton)
   OctreeAgentBlock agent;

   // add items to octree, using agent
   for( int i = 0, end = blocks.size();  i < end;  ++i )
   {
      octree.insert( blocks[i], agent );
   }


   // make a visitor (as many concrete variants and instances as wanted)
   OctreeVisitorExample visitor;

   // execute visitor
   octree.visit( visitor );


   return EXIT_SUCCESS;
}
