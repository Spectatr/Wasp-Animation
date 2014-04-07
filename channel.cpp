#include "channel.h"

void Channel::load(Tokenizer *token)
{
	token->GetToken(exin);
	token->GetToken(exout);
	token->FindToken("keys");
	nkeyframes = token->GetInt();
	keyframes = new Keyframe[nkeyframes];
	token->FindToken("{");
	for (int i = 0; i < nkeyframes; i++)
	{
		keyframes[i].load(token);
	}
}

void Channel::precompute()
{
	if (nkeyframes>1)
	{
		for (int i = 0; i < nkeyframes; i++)
		{
			if (keyframes[i].rin2 == -9999)
			{
				if (!strcmp("flat", keyframes[i].rin))
				{
					keyframes[i].tin = 0;
				}
				else if (!strcmp("linear", keyframes[i].rin))
				{
					if (i == 0)	keyframes[i].tin = 0;
					else keyframes[i].tin = (keyframes[i].v - keyframes[i - 1].v) / (keyframes[i].t - keyframes[i - 1].t);
				}
				else if (!strcmp("smooth", keyframes[i].rin))
				{
					if (i == 0) keyframes[i].tin = 0;
					else if (i == (nkeyframes - 1)) keyframes[i].tin = (keyframes[i].v - keyframes[i - 1].v) / (keyframes[i].t - keyframes[i - 1].t);
					else keyframes[i].tin = (keyframes[i + 1].v - keyframes[i - 1].v) / (keyframes[i + 1].t - keyframes[i - 1].t);
				}
			}
			else
			{
				keyframes[i].tin = keyframes[i].rin2;
			}

			if (keyframes[i].rout2 == -9999)
			{
				if (!strcmp("flat", keyframes[i].rout))
				{
					keyframes[i].tout = 0;
				}
				else if (!strcmp("linear", keyframes[i].rout))
				{
					if (i == (nkeyframes - 1)) keyframes[i].tout = 0;
					else keyframes[i].tout = (keyframes[i + 1].v - keyframes[i].v) / (keyframes[i + 1].t - keyframes[i].t);
				}
				else if (!strcmp("smooth", keyframes[i].rout))
				{
					if (i == 0) keyframes[i].tout = (keyframes[i + 1].v - keyframes[i].v) / (keyframes[i + 1].t - keyframes[i].t);
					else if (i == (nkeyframes - 1)) keyframes[i].tout = 0;
					else keyframes[i].tout = (keyframes[i + 1].v - keyframes[i - 1].v) / (keyframes[i + 1].t - keyframes[i - 1].t);
				}
			}
			else
			{
				keyframes[i].tout = keyframes[i].rout2;
			}
		}

		for (int i = 0; i < (nkeyframes - 1); i++)
		{
			float delta = keyframes[i + 1].t - keyframes[i].t;
			//printf("%i: %f,%f,%f,%f,%f\n",i, keyframes[i].v, keyframes[i + 1].v, keyframes[i].tout, keyframes[i + 1].tin,delta);
			keyframes[i].a = 2 * keyframes[i].v + -2 * keyframes[i + 1].v + delta * keyframes[i].tout + delta * keyframes[i + 1].tin;
			keyframes[i].b = -3 * keyframes[i].v + 3 * keyframes[i + 1].v + -2 * delta * keyframes[i].tout + -1 * delta * keyframes[i + 1].tin;
			keyframes[i].c = delta*keyframes[i].tout;
			keyframes[i].d = keyframes[i].v;
			//printf("A: %f, B: %f, C: %f, D: %f\n", keyframes[i].a, keyframes[i].b, keyframes[i].c, keyframes[i].d);
		}
		channel_x_delta = keyframes[nkeyframes - 1].t - keyframes[0].t;
		channel_y_delta = keyframes[nkeyframes - 1].v - keyframes[0].v;
	}
}

float Channel::evaluate(float time)
{
	if (nkeyframes == 0) return 0;
	else if (nkeyframes == 1) return keyframes[0].v;
	else
	{
		int span = 0, min = 0, max = nkeyframes - 1, offset = 0;

		/*while (max >= min)
		{
			span = (max + min) / 2;
			if (time < keyframes[span].t) max = span - 1;
			else if (keyframes[span].t < time) min = span + 1;
			else return keyframes[span].v;
		}
		if (span == (nkeyframes - 1)) span--;*/
		//printf("TIME: %f,min %f, max %f\n", time, keyframes[0].t, keyframes[max].t);
		if (time < keyframes[0].t)
		{
			if (!strcmp("constant", exin))
			{
				return keyframes[0].v;
			}
			else if (!strcmp("linear", exin))
			{
				return keyframes[0].tout*time + keyframes[0].v - keyframes[0].tout*keyframes[0].t;
			}
			else if (!strcmp("cycle",exin))
			{
				time += channel_x_delta * ((int)((keyframes[0].t - time) / channel_x_delta) + 1);
			}
			else if (!strcmp("cycle_offset", exin))
			{
				offset = ((int)((keyframes[0].t - time) / channel_x_delta) + 1);
				time += channel_x_delta * offset;
			}
			else if (!strcmp("bounce", exin))
			{
				offset = ((int)((keyframes[0].t - time) / channel_x_delta) + 1);
				if (offset % 2)
				{
					time = keyframes[0].t + keyframes[max].t - time;
				}
				else
				{
					time += channel_x_delta * offset;
				}
				offset = 0;
			}
		}
		else if (time > keyframes[max].t)
		{
			if (!strcmp("constant", exout))
			{
				return keyframes[max].v;
			}
			else if (!strcmp("linear", exout))
			{
				return (-keyframes[max].tin)*time + keyframes[max].v + keyframes[max].tin*keyframes[max].t;
			}
			else if (!strcmp("cycle", exout))
			{
				time -= channel_x_delta * ((int)((time - keyframes[max].t) / channel_x_delta) + 1);
			}
			else if (!strcmp("cycle_offset", exout))
			{
				offset = ((int)((time - keyframes[max].t) / channel_x_delta) + 1);
				time -= channel_x_delta * offset;
			}
			else if (!strcmp("bounce", exout))
			{
				if ((int)(time / keyframes[max].t) % 2)
				{
					time = keyframes[0].t + keyframes[max].t - time;
				}
				else
				{
					time = fmod(time, keyframes[max].t);
				}
			}
		}
		//printf("%f, %f\n", time, keyframes[max].t);
		while (span<=max)
		{
			if (time == keyframes[span].t)
			{
				return keyframes[span].v;
			}
			else if (time < keyframes[span+1].t) 
			{
				break;
			}
			else span++;
		}
		//printf("span:%d, max:%d, time:%f\n", span, max, time);

		float u = (time - keyframes[span].t) / (keyframes[span + 1].t - keyframes[span].t);
		//printf("U: %f, Span: %f\n", u,span);
		//printf("A: %f, B: %f, C: %f, D: %f\n", keyframes[span].a, keyframes[span].b, keyframes[span].c, keyframes[span].d);
		return (keyframes[span].d + u*(keyframes[span].c + u*(keyframes[span].b + u*(keyframes[span].a)))) + offset*(channel_y_delta);
	}
}