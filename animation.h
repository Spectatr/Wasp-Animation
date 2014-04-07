#ifndef CSE169_ANIMATION_H
#define CSE169_ANIMATION_H

#include "channel.h"

class Animation
{
public:
	void load(char*);
	void precompute();
	void evaluate(float);
private:
	friend class Animation_Player;
	float tstart, tend;
	Channel *channels;
	int numchannels;
	float *poses;
};

#endif