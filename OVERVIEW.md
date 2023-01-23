# Fall Gals

#### Final project in Game Programming (2022), Group 5



**Game idea**
A crude and much simplified single-player version of *Fall Guys*. The gist: traverse a course of obstacles without being pushed off the course by said obstacles.

- Focus on tech

- 3D

- Physics-based (using *Bullet*)

- C++17


## Project structure

The project is structured in 6 sub-projects across 5 layers:

1. **GameApp** + **Tests**
   - Executables
   - *Tests* contains automatic tests (probably mostly unit tests)
   - *GameApp* is just a thin wrapper that starts the game (this allows the stuff in *Game* to be unit tested)
2. **Game**
   - Concrete scenes, game specific entities, game logic
3. **Core**
   - Think: engine systems
   - Entity system, game controller, scene system, renderer etc.
4. **Utility**
   - General utility that could technically be independent of our code
   - Utilities for math, data structures etc.
   - May be scrapped, if we don't end up needing utilities
5. **External**
   - Dlls and headers from external projects (e.g. *Catch2* that we use for *Unit testing*)

The topmost layer is layer 1, and bottommost is layer 5. Projects may only depend on any projects on a lower layer (*Core* may depend *Utility* and *External* but not on game).

Each project (except for *External*) is a *CMake* project/executable with the name `FallGals.<project-name>`. 

The file structure for projects looks like:

```
src/
	<layer-number>-<project-name>/
		<project-name>/
			<source code here>
        CMakeLists.txt
```

The extra folder allows for including headers from specific projects instead of including all headers in `src`. 

Each project has its own namespace in the format `fg::<project-name>` (e.g. `fg::core` or `fg::game`). *FG* is the project-abbreviation and should be also be used for macros (e.g. `FG_MACRO`).

#### Assets

Assets are stored in `assets` folder, and the entire folder is copied to the execution working directory (same directory as *.exe* file). Thus, paths to assets in the code should be `"assets/<asset-file-name>"`.

#### Unit testing

We use **Catch2 V2** (*not* V3) for unit testing. See https://github.com/catchorg/Catch2/tree/v2.x. 

- Unit tests are *not required*  but may be found useful. Use at your own leisure.

#### Standard

We are using **C++17**.

- This can easily be tweaked across all of our projects by changing `FG_CPP_VERSION` in the root `CMakeLists.txt`.



## Entity system

Our concept of *game objects* is *entities*, but they are similar to *game objects* in that they have

- Virtual callbacks for `onUpdate`, `onRender`, `onKeyboardEvent`, `onMouseMotionEvent`, `onMouseButtonEvent`
- List of child entities, which the parent entity is responsible for updating (done internally).

However, they differ in the following ways:

- *No integrated Transform*  
  Entities don't natively have *Transforms* (position, rotation, scale). Instead, we keep this as a separate class with it's own parent hierarchy, which you can choose add to your entity via regular class composition. 
- *No components*  
  Without the transform, *components* are essentially just entities that another entity can only have one of. Thus we simplify this by **not having components**, and just express everything via child entities. This does mean however, that an entity *can* have multiple of the same type entity as "components", and so a parent entity must keep track of named children, because you can't just "ask for the Sprite component" (through type).
- *Entity class is abstract*  
  There is no reason to just have a bare-bone entity. Thus, the Entity class is abstract. When you have an entity you likely want to express some behavior (or group of behaviors), and so you should create an *explicit* type for that grouping of behaviors (e.g. *Player*, *Obstacle*, *Sprite*) - you do that by inheriting `Entity` class. That inherited class is also the one to decide what children it has, e.g. what behaviors it needs, as there should be no need for external code to  to decide on what behavior an object should or shouldn't have.
- *Child entities do not know about their parents*  
  This is just to simplify things. Any dependencies that the child entity may have on external things, should be passed in constructor.