#ifndef CSE169_TRIANGLE_H
#define CSE169_TRIANGLE_H

#include "token.h"

class Triangle{
public:
	void load(Tokenizer &);
	void draw(float*, float*);
	friend class Skin;
private:
	float* vertices;
	float* normals;
	int* indices;
	int nverts;
	int nnorm;
	int nind;
};

#endif