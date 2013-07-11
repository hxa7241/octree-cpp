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


#ifndef OctreeStreamOut_h
#define OctreeStreamOut_h


#include <iosfwd>

#include "Octree.hpp"




namespace hxa7241_graphics
{


/**
 * Implementation for OctreeStreamOut template.<br/><br/>
 *
 * @see OctreeStreamOut
 */
class OctreeStreamOutImplementation
{
/// standard object services ---------------------------------------------------
public:
            OctreeStreamOutImplementation( std::ostream& outStream,
                                           bool          isLongFormat );

           ~OctreeStreamOutImplementation();
            OctreeStreamOutImplementation(
               const OctreeStreamOutImplementation& );
   OctreeStreamOutImplementation& operator=(
      const OctreeStreamOutImplementation& );


/// commands -------------------------------------------------------------------
           void  visitRoot  ( const OctreeCell* pRootCell,
                              const OctreeData& octreeData,
                              OctreeVisitorV&   visitor );
           void  visitBranch( const OctreeCell* subCells[8],
                              const OctreeData& octreeData,
                              OctreeVisitorV&   visitor );
           void  visitLeafBefore( const OctreeData& octreeData );
           void  visitLeafAfter ();

           bool          isLongFormat()                                   const;
           std::ostream& getOutStream()                                   const;
           std::ostream& indentNewline( dword extraIndent )               const;


/// implementation -------------------------------------------------------------
protected:
   static  void writeDataAndInfo( const OctreeCell& octreeCell,
                                  const OctreeData& octreeData,
                                  std::ostream&     outStream );


/// fields ---------------------------------------------------------------------
private:
   std::ostream* pOutStream_m;
   bool          isLongFormat_m;

   // visit state (only used during a visit)
   const OctreeCell* pCurrentCell_m;
   dword             indent_m;
};





/**
 * An octree visitor for writing a text representation of an octree and its
 * contents.<br/><br/>
 *
 * @see OctreeStreamOutImplementation
 */
template<class TYPE>
class OctreeStreamOut
   : public OctreeVisitor<TYPE>
{
/// standard object services ---------------------------------------------------
public:
            OctreeStreamOut( std::ostream& outStream,
                             bool          isLongFormat );

   virtual ~OctreeStreamOut();
            OctreeStreamOut( const OctreeStreamOut& );
   OctreeStreamOut& operator=( const OctreeStreamOut& );


/// octree visitor overrides
/// commands -------------------------------------------------------------------
protected:
   virtual void  visitRoot  ( const OctreeCell* pRootCell,
                              const OctreeData& octreeData );
   virtual void  visitBranch( const OctreeCell* subCells[8],
                              const OctreeData& octreeData );
   virtual void  visitLeaf  ( const Array<const TYPE*>& items,
                              const OctreeData&         octreeData );


/// fields ---------------------------------------------------------------------
private:
   OctreeStreamOutImplementation implementation_m;
};








/// TEMPLATES ///

/// standard object services ---------------------------------------------------
template<class TYPE>
inline
OctreeStreamOut<TYPE>::OctreeStreamOut
(
   std::ostream& outStream,
   const bool    isLongFormat
)
 : OctreeVisitor<TYPE>()
 , implementation_m   ( outStream, isLongFormat )
{
}


template<class TYPE>
inline
OctreeStreamOut<TYPE>::~OctreeStreamOut()
{
}


template<class TYPE>
inline
OctreeStreamOut<TYPE>::OctreeStreamOut
(
   const OctreeStreamOut& other
)
 : OctreeVisitor<TYPE>()
 , implementation_m   ( other.implementation_m )
{
}


template<class TYPE>
inline
OctreeStreamOut<TYPE>& OctreeStreamOut<TYPE>::operator=
(
   const OctreeStreamOut& other
)
{
   if( &other != this )
   {
      implementation_m = other.implementation_m;
   }

   return *this;
}




/// commands -------------------------------------------------------------------
template<class TYPE>
inline
void OctreeStreamOut<TYPE>::visitRoot
(
   const OctreeCell* pRootCell,
   const OctreeData& octreeData
)
{
   implementation_m.visitRoot( pRootCell, octreeData, *this );
}


template<class TYPE>
inline
void OctreeStreamOut<TYPE>::visitBranch
(
   const OctreeCell* subCells[8],
   const OctreeData& octreeData
)
{
   implementation_m.visitBranch( subCells, octreeData, *this );
}


template<class TYPE>
void OctreeStreamOut<TYPE>::visitLeaf
(
   const Array<const TYPE*>& items,
   const OctreeData&         octreeData
)
{
   implementation_m.visitLeafBefore( octreeData );

   const TYPE* const* ppItem = items.getStorage();
   const TYPE* const* ppEnd  = ppItem + items.getLength();

   for( ;  ppItem < ppEnd;  ++ppItem )
   {
      if( implementation_m.isLongFormat() )
      {
         implementation_m.indentNewline( +1 );
         implementation_m.getOutStream() << "id:" << *ppItem << ": " <<
            **ppItem;
      }
      else
      {
         implementation_m.getOutStream() << "<"  << "id:" << *ppItem <<
            ": " << **ppItem << "> ";
      }
   }

   implementation_m.visitLeafAfter();
}


}//namespace




#endif//OctreeStreamOut_h
