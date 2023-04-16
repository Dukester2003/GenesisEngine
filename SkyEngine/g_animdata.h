#ifndef ANIM_DATA_H
#define ANIM_DATA_H

#include<glm/glm.hpp>
struct BoneInfo
{
	/*id is index in finalBoneMatrices*/
	int id;

	/*offset matrix transforms vertex from model space to bone space*/
	glm::mat4 offset;

};

#endif // !ANIM_DATA_H

