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


#include <math.h>

#include "Vector3r.hpp"


using namespace hxa7241_graphics;




/// standard object services ---------------------------------------------------
Vector3r::Vector3r()
{
   xyz_m[0] = real();
   xyz_m[1] = real();
   xyz_m[2] = real();
}


Vector3r::Vector3r
(
   const real x,
   const real y,
   const real z
)
{
   xyz_m[0] = x;
   xyz_m[1] = y;
   xyz_m[2] = z;
}


//Vector3r::Vector3r
//(
//   const real xyz[3]
//)
//{
//   xyz_m[0] = xyz[0];
//   xyz_m[1] = xyz[1];
//   xyz_m[2] = xyz[2];
//}
//
//
//Vector3r::~Vector3r()
//{
//}
//
//
//Vector3r::Vector3r
//(
//   const Vector3r& other
//)
//{
//   xyz_m[0] = other.xyz_m[0];
//   xyz_m[1] = other.xyz_m[1];
//   xyz_m[2] = other.xyz_m[2];
//}


Vector3r& Vector3r::operator=
(
   const Vector3r& other
)
{
   if( &other != this )
   {
      xyz_m[0] = other.xyz_m[0];
      xyz_m[1] = other.xyz_m[1];
      xyz_m[2] = other.xyz_m[2];
   }

   return *this;
}




/// commands -------------------------------------------------------------------
Vector3r& Vector3r::set
(
   const real x,
   const real y,
   const real z
)
{
   xyz_m[0] = x;
   xyz_m[1] = y;
   xyz_m[2] = z;

   return *this;
}


/*Vector3r& Vector3r::set
(
   const real xyz[3]
)
{
   xyz_m[0] = xyz[0];
   xyz_m[1] = xyz[1];
   xyz_m[2] = xyz[2];

   return *this;
}


Vector3r& Vector3r::setX
(
   const real x
)
{
   xyz_m[0] = x;

   return *this;
}


Vector3r& Vector3r::setY
(
   const real y
)
{
   xyz_m[1] = y;

   return *this;
}


Vector3r& Vector3r::setZ
(
   const real z
)
{
   xyz_m[2] = z;

   return *this;
}


real& Vector3r::operator[]
(
   const int i
)
{
   // careful !
   return xyz_m[i];
}*/


/*Vector3r& Vector3r::negateEq()
{
   xyz_m[0] = -xyz_m[0];
   xyz_m[1] = -xyz_m[1];
   xyz_m[2] = -xyz_m[2];

   return *this;
}


Vector3r& Vector3r::absEq()
{
   if( xyz_m[0] < real() )
   {
      xyz_m[0] = -xyz_m[0];
   }
   if( xyz_m[1] < real() )
   {
      xyz_m[1] = -xyz_m[1];
   }
   if( xyz_m[2] < real() )
   {
      xyz_m[2] = -xyz_m[2];
   }

   return *this;
}


Vector3r& Vector3r::unitizeEq()
{
   const real length = sqrt(
      (xyz_m[0] * xyz_m[0]) +
      (xyz_m[1] * xyz_m[1]) +
      (xyz_m[2] * xyz_m[2]) );
   const real oneOverLength = (length != real()) ?
      real(1.0f) / length : real();

   xyz_m[0] *= oneOverLength;
   xyz_m[1] *= oneOverLength;
   xyz_m[2] *= oneOverLength;

   return *this;
}


Vector3r& Vector3r::crossEq
(
   const Vector3r& v
)
{
   const real x = (xyz_m[1] * v.xyz_m[2]) - (xyz_m[2] * v.xyz_m[1]);
   const real y = (xyz_m[2] * v.xyz_m[0]) - (xyz_m[0] * v.xyz_m[2]);
   const real z = (xyz_m[0] * v.xyz_m[1]) - (xyz_m[1] * v.xyz_m[0]);

   xyz_m[0] = x;
   xyz_m[1] = y;
   xyz_m[2] = z;

   return *this;
}*/


Vector3r& Vector3r::operator+=
(
   const Vector3r& v
)
{
   xyz_m[0] += v.xyz_m[0];
   xyz_m[1] += v.xyz_m[1];
   xyz_m[2] += v.xyz_m[2];

   return *this;
}


Vector3r& Vector3r::operator-=
(
   const Vector3r& v
)
{
   xyz_m[0] -= v.xyz_m[0];
   xyz_m[1] -= v.xyz_m[1];
   xyz_m[2] -= v.xyz_m[2];

   return *this;
}


