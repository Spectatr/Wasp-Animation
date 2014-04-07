#include "skin.h"

void Skin::load(char *filename)
{
	Tokenizer token;
	char temp[256];

	token.Open(filename);
	token.FindToken("positions");
	triangles = new Triangle;
	triangles->load(token);
	token.Close();

	token.Open(filename);
	token.FindToken("skinweights");
	njoints = token.GetInt();
	token.FindToken("{");
	
	while (1)
	{
		token.GetToken(temp);
		if (!strcmp(temp, "}")) break;
		else nweights++;
	}
	skinweights = new float[nweights];
	token.Close();

	token.Open(filename);
	token.FindToken("skinweights");
	njoints = token.GetInt();
	token.FindToken("{");
	for (int i = 0; i < nweights; i++)
	{
		skinweights[i] = token.GetFloat();
	}
	
	token.FindToken("bindings");
	nbind = token.GetInt();
	bindingmatrices_inverse = new Matrix34[nbind];
	float ax, ay, az, bx, by, bz, cx, cy, cz, dx, dy, dz;
	for (int i = 0; i < nbind; i++)
	{
		token.FindToken("matrix");
		token.FindToken("{");		
		ax = token.GetFloat();
		ay = token.GetFloat();
		az = token.GetFloat();
		bx = token.GetFloat();
		by = token.GetFloat();
		bz = token.GetFloat();
		cx = token.GetFloat();
		cy = token.GetFloat();
		cz = token.GetFloat();
		dx = token.GetFloat();
		dy = token.GetFloat();
		dz = token.GetFloat();
		bindingmatrices_inverse[i] = Matrix34(ax, bx, cx, dx, ay, by, cy, dy, az, bz, cz, dz);
		bindingmatrices_inverse[i].Inverse();
	}
	
	newvertices = new float[triangles->nverts * 3];
	newnormals = new float[triangles->nnorm * 3];
	token.Close();
}

void Skin::update(Skeleton *skel)
{
	Matrix34* M = new Matrix34[nbind];
	Tokenizer token;
	int row = 0;

	for (int i = 0; i < nbind; i++)
	{
		M[i].Dot(skel->getworldmatrix(i), bindingmatrices_inverse[i]);
	}
	for (int i = 0; i < nweights; i += skinweights[i] * 2 + 1)
	{
		Vector3 newV, newN;
		for (int j = 0; j < skinweights[i]; j++)
		{
			Vector3 V(triangles->vertices[row], triangles->vertices[row + 1], triangles->vertices[row + 2]),
				N(triangles->normals[row], triangles->normals[row + 1], triangles->normals[row + 2]), 
				tempV, tempN;
			
			M[(int)skinweights[i + 1 + (j * 2)]].Transform(V, tempV);
			newV += (tempV*skinweights[i + 2 + (j * 2)]);
			M[(int)skinweights[i + 1 + (j * 2)]].Transform3x3(N, tempN);
			newN += tempN*skinweights[i + 2 + (j * 2)];

		}
		newN = newN.Normalize();
		newvertices[row] = newV[0];
		newvertices[row+1] = newV[1];
		newvertices[row + 2] = newV[2];
		newnormals[row] = newN[0];
		newnormals[row + 1] = newN[1];
		newnormals[row + 2] = newN[2];

		row += 3;
	}
	delete[] M;
}

void Skin::draw()
{
	triangles->draw(newvertices, newnormals);
}

int Skin::getnjoints()
{
	return nbind;
}
	