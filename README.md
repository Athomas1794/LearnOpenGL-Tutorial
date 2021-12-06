# LearnOpenGL-Tutorial
My first opengl project with notes at each step using Visual Studio 19

## Additional Dependencies
GLFW was compiled manually using CMAKE, and GLAD was used to load windows specific OpenGL functions for use based on your local graphics configurations.

Setup tutorial here: [https://learnopengl.com/Getting-started/Creating-a-window](https://learnopengl.com/Getting-started/Creating-a-window)


In Configuration Properties > VC++ Directories > Library Directories: `C:\OpenGL\lib;$(LibraryPath)`

In Configuration Properties > Linker > Input > Additional Dependencies: `glfw3.lib;opengl32.lib;%(AdditionalDependencies)`
