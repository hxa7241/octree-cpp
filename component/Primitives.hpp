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


#ifndef Primitives_h
#define Primitives_h


#include <limits.h>
#include <float.h>




namespace hxa7241
{


/// types ----------------------------------------------------------------------

// remove definition to set real to double
#define REAL_IS_FLOAT

   typedef  signed   char   byte;
   typedef  unsigned char   ubyte;

   typedef  signed   short  word;
   typedef  unsigned short  uword;

   typedef  signed   int    dword;
   typedef  unsigned int    udword;

#ifdef REAL_IS_FLOAT
   typedef  float           real;
#else
   typedef  double          real;
#endif


/// constants ------------------------------------------------------------------

   // limits.h version

   const byte   BYTE_MIN    = SCHAR_MIN;
   const byte   BYTE_MAX    = SCHAR_MAX;
   const int    BYTE_BITS   = 8;

   const ubyte  UBYTE_MIN   = 0;
   const ubyte  UBYTE_MAX   = UCHAR_MAX;
   const int    UBYTE_BITS  = 8;


   const word   WORD_MIN    = SHRT_MIN;
   const word   WORD_MAX    = SHRT_MAX;
   const int    WORD_BITS   = 16;

   const uword  UWORD_MIN   = 0;
   const uword  UWORD_MAX   = USHRT_MAX;
   const int    UWORD_BITS  = 16;


   const dword  DWORD_MIN   = INT_MIN;
   const dword  DWORD_MAX   = INT_MAX;
   const int    DWORD_BITS  = 32;

   const udword UDWORD_MIN  = 0;
   const udword UDWORD_MAX  = UINT_MAX;
   const int    UDWORD_BITS = 32;


   const float  FLOAT_MIN_POS     = static_cast<float>(FLT_MIN);
   const float  FLOAT_MIN_NEG     = static_cast<float>(-FLT_MAX);
   const float  FLOAT_MAX         = static_cast<float>(FLT_MAX);
   const float  FLOAT_EPSILON     = static_cast<float>(FLT_EPSILON);
   const float  FLOAT_ALMOST_ONE  = static_cast<float>(1.0f - FLT_EPSILON);
   const float  FLOAT_SMALL       = static_cast<float>(1.0e-12f);
   const float  FLOAT_LARGE       = static_cast<float>(1.0e+12f);

   const double DOUBLE_MIN_POS    = static_cast<double>(DBL_MIN);
   const double DOUBLE_MIN_NEG    = static_cast<double>(-DBL_MAX);
   const double DOUBLE_MAX        = static_cast<double>(DBL_MAX);
   const double DOUBLE_EPSILON    = static_cast<double>(DBL_EPSILON);
   const double DOUBLE_ALMOST_ONE = static_cast<double>(1.0 - DBL_EPSILON);
   const double DOUBLE_SMALL      = static_cast<double>(1.0e-96);
   const double DOUBLE_LARGE      = static_cast<double>(1.0e+96);

#ifdef REAL_IS_FLOAT
   const float  REAL_MIN_POS      = FLOAT_MIN_POS;
   const float  REAL_MIN_NEG      = FLOAT_MIN_NEG;
   const float  REAL_MAX          = FLOAT_MAX;
   const float  REAL_EPSILON      = FLOAT_EPSILON;
   const float  REAL_ALMOST_ONE   = FLOAT_ALMOST_ONE;
   const float  REAL_SMALL        = FLOAT_SMALL;
   const float  REAL_LARGE        = FLOAT_LARGE;
#else
   const double REAL_MIN_POS      = DOUBLE_MIN_POS;
   const double REAL_MIN_NEG      = DOUBLE_MIN_NEG;
   const double REAL_MAX          = DOUBLE_MAX;
   const double REAL_EPSILON      = DOUBLE_EPSILON;
   const double REAL_ALMOST_ONE   = DOUBLE_ALMOST_ONE;
   const double REAL_SMALL        = DOUBLE_SMALL;
   const double REAL_LARGE        = DOUBLE_LARGE;
#endif


   // limits version

/*
   const byte   BYTE_MIN    = std::numeric_limits<byte>::min();
   const byte   BYTE_MAX    = std::numeric_limits<byte>::max();
   const int    BYTE_BITS   = 8;

   const ubyte  UBYTE_MIN   = std::numeric_limits<ubyte>::min();
   const ubyte  UBYTE_MAX   = std::numeric_limits<ubyte>::max();
   const int    UBYTE_BITS  = 8;


   const word   WORD_MIN    = std::numeric_limits<word>::min();
   const word   WORD_MAX    = std::numeric_limits<word>::max();
   const int    WORD_BITS   = 16;

   const uword  UWORD_MIN   = std::numeric_limits<uword>::min();
   const uword  UWORD_MAX   = std::numeric_limits<uword>::max();
   const int    UWORD_BITS  = 16;


   const dword  DWORD_MIN   = std::numeric_limits<dword>::min();
   const dword  DWORD_MAX   = std::numeric_limits<dword>::max();
   const int    DWORD_BITS  = 32;

   const udword UDWORD_MIN  = std::numeric_limits<udword>::min();
   const udword UDWORD_MAX  = std::numeric_limits<udword>::max();
   const int    UDWORD_BITS = 32;


   const float  FLOAT_MIN_POS     = std::numeric_limits<float>::min();
   const float  FLOAT_MIN_NEG     =
      static_cast<float>(-std::numeric_limits<float>::max());
   const float  FLOAT_MAX         = std::numeric_limits<float>::max();
   const float  FLOAT_EPSILON     = std::numeric_limits<float>::epsilon();
   const float  FLOAT_ALMOST_ONE  =
      static_cast<float>(1.0f - std::numeric_limits<float>::epsilon());
   const float  FLOAT_SMALL       = static_cast<float>(1.0e-12f);
   const float  FLOAT_LARGE       = static_cast<float>(1.0e+12f);

   const double DOUBLE_MIN_POS    = std::numeric_limits<double>::min();
   const double DOUBLE_MIN_NEG    =
      static_cast<double>(-std::numeric_limits<double>::max());
   const double DOUBLE_MAX        = std::numeric_limits<double>::max();
   const double DOUBLE_EPSILON    = std::numeric_limits<double>::epsilon();
   const double DOUBLE_ALMOST_ONE =
      static_cast<double>(1.0 - std::numeric_limits<double>::epsilon());
   const double DOUBLE_SMALL      = static_cast<double>(1.0e-96);
   const double DOUBLE_LARGE      = static_cast<double>(1.0e+96);

#ifdef REAL_IS_FLOAT
   const float  REAL_MIN_POS      = FLOAT_MIN_POS;
   const float  REAL_MIN_NEG      = FLOAT_MIN_NEG;
   const float  REAL_MAX          = FLOAT_MAX;
   const float  REAL_EPSILON      = FLOAT_EPSILON;
   const float  REAL_ALMOST_ONE   = FLOAT_ALMOST_ONE;
   const float  REAL_SMALL        = FLOAT_SMALL;
   const float  REAL_LARGE        = FLOAT_LARGE;
#else
   const double REAL_MIN_POS      = DOUBLE_MIN_POS;
   const double REAL_MIN_NEG      = DOUBLE_MIN_NEG;
   const double REAL_MAX          = DOUBLE_MAX;
   const double REAL_EPSILON      = DOUBLE_EPSILON;
   const double REAL_ALMOST_ONE   = DOUBLE_ALMOST_ONE;
   const double REAL_SMALL        = DOUBLE_SMALL;
   const double REAL_LARGE        = DOUBLE_LARGE;
#endif
*/


}//namespace




#endif//Primitives_h
