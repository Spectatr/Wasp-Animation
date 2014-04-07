#ifndef CSE169_SKIN_H
#define CSE169_SKIN_H

#include "triangle.h"
#include "skeleton.h"

class Skin{
public:
	void load(char*);
	void draw();
	void update(Skeleton*);
	int getnjoints();
private:
	Triangle* triangles;
	float *skinweights, *newvertices, *newnormals;
	int njoints, nweights = 0;
	Matrix34* bindingmatrices_inverse;
	int nbind;
};


#endif