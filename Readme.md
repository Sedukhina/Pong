**Architecture** is divided into two major parts: Engine and Pong (Game). Each part has its own role. The engine is more oriented towards generic games, and Pong implements game logic. 
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