Vector3r& Vector3r::operator*=
(
   const Vector3r& v
)
{
   xyz_m[0] *= v.xyz_m[0];
   xyz_m[1] *= v.xyz_m[1];
   xyz_m[2] *= v.xyz_m[2];

   return *this;
}


Vector3r& Vector3r::operator/=
(
   const Vector3r& v
)
{
   xyz_m[0] /= v.xyz_m[0];
   xyz_m[1] /= v.xyz_m[1];
   xyz_m[2] /= v.xyz_m[2];

   return *this;
}


Vector3r& Vector3r::operator*=
(
   const real r
)
{
   xyz_m[0] *= r;
   xyz_m[1] *= r;
   xyz_m[2] *= r;

   return *this;
}


Vector3r& Vector3r::operator/=
(
   const real r
)
{
   // need to check if this is still faster

   const real oneOverR = real(1.0f) / r;

   xyz_m[0] *= oneOverR;
   xyz_m[1] *= oneOverR;
   xyz_m[2] *= oneOverR;

   return *this;
}


/*Vector3r& Vector3r::clampMinEq
(
   const Vector3r& min
)
{
   if( xyz_m[0] < min.xyz_m[0] )
   {
      xyz_m[0] = min.xyz_m[0];
   }
   if( xyz_m[1] < min.xyz_m[1] )
   {
      xyz_m[1] = min.xyz_m[1];
   }
   if( xyz_m[2] < min.xyz_m[2] )
   {
      xyz_m[2] = min.xyz_m[2];
   }

   return *this;
}


Vector3r& Vector3r::clampMaxEq
(
   const Vector3r& max
)
{
   if( xyz_m[0] > max.xyz_m[0] )
   {
      xyz_m[0] = max.xyz_m[0];
   }
   if( xyz_m[1] > max.xyz_m[1] )
   {
      xyz_m[1] = max.xyz_m[1];
   }
   if( xyz_m[2] > max.xyz_m[2] )
   {
      xyz_m[2] = max.xyz_m[2];
   }

   return *this;
}


Vector3r& Vector3r::clampEq
(
   const Vector3r& min,
   const Vector3r& max
)
{
   if( xyz_m[0] < min.xyz_m[0] )
      xyz_m[0] = min.xyz_m[0];
   else
   if( xyz_m[0] > max.xyz_m[0] )
      xyz_m[0] = max.xyz_m[0];

   if( xyz_m[1] < min.xyz_m[1] )
      xyz_m[1] = min.xyz_m[1];
   else
   if( xyz_m[1] > max.xyz_m[1] )
      xyz_m[1] = max.xyz_m[1];

   if( xyz_m[2] < min.xyz_m[2] )
      xyz_m[2] = min.xyz_m[2];
   else
   if( xyz_m[2] > max.xyz_m[2] )
      xyz_m[2] = max.xyz_m[2];

   return *this;
}


Vector3r& Vector3r::clamp01Eq()
{
   // [0,1)  0 to almost 1

   if( xyz_m[0] < real() )
      xyz_m[0] = real();
   else
   if( xyz_m[0] >= real(1.0f) )
      xyz_m[0]  = REAL_ALMOST_ONE;

   if( xyz_m[1] < real() )
      xyz_m[1] = real();
   else
   if( xyz_m[1] >= real(1.0f) )
      xyz_m[1]  = REAL_ALMOST_ONE;

   if( xyz_m[2] < real() )
      xyz_m[2] = real();
   else
   if( xyz_m[2] >= real(1.0f) )
      xyz_m[2]  = REAL_ALMOST_ONE;

   return *this;
}*/




/// queries --------------------------------------------------------------------
/*void Vector3r::get
(
   real& x,
   real& y,
   real& z
) const
{
   x = xyz_m[0];
   y = xyz_m[1];
   z = xyz_m[2];
}


void Vector3r::get
(
   real xyz[3]
) const
{
   xyz[0] = xyz_m[0];
   xyz[1] = xyz_m[1];
   xyz[2] = xyz_m[2];
}*/


