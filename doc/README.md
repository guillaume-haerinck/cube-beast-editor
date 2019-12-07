## Post mortem structure

wasm constraints
dev met-ecs lib
unit test
continuous integration
previous project (opengl-playground)
singleton components (overwatch gdc conf and unity api and last project with events everywhere)
constant buffers instead of uniforms
instanced rendering
command pattern for undo and redo
per-vertex normals with a cube with 8 vertex is less efficient as we have to interpolate between 4 each time to get surface normal
rename opengl Object to well known equivalent (uniform buffer object -> constant buffer | framebuffer -> renderTarget | vertex array -> deleted )
