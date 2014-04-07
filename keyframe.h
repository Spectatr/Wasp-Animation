#ifndef CSE169_KEYFRAME_H
#define CSE169_KEYFRAME_H

#include "token.h"
#include "matrix34.h"

class Keyframe
{
public:
	void load(Tokenizer*);
public:
	friend class Channel;
	float t, v, tin, tout, a, b, c, d, rin2 = -9999, rout2 = -9999;
	char rin[256], rout[256];
};

#endif