/*real Vector3r::sum() const
{
   return xyz_m[0] + xyz_m[1] + xyz_m[2];
}


real Vector3r::average() const
{
   static const real ONE_OVER_3 = real(1.0f) / real(3.0f);

   return (xyz_m[0] + xyz_m[1] + xyz_m[2]) * ONE_OVER_3;
}


real Vector3r::smallest() const
{
   real smallest = xyz_m[0] <= xyz_m[1] ? xyz_m[0] : xyz_m[1];
   smallest      = smallest <= xyz_m[2] ? smallest : xyz_m[2];

   return smallest;
}


real Vector3r::largest() const
{
   real largest = xyz_m[0] >= xyz_m[1] ? xyz_m[0] : xyz_m[1];
   largest      = largest  >= xyz_m[2] ? largest  : xyz_m[2];

   return largest;
}*/


real Vector3r::length() const
{
   return sqrt(
      (xyz_m[0] * xyz_m[0]) +
      (xyz_m[1] * xyz_m[1]) +
      (xyz_m[2] * xyz_m[2]) );
}


/*real Vector3r::dot
(
   const Vector3r& v
) const
{
   return
      (xyz_m[0] * v.xyz_m[0]) +
      (xyz_m[1] * v.xyz_m[1]) +
      (xyz_m[2] * v.xyz_m[2]);
}


real Vector3r::distance
(
   const Vector3r& v
) const
{
   const real xDif = xyz_m[0] - v.xyz_m[0];
   const real yDif = xyz_m[1] - v.xyz_m[1];
   const real zDif = xyz_m[2] - v.xyz_m[2];

   return sqrt( (xDif * xDif) + (yDif * yDif) + (zDif * zDif) );
}


real Vector3r::distance2
(
   const Vector3r& v
) const
{
   const real xDif = xyz_m[0] - v.xyz_m[0];
   const real yDif = xyz_m[1] - v.xyz_m[1];
   const real zDif = xyz_m[2] - v.xyz_m[2];

   return (xDif * xDif) + (yDif * yDif) + (zDif * zDif);
}*/


/*Vector3r Vector3r::operator-() const
{
   return Vector3r( -xyz_m[0], -xyz_m[1], -xyz_m[2] );
}


Vector3r Vector3r::abs() const
{
   return Vector3r(
      xyz_m[0] >= real() ? xyz_m[0] : -xyz_m[0],
      xyz_m[1] >= real() ? xyz_m[1] : -xyz_m[1],
      xyz_m[2] >= real() ? xyz_m[2] : -xyz_m[2] );
}


Vector3r Vector3r::unitized() const
{
   const real length = sqrt(
      (xyz_m[0] * xyz_m[0]) +
      (xyz_m[1] * xyz_m[1]) +
      (xyz_m[2] * xyz_m[2]) );
   const real oneOverLength = (length != real()) ?
      real(1.0) / length : real();

   return Vector3r(
      xyz_m[0] * oneOverLength,
      xyz_m[1] * oneOverLength,
      xyz_m[2] * oneOverLength );
}


Vector3r Vector3r::cross
(
   const Vector3r& v
) const
{
   return Vector3r(
      (xyz_m[1] * v.xyz_m[2]) - (xyz_m[2] * v.xyz_m[1]),
      (xyz_m[2] * v.xyz_m[0]) - (xyz_m[0] * v.xyz_m[2]),
      (xyz_m[0] * v.xyz_m[1]) - (xyz_m[1] * v.xyz_m[0]) );
}*/


Vector3r Vector3r::operator+
(
   const Vector3r& v
) const
{
   return Vector3r(
      xyz_m[0] + v.xyz_m[0],
      xyz_m[1] + v.xyz_m[1],
      xyz_m[2] + v.xyz_m[2] );
}


Vector3r Vector3r::operator-
(
   const Vector3r& v
) const
{
   return Vector3r(
      xyz_m[0] - v.xyz_m[0],
      xyz_m[1] - v.xyz_m[1],
      xyz_m[2] - v.xyz_m[2] );
}


Vector3r Vector3r::operator*
(
   const Vector3r& v
) const
{
   return Vector3r(
      xyz_m[0] * v.xyz_m[0],
      xyz_m[1] * v.xyz_m[1],
      xyz_m[2] * v.xyz_m[2] );
}


Vector3r Vector3r::operator/
(
   const Vector3r& v
) const
{
   return Vector3r(
      xyz_m[0] / v.xyz_m[0],
      xyz_m[1] / v.xyz_m[1],
      xyz_m[2] / v.xyz_m[2] );
}


Vector3r Vector3r::operator*
(
   const real r
) const
{
   return Vector3r(
      xyz_m[0] * r,
      xyz_m[1] * r,
      xyz_m[2] * r );
}


