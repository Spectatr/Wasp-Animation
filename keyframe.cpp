#include "keyframe.h"

void Keyframe::load(Tokenizer *token)
{
	char c;
	t = token->GetFloat();
	v = token->GetFloat();
	c = token->GetChar();
	c = token->CheckChar();
	if (!isdigit(c))
	{
		token->GetToken(rin);
		token->GetToken(rout);
	}
	else
	{
		rin2 = token->GetFloat();
		rout2 = token->GetFloat();
	}
	//printf("%f %f %s %s %f %f\n", t, v, rin, rout,rin2,rout2);
}