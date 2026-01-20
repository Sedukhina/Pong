1. The project uses **C++** and STL. Vectors, arrays, smart pointers, and even strings are used everywhere.
2. **OOP** is also everywhere. Almost everything is a class. Inheritance used for SceneObjects, GameStates, etc.
3. **STL** was also used. Both smart pointers and vectors can be found, for example, in the level class. Pairs in arrays in an AssetManager.
4. Code is formated.
5. 8 external libraries were used
6. **Architecture** is divided into two major parts: Engine and Pong (Game). Each part has its own role. The engine is more oriented towards generic games, and Pong implements game logic. 
	1. Inside of the engine, several subdivisions can be found.
		1. The asset folder contains everything related to the media object's lifecycle. Most of the media objects are managed by and stored in the Asset Manager class, so outside from the folder, it's enough to know the file name to load the asset in one line.
		2. The input folder contains a class to manage input by having an interface to add input and receive notifications on input. This class also stores all the input bindings.
		3. Render logic contains all graphics cycle workflow and uses OpenGL.
		4. The scene folder contains Level, which stores every object on the level, and SceneObject, which is the base class for every type of actor in the scene. It also contains a few types of actors.
		5. The general folder also stores a few classes.
			1. Globals. Conceptually, a static class is used to avoid Singletons and place all the globally used objects, such as AssetManager, in one file.
			2. The Engines class, which manages general workflow.
			3. Game State class, which should be overwritten in a game to store variables such as score and end game.
	2. The game part contains pong-specific actor classes that override actor, the pong game state, code for the pong level, and the entry point.
7. **Patterens.**
	1. Observer - InputManager, EndGame notifications
	2. Comand - PlatformMovement bound to input.
	3. Factory? - LoadAsset takes path and loads whateveer it is.
	4. Component - Actor has models and collision objects and knows nothing of its details. Both are used to add function to an actor object.
	5. Flyweight - AssetManager only loads a mesh to a memory once and reuses it if needed by multiple objects.
	6. Template Method - Tick (Everywhere), GameState only defines endgame and binds functions on it. But actual logic is unique for each game.
8. Code is build using CMake. Release v1.0 contsins binaries for Windows.
9. Sounds were added.