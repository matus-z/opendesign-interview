# OpenDesign interview assignment - a simple 2D Renderer
Coding Challenge for the **C++ Developer** position at **Ceros/OpenDesign** - http://labs.ceros.com/jobs/#/job/4423147.

Source: https://github.com/matus-z/opendesign-interview.

## Getting Started
The **Renderer** is a simple `CMAKE` program.

Open the `OpenDesignRenderer` directory and use the `CMakeLists.txt` to make and compile the **Renderer** library and the included test. The test will compile into an executable application that will verify the correctness of **Renderer**'s functionality.

## TODO
Outside of the scope of this assignment are:
- Fully include RGBA Bitmap helper library (https://github.com/bzotto/rgba_bitmap/). Possibly as a git submodule.
- Improve test coverage. Cover all functionality with unit tests.
- Make `Image` and `Renderer` tests independent of the supplied image files. Use procedurally generated images as inputs.
- Use `OpenMP` to parallelize rendering.
- Improve the resampling algorithm for scaling an image - use bilinear or bicubic interpolation - specifically when upscaling the image.

## Know differences with the example output image
- The example output image contains blue background outside of the rectangle. No instruction in the coding-challenge.pdf specifies this.
- Minimal differences on alpha-blended pixels on the image, as a result of imprecision.
- The colors in the scaled image-C are visibly different due to using a different resampling algorithm.
