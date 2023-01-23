# Fall Gals

#### Final project in Game Programming (2022), Group 5

**Game idea**  
A crude and much simplified single-player version of *Fall Guys*. The gist: traverse a course of obstacles without being pushed off the course by said obstacles.

## Building
The project relies on [mortennobel/SimpleRenderEngineProject](https://github.com/mortennobel/SimpleRenderEngineProject) and its setup.

 1. Create folder in SimpleRenderEngineProject named `FallGals`, and copy the content of this repository to that folder.
 2. In `SimpleRenderEngineProject/CMakeLists.txt`, add `add_subdirectory(FallGals)` on the last line
 3. Enable *Bullet* physics engine in CMake (see `SimpleRenderEngineProject/CMakeLists.txt` on how to do that)
 4. Generate the SimpleRenderEngineProject solution (using regular settings)
 5. Build projects `FallGals.Utilities`, `FallGals.Core`, `FallGals.Game` and 
 `FallGals.GameApp`
 6. Find and execute the executable called `FallGals.exe` (in Windows, you probably find that in `SimpleRenderEngineProject/build/FallGals/src/1-GameApp/Debug/`)

__Note: The project has only been tested on Windows__


