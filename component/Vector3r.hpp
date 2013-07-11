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


#ifndef Vector3r_h
#define Vector3r_h


#include "Primitives.hpp"




namespace hxa7241_graphics
{
   using namespace hxa7241;


/**
 * Yes, its the 3D vector class!.<br/><br/>
 *
 * If you write some 3D graphics software, then you MUST write your OWN vector
 * class -- it is the law. So here is mine.<br/><br/>
 *
 * (Unused stuff commented-out)
 */
class Vector3r
{
/// standard object services ---------------------------------------------------
public:
            Vector3r();
            Vector3r( real x,
                      real y,
                      real z );
//   explicit Vector3r( const real xyz[3] );

           ~Vector3r();
            Vector3r( const Vector3r& );
   Vector3r& operator=( const Vector3r& );


/// commands -------------------------------------------------------------------
           Vector3r& set( real x,
                          real y,
                          real z );
//           Vector3r& set( const real xyz[3] );
//
//           Vector3r& setX( real );
//           Vector3r& setY( real );
//           Vector3r& setZ( real );
//           real&     operator[]( int );

//           Vector3r& negateEq ();
//           Vector3r& absEq    ();
//           Vector3r& unitizeEq();
//           Vector3r& crossEq  ( const Vector3r& );

           Vector3r& operator+=( const Vector3r& );
           Vector3r& operator-=( const Vector3r& );
           Vector3r& operator*=( const Vector3r& );
           Vector3r& operator/=( const Vector3r& );
           Vector3r& operator*=( real );
           Vector3r& operator/=( real );

//           Vector3r& clampMinEq( const Vector3r& );
//           Vector3r& clampMaxEq( const Vector3r& );
//           Vector3r& clampEq   ( const Vector3r& min,
//                                 const Vector3r& max );
//           Vector3r& clamp01Eq ();


/// queries --------------------------------------------------------------------
//           void      get( real& x,
//                          real& y,
//                          real& z )                                       const;
//           void      get( real xyz[3] )                                   const;

           real      getX()                                               const;
           real      getY()                                               const;
           real      getZ()                                               const;
           real      operator[]( int )                                    const;

//           real      sum()                                                const;
//           real      average()                                            const;
//           real      smallest()                                           const;
//           real      largest()                                            const;

           real      length()                                             const;
//           real      dot( const Vector3r& )                               const;
//           real      distance ( const Vector3r& )                         const;
//           real      distance2( const Vector3r& )                         const;

//           Vector3r  operator-()                                          const;
//           Vector3r  abs()                                                const;
//           Vector3r  unitized()                                           const;
//           Vector3r  cross( const Vector3r& )                             const;

           Vector3r  operator+( const Vector3r& )                         const;
           Vector3r  operator-( const Vector3r& )                         const;
           Vector3r  operator*( const Vector3r& )                         const;
           Vector3r  operator/( const Vector3r& )                         const;
           Vector3r  operator*( real )                                    const;
           Vector3r  operator/( real )                                    const;

           bool      operator==( const Vector3r& )                        const;
           bool      operator!=( const Vector3r& )                        const;
           bool      isZero()                                             const;

//           // returning vectors of -1.0 or 0.0 or +1.0
//           Vector3r  sign   ()                                            const;
//           Vector3r  compare( const Vector3r& )                           const;
//
//           // returning vectors of static_cast<real>(bool)
//           Vector3r  equal     ( const Vector3r& )                        const;
//           Vector3r  operator> ( const Vector3r& )                        const;
//           Vector3r  operator>=( const Vector3r& )                        const;
//           Vector3r  operator< ( const Vector3r& )                        const;
//           Vector3r  operator<=( const Vector3r& )                        const;

//           Vector3r  clampedMin( const Vector3r& )                        const;
//           Vector3r  clampedMax( const Vector3r& )                        const;
//           Vector3r  clamped   ( const Vector3r& min,
//                                 const Vector3r& max )                    const;
//           Vector3r  clamped01 ()                                         const;

   friend  Vector3r  operator*( real, const Vector3r& );
   friend  Vector3r  operator/( real, const Vector3r& );


/// constants ------------------------------------------------------------------
   static const Vector3r& ZERO();
   static const Vector3r& HALF();
   static const Vector3r& ONE();
//   static const Vector3r& EPSILON();
//   static const Vector3r& ALMOST_ONE();
//   static const Vector3r& MIN();
//   static const Vector3r& MAX();
//   static const Vector3r& SMALL();
//   static const Vector3r& LARGE();
//   static const Vector3r& X();
//   static const Vector3r& Y();
//   static const Vector3r& Z();


/// fields ---------------------------------------------------------------------
private:
   real xyz_m[3];
};








/// friends
Vector3r operator*( real, const Vector3r& );
Vector3r operator/( real, const Vector3r& );




/// INLINES ///
/*
 * These are certain. Leave others to the compiler.
 */


/// standard object services ---------------------------------------------------
/*inline
Vector3r::Vector3r
(
   const real xyz[3]
)
{
   Vector3r::set( xyz );
}*/


inline
Vector3r::~Vector3r()
{
}


inline
Vector3r::Vector3r
(
   const Vector3r& other
)
{
   Vector3r::operator=( other );
}




/// queries --------------------------------------------------------------------
inline
real Vector3r::getX() const
{
   return xyz_m[0];
}


inline
real Vector3r::getY() const
{
   return xyz_m[1];
}


inline
real Vector3r::getZ() const
{
   return xyz_m[2];
}


inline
real Vector3r::operator[]
(
   const int i
) const
{
   // careful !
   return xyz_m[i];
}


}//namespace




#endif//Vector3r_h
