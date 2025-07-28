Temporarely abandoned

2D renderer based on OpenGL.

Features include:
*  Sprite sheets/atlases
      * The .cfg file also defines the sprites within(in pixels) and associates a name with them
*  Batches
      * Several batch types exist, each with varying complexity. Type FreeBatch is the norm for drawing any sprites, at any sizes and rotation angles, anywhere
*  Rendering using an orthogonal 3D projection to allow for intuitive layers based on the z-coordinate
*  Supplementory classes for input tracking, camera management and utility tools for diagnosing OpenGL and more

Incomplete features include:
*  GUI
      * As of now, the UI is in a visual only state. The windows in the UI system are supposed to be fully customisable, based on a 9-patch rectangle model
      * As far as graphics go, for now only the windows' backgrounds are drawn at the proper order and spacing
      * UI widgets are not yet implemented and the Window type's structure is debated
*  Asset manager
      * A proper asset manager that uses files for resource location and allocation
      * As of yet the format and parser for it are somewhat complete
      * It's supposed to be use stage loading, where all data is parsed then executed at the proper times

Future functionality:
*  VAO formatter
      * Idea will be explored in another repo. The concept is to have a format for a string, which would generate VAO formats for instanced and non-instanced
        rendering on the fly. Since OpenGL shaders work based on attribute locations(layout(location = 1)), only locations need to be consistent
      * This should allow for much more flexibility when one builds a game engine with this renderer, allowing for custom data layouts
*  Shader manager
      * This manager should work in cooperation with the resrouce manager, or perhaps simply expand it
      * The core idea is to allow much greater flexibility with the shaders. Since the vertex shader for those 2D sprites are relatively simple and barely change
        between different batch types, the only thing that the user should be able to change is the way the sprites are drawn
      * All shader pair's frag shader will be changed to a single function, which the user will define, possibly using some built-in macroes to quickly go over
        the details needed for a texture sampling function
      * Load-time handling of uniforms must be considered. Whether additional uniforms would be appendable in the source files or be strictly confined to the shader files
        
