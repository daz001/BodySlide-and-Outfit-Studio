Modifications - by daz001
===========================
* Added the ability to keep zap sliders when loading a reference body
* Added 'Slider->Bake Conversion Ref' menu item
   - This menu item can be used wehn converting between body references in order to exclude the need to 'export to nif with reference'. Instead, this will simply bake the bodyslide used from the conversion reference into the mesh and you can simply continue using the mesh.
* Added 'File->Convert Body Reference' menu item
   - This will allow you to 1 click convert between body references (ie. CBBE to BHUNP)
   - All settings that are set on the wizard dialog are remembered between sessions - so you only need to enter them once for all of the outfits you wish to convert
   - How to use:
		1) Open your outfit project
		2) open the wizard ('File->Convert Body Reference')
		3) Select the conversion reference (ie. Convert: CBBE SE to BHUNP)
		4) Select the new body reference (ie. BHUNP 3BBB Advanced ver 2)
		5) Enter any text that you would like removing from outfit name, slider set file, shape data folder and shape data file - including spaces (ie. 'CBBE, Physics, p, P, SSE')
		6) Enter any text that you would like prepending to outfit name, slider set file, shape data folder and shape data file - including spaces (ie. 'BHUNP, Physics, p, P, SSE')
		7) Click OK, wait for the process to complete and you can then instantly save the project (which will use the updated project name values) or adjust weights and then save.
		
		
<img src="https://i.imgur.com/RNCu6Lf.png" width="400">



BodySlide-and-Outfit-Studio
===========================

BodySlide and Outfit Studio, a tool to convert, create, and customize outfits and bodies for The Elder Scrolls and Fallout.

**Created by and/or with the help of:**
* Caliente
* ousnius
* jonwd7 for NIF and general help
* degenerated1123 for help with shaders
* NifTools team

**Libraries used:**
* OpenGL
* OpenGL Image (GLI)
* Simple OpenGL Image Library 2 (SOIL2)
* TinyXML-2
* FSEngine (BSA/BA2 library)
* Autodesk FBX SDK
* half - IEEE 754-based half-precision floating point library
* Miniball
* LZ4(F)
* wxWidgets

https://github.com/ousnius/BodySlide-and-Outfit-Studio/wiki