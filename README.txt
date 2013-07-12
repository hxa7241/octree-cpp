

HXA7241 Octree Component C++ v2.1
======================================================================


Harrison Ainsworth / HXA7241 : 2004-2007.  
http://wwww.hxa7241.org/

2007-08-13




Contents
--------

* Description
* Usage
* Acknowledgements
* License




Description
-----------

This is a C++ octree source-code component. It can be used with any stored-item
type and inspection/search algorithm.

The core interface is like this:
* construction
   * create with size and limits
* commands
   * insert item
   * remove item
* queries
   * visit with user-defined operation

Its features are:
* non-intrusive (doesn't limit or oblige stored-item particularly)
* strongly typed (doesn't accept void pointers)
* doesn't manage item storage (just indexes them)
* minimal in storage space
* numerically sound (no gaps/overlaps between cells)
* moderately-strongly exception-safe (always maintains basic validity)
* moderately easy to use

Also included are source for: a test application, and an example application.

Code size is: core 1000 lines, support 400 lines, samples 900 lines. Exception
handling is used for standard storage exceptions. RTTI is not used. Only basic
use is made of templates. Standard C library dependencies are: limits.h,
float.h, and math.h.

There is an article on its technical-design:
http://www.hxa7241.org/articles/content/octree-general-cpp_hxa7241_2005.html




Usage
-----

The file OctreeExample.cpp contains a minimal example of usage.


### Customisation ###

There are two main things you must do: write derivative classes for
OctreeAgent<> and OctreeVisitor<>.

The OctreeAgent<> derivative requires one method override. This will have to
query your item and return whether it overlaps the supplied cell bound. There is
an optional override for multiple overlap tests at once -- this could be more
efficient since cell boundaries are shared.

The OctreeVisitor<> derivative(s) require three method overrides. These will
have to decide how to traverse and how to react. Any number of derivatives can
be written, one for each kind of search/inspection on the octree.

Both of these definitions, and any use of Octree, require you to #include
"Octree.hpp".


### Use ###

1. instantiate some items and instantiate an Octree
2. instantiate an OctreeAgentDerivative
3. insert/remove items into the Octree
4. instantiate various OctreeVisitorDerivatives, and use them to visit the
Octree
5. maybe repeat 3. and 4. in any order


### Building ###

#### Component

Add these files to your source directories:
* Octree .hpp/.cpp
* OctreeImplementation .hpp/.cpp
* OctreeAuxiliary .hpp/.cpp
* Array .hpp/.cpp
* Vector3r .hpp/.cpp
* Primitives .hpp
Add the .cpp ones to your compile scripts, and the .obj ones produced from the
.cpp ones to your link scripts.

Vector3r could probably easily be replaced by your own equivalent.
Primitives.hpp too maybe.

#### Samples

For Windows, try the build-vc.bat script for MS VC++ 2005. For Linux, try the
build-gcc script for GCC 3.3.5 (or later). Everything needed is in the supplied
archive (assuming the build environment and tools are already prepared). The
result is two programs: octreeexample and octreetest.




Acknowledgements
----------------

### C++ ###

* 'The C++ Programming Language' 3rd ed. - Stroustrup (Addison Wesley)
* 'Effective C++' 3rd ed. - Meyers (Addison Wesley)
* 'More Effective C++' - Meyers (Addison Wesley)
* 'Design Patterns' - Gamma, Helm, Johnson, Vlissides (Addison Wesley)
* 'Object Oriented Software Construction' 2nd ed. - Meyer (PrenticeHall)

### Tools ###

* TextPad 4.7.3 editor  
  http://www.textpad.com/
* MS Visual C++ 2005 compiler for Microsoft Windows  
  http://msdn.microsoft.com/vstudio/express/visualc/
* GCC 3.3.5 compiler for Suse GNU/Linux  
  http://gcc.gnu.org/




License
-------

### (New) BSD ###

Harrison Ainsworth / HXA7241 : 2004-2007.


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
