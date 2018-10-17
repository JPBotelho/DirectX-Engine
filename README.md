# Barebones Direct3D Engine

Written in C++ using Direct3D 11.

This is a very ~~shitty~~ barebones engine made only as a way of learning some more C++ and how engines are built.

### Features

Here's what I implemented and is available in this repository:
  - Constant buffers
  - Texture buffers
  - Vertex/index buffer
  - Vertex/fragment shader loading from file (using D3D functions)
  - Logging to file
  - Obj2World/Projection matrices
  
Here's what's included but was not implemented by me:
  [.Obj Model loader [MIT License]](https://github.com/Bly7/OBJ-Loader)
  
### Issues

The draw call for every frame has a hardcoded number of vertices to draw and does not adjust for the mesh that is being loaded. 
If the hardcoded vertex count is below the one for the mesh, not everything will be drawn.


There might be a few unnecessary classes within the project that I was experimenting with but ended up not being necessary.


There is not a decent pipeline for shaders/models, everything is hardcoded.

## This project is no longer maintained. Code is up for reference.
  
  
  
