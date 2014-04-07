#include "animation.h"

void Animation::load(char *filename)
{
	Tokenizer *token=new Tokenizer;
	char temp[256];

	token->Open(filename);
	token->FindToken("range");
	tstart = token->GetFloat();
	tend = token->GetFloat();
	token->FindToken("numchannels");
	numchannels = token->GetInt();
	poses = new float[numchannels];
	channels = new Channel[numchannels];
	for (int i = 0; i < numchannels; i++)
	{
		token->FindToken("extrapolate");
		channels[i].load(token);
	}

	token->Close();
}

void Animation::precompute()
{
	for (int i = 0; i < numchannels; i++)
	{
		//printf("\nCHANNEL %d:\n", i);
		channels[i].precompute();
	}
}

void Animation::evaluate(float time)
{
	for (int i = 0; i < numchannels; i++)
	{
		//printf("\nCHANNEL %d:\n", i);
		poses[i] = channels[i].evaluate(time);
		//printf("channel %d, output: %f\n",i,poses[i]);
	}
}