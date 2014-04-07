#ifndef CSE169_DOF_H
#define CSE169_DOF_H

#include "core.h"
#include "vector3.h"
#include "matrix34.h"

class DOF{
public:
	DOF() : value(0), _min(-10000), _max(10000) {}
	DOF(float a, float b, float c) : value(a), _min(b), _max(c) {}
	void setvalue(float a) { if (a >= _max){ value = _max; } else if (a <= _min) { value = _min; } else{ value = a; } }
	void setmin(float a) { _min = a; }
	void setmax(float a) { _max = a; }
	float getvalue() { return value; }
	float getmin() { return _min; }
	float getmax() { return _max; }
	float* getpointer() { return &value; }
private:
	float value, _min, _max;
};


#endif