# OpenGL GLSL Demo

Triangle rendered in OpenGL 3.3 Core Profile with animated GLSL shaders.
Built with GLAD and GLFW in C++, Visual Studio 2022.

## What it does
- Renders a triangle with per-vertex colors (red, green, blue)
- Fragment shader animates the color using sin(time) uniform
- Demonstrates the full OpenGL pipeline: VBO, VAO, vertex + fragment shader

## Pipeline
CPU data → VBO (VRAM) → Vertex Shader → Rasterizer → Fragment Shader → Framebuffer

## Setup
1. Clone the repo
2. Open `Opengl.sln` in Visual Studio 2022
3. Download GLFW 64-bit from glfw.org and put `glfw3.lib` in `lib/`
4. Build and run

## Controls
- `ESC` — close window

## Stack
C++ · OpenGL 3.3 · GLSL · GLAD · GLFW
