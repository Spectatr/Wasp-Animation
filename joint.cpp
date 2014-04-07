#include "joint.h"

void Joint::load(Tokenizer &token) {
	char temp[256];
	token.GetToken(temp);
	strcpy(nameofjoint, temp);
	token.FindToken("{");
	while (1) {
		token.GetToken(temp);
		if (strcmp(temp, "offset") == 0) 
		{
			offset.x = token.GetFloat();
			offset.y = token.GetFloat();
			offset.z = token.GetFloat();
		}
		else if (strcmp(temp, "boxmin") == 0)
		{
			boxmin.x = token.GetFloat();
			boxmin.y = token.GetFloat();
			boxmin.z = token.GetFloat();
			
		}
		else if (strcmp(temp, "boxmax") == 0)
		{
			boxmax.x = token.GetFloat();
			boxmax.y = token.GetFloat();
			boxmax.z = token.GetFloat();
		}
		else if (strcmp(temp, "rotxlimit ") == 0)
		{
			x.setmin(token.GetFloat());
			x.setmax(token.GetFloat());
		}
		else if (strcmp(temp, "rotylimit ") == 0)
		{
			y.setmin(token.GetFloat());
			y.setmax(token.GetFloat());
		}
		else if (strcmp(temp, "rotzlimit ") == 0)
		{
			z.setmin(token.GetFloat());
			z.setmax(token.GetFloat());
		}
		else if (strcmp(temp, "pose") == 0)
		{
			x.setvalue(token.GetFloat());
			y.setvalue(token.GetFloat());
			z.setvalue(token.GetFloat());
		}
		else if (strcmp(temp, "balljoint") == 0) {
			Joint *jnt = new Joint;
			jnt->load(token);
			addchild(jnt);
		}
		else if (strcmp(temp, "}") == 0) break;
		else token.SkipLine(); // Unrecognized token}
	}
}

void Joint::addchild(Joint * child)
{
	if (child != nullptr)
	{
		child->parent = this;
		children.push_back(child);
		
	}
}

void Joint::update(void)
{
	if (parent != nullptr)
	{
		Matrix34 translation;
		translation.MakeTranslate(offset);
		local.FromEulers(x.getvalue(), y.getvalue(), z.getvalue(), 0);
		local.Dot(translation,local);
		world.Dot(parent->world, local);
	}
	else
	{
		Matrix34 translation;
		translation.MakeTranslate(offset);
		world.FromEulers(x.getvalue(), y.getvalue(), z.getvalue(), 0);
		world.Dot(translation, world);
	}

	if (!children.empty())
	{
		for (unsigned int i = 0; i < children.size(); i++)
		{
			if (children[i] != NULL)
			{
				children[i]->update();
			}
		}
	}
}

void Joint::draw()
{
	glLoadMatrixf(world);
	drawWireBox(boxmin[0], boxmin[1], boxmin[2], boxmax[0], boxmax[1], boxmax[2]);
	
	if (!children.empty())
	{
		for (unsigned int i = 0; i < children.size(); i++)
		{
			if (children[i] != NULL)
			{
				children[i]->draw();
			}
		}
	}
}

void Joint::getworldmatrix(Matrix34 &answer, int a, int &count)
{
	if (count == a){
		answer = world;
	}
	count++;
	if (!children.empty())
	{
		for (unsigned int i = 0; i < children.size(); i++)
		{
			if (children[i] != NULL)
			{
				children[i]->getworldmatrix(answer, a, count);
			}
		}
	}
}

void Joint::getdofs(int a, int &count)
{
	
	if (count == a){
		printf("DOF Name: %s\nDOF Values: [%f, %f, %f]\n\n", nameofjoint, x.getvalue(), y.getvalue(), z.getvalue());
	}
	count++;
	if (!children.empty())
	{
		for (unsigned int i = 0; i < children.size(); i++)
		{
			if (children[i] != NULL)
			{
				children[i]->getdofs(a, count);
			}
		}
	}
}

void Joint::getxdof(int a, int &count, float *&rvalue)
{

	if (count == a){
		rvalue = x.getpointer();
	}
	count++;
	if (!children.empty())
	{
		for (unsigned int i = 0; i < children.size(); i++)
		{
			if (children[i] != NULL)
			{
				children[i]->getxdof(a, count, rvalue);
			}
		}
	}
}

void Joint::getydof(int a, int &count, float *&rvalue)
{

	if (count == a){
		rvalue = y.getpointer();
	}
	count++;
	if (!children.empty())
	{
		for (unsigned int i = 0; i < children.size(); i++)
		{
			if (children[i] != NULL)
			{
				children[i]->getydof(a, count, rvalue);
			}
		}
	}
}

void Joint::getzdof(int a, int &count, float *&rvalue)
{

	if (count == a){
		rvalue = z.getpointer();
	}
	count++;
	if (!children.empty())
	{
		for (unsigned int i = 0; i < children.size(); i++)
		{
			if (children[i] != NULL)
			{
				children[i]->getzdof(a, count, rvalue);
			}
		}
	}
}

void Joint::settranslation(int a, int &count, float x, float y, float z)
{

	if (count == a){
		offset.Set(x, y, z);
	}
	count++;
	if (!children.empty())
	{
		for (unsigned int i = 0; i < children.size(); i++)
		{
			if (children[i] != NULL)
			{
				children[i]->settranslation(a, count, x, y, z);
			}
		}
	}
}

void Joint::getnameofjoint(int a, int &count, char *name)
{

	if (count == a){
		strcpy(name, nameofjoint);
	}
	count++;
	if (!children.empty())
	{
		for (unsigned int i = 0; i < children.size(); i++)
		{
			if (children[i] != NULL)
			{
				children[i]->getnameofjoint(a, count, name);
			}
		}
	}
}