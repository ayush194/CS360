# RoomShader
                                                                  _               _           
                                  _ __ ___   ___  _ __ ___    ___| |__   __ _  __| | ___ _ __ 
                                 | '__/ _ \ / _ \| '_ ` _ \  / __| '_ \ / _` |/ _` |/ _ \ '__|
                                 | | | (_) | (_) | | | | | | \__ \ | | | (_| | (_| |  __/ |   
                                 |_|  \___/ \___/|_| |_| |_| |___/_| |_|\__,_|\__,_|\___|_|   
                                 

---------------

This is a simple room shader built for Assignment 2 of CS360 course at IIT Kanpur. The room has been modelled and uv mapped in Blender. The source code provides the implementation for loading the object files and the textures (stb_library has been used for loading images as textures) as well as for rendering the loaded object. The shaders implement the Blinn-Phong model for Diffuse and Specular shading.

Use the mouse to look around, pan to zoom in and out, and the keys A, S, W, D to move left, backwards, forwards and right respectively.

---------------

## Build Instructions:

To build the project, a working C++11 compiler is needed.
For linux one can use the preinstalled g++ compiler.
On OSX, the clang compiler comes packaged with XCode. 

## Dependencies
1. [GLFW](https://www.glfw.org/) is a window loading library for OpenGL. It initializes the window for rendering. It also provides event handlers for receiving input and displaying the output.

2. [GLEW](http://glew.sourceforge.net/) is a function loading library for OpenGL. It loads OpenGL function pointers at runtime.

3. [glm](https://glm.g-truc.net/0.9.9/index.html) is a library for performing efficient matrix operations. Matrix operations are frequently performed in OpenGL.

Install these dependencies as they are required for building the project.

### For Linux
```
sudo apt-get update
sudo apt-get install libglfw3-dev libglew-dev libglm-dev
```

### For OSX
The easiest way to install these libraries is using the brew package manager. For a more custom install, build using the source files from the latest development branch.
```
brew install glew glfw glm
```

## Compiling and Running
Now simply clone the this repository and build using the shell script provided.

### For Linux and OSX
```
https://github.com/ayush194/CS360.git
cd CS360/RoomShader/
sh compile.sh
./roomshader
```

---------------

## Assignment 2

Built for course CS360 (Introduction to Computer Graphics)
Name: Ayush Kumar
Roll No: 170195
B.Tech CSE, IIT Kanpur

