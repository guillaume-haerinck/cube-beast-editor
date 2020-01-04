# Cube Beast Editor post-mortem

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

[**IV - Planned features**](#iv---planned-features)

[**Conclusion**](#conclusion)

[**References**](#references)

</p>
</details>

## Specifications

Beast Voxel Editor is a software which allows you to easily generate, visualize and edit 3D scenes. It has been started as a class project for the [IMAC engineering school](https://www.ingenieur-imac.fr/), and had some requirement to meet in order to be evaluated by our teachers. The original specifications can be found [here](https://docs.google.com/document/d/1aDahRcA8aeTSNqdqXv5i8FGPoVUmYzQ-k4z49wlu1K0/edit?usp=sharing), and we made a summary of these features below. 

<details><summary>Show Specifications</summary>
<p>

The *D* column means *Done* and the *O* column means *Required*

| D | R | Description | Comments | Part |
| --- | --- | --- | --- | --- |
| X | X | Render a 3D scene with cubes | | [II - A](#a---renderer) |
| | X | Select cube with keyboard | Replaced by mouse select| [II - B](#b---mouse-selection) |
| X | | Select cube with mouse | Raycast & Framebuffer color picking | [II - B](#b---mouse-selection) |
| X | X | Show selected cube with an outline | Multiple modes | [II - D](#d---interface-and-user-controls) |
| X | X | Paint selected cube | | [II - C](#c---brushes) |
| X | X | Add and Remove cubes | | [II - C](#c---brushes) |
| | X | Extrude tool | | [II - C](#c---brushes) |
| X | X | Procedural terrain generation with RBF | | [II - E](#e---procedural-terrain-generation) |
| X | X | Directionnal light | | [II - A](#a---renderer) |
|  | X | Point light | | [II - A](#a---renderer) |
| | | Paint adjacent faces | | [II - C](#c---brushes) |
| | | Proportional edit | | III - |
| | | Save scene to custom file | | III - |
| | | Load a 3D file (.gltf) | | III - |
| | | Change grid size | | III -  |
| | | Textured cubes | | III - |

</p>
</details>

Additionaly, we added features not mentionned in the specifications.

<details><summary>Show Additionnal features</summary>
<p>

| Description | Comments | Part |
| --- | --- | --- |
| [Met ECS library](https://github.com/guillaume-haerinck/met-ecs) | A post-mortem for this library is available [here](https://github.com/guillaume-haerinck/met-ecs/tree/master/docs) | [I - B](#b---architecture-and-data-structure) |
| Shadows | | III - B |
| Instanced rendering | | [II - A](#a---renderer) |
| Deferred shading | | [II - A](#a---renderer) |
| ArcBall Camera | | [II - D](#d---interface-and-user-controls) |
| Benchmarking | | III - C |
| WASM build support | | III - A |
| ImGui Docking branch | | [II - D](#d---interface-and-user-controls) |

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

That's why we came up with another design for singleton components. **Simply store them inside a struct**. This struct will be passed by reference to our systems like the registry. The **data is private, but public function exists** to get const references to them. We used the concept of **C++ friend classes** to make those component modifiable by one or two classes only, and read only for all other.

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

To get to this first version on time, we knew we had follow a planning. While some priorities where obvious, like rendering a cube on a scene, it remained difficult to make a choice about which optional features to support rapidly. We knew what the other voxel editors had, but **what was used the most often, what was the core ?** To awnser this question, we simply took a look on youtube to see how artists works.

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

Finally, we made a [Changelog](https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/CHANGELOG.md) file, and opened some [Github Projects](https://github.com/guillaume-haerinck/cube-beast-editor/projects) to show our progress publicly.

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

In a - not so far - future, we want to be able to handle emissive materials for our cubes in real time. This mean that we would have to handle an important number of lights in our 3D scene. If we did this directly, we would have to process each light againts each fragment in our scene, which is way too much to handle. Instead, we want to **only compute lighting for the objects that are visible by the user**. 

A great way to handle this is to make use of **multiple render passes**. Different models exists to make the performance gain even more substantial (like showed by [Jeremiah on 3dgep](https://www.3dgep.com/forward-plus/) in 2015), but we decided as always to **stick to the simpler version for now**.

With deferred shading, you have at least 2 render passes :

1. **Geometry pass** : Where you output your geometry data to textures. Things like positions, normals, albedo colors. To be reused with lighting calculation.

2. **Lighting pass**: Where you compute your lights against the geometry textures, and output the final results.

At the time of this writing, our software has 6 passes per frame (with 3 only dedicated to the cubes). We won't go into the detail of the ligthing equation as many sources are available online, but this is what you got to know :

- We have a small ambient component to simulate global illumination. The cubes are never pitch black.
- We use the [lambert diffuse](https://www.scratchapixel.com/lessons/3d-basic-rendering/introduction-to-shading/diffuse-lambertian-shading) component for our main lighting.
- There is no specularity component, but if we had to implement it, we would have used the [Cook-Torrance](https://learnopengl.com/PBR/Theory) [PBR model](http://www.pbr-book.org/).
- At the end of our lighting pass, we do not apply any [Gamma correction](https://learnopengl.com/Advanced-Lighting/Gamma-Correction), or [HDR correction](https://learnopengl.com/Advanced-Lighting/HDR) as it wasn't matching perfectly with our brush color.

As you can see below, we also generate a [shadow map](https://en.wikipedia.org/wiki/Shadow_mapping) for the directionnal light, and we will add screen-space ambient occulusion (SSAO) in the future. More on that on part III.

<br>

![Defered shading](https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/renderer/defered-shading.png?raw=true)

<br>

#### Render System

![Pipeline](https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/renderer/pipeline.png?raw=true)

![NVidia NSight](https://github.com/guillaume-haerinck/cube-beast-editor/blob/master/doc/post-mortem-img/renderer/nsight.png?raw=true)


[https://fr.dolphin-emu.org/blog/2017/07/30/ubershaders/?nocr=true](https://fr.dolphin-emu.org/blog/2017/07/30/ubershaders/?nocr=true)

___
### B - Mouse selection

- Framebuffer with color picking
- Raycasting

___
### C - Brushes

- Voxel add tool, explosion if just add. Must limit to 1 add in height per click.
- Color painting

___
### D - Interface and User controls

- Docking branch imgui
- imgui imagebutton not working correctly when pressed, use font icon instead
- Arcball camera Panning

___
### E - Procedural terrain generation

- Ui choices todo
- RBF maths functions testing

## III - Additional features
> It's great if it works, it's better if it works well

___
### A - WASM

- OpenGL es constraints
- Using emscripten directly

___
### B - Shadows

- Shadow mapping
- SSAO
[https://0fps.net/2013/07/03/ambient-occlusion-for-minecraft-like-worlds/](https://0fps.net/2013/07/03/ambient-occlusion-for-minecraft-like-worlds/)

___
### C - Benchmarking

- Open source chrome tracing
- KHR debug opengl extension
- [https://renderdoc.org/docs/window/event_browser.html](https://renderdoc.org/docs/window/event_browser.html)

___
### D - Undo & redo

- First think with enum
- command pattern

## IV - Planned features
> A sort of to-do list but with more research

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
