# Cube Beast Editor post-mortem

![Imac logo](https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/imac.png?raw=true)

**Computer Graphics & Mathematics classes common project**

*Guillaume Haerinck, Gaelle Vallet - 2021 Promotion*

___

<details><summary>Table of Contents</summary>
<p>

[**Specifications**](#specifications)

[**Introduction**](#introduction)

[**I - Planning, Research and Analysis**](#i---planning-research-and-analysis)
*	[**Voxel editors analysis**](#a---voxel-editors-analysis)
*	[**Architecture and Data structure**](#b---architecture-and-data-structure)
*	[**OpenGL API abstraction**](#c---opengl-api-abstraction)
*	[**Laying out the tasks**](#d---laying-out-the-tasks)

[**II - Core features**](#ii---core-features)
*	[**Renderer**](#a---renderer)
*	[**Mouse selection**](#b---mouse-selection)
*	[**Brushes**](#c---brushes)
*	[**Interface and User controls**](#d---interface-and-user-controls)
*	[**Procedural terrain generation**](#e---procedural-terrain-generation)

[**III - Additional features**](#iii---additional-features)
*	[**WASM**](#a---wasm)
*	[**Shadows**](#b---shadows)
*	[**Benchmarking**](#c---benchmarking)
*	[**History**](#d---history)

[**Conclusion**](#conclusion)

[**References**](#references)

</p>
</details>

## Specifications

Beast Voxel Editor is a software which allows you to easily generate, visualize and edit 3D scenes. It has been started as a class project for the [IMAC engineering school](https://www.ingenieur-imac.fr/), and had some requirement to meet in order to be evaluated by our teachers. The original specifications can be found [here](https://docs.google.com/document/d/1aDahRcA8aeTSNqdqXv5i8FGPoVUmYzQ-k4z49wlu1K0/edit?usp=sharing), and we made a summary of these features below. 

<details><summary>Show Specifications</summary>
<p>

The *D* column means *Done* and the *R* column means *Required*

| D | R | Description | Comments | Part |
| --- | --- | --- | --- | --- |
| X | X | Render a 3D scene with cubes | | [II - A](#a---renderer) |
| | X | Select cube with keyboard | Replaced by mouse select| [II - B](#b---mouse-selection) |
| X | | Select cube with mouse | Raycast & Framebuffer color picking | [II - B](#b---mouse-selection) |
| X | X | Show selected cube with an outline | Multiple modes | [II - D](#d---interface-and-user-controls) |
| X | X | Paint selected cube | | [II - C](#c---brushes) |
| X | X | Add and Remove cubes | | [II - C](#c---brushes) |
| X | X | Extrude tool | Use box mode with add brush | [II - C](#c---brushes) |
| X | X | Procedural terrain generation with RBF | | [II - E](#e---procedural-terrain-generation) |
| X | X | Directionnal light | | [II - A](#a---renderer) |
|  | X | Point light | | [II - A](#a---renderer) |
| | | Paint adjacent faces | | [II - C](#c---brushes) |
| | | Proportional edit | | |
| | | Save scene to custom file | | |
| X | X | Load a custom file (.cbs) | | [II - E](#e---procedural-terrain-generation) |
| | | Load a 3D file (.gltf) | | |
| | | Change grid size | | |
| | | Textured cubes | | |

</p>
</details>

Additionaly, we added features not mentionned in the specifications.

<details><summary>Show Additionnal features</summary>
<p>

| Description | Comments | Part |
| --- | --- | --- |
| [Met ECS library](https://github.com/guillaume-haerinck/met-ecs) | A post-mortem for this library is available [here](https://github.com/guillaume-haerinck/met-ecs/tree/master/docs) | [I - B](#b---architecture-and-data-structure) |
| Shadows | | [III - B](#b---shadows) |
| Instanced rendering | | [II - A](#a---renderer) |
| Deferred shading | | [II - A](#a---renderer) |
| ArcBall Camera | | [II - D](#d---interface-and-user-controls) |
| Benchmarking | | [III - C](#c---benchmarking) |
| WASM build support | | [III - A](#a---wasm) |
| UI Improvements | | [II - D](#d---interface-and-user-controls) |
| History | *Soon to be implemented* | [III - D](#d---history) |

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

It cannot be dissected by renderdoc as the OpenGL context created is legacy, but it kind-of works with [Nvidia Nsight](https://developer.nvidia.com/nsight-visual-studio-edition). And that's when we have a surprise, "An unknown objet as been sent to the API". *mmmh, m'ok*. So let's capture a frame and see what we get.

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

We see that the rendering is done elsewhere - maybe done by this unknown object - and then the result is split into smaller textures that are rendered by OpenGL. That's an interesting design, but I have no clue about how it works.

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

Instead, we had to go for **split data storage**. For exemple one array for positions, another for materials, etc. Also known as a structure of array (SOA). There are tons of data structures which could have been usefull for us, but **premature optimisation is the source of all evil**. Instead, we decided to use a structure that we know, that we used in the past, and which encourage changes and prototyping. Once that it would be stable and benchmarked, we could think about improving our data storage.

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

That's why we came up with another design for singleton components. **Simply store them inside a struct**. This struct will be passed by reference to our systems like the registry. The **data is private, but public function exists** to get const references to them. We used the concept of **C++ friend classes** to make those component modifiable by one or two classes only, and read only for all others.

<br>

<p align="center">
<img width="700" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/uml-2.png?raw=true" alt="UML"/>
</p>

<br>

#### Interfaces

We used the [ImGui](https://github.com/ocornut/imgui) library to make our interfaces. As an imediate-mode stateless library, we have to update it each frame. To ease the code organisation, we split the different imgui windows into different classes. As for systems, those classes all inherits an interface, and they have access to the registry and singleton components. 

We knew that we might need to handle **inter-interfaces communication**, so we made a simple event system, a modified version of the Observer pattern. The idea is simple, we have a **stack of events (just enums)**, that every interface can fill if they want to. When there is an event in the stack, we call the *onEvent()* function for each IGui* instance. Then each of the interfaces can handle it in a switch-case structure if they need to.

<details><summary>Show code example</summary>
<p>

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
</p>
</details>


The event themselves do not contain data. Different interfaces might need differents things for the same type of event, and the singleton components already store the states of the application. **We haven't used events yet**, but they might come in handy if we do not want to check singleton components, or if a special event can't be described by a change in singleton components.

<br>

<p align="center">
<img width="600" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/uml-3.png?raw=true" alt="UML"/>
</p>

<br>

___

### C - OpenGL API Abstraction

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

You have to create a Vertex Array, assign your data to it, and finally describe what is inside with [glVertexAttribPointer](http://docs.gl/es3/glVertexAttribPointer). Here, **the description is not assigned to your vertex shader, but to your buffers**. This means that if you have different meshes with the same kind of data, you will have to repeat this step for each different buffer. It is very error-prone.

We can't change the inner working of OpenGL, but we can emulate some DirectX clever ideas, and that's what we made (helped a lot by [The Cherno Hazel Engine](https://www.youtube.com/watch?v=jIJFM_pi6gQ)). We created a class *PipelineInputDescription* which does exactly as DirectX. When we create a Vertex Array, our utility function take it amongst its parameters.

``` C++
const PipelineInputDescription inputDescription = {
	{ ShaderDataType::Float3, "Position" },
	{ ShaderDataType::Float3, "Translation", BufferElementUsage::PerInstance }
};
```

As we are not huge fans of the concept of *Vertex Arrays*, we've hidden it in our abstraction and renamed it *Vertex Buffer*. The idea is that to create a *Vertex Buffer*, you need 2 things :

1. A *Pipeline Input Description*
2. One or more *Attribute Buffers*

These attributes buffers, are nothing more than OpenGL original vertex buffers, with the specific need that these **buffers are not interleaved** : They only contains one type of data, for one specific usage. That's a requirement to load `.gltf` models easily (even though we don't have that right now), and for our instanced rendering (more details on part II).

```
INTERLEAVED BUFFER (not used) : [XYZXYZXYZXYZXYZXYZ]
SEPARATE BUFFER (used) : [XXXX] [YYYY] [ZZZZ]
```

#### Pipeline output description

Our application uses **multiple render targets** and differents passes. This means that our fragment shaders will sometimes output to multiple textures (more on that on Part II). In OpenGL, you have to create a Framebuffer and assign textures to it. The [plethoras of options](http://docs.gl/es3/glTexImage2D) available for those textures can be very cumberstone to use, so we decided to abstract it.

We had an inputDescription, now we would have an outputDescription that we use when we create our FrameBuffers. For exemple :

```C++
const PipelineOutputDescription outputDescription = {
	{ RenderTargetUsage::Color, RenderTargetType::Texture, RenderTargetDataType::FLOAT, RenderTargetChannels::RGBA, "Geometry_Albedo" },
	{ RenderTargetUsage::Color, RenderTargetType::Texture, RenderTargetDataType::FLOAT, RenderTargetChannels::RGBA, "Geometry_Normal" }
	{ RenderTargetUsage::Depth, RenderTargetType::RenderBuffer, RenderTargetDataType::FLOAT, RenderTargetChannels::R, "Depth" }
};
```

#### Constant buffers

DirectX 11 doesn't have any notion of uniforms. To set a shader variable value from the application, it uses the concept of Constant Buffers, similar but not exactly the same. Where uniforms where used to modify one variable, **constant buffers are describing a struct**. Moreover, ConstantBuffers are **shared between different pipelines**, limiting the number of calls to make to the API.

Traditionnaly, these struct are named by their update frequency. *PerFrame*, *PerLightChange*, etc, it's up to the developer to decide how to handle them. Their equivalent in OpenGL are called [**Uniform Buffer Objects**](https://www.khronos.org/opengl/wiki/Uniform_Buffer_Object), and they are very similar.

We decided to use them early on to improve our data-management. We have a few sets of pre-defined constant buffers, and when we create a pipeline (a vertex shader + a fragment shader), the developer has to indicate which Constant Buffer its pipeline uses so that the constant buffers can be associated with the pipeline.

```C++
// Constant buffer app-side
struct perFrame {
	glm::mat4x4 matViewProj;
	glm::vec3 cameraPos;
	char padding[4];
};

// Constant buffer shader-side
layout (std140) uniform perFrame {
	mat4 matViewProj;
	vec3 cameraPos;
};
```

You might have noticed the `char padding[4]`, this is because the byteWidth of a constant buffer must always be a multiple of 16, so we simply fill the gap.

#### Render command

We explained how we abstracted our data for OpenGL, but we still haven't say much about the way we interact with the API. Spoiler alert, it's in the title. We use an object caller *RenderCommand* - stateless for now - which **regroups every call made to OpenGL**. There is not a single glXXX outside of this class in our entire application.

Why this choice ? There are a few reasons actually.

1. OOP for OpenGL Object creation and destruction hasn't worked well for us in the past with an ECS architecture. The destructor would be called when assigning components, so we had to move the glDeleteXXX elsewhere. Moreover components are not supposed to have any logics inside of them.

2. We are not using a whole lot of functionnalities from the API. So our calls could be contained in one file without being too gigantic (we tops at 700 lines in the .ccp. It is important but still managable as function are grouped by types like creation, deletion, updates, etc)

3. Reusability and portability. Because when your whole use of an API is contained in only 1 file, you know where to look for when there is a bug or if you need a specific feature inside another project.

4. Multi-API support. This one is not nescessarly true as we do not plan to do it, but using an interface for the RenderCommand and the Strategy pattern, and we can almost switch from OpenGL to DirectX in real time. It would be really cool to make, but not that usefull for now.

We took a look at different data-oriented way to make a renderer. These people building "Our Machinery" have interesting [blog posts](https://ourmachinery.com/post/ecs-and-rendering/) about their architecture, and a talk at the GDC 2015 about the [rendering structure of Destiny](https://www.youtube.com/watch?v=0nTDFLMLX9k) is interesting as well. But all of these architecture were overkill to say the least for our needs, so we simply stick to a simple and managable solution.

#### Debug draw

<p align="center">
<img width="700" src="https://raw.githubusercontent.com/glampert/debug-draw/master/samples/images/shapes.png" alt="UML"/>
</p>

> Capture of the *Debug Draw* library by [glampert](https://github.com/glampert/debug-draw). (not used by our project)

We knew that we would also need a class to render debug primitives while developing. We wanted to have the ability to **visualize a raycast with one line of code**, or to draw a bounding box to check collisions. The idea came from the book [Game Engine Architecture](https://www.gameenginebook.com/) by Jason Gregory in the chapter where he talks about the debug tools used at Naughty Dog.

This class needed to be available everywhere, handle any kind of data, draw a specific primitive for any amount of time, and be rendered in our RenderSystem to be in sync with the other passes. We started to work on it, enough to visualize a raycast which was our need, but it hasn't came back yet so **we commented its render code out**.

Render command and Debug draw are two objects which needed to be available everywhere, like the registry. So instead of passing the 3 inside many constructors, **we simply put those 3 in a struct called Context that is passed around instead**.

<br>

<p align="center">
<img width="700" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/uml-4.png?raw=true" alt="UML"/>
</p>

<br>

___

### D - Laying out the tasks

To get to this first version on time, we knew we had follow a planning. While some priorities where obvious, like rendering a cube on a scene, it remained difficult to make a choice about which optional features to support quickly. We knew what the other voxel editors had, but **what was used the most often, what was the core ?** To awnser this question, we simply took a look on youtube to see how artists works.

#### Artist work analysis

[Anna Lepeshkina - Island](https://www.youtube.com/watch?v=yKO1Q5QRZic)

<details><summary>Show work</summary>
<p>

| <img width="450" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/anna-lepeshkina-island/1.jpg?raw=true" alt="Island voxel"/> | <img width="450" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/anna-lepeshkina-island/2.jpg?raw=true" alt="Island voxel"/> |
| :---: | :---: |
| Quick drawing | Block-Out with Box brush |
| <img width="450" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/anna-lepeshkina-island/3.jpg?raw=true" alt="Island voxel"/> | <img width="450" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/anna-lepeshkina-island/4.jpg?raw=true" alt="Island voxel"/> |
| Add details with Box brush | Paint faces |
| <img width="450" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/anna-lepeshkina-island/5.jpg?raw=true" alt="Island voxel"/> | <img width="450" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/anna-lepeshkina-island/6.jpg?raw=true" alt="Island voxel"/> |
| Paint voxel & faces | Paint details voxel & Voxel brush |
| <img width="450" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/anna-lepeshkina-island/7.jpg?raw=true" alt="Island voxel"/> | <img width="450" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/anna-lepeshkina-island/8.jpg?raw=true" alt="Island voxel"/> |
| Copy and Paste object groups. Ereaser by voxel | Block brush |
| <img width="450" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/anna-lepeshkina-island/9.jpg?raw=true" alt="Island voxel"/> | |
| Select all & extrude face | |

</p>
</details>

[Elliot GK - Tiny Canyon](https://www.youtube.com/watch?v=I-VxX0Ks_ZM)

<details><summary>Show work</summary>
<p>

| <img width="450" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/elliot-gk-tiny-canyon/1.jpg?raw=true" alt="Canyon voxel"/> | <img width="450" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/elliot-gk-tiny-canyon/2.jpg?raw=true" alt="Canyon voxel"/> |
| :---: | :---: |
| Top view pixel brush | Fill area and extrude face |
| <img width="450" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/elliot-gk-tiny-canyon/3.jpg?raw=true" alt="Canyon voxel"/> | <img width="450" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/elliot-gk-tiny-canyon/4.jpg?raw=true" alt="Canyon voxel"/> |
| Fill area and extrude face | Box brush |
| <img width="450" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/elliot-gk-tiny-canyon/5.jpg?raw=true" alt="Canyon voxel"/> | <img width="450" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/elliot-gk-tiny-canyon/6.jpg?raw=true" alt="Canyon voxel"/> |
| Eraser cube & paint area | Paint cube & add voxel |
| <img width="450" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/elliot-gk-tiny-canyon/7.jpg?raw=true" alt="Canyon voxel"/> | <img width="450" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/elliot-gk-tiny-canyon/8.jpg?raw=true" alt="Canyon voxel"/> |
| Object mode| Brush voxel mode & Paint voxel |
| <img width="450" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/elliot-gk-tiny-canyon/9.jpg?raw=true" alt="Canyon voxel"/> | |
| Erase area | |

</p>
</details>

We see that the box brush is heavily used to block out shapes, then they went to pixel mode. There is also the face selection here and there. We will start with the pixel brush, then the box. The line brush and sphere brush are secondary.

#### Development start & Code Guidelines

Now that we had an architecture, and a list of tasks, we could get some work done. But first, we wanted to be certain that we wouldn't be breaking functionnalities as we were developing, and we wanted to be as transparent as possible aubout our progress.

First, we created  **automatic builds** with [Github actions](https://github.com/features/actions). The idea is that each time we push to the repository, a remote server builds our application for linux and wasm. This way, we know when something breaks. It is represented by these two badges (let's hope they're still green) :

[![Actions Status](https://github.com/guillaume-haerinck/voxel-editor/workflows/cpp/badge.svg)](https://github.com/guillaume-haerinck/voxel-editor/actions)
[![Actions Status](https://github.com/guillaume-haerinck/voxel-editor/workflows/wasm/badge.svg)](https://github.com/guillaume-haerinck/voxel-editor/actions)

Then we added unit **unit tests** with the [Catch2 library](https://github.com/catchorg/Catch2). We can't really test OpenGL, but we can test our [Maths functions](https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/test/maths/rbf.test.cpp). It one of it fail, this badge would turn red :

[![Actions Status](https://github.com/guillaume-haerinck/voxel-editor/workflows/unit-test/badge.svg)](https://github.com/guillaume-haerinck/voxel-editor/actions)

Finally, we made a [Changelog](https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/CHANGELOG.md) file (which needs to be updated), and opened some [Github Projects](https://github.com/guillaume-haerinck/cube-beast-editor/projects) to show our progress publicly.

<br>

<p align="center">
<img width="900" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/github-project.png?raw=true" alt="UML"/>
</p>

<br>

## II - Core features

> The needed bits and parts of the app. 

These set of features has been the top priority for us. They shape our application and without them there would be nothing to show. As always in development, it's never over and there is still a lots of improvements to be made. Yet, we are satisfied with the work we have done, and it will be easy and a pleasure to improve when we have time to do so.

___
### A - Renderer

We have an abstracted OpenGL API to call, but now we have to figure out what we want to call. How can we render a large number of cubes on a 3D scene and still be efficient in terms of performance ? We are not the first ones to ask this question, and because of [Minecraft](https://classic.minecraft.net/?join=CZpjn0QLBtY2c5Dc)'s success, we've seen a lot of [open-source projects](https://github.com/minetest/) trying to replicate or improve its inner working.

There are some **really good litterature available online** on this subject. In particular [a few](https://0fps.net/2012/01/14/an-analysis-of-minecraft-like-engines/) [blog](https://0fps.net/2012/06/30/meshing-in-a-minecraft-game/) [posts](https://0fps.net/2012/07/07/meshing-minecraft-part-2/) in 2012 by Mikola Lysenko on his blog 0fps. There is also [this great talk](https://www.youtube.com/watch?v=8gM3xMObEz4) called *Minecraft.js* from Max Ogden which took place during the 2013 NodePDX. Finally, from the page 578 of the book [Real-Time Rendering Third Edition](https://www.realtimerendering.com/) we get some good insights on voxels.

If they can make a [voxel engine](https://medium.com/@deathcap1/six-months-of-voxel-js-494be64dd1cc) running in WebGL, why can't we ?

Well, we want this first version to be simple and fast to create. Yet all of these sources are making their geometry data procedurally based on the position of their cubes. They call this step **Meshing**, and it's not that simple (though we could simply follow [youtube tutorials](https://www.youtube.com/watch?v=jHtqA6j9UMg)). Do we really need this much optimisation about our geometry data for small scenes ? Is this really efficient if the scene is modified a lot like in our case ? Is rendering going to be a bottleneck now, or will it be our data storage ?

In front of these question, we decided to go for a much simpler solution for now. The one that is typically used when you need to render the same mesh many times in a frame. We decided to go for instanced rendering.

#### Instancing

The idea of  instanced rendering is simple. As calling OpenGL from the CPU takes time, why don't we call it only once to render thousands of meshes ? It's not difficult to use, but it does changes the way we assign our positions. Typically, we would send a **world matrix in a constant buffer** to move the object on the 3D scene. But constant buffers are changed between drawcalls, and constant during this call. Meaning that we have to send an array for our translations.

It would be possible to send our constant buffer as an array, but instead it is much more efficient to use one of our attribute buffers. While the geometry data will stay the same when the pipeline is replayed, the translation given in the vertex shader will be the next index of its array.

<br>

![Instanced rendering](https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/renderer/instancing.png?raw=true)


<br>

Instancing becomes efficient when we start to render an important number of primitives, with important number of vertices. However for very simple primitives like cube, it seems like that it doesn't change much as explained in Real Time Rending Third Edition page 796. Yet there is only one way to be certain, it's to benchmark (more on that an part III). There is also an [interesting post](https://nbertoa.wordpress.com/2016/02/02/instancing-vs-geometry-shader-vs-vertex-shader/) by Nicolas Bertoa about Geometry Shader to create our cubes. Howerever these types of shaders are neither supported by OpenGL ES, and neither a good awnser to many problems. Their use is discouraged by a lot of people.

<p align="center">
<img width="600" src="https://software.intel.com/sites/default/files/managed/58/12/instancing2.png" alt="UML"/>
</p>

> [OpenGL ES 3.0 instanced rendering](https://software.intel.com/en-us/articles/opengl-es-30-instanced-rendering), Intel Developer Zone, by Cristiano F., 2014

#### Deferred Shading

In a - not so far - future, we want to be able to handle emissive materials for our cubes in real time. This mean that we will have to handle an important number of lights in our 3D scene. If we did this directly, we would have to process each light againts each fragment in our scene, which is way too much to handle. Instead, we want to **only compute lighting for the objects that are visible by the user**. 

A great way to handle this is to make use of **multiple render passes**, so that you put what is visible inside a texture, and only then compute lighting. Different models exists to make the performance gain even more substantial (like showed by [Jeremiah on 3dgep](https://www.3dgep.com/forward-plus/) in 2015), but we decided as always to **stick to the simpler version for now**.

With deferred shading, you have at least 2 render passes :

1. **Geometry pass** : Where you output your geometry data to textures. Things like positions, normals, albedo colors. To be reused with lighting calculation.

2. **Lighting pass**: Where you compute your lights against the geometry textures, and output the final results.

At the time of this writing, our software has 6 passes per frame (with 3 only dedicated to the cubes). We won't go into the detail of the ligthing equation as many sources are available online, but this is what you got to know :

- We have a small ambient component to simulate global illumination. The cubes are never pitch black.
- We use the [lambert diffuse](https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/diffuse-lambertian-shading) component for our main lighting.
- There is no specularity component, but if we had to implement it, we would have used the [Cook-Torrance](https://learnopengl.com/PBR/Theory) [PBR model](http://www.pbr-book.org/).
- At the end of our lighting pass, we do not apply any [Gamma correction](https://learnopengl.com/Advanced-Lighting/Gamma-Correction), or [HDR correction](https://learnopengl.com/Advanced-Lighting/HDR) as it wasn't matching perfectly with our brush color.

As you can see below, we also generate a [shadow map](https://en.wikipedia.org/wiki/Shadow_mapping) for the directionnal light, and we will add screen-space ambient occulusion (SSAO) in the future. More on that on part III. Below you can see our first 4 passes, the 2 missings ones dedicated to the Gui (we do not want to spoil).

<br>

![Defered shading](https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/renderer/defered-shading.png?raw=true)

<br>

#### Render System

All of our rendering logic has been put into the **RenderSystem** class. There we have an absolute control about **what we give to OpenGL, and in what order**. The first steps is to update our constant buffers if they need to. While the View-Projection matrix is updated each frame, our lights and material informations will only be updated if there has been a change.

Then we preparre for the geometry pass by updating our attribute buffers. We have translations to place our cubes in the scene, an index to the array of materials available in a constant buffer, and finally the id to the entity being drawn (more explanation on this on the selection part). Finally, we can make multiple passes for the cubes, and after all this the Gui.

<br>

![Pipeline](https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/renderer/pipeline.png?raw=true)

<br>

When we open up our App in debug mode to check its render time with NVidia NSight. We see that the render time for the **geometry pass and the shadow map pass increase linearly with the number of cubes**. The lighting pass stays the same as well as for the ImGui render time.

<br>

> 10x10x10 Voxel count
![NVidia NSight](https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/renderer/nsight-debug-10x10x10.png?raw=true)

> 20x20x10 Voxel count
![NVidia NSight](https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/renderer/nsight-debug-20x20x10.png?raw=true)

> 20x20x20 Voxel count
![NVidia NSight](https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/renderer/nsight-debug-20x20x20.png?raw=true)

<br>

We know that we can do and we will need to do way better than this. Between this 3 tests, the screen is almost the same for the user : it's just a cube getting bigger or smaller. But for OpenGL, that's 2 or 3 times more geometry to render. This giant cube is made of smaller cubes.

We know for a fact that WebGL doesn't keep up when the number of meshes to draw start increase too much. This as been showed during the [Google I/0 2019](https://youtu.be/K2JzIUIHIhc?t=1079) as plus to use the new API WebGPU. If we want to target the web, we will need to do procedural mesh as [we've seen before](https://0fps.net/2012/06/30/meshing-in-a-minecraft-game/).

During our research, we've also found that changing OpenGL states can quickly become a bottleneck as well.  That's when we encountered the concept of **UberShaders**. The developers from dolphin emulator made [a really interesting article](https://fr.dolphin-emu.org/blog/2017/07/30/ubershaders/?nocr=true) about this in 2017. The idea is to put as much stuff as possible inside the same set of shaders. In a way, we are merging passes and shaders together. 

**Their problem wasn't really about switching state, it was more about compiling shaders**, but the idea is worth mentioning. It would be fun to see if we could suppess the shadow map pass and handle it in inside of the geometry pass. We might loose performance, as writing a depth map manually might not be that optimized, but we never know.
___
### B - Mouse selection

To pick an object on the screen with a mouse, there are basically 2 techniques : **color picking** and **raycasting**. Well we implemented both, not by fun (though it was), but by nescessity (it wasn't really). We started with color picking as it would be usefull to test our render target abstraction. Our logic went to the **SelectionSystem** class.

#### Color picking

To do color picking, you have to render each mesh with an unique identifier to a texture. Once that is is generated, you can ask OpenGL the color of the texture under the mouse cursor's position. With the color, you find back your identifier and know which object it is related to. We generate the following texture during the geometry pass.

<p align="center">
<img width="300" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/renderer/color-picking.jpg?raw=true" alt="UML"/>
</p>

It is easy, simple, linear in time and yet there is a problem. **It is super slow** if we don't take gloves to implement this solution as you can see below. The problem is that to give us the color under the mouse, OpenGL has to finish its currents tasks, and that take time.

![Profiling](https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/profiling-selection-before.jpg?raw=true)

Fortunately, there is a way to fix this and make it **uber-fast**. The idea is to do this asynchronously. With a special element called a [Pixel Buffer Object](https://www.khronos.org/opengl/wiki/Pixel_Buffer_Object), we can say to OpenGL that we're going to need the color of a pixel at said position. It will continue to work but from now on, if it has time to spare, it will fill our Pixel Buffer with the data we need. Then a few times later in the same frame, we can ask for it, and it will be ready. Using this trick got us **from 0.307ms to 0.034ms**, that's an order of magnitude faster.

![Profiling](https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/profiling-selection-after.jpg?raw=true)

#### Raycasting

Now that we had color picking, why use another technique ? Simply because color picking is not a good fit for every problem. In our case, now that we could pick an object on the screen, we wanted to **pick a cell on  the grid**. Each cells on the grid are not separate objects, it's just a texture on a cube. This meant that we had to check for the point of intersection, we had to go for raycasting.

The first step consist of creating a ray which goes from the near plane to the far plane, as seen below. To do so, we create a matrix to go from the Normalized Device Coordinates (NDC) to the World Coordinates. The formula to get there is straightaway `NDCtoWorld = inverse(matProj * matView)`. 

<br>

<p align="center">
<img width="750" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/ndc-cube.png?raw=true" alt="UML"/>
</p>

<br>

Then we can get our vector going from the near plane to the far plane. Notice the perspective divide at the end.

```C++
vec4 from = NDCtoWorld * vec4(posX, posY, -1, 1);
from /= from.w;
```

If we draw our vector from the camera position, we can ensure that it works using our DebugDraw class.

<p align="center">
<img width="300" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/debug-draw-raycast.gif?raw=true" alt="UML"/>
</p>

From there, it is just a question of line / plane intersection, with [plenty](http://www.jeffreythompson.org/collision-detection/table_of_contents.php) [of litterature](https://www.youtube.com/watch?v=fIu_8b2n8ZM) [online](https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-plane-and-ray-disk-intersection).

___
### C - Brushes

We've seen in the part I that 2 brushes are used the most. The box brush to quickly create shapes, and the voxel brush to add details. We implemented both in a class called **BrushSystem** as you can see below.

| | Add | Remove | Paint |
| :---: | :---: | :---: | :---: |
| **Voxel** | <img width="200" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/add-vox.gif?raw=true" alt="Tool"/> | <img width="200" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/delete-vox.gif?raw=true" alt="Tool"/> | <img width="200" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/paint-vox.gif?raw=true" alt="Tool"/> |
**Box** | <img width="200" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/add-box.gif?raw=true" alt="Tool"/> | <img width="200" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/delete-cube.gif?raw=true" alt="Tool"/> | <img width="200" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/paint-cube.gif?raw=true" alt="Tool"/> |

However, we quickly found a bottleneck using the Box Brush. Because of our data structure, our random access time for a position is O(n), we don't know if a cube exist at a certain position if we don't iterate over our list. As we have to do this for every position on our selection, the **complexity goes to O(n^2) which is really bad**. 

We can see below that we've got quite the bottleneck. It starts slow, but it increase really fast as the complexity says.

![NVidia NSight](https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/brush-bottleneck.png?raw=true)

This means that we quickly have to find a way to get random access for our position at O(1). There are multiple ways to do so, but we are still unsure about which one to use. In particular, if we should keep ECS and use one of these methods to store an ID rather than a position.

| Name | Description | Pluses | Problems |
| --- | --- | --- | --- |
| Look-Up-Table | An array, size of the scene. The index in the array represents a position. Can store only a boolean to say wether or not there is a cube | Easy | Lots of empty areas, so a lot of cache misses during iteration |
| [Sparse set](https://www.geeksforgeeks.org/sparse-set/) | Two arrays, one dense, one sparse. The dense contains our position without holes. The sparse is of the size of the scene, and contains indices to the dense array. | Easy. Can iterate on dense without holes | Wasted memory with sparse array |
| [Interval tree](https://www.geeksforgeeks.org/interval-tree/) | Similar to a binary tree, each parents contains an interval of min-max value. | No unsused memory | Can be tricky to implement correctly. Random access is quick but not exactly O(1) |
| Octree | A tree that recursivly subdivides space into equal sized octants| No unsused memory | [Said](https://0fps.net/2012/01/14/an-analysis-of-minecraft-like-engines/) [to be](http://www.sea-of-memes.com/LetsCode32/LetsCode32.html) not that efficient for voxel-like engines because of cache misses and memory fragmentation |

We will try a few of them in the future and benchmark the results. Our main criteria being speed and not memory usage.

___
### D - Interface and User controls

One of the most important part of a software. You can make the most performant and the most features-full editor, but **if it's a pain to use, no one is going to care about it**. We are not really UI Designer ourselves, but we know were to look for inspiration and what kind of toolset we need to make this happen.

#### ImGui pimping

It's kind of classic now to be using [ImGui](https://github.com/ocornut/imgui) to make the GUI of an editor. Performant, easy to use - though a real documentation would be nice - and to maintain, it's the tool of choice of us. Yet to make a software-like interface, and by that I mean with a resizable viewport and side-windows, it can be tricky to get there on the master branch.

Fortunately for us, the **Docking branch** gives us exactly that. You might have to look on the forums to get enough understanding on how to use it, but once that it is set if offers great control to structure the layout of our application.

<p align="center">
<img width="500" src="https://user-images.githubusercontent.com/8225057/46304087-00035580-c5ae-11e8-8904-f27a9434574a.gif" alt="UML"/>
</p>

As you'll see below. We also modified the imgui styling to **depart from the easily recognizable imgui-look**. There are some great exemples on the [realeases page](https://github.com/ocornut/imgui/releases). The pimping isn't as full as can be the CSS language for exemple, but you can make really clean interfaces from it.

#### Icons support

We knew that we would need to handle icons in our App. It is a way user-friendly way to present your tabs and buttons. We first tried it by using an image-texture (below) applied on buttons, but the library had some problems [handling mouse clicks](https://github.com/ocornut/imgui/issues/2464).

<p align="center">
<img width="200" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/icons-test.png?raw=true" alt="UML"/>
</p>

Then we moved to font-icons, the idea of using a font to show your icons, and it went well. We used juliettef's [IconFontCppHeaders](https://github.com/juliettef/IconFontCppHeaders) with [Awesome Icons](https://fontawesome.com/icons?d=gallery) to get there. As ImGui rasterizes the fonts for display, we have to generate 2 sets of textures and switch between them to change the size of the font.

#### Feedbacks

To let the user know of what he is using and how to use it, we implemented a few behaviors into our app. 

The first one is a context bar available at the bottom. Depending on the tool selected and the position hovered, it will give to the user available actions.

<p align="center">
<img width="800" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/changelog-img/v0-5-0.png?raw=true" alt="UML"/>
</p>

Then there is the hovered area, which is a plane of color as MagicaVoxel does. Me might add different representation depending on the selected tool in the future, or even let the choice to the user.

<p align="center">
<img width="500" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/hover-plane.gif?raw=true" alt="UML"/>
</p>

#### Arcball camera

Unity, Blender, MagicaVoxel, Unreal Engine, they are all using the same kind of camera for their editor, an [arcball camera](https://threejs.org/examples/#misc_controls_orbit). It's commonly used and yet, the documentation you can find is always lacking one crucial part : **camera panning**.

<p align="center">
<img width="500" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/arcball.jpg?raw=true" alt="UML"/>
</p>

We fough hard, and helped by [a dear friend](https://github.com/JulesFouchy), we finally managed to get there using this simple formula (dx and dy being the wanted amount of panning) :

```
vec4 col0 = normalize(inverse(matView).col0)
vec4 col1 = normalize(inverse(matView).col1)
vec4 movement = col0 * dx + col1 * dy;
camera.target += movement.xyz;
```

The logic for the camera is handled solely in the **CameraSystem**.

___
### E - Procedural terrain generation

#### Radial basis functions

The procedural terrain generation uses Radial Basic Function (RBF) interpolation. The users gives controle points, associatied weights and a chosen radial basis function. From these parameters we approximate a `R2 -> R3` function.

<p align="center">
<img width="600" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/rbf-schema.png?raw=true" alt="UML"/>
</p>

Every point in the series has a field around it : an RBF (in red). Each point has a position `(Xi, Yi, Zi)`. We want to find the image of an arbitrary point `(x, y, z)` knowing x and y considering the others points. We can approximate the position of this point by adding a linear combination of the RBFs.

<p align="center">
<img width="300" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/rbf-1.png?raw=true" alt="UML"/>
</p>

`|C-Ci|` is the distance between the point to interpolate and the i-th control point.

We just need to find the w_i coefficients. We use the known points values from the N points to create a system of N linear equations and N unknowns.

<p align="center">
<img width="600" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/rbf-2.png?raw=true" alt="UML"/>
</p>

#### Available parameters

To generate a terrain, the user gives control points, weights, and a radial basis function.

We have a function to determine `Wi` coefficients, and a function that interpolate each `(x, y)` point of the grid. We can see those points as a partial 3D-surface.

To generate the terrain, we browse the grid and generate cubes on each case. If the z-coordonate is lower than the z-coordonate of the interpolated point. We generate cubes below the 3D-surface we interpolated.

#### Parsing a file

Procedural generation wouldn't be that usefull if the user had no say in it. While we're still working on the user interface part, we allowed the user to **load a custom scene file**, which can contains procedural-generation data in it.

Our main inspiration for this format is the [gltf 2.0](https://github.com/KhronosGroup/glTF-Tutorials/tree/master/gltfTutorial), which had the clever idea to mix .json data structure and binary files. Without further detail, let's show an exemple of our format (work in progress).

<details><summary>Show .cbe file example</summary>
<p>

```json
{
    "software": "cube-beast-editor",
    "version": "0.0.1",
    "geometry": [
        {
            "cube": {
                "from": [0, 0, 0],
                "to": [3, 3, 3],
                "paletteIndex": 1
            },
            "voxel": {
                "position": [5, 5, 5],
                "paletteIndex": 1
            },
            "voxelChunk": {
                "byteLength": 1024,
                "uri": "3x3x3.bin"
            }
        }
    ],
    "palette": [
        {
            "color": [255, 255, 255]
        },
        {
            "color": [0, 255, 0]
        }
    ],
    "generation": [
        {
            "type": "rbf",
            "interpolation": "linear",
            "mode": "move",
            "controlPoints": [
                {
                    "position": [0, 0, 0],
                    "weight": 5
                },
                {
                    "position": [5, 5, 5],
                    "weight": 1
                }
            ]
        }
    ]
}
```

</p>
</details>

ImGui can create a menu which asks the user to open a file, but it doesn't know how to access the OS-specific API to open a window to get a file. Fortunately for us, we've found a cross-platform single-file library to handle this labour for us. It is called [native file dialog](https://github.com/mlabbe/nativefiledialog), and it is magic.

Then, one that we get the file path, we use yet another library - [nlohmann's json](https://github.com/nlohmann/json) - to parse the Json format. As we plan to support .gltf model import in the future (already done in other projects), we will need it anyway, and it's a really solid library.

#### Trial

We tried a few configuration with more or less control points. As our scene is not enormous, the benefits of RBF are particularly visible to create complex curvatures with voxels. With those we can emulate a valley, an important or small slope, or a mountain.

| <img width="300" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/0.0.0-0.20.0-20.0.20.png?raw=true" alt="Tool"/> | <img width="300" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/0.0.0-20-20-20.png?raw=true" alt="Tool"/> | <img width="300" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/10.10.10-0.20.0.png?raw=true" alt="Tool"/> |  <img width="300" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/16.20.18-20.0.20.png?raw=true" alt="Tool"/> |
| --- | --- | --- | --- |

## III - Additional features
> It's great if it works, it's better if it works well

You have already witnessed some of them previously, but now it's time to talk about our freebies. These little features that add to the value of this project. Some of them can make our life miserable, I'm looking at you WASM, but some of them can be sources of great joy of discoveries, like benchmark my dear boy. Without further ado, let's get into this.

___
### A - WASM

[Web Assembly](https://webassembly.org/) is the new cool kid in a town of web developers. It is a binary instruction format playable on a virtual-machine run by a web-browser. It allows to **write a piece of code in C++, C#, Go, Python, Rust or anything else, and be called by Javascript**. Kind of impressive right ? (for more details about how it works, go check [mozilla](https://hacks.mozilla.org/2017/02/what-makes-webassembly-fast/) [hacks](https://hacks.mozilla.org/2017/02/a-cartoon-intro-to-webassembly/) by Lin Clark).

<p align="center">
<img width="300" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/wasm.jpeg?raw=true" alt="Tool"/>
</p>

Our project uses C++, and SDL2 with OpenGL ES for graphics. The main C++ to WASM compiler that exist, [Emscripten](https://emscripten.org/), supports exactly that workflow. It does comes with some constraints, and it is not always easy to play with it, but we think that this feature is worth some disagreements.

#### Integration

The first big change is the application loop that you can find in `main.cpp`. Instead of just running your app, you must put it inside a free function and call `emscripten_set_main_loop_arg()` with it on Emscripten builds.

Then, [glad](https://glad.dav1d.de/), the OpenGL function loading library that we use, is incompatible with WASM. Meaning that we have to define-out portions of code which are including glad to include `<GLES3/gl3.h>` instead. Fortunately for us, with our architecture there are only 3 places where this happens : *render-command.h*, *opengl-exception.h* and  *app.h*.

Finally, Emscripten is capable to emulate a [file-system](https://emscripten.org/docs/porting/files/file_systems_overview.html), yet this feature takes place in the build size (which is an important point when you target the web) and we tried to not-use it. To do so, our shaders are defined with raw-string literals and included in our sources files with a little trick. With that we do not have to load them at runtime.

```C++
// MyShader.frag
R"(MyShaderContent
)"

// C++ code
const char* myShader =
	#include  "MyShader.frag"
;
```

Then, we use filesystem to allow the user to open a `.cbe` model. For now, we simply defined-out the code to handle this, but we are seeking a better way to handle this in the future.

#### Incompatibilities

OpenGL ES 3.0 and [WebGL 2.0](https://www.khronos.org/registry/webgl/specs/latest/2.0/) specifications are really close to each other. This allows Emscripten to **replace OpenGL ES function calls by their WebGL equivalents**. Yet, the specification differs at some points, a feature might be available, but the way to handle it changes.

It exactly what happened when we tried to implement Pixel Buffer Objects to read our Render Target asynchroniously when dealing with selection. We cannot read it using [glMapBufferRange()](https://www.khronos.org/registry/webgl/specs/latest/2.0/#5.14). There is an equivalent called glGetBufferSubData(), but we cannot use it as it does not exist in the OpenGL ES specification, so we cannot build with it on Emscripten.

For now, we read synchroniously our framebuffer when we are with Emscripten, but this has a performance hit. We are [not the firsts](https://github.com/emscripten-core/emscripten/issues/5861) to encounter this issue and there seems to have ways to fix it, but it wasn't a priority for now.

There is also a problem with **[non-power of 2](https://www.khronos.org/opengl/wiki/NPOT_Texture) Render Targets**. It works for textures, but not for deepth buffer. As the framebuffer is always the size of the viewport, we had to round our Shadow Map to the upper-power of 2. This means that on Emscripten builds, shadows do not behave that well. As well, we will try to only use power-of-2 render targets in the future, but it is not a priority.

___
### B - Shadows

It is when they are missing that we notice them. They show deepth, allows to separate objects, and are just giving a fine look to your 3D scene. We have implemented a shadow map for the main directionnal light, and we are eager to use SSAO in the near future.

#### Shadow mapping

#### SSAO

[https://0fps.net/2013/07/03/ambient-occlusion-for-minecraft-like-worlds/](https://0fps.net/2013/07/03/ambient-occlusion-for-minecraft-like-worlds/)

___
### C - Benchmarking

You have seen in the previous posts that we have the capacity to measure the time of special portion of our code and display it visualy. It goes the same for our OpenGL Calls, which are grouped by names under RenderDoc or NSight. These are 2 different features that we will cover.

#### C++ Visual Benchmarking

It's a video by [TheCherno](https://www.youtube.com/watch?v=xlAH4dbMVnU) that got us started on this subject. On it, he presents a fast an easy way to measure the time of a function, and outputs the result in a .json file. [The format](https://docs.google.com/document/d/1CvAClvFfyA5R-PhYUmn5OOQtYMH4h6I0nSsKchNAySU/preview) is the same one used by Google Chrome performance tracer, and allows visualisation on this link : [chrome://tracing](chrome://tracing).

<p align="center">
<img width="400" src="https://www.chromium.org/_/rsrc/1441252029512/developers/how-tos/trace-event-profiling-tool/trace-event-reading/Screen%20Shot%202015-09-02%20at%208.46.48%20PM.png?width=700" alt="Tool"/>
</p>

It is so usefull that many [game developers](https://docs.google.com/presentation/d/1ST3mZgxmxqlpCFkdDhtgw116MQdCr2Fax2yjd8Az6zM/edit?usp=sharing) are using it, and we can understand them. Free, Open source, IDE-independant and available everywhere, you can shape the data in the way you like with the categories you like.

For our needs, we defined a Macro called, `PROFILE_SCOPE(myName)` which start a timer on creation, and stops it when we reach the end of the scope (the next `}`). The Macro is replaced by nothing if we do not want profiling available, so that there is no performance impact on release builds.

The code is available on the **lib** folder, inside the profiling section.

#### OpenGL Debugging

From OpenGL 4.4, you can get very detailled error information, and annotates your calls so that they are more easily debbugable. Yet, it is not available for previous versions so you have to find another workaround.

Again, [TheCherno](https://www.youtube.com/watch?v=FBbPWSOQ0-w) made a video on this subject where he defines a macro called `GLCall`. The idea is that you wrap every of your glXXX functions inside of it. The macro will erease the error stack, ask OpenGL if there is any, and output the result if there is. Again, this behavior is erased on Release builds. (code is available on `src/graphics/gl-exception` file)

We were lucky that, for our OpenGL target, we could use the **KHR debug extension**, which is the OpenGL 4.4 feature before that it was merged into Core specification. Because of it, we get a nicer debbuging experience with [RenderDoc](https://renderdoc.org/docs/window/event_browser.html) and way more detailled error messages.

The calls are grouped with the macro `OGL_SCOPE(myName)` which also profile the calls made (we enabled synchronious mode in debug so that we get the real OpenGL time).

<p align="center">
<img width="300" src="https://renderdoc.org/docs/_images/QuickStart41.png" alt="Tool"/>
</p>

___
### D - History

Handling `ctrl + z` and `ctrl + y` (undo & redo) can be way more tricky to implement than it seems like at the first look. We were lucky that we encountered the [Command Pattern](https://gameprogrammingpatterns.com/command.html) to help us in its integration (even though it is not used yet, because we will soon change our data structure for random position O(1) access).

#### IHistory abstract class

As any other design pattern, our story starts in a little village, with an abstract class. This class, IHistory has 2 important methods : undo() & redo(). Each classes that will implement those will need to be able to handle them. To do so, they must have access to some data, so each of these concrete classes have specific constructor to handle their specific data needs.

#### History handler

To handle our stack of *IHistory**, we have another class called HistoryHandled globally accessible from the Context. With it, you can know the size of the current available history, add a new IHistory and of course, undo and redo with no limits.

As we've said, everything is in place but we are wainting to change our data structure before using it, it it would be a waste of time to write to same kind of algorithm multiple times.

<p align="center">
<img width="300" src="https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/uml-5.png?raw=true" alt="Tool"/>
</p>

## Conclusion

You might have understood that it is difficult to call this part *Conclusion* as **we will continue to work on this project for a while**, yet, we can have a look at what we've achieved during this month of code and see what we've learnt from it. Starting by getting our hands in the machinery of other engines in particular was determinant for us. Because of that we could better see what we were aiming for and what might be the steps to get there. It really shaped the project and put us on good tracks.

Our first big achievement was the creation of the [Met ECS library](https://github.com/guillaume-haerinck/met-ecs), where we learnt a lot about data-oriented design and C++ template meta-programming. Even though we will probably discard this library in the future, it was great to make and use it. From this point, we could make our first 3D scene and start to abstract OpenGL from our DirectX and previous project knowledge. It took 2 weeks to have the **basic sets of raw functionnalities** (instancing, selecting, adding).

Then we could **start to clean up, refine and add features**. Using the Docking branch of ImGui and getting the interface right was a big step of this and a lot of effort went into it. As what came after, we took the time to understand how it worked, so that the implementation was working as intented without a prototyping-feeling.

Writing the post-mortem took a fair-amount of time as well, but it was important for us to explain our development process, and helps criticism about the whole architecture. We feel satisfied of our work, and ready to improve it when we will have time to spare.
