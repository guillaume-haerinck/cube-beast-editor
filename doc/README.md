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
Uber shader to make only one pass instead of multiple with lots of binding and unbinding (measure performance gain)
Render target with ints are a pain to use (unknown values, clear different, etc), so cast entities to float to improve it
Voxel add tool, explosion if just add. Must limit to 1 add in height per click.
Sparse set might be way more efficient to store and access cube data, might ditch ecs in the future as need access in O(1) of pos for advanced brushes, and cube storage in multiple chunks instead of one big array
See if can store neightboring cubes as one rectangle, multiple is there is a hole. Render with instanced plane, but different model mats
-> ECS might not be the best solution, but it's the one which will help me learn more for now (benchmarking, ecs use case, improving my lib)

## Planned (high to low priority)

handle cube color
procedural terrain creation 

shadows
select in xray
select face
select neightbooring color
edit mode and world move mode
remesh (render only visible faces instead of cubes, maybe can be stored like plane as well)
benchmarking
groups / collection / layers
modifiers (array, mirror, etc, ...)
parenting

boolean operators
rigging
animation
