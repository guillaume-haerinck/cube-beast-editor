# Beast Voxel Editor post-mortem

<details><summary>Table of Content</summary>
<p>

[**Specifications**](#specifications)

[**Introduction**](#introduction)

[**I - Planning Research and Analysis**](#i---planning-research-and-analysis)
*	[**Voxel editors analysis**](#a---voxel-editors-analysis)

[**Conclusion**](#conclusion)

</p>
</details>

## Specifications

Beast Voxel Editor is a software which allows you to easily generate, visualize and edit 3D scenes. It has been started as a class project for the [IMAC engineering school](https://www.ingenieur-imac.fr/), and had some requirement to meet in order to be evaluated by our teachers. The original specifications can be found [here](https://docs.google.com/document/d/1aDahRcA8aeTSNqdqXv5i8FGPoVUmYzQ-k4z49wlu1K0/edit?usp=sharing), and we made a summary of these features below. 

<details><summary>Show Specifications</summary>
<p>

The *D* column means *Done* and the *O* column means *Required*

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

</p>
</details>

Additionaly, we added features not mentionned in the specifications.

<details><summary>Show Additionnal features</summary>
<p>

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

</p>
</details>

## Introduction

From the start, we wanted to create a **robust piece of software**. The kind of project that has its place amongst other similar applications. One that could compete with an elegant interface, a basic but well-designed set of tools, and a optimized and yet easy to maintain architecture. That was the dream, and it was ambitious.

As the **4rd iteration** of its creators to create a 3D data-driven real-time application, we knew that we could build something better out of our past mistakes, but **a lot was still to learn**. [Mirrors, lasers and robots](https://github.com/guillaume-haerinck/imac-tower-defense) helped us to reason about the communication inside of our app, and playground apps for [OpenGL](https://github.com/guillaume-haerinck/opengl-playground) and [DirectX](https://github.com/guillaume-haerinck/directx11-playground) gave us good intuitions to abstract our render engine. Do you want to know what this first version of Beast Voxel Editor teached us ?

Through this document, we will describe as simply and thoroughly as we can our development process. We will talk about the problems we ecountered, the solutions we found for them, and the missing bits of this first version (because we can make a good software, but we can't make time).

## I - Planning, research and analysis

> Where we take a step back to look at our surroundings and preparre for the task at hand.

Making a 3D voxel editor in 2019 can not be done in a Vacuum. Many other applications of this sort exist, are free, used by thousands of artists, and have made and fixed mistakes that we haven't encountered yet. That's why when we started, our first goal was to get **as much detail and information** as we could about the structure and inner working of these engines.

Based on those clues, we could start to plan our work, our application architecture and how to make the best out of it. We can never plan everything, but taking the time to think about our real needs was very important for us.
___

### A - Voxel editors analysis

We put 4 softwares under our microscopes. Both in terms of user-interface, functionalities and render pipelines. Most of the time it was difficult to get more information on the rendering part, but we did get some usefull insights.

<br>

<p align="left">
<img width="80" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/voxedit/logo.png?raw=true" alt="Goxel logo"/>
</p>

#### [VoxEdit](https://www.voxedit.io/)

Free software, it has been made in Java, and targets younger artist with a simplified interface. The big icon buttons on the left side are great to see which tool is selected and how it is used, and some options are available at the top. We can't get much information about rendering as  our debugging softwares cannot connect to a Java context.

<p align="center">
<img width="600" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/voxedit/final.jpg?raw=true" alt="VoxEdit interface"/>
</p>

<details><summary>Show VoxEdit Summary</summary>
<p>

| Pluses | Downsides
| ---- | --- |
| Easy to get onboard | Cannot move interface blocs |
| Accessible palette and Picker | No layers |
| Good communication and target | Too much visual feedback when using tools |
| Rigging and animation support | Cannot customize controls |

</p>
</details>

<br>

<p align="left">
<img width="80" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/goxel/logo.png?raw=true" alt="Goxel logo"/>
</p>

#### [Goxel](https://goxel.xyz/)

Goxel is a free and [Open-Source](https://github.com/guillaumechereau/goxel) software made in C (with a bit of C++ templates) with OpenGL powered graphics. It is not the most beautiful editor there is, but it has a fair bit of features (layers, rendering, lighting, export in multiple formats). It even has the luxury to a a [browser-based version](https://goxel.xyz/live/) and a [development blog](https://blog.noctua-software.com/) with insights on its inner working. It's a shame that the application architecture in C with free functions and pointers to struct everywhere make the code a bit cryptic to understand without a deep delve in with a debugger.

<p align="center">
<img width="600" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/goxel/final.jpg?raw=true" alt="Goxel logo"/>
</p>

As a C software, with a modern OpenGL context, it is possible to analyse its rendering pipeline. With took [RenderDoc](https://renderdoc.org/) to get the job done and this is what we got :

<details><summary>Show Goxel Analysis</summary>
<p>

| Capture | Description |
| --- | --- |
| <img width="1000" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/goxel/pass-1.jpg?raw=true" alt="Goxel pass 1"/> |  There is a first pass done for color picking with framebuffer. Each cube as an unique IDs which allows for mouse selection. The voxels are separated on different smaller blocs (the current one is in yellow wireframe). It is a strange choice as it makes more draw calls and the cubes are not instanced. |
| <img width="300" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/goxel/pass-2.jpg?raw=true" alt="Goxel pass 2"/> | A shadow map is generated for the directional light. We see the scene from the light point of view, in orthogonal projection. There is only deepth information. |
| <img width="300" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/goxel/pass-3.jpg?raw=true" alt="Goxel pass 3"/> | We skipped the GUI drawing to get to the cube part. As before, they are splits in smaller areas drawn with successive non-instanced draw calls. Lighting is done on this pass. |
| <img width="300" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/goxel/pass-4.jpg?raw=true" alt="Goxel pass 4"/> | The grid is rendererred in one pass with a big inverted cube. That's a great optimisation to use this instead of many planes. |

</p>
</details>

From the [dev. blog](https://blog.noctua-software.com/goxel-internals-part2-data-structure.html), we can read that the voxels are stored in larger structures and that a scene is made of multiple of those structs. We have an explanation about why the cubes on screens are rendered with different sub-groups.

<details><summary>Show VoxEdit Summary</summary>
<p>

| Pluses | Downsides
| --- | --- |
| Open source | Not great UI |
| Lots of features | Strange presentation of tools (ex: laser mode instead of eraser) |
| Cross platform and online | A pain to build on Windows (Lots of things to install) |
| Easy to debug | No longer in active development |
|  | Codebase not that easy to enter |

</details>
</p>

<br>

<p align="left">
<img width="80" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/qubicle/logo.png?raw=true" alt="Cubicle logo"/>
</p>

#### [Qubicle](https://www.minddesk.com/)

Qubicle is a paid-software (from 75$ to 175$ for the pro version) which was used by a few well-known voxel games to create their models. It can handle large scenes and separate objects to place them in the world. It is filled with features and has a quite a great interface (but not that intuitive when you open it for the first time).

<p align="center">
<img width="600" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/qubicle/final.jpg?raw=true" alt="Goxel logo"/>
</p>

It cannot be analysed by renderdoc as the OpenGL context created is legacy, but it kind-of works with [Nvidia Nsight](https://developer.nvidia.com/nsight-visual-studio-edition). And that when we have a surprise, "An unknown objet as been sent to the API". *mmmh, m'ok*. So let's capture a frame and see what we get.

<details><summary>See what we get</summary>
<p>

<details><summary>Almost there</summary>
<p>

<p align="center">
<img width="600" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/qubicle/pass-1.jpg?raw=true" alt="Goxel logo"/>
</p>

</details>
</p>

</details>
</p>

We get that the rendering is done - maybe done by this unknown object - and then the result is split into smaller textures that are rendered by OpenGL. That's an interesting design, but I have no clue about how it works.

<details><summary>Show Qubicle Summary</summary>
<p>

| Pluses | Downsides
| --- | --- |
| Full of features | Paid |
| Some tools have a really great feeling | Strange to get onboard (must create a matrix first) |
| Great documentation | Strange behavior of some tools (difficult to un-select, must press enter a lot, etc...) |
| Used by many projects | |

</details>
</p>

<br>

<p align="left">
<img width="80" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/magica-voxel/logo.png?raw=true" alt="Magicavoxel logo"/>
</p>

#### [MagicaVoxel](https://ephtracy.github.io/)

MagicaVoxel is, without contest, the most known and used Voxel Editor out there. It might not be open-source, but it's free (*for how long ?*). It has a lot of features (but some are a bit hidden), and slick interface (which could be made more intuitive). Tools have a really great feeling, maybe the best amongst all voxel editors. It can handle large scene, group objects and move them in the world, but it does have a limit on number of objects. I also like a lot its renderrer and the viewport options (show outlines, disable shadows, etc...)

<p align="center">
<img width="600" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/magica-voxel/final.jpg?raw=true" alt="Magica voxel"/>
</p>

The OpenGL context created being inferior to 3.2, we need to use NvidiaNsight to get some information about its rendering pipeline.

<details><summary>Show MagicaVoxel Analysis</summary>
<p>

| Capture | Description |
| --- | --- |
| <img width="1000" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/magica-voxel/pass-1.jpg?raw=true" alt="MagicaVoxel pass 1"/> | We skipped the interface to get to the cube. There we can seen that the grid is rendered in one pass like with Goxel. |
| <img width="1000" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/magica-voxel/pass-2.jpg?raw=true" alt="MagicaVoxel pass 2"/> | We see that only the top of the surfaces are drawn. It is not instanced, and is using a plane for each cube. We do not see a shadow map pass, or a pass that fill the sides which is really strange. |
| <img width="1000" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/magica-voxel/hdri.jpg?raw=true" alt="MagicaVoxel pass 2"/> | Looking at the bound ressources, we can see that there is an HDRI texture used for lighting on PBR pipelines, and that the color palette is sent as one texture. |

</p>
</details>

It might be interesting to delve deeper into its rendering structure, as the project seemed to have been open source at a time, and that the viewport is only rendered with 2 draw calls.

<details><summary>Show MagicaVoxel Summary</summary>
<p>

| Pluses | Downsides
| --- | --- |
| Free | Not open source |
| Used by a lot of artists| Many hidden gems in the interface |
| Overflowing with features | Powerfull but not intuitive enough |
| Great interface | |
| Tools have an amazing feeling | |

</details>
</p>

#### What can we get from this ?

- There are already a lot of great and free voxel editor availables
- VoxEdit simple UI is a charm when you discover it for the first time
- MagicaVoxel set of Tools have the best feeling and usages
- There is no viable online version for now (Goxel online is only a prototype)
- MagicaVoxel would be the perfect tool if the interface was a bit more intuitive
- There is not a one-go solution when it comes to rendering

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
