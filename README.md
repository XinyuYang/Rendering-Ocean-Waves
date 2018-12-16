# Rendering-Ocean-Waves
A project exploring ocean waves rendering techniques in OpenGL

## Overview
The project is created by Haimeng Zhang and Xinyu Yang.

It uses C++ to implement a shader for ocean wave texture. The project was developed with macOS systems and basicgraphics library.

## Libraries and other tools
* `BasicGraphics` for abstraction of OpenGL classes, e.g. loading model mesh, texture and shaders
* `GLFW` for managing windows
* `Blender` tool for creating the 3D model mesh

## References
* Real-time Realistic Ocean Lighting using Seamless Transitions from Geometry to BRDF (Bruneton, 2010)
* On Modeling and Rendering Ocean Scenes (Gonzato & Le Saec)

## Details
Basic ocean mesh was downloaded from [https://www.turbosquid.com/3d-model/ocean](https://www.turbosquid.com/3d-model/ocean) and modified in Blender

Sky box images were downloaded from http://mi.eng.cam.ac.uk/~agk34/resources/textures/sky/?C=S;O=A.

We followed the illumination model of Output color = OceanColor + F * SkyReflection +( Ambient +  Diffused)


## Results



