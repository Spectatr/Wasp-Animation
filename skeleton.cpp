#include "skeleton.h"

void Skeleton::load(char *filename)
{
	Tokenizer token;
	token.Open(filename);
	token.FindToken("balljoint");
	root = new Joint;
	root->load(token);
	token.Close();
}

void Skeleton::draw()
{
	if (root != nullptr)
		root->draw();
}

void Skeleton::update()
{
	if (root != nullptr)
		root->update();
}

Matrix34 Skeleton::getworldmatrix(int a)
{
	Matrix34 temp;
	int count = 0;
	root->getworldmatrix(temp, a, count);
	return temp;
}

void Skeleton::getdofs(int value)
{
	int counter = 0;
	root->getdofs(value, counter);
}

float* Skeleton::getdofpointers(int value, int n)
{
	int counter = 0;
	float *temp;
	switch (n)
	{
	case 0:root->getxdof(value, counter, temp); return temp; break;
	case 1:root->getydof(value, counter, temp); return temp; break;
	case 2:root->getzdof(value, counter, temp); return temp; break;
	}
	
}

void Skeleton::settranslations(int a, float x, float y, float z)
{
	int counter = 0;
	root->settranslation(a, counter, x, y, z);
}

void Skeleton::getnameofjoint(int value, char *name)
{
	int counter = 0;
	root->getnameofjoint(value, counter, name);
}