#ifndef LVL_H
#define LVL_H

#include "g_model.h"
#include "game_obj.h"
class Level
{
	public:
		Level(Model lvlModel);
		Level();
		Model model;
		Material material;
		void DrawLevel(Model model, Shader shader);
};

#endif // !LVL_H
