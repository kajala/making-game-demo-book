Zax Demo v0.7 Zeta
------------------

Character controls
------------------
Mouse           Turn left/right, up/down
W               Walk forward
A               Step left
D               Step right
Space           Jump Up
Up+Space        Jump Out
Left button     Draw weapon / shoot
Backspace       Drop weapon


Other keys
----------
F4              toggle slow motion (10%)
Shift+F4        toggle fast forward (3x)
F5              restart
F8              next debug info mode
Shift+F8        previous debug info modes
F9              start capturing frames
F11             play recorded key presses
F12             save key presses


Exporting level from 3dsmax
---------------------------

- PLAYER_START_POINT dummy must be defined somewhere

- Select 'Scale world units to meters' in export dialog


Static level lighting in 3dsmax using vertex colors
---------------------------------------------------

1. Select all objects

2. Use 3dsmax 'Assign Vertex Colors' utility plugin
   with following settings:
   - Channel: Vertex Color
   - Light Model: Shaded / Lighting (try both!)
   - Color assignment: Color by Vertex
   - Checked: Calculate Shadows
   - Checked: Use maps

3. Assign vcolor-tex, vcolor-alpha or vcolor-glow shaders to the materials 
   (For a quick test you can remove all shader tags with maxscript
   and then set both default diffuse and bump map material shaders 
   as 'vcolor-tex' in the export dialog. You can get glow
   by assigning vcolor-glow shaders later.)

4. Export scene using following settings:
   - Checked: Export Vertex Colors
   - Checked: Scale world units to meters

Note1: These instructions make the whole level shaded using 
texture modulated by vertex colors. You don't need to do this
for all objects if you don't want to.

Note2: Vertex colors can be displayed in 3dsmax viewports
by using check box 'Vertex Channel Display'
from Object Properties dialog.

Note3: vcolor-glow uses different glow strength channel
than bump-glow (vcolor-glow uses diffuse map alpha channel and
bump-glow uses normal map alpha channel). This is because 
vcolor-shaders use only one texture, and bump shaders use two.


Collision objects
-----------------

- 'Collision=only' user defined object property can be used to add 
  static level collision objects which are not visible in the level
  
- 'Collision=none' user defined object property can be used to 
  add level objects which are not collided against. All objects
  which can never be collided against should be tagged this way.


Particle effects
----------------

- 'Particle=name' user defined object property can be used to 
  add particle systems parented to objects, for example
  Particle=fire


Rigid body physics
------------------

- 'Physics=x', where x is box or sphere, enables rigid body
  physics for the object. Body size is computed automatically
  from object bounding box. See also 'Mass' and 'Density' below.
  
- 'Mass=x' sets total mass for the object when 'Physics=x' is 
  used. Note that 'Mass' and 'Density' are mutually exclusive.

- 'Density=x' sets size-relative mass for the object when 'Physics=<x>' is 
  used. Note that 'Mass' and 'Density' are mutually exclusive.


Exporting Zax character mesh
----------------------------

- Character mesh scene is expected to be in inches,
  but do NOT toggle 'Scale world units to meters' in export!

- Otherwise export mesh as usual scene to data/mesh/. 
  If the mesh scene contains animation it is ignored in the game.

 

Exporting character animations
------------------------------

- Animations are expected to be in inches as the character mesh,
  but do NOT toggle 'Scale world units to meters' in export!

- Export (only) with 'Animation only' checkbox checked to data/anim/.

- Movement speed of character is extracted directly from animations,
  so 'in-place' animation edit mode should be disabled while exporting,
  or otherwise the character won't be moving in the game either


Changes by version
------------------

0.7:
- Normal mapped lightmapping
- New bump and parallax mapping shaders 
  (parallax mapping not used in the demo tho)
- Added simple replay recording (F12) and playback (F11),
  works only for the character (can be replayed in the
  next run tho to repeat whole sequence).

0.6:
- Added basic game character scripting support
- Startpoint direction extracted from dummy object rotation
- Bugfix: Fixed 'character blackness' debug build bug
  (bug: ambient lighting variable was not reset at init)
- Bugfix: Fixed double jump
- Bugfix: Fixed crash if no user properties in level scene

0.5:
- Added lightmap support
- Added lightmap scene raytracing support (character shading)
- Object-based laser muzzleflash
- Weapon shaking fixed?

0.4:
- Proper aiming/shooting; now shooting direction is screen center
- Weapon rotation is aligned to match aiming direction
- Added projectiles
- Added dynamic objects (kind of was there already before)
- Dynamic objects can be shot at (shots have also impulse, 
  so you can 'shoot cans'. Currently the impulse is pretty 
  high, 0.5kg 100m/s, so 100kg crates work as cans too...)
- Weapon can be dropped (backspace)
- Made shooting particle to more laser-like
  (using 'sprite elasticity' particle system property)
- Added crosshair
- Crosshair shown only when character aiming/shooting
- Added simple configuration script to toggle fullscreen mode

0.3:
- Turning/tilting with mouse movement
- Aiming/shooting up/down
- Debug test: Ammo with lines

0.2.2:
- Added 'sound' property (doesn't do anything yet, tho)
- Added property type checking to loading

0.2.1:
- Changed 'body' tag to 'physics'
- Added docs about vertex lighting
- Re-organized docs (about physics, particles, collision objects)

0.2:
- Rigid body physics (try kicking crates around!)
- Added capped cylinder / capsule collisions to character
- Added fog rendering
- Added multiple debug info displays (F5 toggles, F6/Shift+F6 cycles)
- Added rendering pass toggling (F11-F12, +Shift)
- More accurate view frustum checks
- Changed particle effects' particle kill modes to RANDOM
- Changed Zax jumping to be exaggerated x5 (mainly test for now)
- Various render pipeline optimizations
- Bugfix: Fixed particle visibility issues

0.1:
- Initial version



All source code and executables copyright (C) 2005 Jani Kajala.
3D models, textures and other art copyright (C) 2005 Chad Walker.
