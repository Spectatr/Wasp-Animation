#include "triangle.h"

void Triangle::load(Tokenizer &token) {
	char temp[256];

	nverts = token.GetInt();
	vertices = new float[nverts * 3];
	token.FindToken("{");
	for (int i = 0; i < nverts * 3; i++) {
		vertices[i] = token.GetFloat(); 
	}
	token.FindToken("normals");
	nnorm = token.GetInt(); 
	normals = new float[nnorm * 3];
	token.FindToken("{");
	for (int i = 0; i < nnorm * 3; i++) {
		normals[i] = token.GetFloat();
	}
	token.FindToken("triangles");
	nind = token.GetInt();
	indices = new int[nind * 3];
	token.FindToken("{");
	for (int i = 0; i < nind * 3; i++) {
		indices[i] = token.GetInt();
	}
}

void Triangle::draw(float  *vertices, float *normals)
{
	glLoadIdentity();
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glNormalPointer(GL_FLOAT, 0, normals);

	glDrawElements(GL_TRIANGLES, nind * 3, GL_UNSIGNED_INT, indices);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

}