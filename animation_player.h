#ifndef CSE269_ANIMATION_PLAYER_H
#define CSE269_ANIMATION_PLAYER_H

#include "animation.h"
#include "skin.h"

class Animation_Player
{
public:
	Animation_Player(Skeleton *, Animation *, Skin *, const float t = 0.004);
	void update();
	void draw();
	void drawchannelspans();
	void pause();
	float delta;
	float gettime(){ return time; }
	int gluijoint = 38;
private:
	Skeleton *skel;
	Animation *anim;
	Skin *skin;
	float time;
	float **dofpointers;
	int _pause = 0;
};

#endif