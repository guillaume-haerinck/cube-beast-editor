# Beast Voxel Editor post-mortem

## Table of Content

[**Specifications**](#specifications)

[**Introduction**](#introduction)

[**I - Planning Research and Analysis**](#i---planning-research-and-analysis)
*	[**Voxel editors analysis**](#a---voxel-editors-analysis)

[**Conclusion**](#conclusion)

## Specifications

Beast Voxel Editor is a software which allows you to easily generate, visualize and edit 3D scenes. It has been started as a class project for the [IMAC engineering school](https://www.ingenieur-imac.fr/), and had some requirement to meet in order to be evaluated by our teachers. The original specifications can be found [here](https://docs.google.com/document/d/1aDahRcA8aeTSNqdqXv5i8FGPoVUmYzQ-k4z49wlu1K0/edit?usp=sharing), and we made a summary of these features below. 

The *D* column means *Done ?* and the *O* column means *Required ?*

| D | R | Description | Comments | Part |
| --- | --- | --- | --- | --- |
| X | X | Render a 3D scene with cubes | | I - C |
| | X | Select cube with keyboard | Replaced by mouse select| II - A |
| X | | Select cube with mouse | Raycast & Framebuffer color picking | II - A |
| X | X | Show selected cube with an outline | Multiple modes | |
| X | X | Paint selected cube | | |
| X | X | Add and Remove cubes | | I - C |
| | X | Extrude tool | TODO Use face mode with the pencil brush | |
| X | X | Procedural terrain generation with RBF | | |
| X | X | Directionnal light | | |
|  | X | Point light | TODO | |
| | | Paint adjacent faces | | |
| | | Proportional edit | |
| | | Save scene to custom file | |
| | | Load a 3D file (.gltf) | |
| | | Change grid size | |
| | | Textured cubes | |

Additionaly, we added features not mentionned in the specifications

| Description | Comments | Part |
| --- | --- | --- |
| Met ECS library | | |
| Shadows | | |
| Deferred shading | | |
| ArcBall Camera | |
| Benchmarking | | |
| WASM build support | | |
| ImGui Docking branch | | |
| SSAO | TODO | |

## Introduction

From the start, we wanted to create a **robust piece of software**. The kind of project that has its place amongst other similar applications. One that could compete with an elegant interface, a basic but well-designed set of tools, and a performant and yet easy to maintain architecture. That was the dream, and it was ambitious.

As the **4rd iteration** of its creators to create a 3D data-driven real-time application, we knew that we could build something better out of our past mistakes, but **a lot was still to learn**. [Mirrors, lasers and robots](https://github.com/guillaume-haerinck/imac-tower-defense) helped us to reason about the communication inside of our app, and playground apps for [OpenGL](https://github.com/guillaume-haerinck/opengl-playground) and [DirectX](https://github.com/guillaume-haerinck/directx11-playground) helped a lot to abstract our render engine. Do you want to know what this first version of Beast Voxel Editor teached us ?

Through this document, we will describe as simply and thoroughly as we can our development process. We will talk about the problems we ecountered, the solutions we found for them, and the missing bits of this first version (because we can make a good software, but we can't make time).

## I - Planning, research and analysis

> Where we take a step back to look at our surroundings and preparre for the task at hand.

Making a 3D voxel editor in 2019 can not be done in a Vacuum. Many other applications of this sort exist, are free, used by thousands of artists, and have made and fixed mistakes that we haven't encountered yet. That's why when we started, our first goal was to get **as much detail and information** as we could about the structure and inner working of these engines.

Based on those clues, we could start to plan our work, our application architecture and how to make the best out of it. We can never plan everything, but taking the time to think about our real needs was very important for us.
___

### A - Voxel editors analysis

We put 5 well-known softwares under our microscopes. Both in terms of user-interface, functionalities and render pipelines. Most of the time it was difficult to get more information on the rendering part, but we did get some usefull insights.

#### Goxel

Goxel is a

- minecraft online and goxel online

#### MagicaVoxel

- Cannot get something out of voxedit, and magicavoxel is opaque. Need nsight

#### Qubicle

#### Blender

- Stops to render if there is no move

### B - Architecture and Data Structure

- ECS
- Met ecs
- Singleton components
- previous project (opengl-playground)

### C - OpenGL renderer abstraction

- constant buffers instead of uniforms
- instanced rendering
- rename opengl Object to well known equivalent closer to DirectX (uniform buffer object -> constant buffer | framebuffer -> renderTarget | vertex array -> deleted )

### D - Laying out the tasks

- Continuous integration
- Trello calender
- Unit test
- Automatic deploy
- Artists speed modeling

## II - Core features

> The needed bits and parts of the app. 

### Selection

- Framebuffer with color picking
- Raycasting

### Interface and User controls

- Docking branch imgui
- imgui imagebutton not working correctly when pressed, use font icon instead
- Arcball camera Panning

### Rendering system and passes

- per-vertex normals with a cube with 8 vertex is less efficient as we have to interpolate between 4 each time to get surface normal
- Uber shader to make only one pass instead of multiple with lots of binding and unbinding (measure performance gain)
- Render target with ints are a pain to use (unknown values, clear different, etc), so cast entities to float to improve it
- glVertexAttribPointer and glVertexAttribIPointer not to interpolate int values

### Brushes

- Voxel add tool, explosion if just add. Must limit to 1 add in height per click.
- Color painting

### Procedural terrain generation




## III - Additional features

### Wasm and Emscripten

- OpenGL es constraints
- Using emscripten directly

### Undo & redo

- First think with enum
- command pattern

### Shadows

- Shadow mapping
- SSAO

### Benchmarking

- Open source chrome tracing
- KHR debug opengl extension
- [https://renderdoc.org/docs/window/event_browser.html](https://renderdoc.org/docs/window/event_browser.html)

## IV - Planned features

### Exporting

### Improved data structure

Sparse set might be way more efficient to store and access cube data, might ditch ecs in the future as need access in O(1) of pos for advanced brushes, and cube storage in multiple chunks instead of one big array
-> ECS might not be the best solution, but it's the one which will help me learn more for now (benchmarking, ecs use case, improving my lib)

### Remesh
See if can store neightboring cubes as one rectangle, multiple is there is a hole. Render with instanced plane, but different model mats

### Object mode

### Parenting

### Modifiers

### Custom preferences

- Keymapping
- Color editing

## Conclusion

## References

### Selection

| Link | Type | Description | Date |
| --- | --- | --- | --- | --- |
| TODO

### Shadow maps
