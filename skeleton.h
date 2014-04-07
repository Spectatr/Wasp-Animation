#ifndef CSE269_SKELETON_H
#define CSE269_SKELETON_H

#include "joint.h"
#include <vector>

class Skeleton{
public:
	Skeleton() : root(nullptr) {}
	void load(char *file="test.skel");
	void update();
	void draw();
	Matrix34 getworldmatrix(int);
	void getdofs(int);
	void getnameofjoint(int, char*);
	float* getdofpointers(int, int);
	void settranslations(int, float, float, float);
private:
	Joint* root;
};

#endif
