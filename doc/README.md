# Cube Beast Editor post-mortem

<details><summary>Table of Content</summary>
<p>

[**Specifications**](#specifications)

[**Introduction**](#introduction)

[**I - Planning, Research and Analysis**](#i---planning-research-and-analysis)
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
| [Met ECS library](https://github.com/guillaume-haerinck/met-ecs) | A post-mortem for this library is available [here](https://github.com/guillaume-haerinck/met-ecs/tree/master/docs) | I - B |
| Shadows | | |
| Deferred shading | | |
| ArcBall Camera | |
| Benchmarking | | |
| WASM build support | | |
| ImGui Docking branch | | |

</p>
</details>

## Introduction

From the start, we wanted to create a **robust piece of software**. The kind of project that has its place amongst other similar applications. One that could compete with an elegant interface, a basic but well-designed set of tools, and a optimized and yet easy to maintain architecture. That was the dream, and it was ambitious.

As the **4rd iteration** of its creators to create a 3D data-driven real-time application, we knew that we could build something better out of our past mistakes, but **a lot was still to learn**. [Mirrors, lasers and robots](https://github.com/guillaume-haerinck/imac-tower-defense) helped us to reason about the communication inside of our app, and playground apps for [OpenGL](https://github.com/guillaume-haerinck/opengl-playground) and [DirectX](https://github.com/guillaume-haerinck/directx11-playground) gave us good intuitions to abstract our render engine. Do you want to know what this first version of Beast Voxel Editor teached us ?

Through this document, we will describe as simply and thoroughly as possible our development process. We will explain the problems we ecountered, the solutions we found for them, and the missing bits of this first version (because we can make a good software, but we can't make time).

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

MagicaVoxel is, without contest, the most known and used Voxel Editor out there. It might not be open-source, but it's free (*for how long ?*). It has a lot of features (but some are a bit hidden), and slick interface (which could be made more intuitive). Tools have a really great feeling, maybe the best amongst all voxel editors. It can handle large scene, group objects and move them in the world, but it does have a limit on number of objects. I also like a lot its renderer and the viewport options (show outlines, disable shadows, etc).

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

___

### B - Architecture and Data Structure

A good advice from *Mike Acton* before starting to write a piece of software is to *"know your data"*. In our case, we knew that we would have to store, traverse and write a lot of positions for our cubes. It would be the barebone of our app. We also knew that we might have to add a lot of features in the future, so storing our data in an **array of structures (AOS) would have been super-slow** as we would have ended up with a lot of cold data, unused most of the time. This meant **no Cube class**.

Instead, we had to go for **split data storage**. For exemple one array for positions, another for materials, etc. Also known as a structure of array (SOA). There are tons of data structures which could have been usefull for us, but *premature optimisation is the source of all evil*. Instead, we decided to use a structure that we know, that we used in the past, and which encourage changes and prototyping.

#### Entity Component System

Mostly popular amongst game developers, this data-oriented way to program allows easy operation on large groups, prototyping and a SOA storage. In the past we've used the [ENTT library](https://github.com/skypjack/entt) to handle this for us. However this time it wasn't allowed by our teachers, so we had to create a [simple ECS library](https://github.com/guillaume-haerinck/met-ecs).

We won't go into many details here as a [post-mortem](https://github.com/guillaume-haerinck/met-ecs/tree/master/docs) is available for this project as well. But if you are new to ECS, you got to know that it is made out of 3 pieces. Entites, Components and Systems.

-   Entities are unique identifiers
-   Components are plain datatypes (Positions, Colors, etc...)
-   Entities can possess zero or more components
-   Systems are logic executed on entities with matching component sets
- Components have no functions, and systems have no states

All of this orchestra is handled in our library by an object called **Registry**. It is the one that creates entities, assign components to them and can get them back. It is an object that is supposed to be used accross all systems.

This gave us a **starting point to structure our application**. We knew that we would need of folder with all sorts of components, another for all systems (RenderSystem, SelectionSystem, etc) and an **App** class to order all of this.

<br>

<p align="center">
<img width="700" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/uml-1.png?raw=true" alt="UML"/>
</p>

<br>

#### The case of static data

One problem that comes after starting to develop in ECS is **how to store,  get and where to modify static data**. By static data, we mean global states of the app, the sort of things that are needed but not modified by our systems. We are talking about input handling, user preferences, graphic objects like shaders and framebuffers, etc.

There are a lot of ways to handle this sort of things. One might put it outside of the ECS, maybe by using a Singleton or the Service locator pattern. Another could use pointers inside of components, or be Event-based and call a set of function whenever the states are changed. We tried a few a this options on our past project and not one has been good enough to carry on.

Overwatch developers [encountered this problem](https://youtu.be/W3aieHjyNvw?t=548) as well, and after a while they came up with a simple and yet powerfull idea : [Singleton Components](https://youtu.be/W3aieHjyNvw?t=741).

> It's ok to define a component type that will only ever have one instance. - Timothy Ford, *GDC 2017*, Overwatch Gameplay Architecture and Netcode.

Yet, there are still **2 problems** with their implementation that didn't worked well with our library.

1. Singleton components could be modified everywhere like normal components. But the fact is that they where **supposed to be read-only** for everyone except one owner class. Using a plain entity to store them was a danger.

2. In the way the library is structured, getting singleton components would have taken **at least 4 lines of code**, as we would have to check if the component existed. This is was pain and not developer-friendly.

That's why we came up with another design for singleton components. **Simply store them inside a struct**. This struct will be passed by reference to our systems like the registry. The **data is private, but public function exists** to get const references to them. We used the concept of **C++ friend classes** to make those component modifiable by one or two classes only, and read only for all other.

<br>

<p align="center">
<img width="700" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/uml-2.png?raw=true" alt="UML"/>
</p>

<br>

#### Interfaces

We used the [ImGui](https://github.com/ocornut/imgui) library to make our interfaces. As an imediate-mode stateless library, we have to update it each frame. To ease the code organisation, we split the different imgui windows into different classes. As for systems, those classes all inherits an interface, and they have access to the registry and singleton components. 

We knew that we might need to handle **inter-interfaces communication**, so we made a simple event system, a modified version of the Observer pattern. The idea is simple, we have a **stack of events (just enums)**, that every interface can fill if they want to. When there is an event in the stack, we call the *onEvent()* function for each IGui* instance. Then each of the interfaces can handle it in a switch-case structure if they need to.

```C++
void App::update() {
	// ...
	for (const auto& event : m_eventStack) {
		for (const auto& gui : m_guis) {
			gui.onEvent(event);
		}
	}
	m_eventStack.emptyOldEvents();
	// ...
}

void MyGui::onEvent(GuiEvent  e) {
	switch (e) {
		case  GuiEvent::MY_EVENT: mySpecialFunction(); break;
		default: /* Other events are not handled by this class */ break;
	}
}
```

The event themselves do not contain data. Different interfaces might need differents things for the same type of event, and the singleton components already store the states of the application. **We haven't used events yet**, but they might come in handy if we do not want to check singleton components, or if a special event can't be described by a change in singleton components.

<br>

<p align="center">
<img width="600" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/uml-3.png?raw=true" alt="UML"/>
</p>

<br>

___

### C - OpenGL renderer abstraction

The history of OpenGL [starts a way back](https://openglbook.com/chapter-0-preface-what-is-opengl.html) to the beginning of dedicated 3D hardware in the early 1990's. For all those years, the API had time for evolution and new features, but the biggest change happened in 2004 with the release of the version 2.0. This version, which is now called the *Core profile* allowed developer to use a **programmable pipeline** (shaders), and to send only the geometry data once instead of each frame.

From this point, **features were added to the API, but not much changed in the way to use its core**. It kept the c-like API as opposed to the Object-Oriented way of Microsoft's DirectX, and finally in 2016, a new API called [Vulkan](https://fr.wikipedia.org/wiki/Vulkan_%28API%29) was released by the *Khronos group* (in charge of OpenGL).

Does that mean that OpenGL is dead ? **Not at all**. Its specification is still updated frequently and used accross millions of devices. Vulkan simply does not play in the same league or have the same goal. It is much more complicated to use, can lead to great performance gain, but also some disasters.

For this project, we decided to use the OpenGL ES 3.0 specification. It has enough features for our needs, can be played on many devices, and can be exported to be played on a web-browser (more on that in the part III). With the insights we got from our past experience with DirectX 11, we knew that we could abstract OpenGL in a way that would be the **less error-prone, less verbose and easier to understand**.

#### Pipeline Input Description

With DirectX 11, when you send data to your graphic card you have describe it with an array of struct called [InputElementDesc](https://docs.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_input_element_desc). You assign assign this array to your vertex shader right after its creation, and everyone is happy.

```C++
// My buffer will be holding Float3 position. Not-instanced.
const D3D11_INPUT_ELEMENT_DESC VertexPosition::InputElements[] = {
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
```

It's OpenGL counterpart is not so easy to use.  

You have to create a Vertex Array, assign your data to it, and finally describe what is inside with [glVertexAttribPointer](http://docs.gl/es3/glVertexAttribPointer). Here, **the description is not assigned to your vertex shader, but to your buffers**. This means that if you have different meshes with the same kind of data, you will have to repeat this step for each different buffer. It is very error-prone, there are a lot of steps, a lot of hidden states, and the code is difficult to read.

We can't change the inner working of OpenGL, but we can emulate some DirectX clever ideas, and that's what we made (helped a lot by [The Cherno Hazel Engine](https://www.youtube.com/watch?v=jIJFM_pi6gQ))

``` C++
// Cube mesh layout
PipelineInputDescription inputDescription = {
	{ ShaderDataType::Float3, "Position" },
	{ ShaderDataType::Float3, "Normal" },
	{ ShaderDataType::Float3, "Translation", BufferElementUsage::PerInstance },
	{ ShaderDataType::Float3, "EntityId", BufferElementUsage::PerInstance },
	{ ShaderDataType::UInt, "MaterialIndex", BufferElementUsage::PerInstance }
};
```

#### Pipeline output description

```C++
// Geometry pass fragment shader output
PipelineOutputDescription outputDescription = {
	{ RenderTargetUsage::Color, RenderTargetType::Texture, RenderTargetDataType::FLOAT, RenderTargetChannels::RGBA, "Geometry_Albedo" },
	{ RenderTargetUsage::Color, RenderTargetType::Texture, RenderTargetDataType::FLOAT, RenderTargetChannels::RGBA, "Geometry_Normal" },
	{ RenderTargetUsage::Color, RenderTargetType::Texture, RenderTargetDataType::FLOAT, RenderTargetChannels::RGBA, "Geometry_LightSpacePosition" },
	{ RenderTargetUsage::Color, RenderTargetType::RenderBuffer, RenderTargetDataType::UCHAR, RenderTargetChannels::RGBA, "EntityIdToColor", RenderTargetOperation::ReadPixel },
	{ RenderTargetUsage::Depth, RenderTargetType::RenderBuffer, RenderTargetDataType::FLOAT, RenderTargetChannels::R, "Depth" }
};
```

- constant buffers instead of uniforms
- instanced rendering
- rename opengl Object to well known equivalent closer to DirectX (uniform buffer object -> constant buffer | framebuffer -> renderTarget | vertex array -> deleted )


#### Render command

[https://ourmachinery.com/post/ecs-and-rendering/](https://ourmachinery.com/post/ecs-and-rendering/)

#### Debug draw



<p align="center">
<img width="700" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/uml-4.png?raw=true" alt="UML"/>
</p>

___

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

[https://fr.dolphin-emu.org/blog/2017/07/30/ubershaders/?nocr=true](https://fr.dolphin-emu.org/blog/2017/07/30/ubershaders/?nocr=true)

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
