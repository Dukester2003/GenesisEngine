#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include "../Scene/Scene.h"
#include "../Scene/GUI.h"
#include "../game.h"

enum EngineState
{
	ENGINE_SCENE,
	ENGINE_INGAME,
	ENGINE_PAUSE
};
class Engine
{
public:
	EngineState state;
	Scene scene;
	Game game;
	GUI gui;

	void Initiate();
	void Start();
	void Update(float dt);
};

#endif // !ENGINE_H

