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


#include "OctreeAuxiliary.hpp"


using namespace hxa7241_graphics;




/// OctreeDimensions ///////////////////////////////////////////////////////////


// accommodates scene including sun and earth, down to cm cells (use 47 for mm
// cells)
const dword OctreeDimensions::MAX_LEVEL = 44;
// resolves to nanometre scale
const real  OctreeDimensions::MIN_SIZE  = 1.0f / static_cast<float>(0x40000000);


/// standard object services ---------------------------------------------------
OctreeDimensions::OctreeDimensions
(
   const Vector3r& positionOfLowerCorner,
   const real      size,
   const dword     maxItemsPerCell,
   const dword     maxLevelCount,
   const real      minCellSize
)
 : positionOfLowerCorner_m( positionOfLowerCorner )
 , size_m                 ( size            >= 0.0f ? size          : -size   )
 , maxItemsPerCell_m      ( maxItemsPerCell >  0    ? maxItemsPerCell   : 1   )
 , maxLevel_m             ( maxLevelCount   >  0    ? maxLevelCount - 1 : 0   )
 , minSize_m              ( minCellSize <= size_m   ? minCellSize   : size_m  )
{
   if( maxLevel_m > MAX_LEVEL )
   {
      maxLevel_m = MAX_LEVEL;
   }
   if( minSize_m < MIN_SIZE )
   {
      minSize_m = MIN_SIZE;
   }
}


OctreeDimensions::~OctreeDimensions()
{
}


OctreeDimensions::OctreeDimensions
(
   const OctreeDimensions& other
)
 : positionOfLowerCorner_m( other.positionOfLowerCorner_m )
 , size_m                 ( other.size_m )
 , maxItemsPerCell_m      ( other.maxItemsPerCell_m )
 , maxLevel_m             ( other.maxLevel_m )
 , minSize_m              ( other.minSize_m )
{
}


OctreeDimensions& OctreeDimensions::operator=
(
   const OctreeDimensions& other
)
{
   if( &other != this )
   {
      positionOfLowerCorner_m = other.positionOfLowerCorner_m;
      size_m                  = other.size_m;
      maxItemsPerCell_m       = other.maxItemsPerCell_m;
      maxLevel_m              = other.maxLevel_m;
      minSize_m               = other.minSize_m;
   }

   return *this;
}




/// queries --------------------------------------------------------------------
bool OctreeDimensions::isSubdivide
(
   const dword itemCount,
   const dword level,
   const real  size
) const
{
   return (itemCount > maxItemsPerCell_m) &
      (level < maxLevel_m) & (size > (minSize_m * 2.0f));
}








/// OctreeBound ////////////////////////////////////////////////////////////////


/// standard object services ---------------------------------------------------
OctreeBound::OctreeBound()
 : positionOfLowerCorner_m( Vector3r::ZERO() )
 , positionOfUpperCorner_m( Vector3r::ONE() )
 , center_m               ( Vector3r::HALF() )
 , circumSphereRadius_m   ( Vector3r::HALF().length() )
{
}


OctreeBound::OctreeBound
(
   const Vector3r& positionOfLowerCorner,
   const real      size
)
 : positionOfLowerCorner_m( positionOfLowerCorner )
 , positionOfUpperCorner_m( positionOfLowerCorner + Vector3r(size, size, size) )
 , center_m               ( (positionOfLowerCorner_m + positionOfUpperCorner_m)
                            *= 0.5f )
 , circumSphereRadius_m   ( (Vector3r::HALF() * size).length() )
{
}


OctreeBound::OctreeBound
(
   const OctreeBound& parentCellBound,
   const dword        subCellIndex
)
{
   {
      const Vector3r* lowMidHigh[] =
      {
         &(parentCellBound.positionOfLowerCorner_m),
         &(parentCellBound.center_m),
         &(parentCellBound.positionOfUpperCorner_m)
      };

      positionOfLowerCorner_m.set(
         lowMidHigh[ subCellIndex       & 1]->getX(),
         lowMidHigh[(subCellIndex >> 1) & 1]->getY(),
         lowMidHigh[(subCellIndex >> 2) & 1]->getZ() );
      positionOfUpperCorner_m.set(
         (lowMidHigh+1)[ subCellIndex       & 1]->getX(),
         (lowMidHigh+1)[(subCellIndex >> 1) & 1]->getY(),
         (lowMidHigh+1)[(subCellIndex >> 2) & 1]->getZ() );
   }

   ((center_m = positionOfLowerCorner_m) += positionOfUpperCorner_m) *= 0.5f;
   circumSphereRadius_m = parentCellBound.circumSphereRadius_m * 0.5f;
}


OctreeBound::~OctreeBound()
{
}


OctreeBound::OctreeBound
(
   const OctreeBound& other
)
 : positionOfLowerCorner_m( other.positionOfLowerCorner_m )
 , positionOfUpperCorner_m( other.positionOfUpperCorner_m )
 , center_m               ( other.center_m )
 , circumSphereRadius_m   ( other.circumSphereRadius_m )
{
}


OctreeBound& OctreeBound::operator=
(
   const OctreeBound& other
)
{
   if( &other != this )
   {
      positionOfLowerCorner_m = other.positionOfLowerCorner_m;
      positionOfUpperCorner_m = other.positionOfUpperCorner_m;
      center_m                = other.center_m;
      circumSphereRadius_m    = other.circumSphereRadius_m;
   }

   return *this;
}








/// OctreeData /////////////////////////////////////////////////////////////////


/// standard object services ---------------------------------------------------
OctreeData::OctreeData
(
   const OctreeDimensions& dimensions
)
 : bound_m      ( dimensions.getPosition(), dimensions.getSize() )
 , level_m      ( 0 )
 , pDimensions_m( &dimensions )
{
}


OctreeData::OctreeData
(
   const OctreeData& parentCellData,
   const dword       subCellIndex
)
 : bound_m      ( parentCellData.bound_m, subCellIndex )
 , level_m      ( parentCellData.level_m + 1 )
 , pDimensions_m( parentCellData.pDimensions_m )
{
}


OctreeData::OctreeData
(
   const OctreeData&       other,
   const OctreeDimensions& dimensions
)
 : bound_m      ( other.bound_m )
 , level_m      ( other.level_m )
 , pDimensions_m( &dimensions )
{
}


OctreeData::~OctreeData()
{
}


OctreeData::OctreeData
(
   const OctreeData& other
)
 : bound_m      ( other.bound_m )
 , level_m      ( other.level_m )
 , pDimensions_m( other.pDimensions_m )
{
}


OctreeData& OctreeData::operator=
(
   const OctreeData& other
)
{
   if( &other != this )
   {
      bound_m       = other.bound_m;
      level_m       = other.level_m;
      pDimensions_m = other.pDimensions_m;
   }

   return *this;
}
