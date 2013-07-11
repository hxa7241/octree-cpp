@echo off


rem --- using: MS VC++ 2005 ---


rem -- set constants -----------------------------------------------------------
set COMPILE=cl /c /O2 /GL /fp:fast /EHsc /GR- /GS- /MT /W4 /WL /nologo /Icomponent

set LINKE=link /LTCG /OPT:REF /OPT:NOWIN98 /VERSION:2.1 /NOLOGO
set LIBRARIES=kernel32.lib


mkdir obj
del /Q obj\*


rem -- compile -----------------------------------------------------------------
@echo.
@echo --- compile --

rem -- compile component --
%COMPILE% component/Array.cpp /Foobj/Array.obj
%COMPILE% component/Vector3r.cpp /Foobj/Vector3r.obj
%COMPILE% component/OctreeAuxiliary.cpp /Foobj/OctreeAuxiliary.obj
%COMPILE% component/OctreeImplementation.cpp /Foobj/OctreeImplementation.obj
%COMPILE% component/Octree.cpp /Foobj/Octree.obj

rem -- compile samples --
%COMPILE% /Isamples samples/OctreeStreamOut.cpp /Foobj/OctreeStreamOut.obj
%COMPILE% /Isamples samples/OctreeTest.cpp /Foobj/OctreeTest.obj
%COMPILE% /Isamples samples/OctreeExample.cpp /Foobj/OctreeExample.obj


rem -- link --------------------------------------------------------------------
@echo.
@echo --- link --

rem -- link test sample --
%LINKE% /OUT:octreetest.exe %LIBRARIES% obj/Array.obj obj/Vector3r.obj obj/OctreeAuxiliary.obj obj/OctreeImplementation.obj obj/Octree.obj obj/OctreeStreamOut.obj obj/OctreeTest.obj

rem -- link example sample --
%LINKE% /OUT:octreeexample.exe %LIBRARIES% obj/Array.obj obj/Vector3r.obj obj/OctreeAuxiliary.obj obj/OctreeImplementation.obj obj/Octree.obj obj/OctreeExample.obj


@echo.
@echo --- done --


del /Q obj\*
