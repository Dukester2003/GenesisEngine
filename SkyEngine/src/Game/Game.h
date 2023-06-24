#ifndef GAME_H
#define GAME_H


#include "../Scene/Level.h"
enum GameState
{
	MENU,
	ACTIVE,
	PAUSED,
};

class Game
{
	GameState               State;
	bool                    Keys[1024];
	bool                    KeysProcessed[1024];
	unsigned int            Width, Height;
	std::vector<Level>		Levels;
	unsigned int            LevelID;

	// constructor/destructor
	Game(unsigned int width, unsigned int height);
	~Game();
	// initialize game state (load all shaders/textures/levels)
	void Init();
	// game loop
	void ProcessInput(float dt);
	void Update(float dt);

};

#endif // !GAME_H