Vector3r Vector3r::operator/
(
   const real r
) const
{
   const real oneOverR = real(1.0f) / r;

   return Vector3r(
      xyz_m[0] * oneOverR,
      xyz_m[1] * oneOverR,
      xyz_m[2] * oneOverR );
}


bool Vector3r::operator==
(
   const Vector3r& v
) const
{
   return
      (xyz_m[0] == v.xyz_m[0]) &
      (xyz_m[1] == v.xyz_m[1]) &
      (xyz_m[2] == v.xyz_m[2]);
}


bool Vector3r::operator!=
(
   const Vector3r& v
) const
{
   return
      (xyz_m[0] != v.xyz_m[0]) |
      (xyz_m[1] != v.xyz_m[1]) |
      (xyz_m[2] != v.xyz_m[2]);
}


bool Vector3r::isZero() const
{
   return bool(
      (xyz_m[0] == real()) &
      (xyz_m[1] == real()) &
      (xyz_m[2] == real()) );
}


/*Vector3r Vector3r::sign() const
{
   return compare( Vector3r::ZERO() );
}


Vector3r Vector3r::compare
(
   const Vector3r& v
) const
{
   return Vector3r(
      xyz_m[0] > v.xyz_m[0] ?
         real(+1.0f) : (xyz_m[0] == v.xyz_m[0] ? real() : real(-1.0f)),
      xyz_m[1] > v.xyz_m[1] ?
         real(+1.0f) : (xyz_m[1] == v.xyz_m[1] ? real() : real(-1.0f)),
      xyz_m[2] > v.xyz_m[2] ?
         real(+1.0f) : (xyz_m[2] == v.xyz_m[2] ? real() : real(-1.0f)) );
}


Vector3r Vector3r::equal
(
   const Vector3r& v
) const
{
   return Vector3r(
      static_cast<real>( xyz_m[0] == v.xyz_m[0] ),
      static_cast<real>( xyz_m[1] == v.xyz_m[1] ),
      static_cast<real>( xyz_m[2] == v.xyz_m[2] ) );
}


Vector3r Vector3r::operator>
(
   const Vector3r& v
) const
{
   return Vector3r(
      static_cast<real>( xyz_m[0] > v.xyz_m[0] ),
      static_cast<real>( xyz_m[1] > v.xyz_m[1] ),
      static_cast<real>( xyz_m[2] > v.xyz_m[2] ) );
}


Vector3r Vector3r::operator>=
(
   const Vector3r& v
) const
{
   return Vector3r(
      static_cast<real>( xyz_m[0] >= v.xyz_m[0] ),
      static_cast<real>( xyz_m[1] >= v.xyz_m[1] ),
      static_cast<real>( xyz_m[2] >= v.xyz_m[2] ) );
}


Vector3r Vector3r::operator<
(
   const Vector3r& v
) const
{
   return Vector3r(
      static_cast<real>( xyz_m[0] < v.xyz_m[0] ),
      static_cast<real>( xyz_m[1] < v.xyz_m[1] ),
      static_cast<real>( xyz_m[2] < v.xyz_m[2] ) );
}


Vector3r Vector3r::operator<=
(
   const Vector3r& v
) const
{
   return Vector3r(
      static_cast<real>( xyz_m[0] <= v.xyz_m[0] ),
      static_cast<real>( xyz_m[1] <= v.xyz_m[1] ),
      static_cast<real>( xyz_m[2] <= v.xyz_m[2] ) );
}*/


