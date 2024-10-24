# Physis
Physis is a physics simulator with a dedicated physics and rendering engine.  Its purpose is to provide an educational demonstration of well-known physical laws, and its modular architecture allows client applications to customize the simulated physics.  Its name is derived from the Greek term _[physis](https://en.wikipedia.org/wiki/Physis)_, which is typically translated into English as "nature".

## Supported Platforms
Physis is currently a Windows-only 64-bit application.  Support for additional platforms will be considered in the future.

## Setup
### Tools
It is recommended that you use [Visual Studio 2022 Community](https://visualstudio.microsoft.com/vs/community/) to compile this source code.  It is possible to compile using a different version of Visual Studio, a different IDE, or by manually compiling with MSBuild, but these methods have not been explicitly checked by the developer and may require additional configuration steps.  If you do use Visual Studio, be sure to include "Desktop development with C++" in your installation.

Once you have a proper build environment set up, clone this repository into a suitable location on your local machine.

### Dependencies
At this time, Physis will not build out of the box.  There are a few external dependencies not committed to source control that you need to add.  An automated setup pipeline will be considered in the future.  For now, please follow the instructions in this section to resolve build errors.

First, create a folder at the solution level called `extern`.  This will contain the external dependencies.

#### GLFW
OpenGL is the graphics API used by Physis' rendering engine, and GLFW is the OpenGL library used by Physis.
1. Create a subdirectory inside `extern` called `OpenGL`.
2. Go to https://www.glfw.org/ and navigate to Downloads.  Then, click the button to download the 64-bit Windows binaries.  Note: as of this writing, I am using GLFW 3.4.
3. Unzip the download (it should be called `glfw-3.4.bin.WIN64`).
4. Copy the `include` folder into your `OpenGL` folder.
5. Copy the `lib-vc2022` folder into your `OpenGL` folder and delete the dynamic libraries (`glfw3dll.lib` and `glfw3.dll`).

#### GLEW
You need GLEW to link the OpenGL API calls in Physis to the actual implementations that reside on your machine’s GPU drivers.
1. Go to https://glew.sourceforge.net/ and download the binaries.  As of this writing, `glew-2.1.0-win32` was used.
2. Extract the files, rename the folder to `GLEW`, and paste it inside your `OpenGL` folder.

#### Catch2
Catch2 is the C++ unit testing framework used by Physis.  Note that this section is technically optional - if you'd like to skip it, be sure to remove `Physis.Test` from your build process.
1. Create a subdirectory inside “extern” called “catch2”.
2. This project currently uses the previous (as of this writing) release of catch2, catch2 2.x, in which the entire dependency is contained in a single header file.  Go to https://github.com/catchorg/Catch2/blob/v2.x/docs/tutorial.md#top, download the catch.hpp header file (there should be a link in the tutorial), and place it inside your “catch2” subdirectory.

If you’d like to download directly from Powershell, navigate into your “catch2” subdirectory, and simply type `Invoke-WebRequest https://raw.githubusercontent.com/catchorg/Catch2/v2.x/single_include/catch2/catch.hpp -OutFile catch.hpp`

#### pybind11
Pybind11 provides Python bindings to C++ code, enabling Python clients (such as `PhysisPlotter`) to configure and run the Physis engine.
1. Create a subdirectory inside “extern” called “pybind11”.
2. Go to https://pybind11.readthedocs.io/en/latest/installing.html and follow the instructions for including pybind11 as a submodule.  If that doesn’t work (and it didn’t work for me), try the next step.
3. Go to https://github.com/pybind/pybind11 and clone the pybind11 source code into “extern”.  You should now have a “pybind11” folder inside “extern”.

## Running the Application
Once you've installed all the dependencies, load the solution in Visual Studio (or your IDE of choice), set `PhysisWindowApp` as your startup project and run!  You should see balls bouncing around a 2D box.  You can edit the main method if you’d like to experiment with different numbers of balls, different kinematic parameters, etc.

Note that there is ongoing work to optimize the physics and rendering.

https://github.com/user-attachments/assets/3aad8aab-1c2b-4b60-a75f-15b9de457f74

## PhysisPlotter
PhysisPlotter is a Python application that runs the Physis engine and collects various benchmarking or scientific data from the simulation.  It is currently under construction and may not work out of the box.

Below is a sample plot of a projectile's trajectory, as rendered by Physis under different rendering conditions, compared with the analytic solution.  The bottom plot shows the error in the actual position of the particle compared with the expected position from the analytic solution.

![no interpolation semiimpliciteuler](https://github.com/user-attachments/assets/c0515b9d-bed0-420f-9837-e3c66d1c2ff6)
