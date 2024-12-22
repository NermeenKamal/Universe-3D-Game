# OpenGL Universe

This is an OpenGL project that demonstrates the rendering of a simple 3D environment with rotating spheres (Earth and Moon) and a textured background. The project also includes lighting effects and basic keyboard controls to manipulate the scene.

## Features
- 3D rendering of Earth and Moon with realistic lighting effects (ambient, diffuse, and specular).
- Background image with texture mapping.
- Support for continuous auto-rotation of objects.
- Interactive keyboard controls for manipulating the scene.
- Support for controlling the rotation of objects along X, Y, and Z axes.

## Requirements
- OpenGL (GLUT) for rendering 3D objects.
- `stb_image` library for loading images.
- SDL2 (optional) for audio functionality (currently commented out in the code).

### Dependencies:
- GLUT (for OpenGL window creation and handling input events)
- stb_image.h (for loading images)
- SDL2 (for audio, if desired)

You can install the required libraries as follows:
- **GLUT**: Follow the installation guide for your operating system.
- **stb_image.h**: Download the file from [stb_image](https://github.com/nothings/stb).
- **SDL2** (optional): You can download and install SDL2 from [SDL2's website](https://www.libsdl.org/).

## Installation

1. Clone this repository:
    ```bash
    git clone https://github.com/yourusername/OpenGLUniverse.git
    ```

2. Navigate to the project directory:
    ```bash
    cd OpenGLUniverse
    ```

3. Make sure you have the required dependencies installed (GLUT, stb_image, SDL2).

4. Compile and run the project:
    - On Linux/macOS:
      ```bash
      g++ -o universe main.cpp -lGL -lGLU -lglut -lm
      ./universe
      ```
    - On Windows, use your preferred IDE or a compiler like MinGW with appropriate linking for OpenGL and GLUT.

## Controls

- **Arrow Keys**: Rotate the scene (Up, Down, Left, Right).
- **S**: Reset rotation to 0 for all axes and stop auto-rotation.
- **A**: Start auto-rotation of objects.
- **O**: Move the object along the X-axis (open).
- **C**: Move the object along the X-axis (close).
- **Z/X**: Rotate the object along the Z-axis.
- **ESC**: Exit the program.

## Lighting
The project supports multiple lighting modes:
1. Ambient lighting only.
2. Diffuse lighting only.
3. Specular lighting only.
4. Combined lighting (Ambient + Diffuse + Specular).

The lighting setup is customizable within the `setupLighting` function.

## Background Image
A background image is loaded using the `stb_image` library. Replace the `uni.png` file path with your desired image for the background texture.

## Notes
- The SDL2 audio functionality is currently commented out in the code. To enable audio, uncomment the relevant code sections and ensure you have the appropriate audio file and SDL2 installed.
- The camera is set up to view the scene from a fixed position and allows rotation of the Earth and Moon using the keyboard.
ing README.md…]()
