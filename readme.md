## About

This is a HLSL tilemap implementation. The tilemap itself is rendered on a single quad, so the pixel shader does the heavy lifting. Using the UV, a given pixel's tile position is found. This is then used to index a structured buffer called `tiles` which holds tile type info. Using the tile type, the pixel will then calculate and sample the correct region of the texture atlas. Because all tilemap rendering is done on a per-pixel basis, there are no limits to the size of a tilemap with this implementation, unlike in others. Currently a compute shader (world_generator.hlsl) is running on startup to fill the `tiles` buffer, though custom tile data could also be uploaded if desired.

Relevant files are located in:
- shaders/tilemap.hlsl
- shaders/world_generator.hlsl
- include/tilemap.h
- src/tilemap.cpp

<p>Below is a 4096 x 4096 tilemap.</p>
<img src="./textures/screenshot.png?raw=true"  width="600" height="600">
<img src="./textures/video.gif?raw=true"  width="600" height="600">

## Performance

On a RTX 3090 at resolution of 1080 x 1080, this implementation runs at ~12,000 fps.

## Building
1. Navigate to `/build` directory
2. Run `cmake .` command.
3. Specify desired settings in `include/settings.h` file.
4. Run `make` command.
5. Executable should be ready.

## Controls
- WASD to move camera
- Mouse scroll wheel to zoom in and out

## Future Plans
- Multiple layers
- Isometric tilesets
- Mip mapping
- Custom depth buffer values, allowing non-tilemap sprites to be y sorted

## Acknowledgements

Textures borrowed and slightly modified from https://opengameart.org/content/overworld-grass-biome.