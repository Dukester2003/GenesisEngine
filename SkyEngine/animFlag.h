#ifndef ANIM_FLAG_H
#define ANIM_FLAG_H

enum AnimationID
{
	PLAYER_IDLE_ID,
	PLAYER_CROUCH_ID,
	PLAYER_CROUCH_TRANSITION_ID,
	PLAYER_WALK_ID,
	PLAYER_RUN_ID,
	PLAYER_JUMP_ID,
	PLAYER_SECOND_JUMP_ID,
	PLAYER_SUPER_JUMP_ID,
	PLAYER_BACK_JUMP_ID
};

struct AnimationFlag
{
	bool playerIdle;
	bool playerCrouchTransition;
	bool playerCrouch;
	bool playerWalk;
	bool playerRun;
	bool playerJump;
	bool playerSecondJump;
	bool playerSuperJump;
	bool playerBackJump;
};
#endif