#include "animation_player.h"

Animation_Player::Animation_Player(Skeleton *a, Animation *b, Skin *c, const float t)
{
	skel = a; 
	anim = b; 
	skin = c; 
	delta = t;
	time = anim->tstart;
	dofpointers = new float*[skin->getnjoints() * 3];
	for (int i = 0, ii = 0; i < skin->getnjoints(); i++, ii += 3)
	{
		dofpointers[ii] = skel->getdofpointers(i, 0);
		dofpointers[ii + 1] = skel->getdofpointers(i, 1);
		dofpointers[ii + 2] = skel->getdofpointers(i, 2);
	}
}

void Animation_Player::update()
{
	if (time <= anim->tend && _pause == 0)
	{
		time += delta;
		anim->evaluate(time);
		skel->settranslations(0, anim->poses[0], anim->poses[1], anim->poses[2]);
		for (int i = 0; i < (anim->numchannels-3); i++)
		{
			*dofpointers[i] = anim->poses[i+3];
		}
	}
	else if (_pause == 0)
	{
		time = anim->tstart;
	}

	skel->update();
	skin->update(skel);
}

void Animation_Player::draw()
{
	if (anim->channels[gluijoint].nkeyframes > 1)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		//glTranslatef(-3, -3, 0);
		glEnable(GL_POINT_SMOOTH);
		glDisable(GL_LIGHTING);
		glPointSize(3.0);
		glBegin(GL_LINE_STRIP);
		glColor3f(1, 1, 0);
		for (int i = 0; i < anim->channels[gluijoint].nkeyframes - 1; i++)
		{
			for (float ii = anim->channels[gluijoint].keyframes[i].t; ii < anim->channels[gluijoint].keyframes[i + 1].t; ii += .01)
			{
				float u = (ii - anim->channels[gluijoint].keyframes[i].t) / (anim->channels[gluijoint].keyframes[i + 1].t - anim->channels[gluijoint].keyframes[i].t);
				float value = (anim->channels[gluijoint].keyframes[i].d + u*(anim->channels[gluijoint].keyframes[i].c + u*(anim->channels[gluijoint].keyframes[i].b + u*(anim->channels[gluijoint].keyframes[i].a))));
				glVertex2f(ii, value);
			}
		}

		glEnd();
		glColor3f(1, 1, 1);
		glBegin(GL_POINTS);
		for (int i = 0; i < anim->channels[gluijoint].nkeyframes; i++)
		{
			glVertex2f(anim->channels[gluijoint].keyframes[i].t, anim->channels[gluijoint].keyframes[i].v);
		}
		glEnd();

		glColor3f(1, 0, 0);
		glBegin(GL_LINES);
		for (int i = 0; i < anim->channels[gluijoint].nkeyframes; i++)
		{
			float ii = .09;
			float tout = anim->channels[gluijoint].keyframes[i].tout*(anim->channels[gluijoint].keyframes[i].t + ii) + (anim->channels[gluijoint].keyframes[i].v - anim->channels[gluijoint].keyframes[i].tout*anim->channels[gluijoint].keyframes[i].t);
			float tin = (-anim->channels[gluijoint].keyframes[i].tin)*(anim->channels[gluijoint].keyframes[i].t - ii) + (anim->channels[gluijoint].keyframes[i].v + anim->channels[gluijoint].keyframes[i].tin*anim->channels[gluijoint].keyframes[i].t);
			glVertex2f(anim->channels[gluijoint].keyframes[i].t - ii, tin);
			glVertex2f(anim->channels[gluijoint].keyframes[i].t, anim->channels[gluijoint].keyframes[i].v);
			glVertex2f(anim->channels[gluijoint].keyframes[i].t, anim->channels[gluijoint].keyframes[i].v);
			glVertex2f(anim->channels[gluijoint].keyframes[i].t + ii, tout);
		}

		glEnd();
		glEnable(GL_LIGHTING);
		glDisable(GL_POINT_SMOOTH);
		glPopMatrix();
	}
	skel->draw();
	skin->draw();
}


void Animation_Player::drawchannelspans()
{
	if (anim->channels[gluijoint].nkeyframes > 1)
	{
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		//glTranslatef(-3, -3, 0);
		glEnable(GL_POINT_SMOOTH);
		glDisable(GL_LIGHTING);
		glPointSize(3.0);
		glBegin(GL_LINE_STRIP);
		glColor3f(1, 1, 0);
		for (int i = 0; i < anim->channels[gluijoint].nkeyframes - 1; i++)
		{
			for (float ii = anim->channels[gluijoint].keyframes[i].t; ii < anim->channels[gluijoint].keyframes[i + 1].t; ii += .01)
			{
				float u = (ii - anim->channels[gluijoint].keyframes[i].t) / (anim->channels[gluijoint].keyframes[i + 1].t - anim->channels[gluijoint].keyframes[i].t);
				float value = (anim->channels[gluijoint].keyframes[i].d + u*(anim->channels[gluijoint].keyframes[i].c + u*(anim->channels[gluijoint].keyframes[i].b + u*(anim->channels[gluijoint].keyframes[i].a))));
				glVertex2f(ii, value);
			}
		}

		glEnd();
		glColor3f(1, 1, 1);
		glBegin(GL_POINTS);
		for (int i = 0; i < anim->channels[gluijoint].nkeyframes; i++)
		{
			glVertex2f(anim->channels[gluijoint].keyframes[i].t, anim->channels[gluijoint].keyframes[i].v);
		}
		glEnd();

		glColor3f(1, 0, 0);
		glBegin(GL_LINES);
		for (int i = 0; i < anim->channels[gluijoint].nkeyframes; i++)
		{
			float ii = .09;
			float tout = anim->channels[gluijoint].keyframes[i].tout*(anim->channels[gluijoint].keyframes[i].t + ii) + (anim->channels[gluijoint].keyframes[i].v - anim->channels[gluijoint].keyframes[i].tout*anim->channels[gluijoint].keyframes[i].t);
			float tin = (-anim->channels[gluijoint].keyframes[i].tin)*(anim->channels[gluijoint].keyframes[i].t - ii) + (anim->channels[gluijoint].keyframes[i].v + anim->channels[gluijoint].keyframes[i].tin*anim->channels[gluijoint].keyframes[i].t);
			glVertex2f(anim->channels[gluijoint].keyframes[i].t - ii, tin);
			glVertex2f(anim->channels[gluijoint].keyframes[i].t, anim->channels[gluijoint].keyframes[i].v);
			glVertex2f(anim->channels[gluijoint].keyframes[i].t, anim->channels[gluijoint].keyframes[i].v);
			glVertex2f(anim->channels[gluijoint].keyframes[i].t + ii, tout);
		}

		glEnd();
		glEnable(GL_LIGHTING);
		glDisable(GL_POINT_SMOOTH);
		glPopMatrix();
	}
}

void Animation_Player::pause()
{
	_pause = ++_pause % 2;
}

