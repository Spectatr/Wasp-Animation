#ifndef CSE169_JOINT_H
#define CSE169_JOINT_H

#include "token.h"
#include <vector>
#include "dof.h"

class Joint{
public:
	Joint() : boxmin(Vector3(0, 0, 0)), boxmax(Vector3(0, 0, 0)) {};
	void update();
	void load(Tokenizer &);
	void draw();
	void addchild(Joint *);
	void getworldmatrix(Matrix34&, int, int &);
	void getdofs(int, int&);
	void getxdof(int, int&, float*&);
	void getydof(int, int&, float*&);
	void getzdof(int, int&, float*&);
	void settranslation(int, int&, float, float, float);
	void getnameofjoint(int, int&, char*);
private:
	DOF x, y, z;
	Vector3 offset, boxmin, boxmax;
	Matrix34 local, world;
	std::vector<Joint*> children;
	Joint* parent;
	char nameofjoint[256];
protected:

};

#endif