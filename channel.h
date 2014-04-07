#ifndef CSE169_CHANNEL_H
#define CSE169_CHANNEL_H

#include "keyframe.h"

class Channel
{
public:
	void load(Tokenizer*);
	void precompute();
	float evaluate(float);
public:
	int nkeyframes;
	char exin[256], exout[256];
	Keyframe *keyframes;
	float channel_x_delta, channel_y_delta;
};

#endif