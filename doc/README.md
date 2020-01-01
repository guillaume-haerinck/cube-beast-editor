# Beast Voxel Editor post-mortem


## Introduction

## I - Planning, research and analysis

### App & data structure

- ECS
- Met ecs
- Singleton components
- previous project (opengl-playground)

### Voxel editors analysis

- Blender, voxedit, magicavoxel, goxel
- Gui
- Renderer workflow with renderdoc & nvidia nsight

### Wasm and Emscripten

- OpenGL es constraints
- Using emscripten directly

### OpenGL renderer abstraction

- constant buffers instead of uniforms
- instanced rendering
- rename opengl Object to well known equivalent closer to DirectX (uniform buffer object -> constant buffer | framebuffer -> renderTarget | vertex array -> deleted )

### Continous integration

- Unit test
- Automatic deploy

## II - Core features

### Arcball Camera

- Panning

### Rendering system and passes

- per-vertex normals with a cube with 8 vertex is less efficient as we have to interpolate between 4 each time to get surface normal
- Uber shader to make only one pass instead of multiple with lots of binding and unbinding (measure performance gain)
- Render target with ints are a pain to use (unknown values, clear different, etc), so cast entities to float to improve it
- glVertexAttribPointer and glVertexAttribIPointer not to interpolate int values

### Brushes

- Voxel add tool, explosion if just add. Must limit to 1 add in height per click.
- Color painting

### Procedural terrain generation

### Selection

- Framebuffer with color picking
- Raycasting

## III - Optionnal (but super cool) features

### ImGui docking branch

imgui imagebutton not working correctly when pressed, use font icon instead

### Undo & redo

- First think with enum
- command pattern

### Shadow mapping

### SSAO

### Benchmarking

## IV - Planned features

### Improved data structure

Sparse set might be way more efficient to store and access cube data, might ditch ecs in the future as need access in O(1) of pos for advanced brushes, and cube storage in multiple chunks instead of one big array
-> ECS might not be the best solution, but it's the one which will help me learn more for now (benchmarking, ecs use case, improving my lib)

### Remesh
See if can store neightboring cubes as one rectangle, multiple is there is a hole. Render with instanced plane, but different model mats

### Object mode

### Parenting

### Modifiers

## Conclusion
