#ifndef LVL_H
#define LVL_H

#include "g_model.h"
class Level
{
	public:
		Level(Model lvlModel);
		Level();
		Model model;
		void DrawLevel(Model model, Shader shader);
};

#endif // !LVL_H