/*Vector3r Vector3r::clampedMin
(
   const Vector3r& min
) const
{
   Vector3r v( *this );

   if( v.xyz_m[0] < min.xyz_m[0] )
   {
      v.xyz_m[0] = min.xyz_m[0];
   }
   if( v.xyz_m[1] < min.xyz_m[1] )
   {
      v.xyz_m[1] = min.xyz_m[1];
   }
   if( v.xyz_m[2] < min.xyz_m[2] )
   {
      v.xyz_m[2] = min.xyz_m[2];
   }

   return v;
}


Vector3r Vector3r::clampedMax
(
   const Vector3r& max
) const
{
   Vector3r v( *this );

   if( v.xyz_m[0] > max.xyz_m[0] )
   {
      v.xyz_m[0] = max.xyz_m[0];
   }
   if( v.xyz_m[1] > max.xyz_m[1] )
   {
      v.xyz_m[1] = max.xyz_m[1];
   }
   if( v.xyz_m[2] > max.xyz_m[2] )
   {
      v.xyz_m[2] = max.xyz_m[2];
   }

   return v;
}


Vector3r Vector3r::clamped
(
   const Vector3r& min,
   const Vector3r& max
) const
{
   Vector3r v( *this );

   if( v.xyz_m[0] < min.xyz_m[0] )
      v.xyz_m[0] = min.xyz_m[0];
   else
   if( v.xyz_m[0] > max.xyz_m[0] )
      v.xyz_m[0] = max.xyz_m[0];

   if( v.xyz_m[1] < min.xyz_m[1] )
      v.xyz_m[1] = min.xyz_m[1];
   else
   if( v.xyz_m[1] > max.xyz_m[1] )
      v.xyz_m[1] = max.xyz_m[1];

   if( v.xyz_m[2] < min.xyz_m[2] )
      v.xyz_m[2] = min.xyz_m[2];
   else
   if( v.xyz_m[2] > max.xyz_m[2] )
      v.xyz_m[2] = max.xyz_m[2];

   return v;
}


Vector3r Vector3r::clamped01() const
{
   // [0,1)  0 to almost 1

   Vector3r v( *this );

   if( v.xyz_m[0] < real() )
      v.xyz_m[0] = real();
   else
   if( v.xyz_m[0] >= real(1.0f) )
      v.xyz_m[0] = REAL_ALMOST_ONE;

   if( v.xyz_m[1] < real() )
      v.xyz_m[1] = real();
   else
   if( v.xyz_m[1] >= real(1.0f) )
      v.xyz_m[1] = REAL_ALMOST_ONE;

   if( v.xyz_m[2] < real() )
      v.xyz_m[2] = real();
   else
   if( v.xyz_m[2] >= real(1.0f) )
      v.xyz_m[2] = REAL_ALMOST_ONE;

   return v;
}*/




/// constants ------------------------------------------------------------------
const Vector3r& Vector3r::ZERO()
{
   static const Vector3r k( real(0.0f), real(0.0f), real(0.0f) );
   return k;
}


const Vector3r& Vector3r::HALF()
{
   static const Vector3r k( real(0.5f), real(0.5f), real(0.5f) );
   return k;
}


const Vector3r& Vector3r::ONE()
{
   static const Vector3r k( real(1.0f), real(1.0f), real(1.0f) );
   return k;
}


/*const Vector3r& Vector3r::EPSILON()
{
   static const Vector3r k( REAL_EPSILON, REAL_EPSILON, REAL_EPSILON );
   return k;
}


const Vector3r& Vector3r::ALMOST_ONE()
{
   static const Vector3r k( REAL_ALMOST_ONE, REAL_ALMOST_ONE, REAL_ALMOST_ONE );
   return k;
}


const Vector3r& Vector3r::MIN()
{
   static const Vector3r k( REAL_MIN_NEG, REAL_MIN_NEG, REAL_MIN_NEG );
   return k;
}


const Vector3r& Vector3r::MAX()
{
   static const Vector3r k( REAL_MAX, REAL_MAX, REAL_MAX );
   return k;
}


const Vector3r& Vector3r::SMALL()
{
   static const Vector3r k( REAL_SMALL, REAL_SMALL, REAL_SMALL );
   return k;
}


const Vector3r& Vector3r::LARGE()
{
   static const Vector3r k( REAL_LARGE, REAL_LARGE, REAL_LARGE );
   return k;
}


const Vector3r& Vector3r::X()
{
   static const Vector3r k( real(1.0f), real(0.0f), real(0.0f) );
   return k;
}


const Vector3r& Vector3r::Y()
{
   static const Vector3r k( real(0.0f), real(1.0f), real(0.0f) );
   return k;
}


const Vector3r& Vector3r::Z()
{
   static const Vector3r k( real(0.0f), real(0.0f), real(1.0f) );
   return k;
}*/




/// friends --------------------------------------------------------------------
Vector3r hxa7241_graphics::operator*
(
   const real      r,
   const Vector3r& v
)
{
   return Vector3r(
      r * v.xyz_m[0],
      r * v.xyz_m[1],
      r * v.xyz_m[2] );
}


Vector3r hxa7241_graphics::operator/
(
   const real      r,
   const Vector3r& v
)
{
   return Vector3r(
      r / v.xyz_m[0],
      r / v.xyz_m[1],
      r / v.xyz_m[2] );
}
