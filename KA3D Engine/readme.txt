KA3D 1.0 For Win32/DirectX
--------------------------


Overview
--------

This package contains Win32/DirectX version of KA3D 3D-engine.


Runtime system requirements
---------------------------

- Windows XP or 2000
- DirectX 9.0 April 2005 Update SDK

Engine does not require hardware pixel shader support, but currently 
fixed pipeline implementation of shaders is not yet available.

Note that the Debug Build examples don't start unless you have
DirectX 9.0 SDK installed on your computer. Release Build works
even without SDK as long as DirectX 9.0 Runtime has been installed.


Supported compilers
-------------------

In this release, only Microsoft Visual Studio Net 2003 is supported.
Mingw32-gcc makefile version is also available for Win32/DirectX,
but it is not included in this release.


Directory structure
-------------------

build/
  msvc7/
  - Microsoft Visual Studio 7 project files
  
data/
  images/
  - Test images
  
  lua/
  - Test Lua scripts
  
  meshes/
  - Test scenes (.max and exported .hgr)
  
  shaders/
  - current shader set
  
docs/
- Engine documentation

include/
- Include files. Add this and include/config/msvc7 to include path.

lib/
- Library files. Add lib/msvc7 to library search path.

samples/
- Sample code for the engine


How to get started
------------------

1) Set KA3D environment variable to point to the install directory
   of the engine.
   
2) Check that the engine works on your computer: Run hgrviewer
   (compiled binary executable included in tools package)
   and try to open scene files in samples/ directory.
   If the scenes don't start, you probably don't have
   3D-graphics hardware which supports Pixel Shader 1.1

3) Browse API reference a bit to get an overview what's inside.
   Read at least overviews of each module included in the core engine.

4) Step through the tutorials.
   By stepping through from low level to high level you gain better
   undestanding what is happening inside and what support which   
   components provides in the graphics engine. By starting from low 
   level you have less abstract components to understand.

5) Read docs/project_setup_guide.txt for instructions to 
   create your own MSVC7 project file.
   
6) Test you you got the settings right by compiling some (or all) of the
   sample applications included in samples/ directory.


Known issues
------------

- Doxygen does not generate documentation for all classes properly


Changes by version
------------------

0.2.1.0:
- Added unlit and diffuse skinned shaders
- DirectX 9.0 February 2005 version

0.2.0.0:
- Improved shader set (better normal mapping etc.)
- Parallax normal mapping shader
- Normal mapped lightmapping
- Support for loading normal textures as cube textures
- Rectangle based blt support to Texture

0.1.9.0:
- Changed internal shader matrix setting (to conform better 3dsmax7)
- Added Scene::getBoundBox and Lines::addCylinder

0.1.8.0:
- Added printf(x,y,...) and drawImage(x,y,subimage,...) to Console
- Added globals() table returning function to LuaState
- Added LuaTable::size() which returns num of elements
- Added background color setting to present()
- Changed to reflection vector usage in bump-refl shader
- Fixed reflection channel exporter in Std mtl / 3dsmax

0.1.7.0:
- Updated tutorial source code & data

0.1.6.0:
- Added cube map support
- Added bump reflection shader (bump-refl.fx)

0.1.5.0:
- Added lightmap support
- Added pixel shader 2.0 support
- Added texture locking support
- Added mouse input support to sample framework
- Added more tutorials
- Added simple key press recording functionality to the framework
- Improved physics docs a bit and added some wrapper functions to odex.h
- Renamed physics 'Body' tag to easier to understand 'Physics' tag

0.1.4.0:
- Added rigid body physics and collisions support (using ODE & OPCODE)
- Added text rendering console for debug/UI (see hgr::Console)
- Shared rendering pipeline setup for multipass rendering
- Added fog pipeline
- More accurate view frustum checks
- Optimized particle rendering
- Added box and sphere drawing to hgr::Lines
- Added bounding box to visuals (either in world or model space)
- Better reporting of unset shader parameters
- Cleaned up inconsistency between static/dynamic shader parameters
- Added high resolution profiling class (see lang::Profile)
- Added 'KA3D' environment variable, see 'How to get started'
  (and removed 'PARTICLES' and 'SHADERS' environment variables)
- Added min/max priority range selection to rendering
- Various render pipeline and low level math optimizations
- Bugfix: Fixed particle visibility issues

0.1.3.1:
- Added support for Mesh 'local lights' (=more efficients light management)
- Added more keys and application requested quit to application framework
- Bugfix: float3x4::finite() accessed out of range row
- Fixed issues (animations not updated etc.) changes related
  to new DirextX SDK, DirectX Summer Update 2004, Effect changes

0.1.3.0:
- Added character animation and draw triangle tutorials
- Added hgr 1.3 and 1.4 file format support (see hgr-fileformat.txt)
- Added LuaTable serialization (no user read/write code needed)
- Added LuaState* convention user method registration support to Lua wrapper
- Added optional start delay to ParticleSystem
- Bugfix: Texture loading error message was formatted incorrectly
- Bugfix: Scene loading did not support multiple instances of same particle fx
- Bugfix: Fixed on-screen 'pixel garbage' on render-to-texture effects
- Bugfix: Glow effect was not occluded by non-glowing geometry

0.1.2.0:
- Particle system support (see hgr::ParticleSystem), incl. sceneviewer
- Added random variable utilities (see math::RandomUtil and math::Domain)
  useful in various context, including particle systems and AI
- Added user property support etc. to hgr-fileformat (v1.2), but
  the scene file loader is also compatible with v1.1 file format too
- Support for void return value C++ methods in Lua wrapper (see lua::Object)
- Added io::PropertyParser (to be used for example with node user properties)
- Added io::FindFile (and changed framework and sceneviewer to use it)
- Added new sample application, particle_test

0.1.1.0:
- Bugfix: Render target texture was not reset after task switch
- Bugfix: DX_Texture.cpp did not compile because of (unused) extern dependency
- Added screen capture support (jpg)
- Added scene merging support
- Minor file format changes (enum type independent shader params), hgr 1.1
- More keys supported in application framework
- Organized global variables (used in performance optimizations) 
  so that the whole core can be made re-entrant (and still efficient) 
  if needed by using thread local storage
- More unit/regression tests (lang)
- Documentation improvements

0.1.0.0:
- Initial version


Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
