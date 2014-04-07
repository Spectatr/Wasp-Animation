////////////////////////////////////////
// tester.cpp
////////////////////////////////////////

#include "tester.h"

#define WINDOWTITLE	"Animation"

////////////////////////////////////////////////////////////////////////////////

static Tester *TESTER;
Skeleton *skel=new Skeleton;
Skin *skin=new Skin;
Animation *anim=new Animation;
Animation_Player *anim_play;
GLUI_StaticText *Timer;

////////////////////////////////////////////////////////////////////////////////

// These are really HACKS to make glut call member functions instead of static functions
static void display()									{ TESTER->Draw(); }
static void display2()									{ TESTER->Draw2(); }
static void idle()										{ TESTER->Update(); }
static void resize(int x, int y)							{ TESTER->Resize(x, y); }
static void keyboard(unsigned char key, int x, int y)		{ TESTER->Keyboard(key, x, y); }
static void speckeyboard(int key, int x, int y)		{ TESTER->Speckeyboard(key, x, y); }
static void mousebutton(int btn, int state, int x, int y)	{ TESTER->MouseButton(btn, state, x, y); }
static void mousemotion(int x, int y)					{ TESTER->MouseMotion(x, y); }

////////////////////////////////////////////////////////////////////////////////

void Tester::Draw2() 
{
	


	glViewport(0, 0, 640, 480);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Draw components
	Cam.Draw();		// Sets up projection & viewing matrices
	//Cube.Draw();


	glPushMatrix(); // push current state of MODELVIEW matrix to stack
	glLoadIdentity(); // reset it again. (may not be required, but it my convention)


	//skel->draw();
	//skin->draw();
	anim_play->drawchannelspans();
	// Finish drawing scene 
	glFinish();
	glutSwapBuffers();

}

void pause_callback()
{
	anim_play->pause();
}

int main(int argc, char **argv) {
	float position[] = { -2.0, 10.0, 1.0, 0.0 };
	float position2[] = { -2.0, 10.0, 5.0, 0.0 };
	skel->load("wasp.skel");
	skin->load("wasp.skin");
	anim->load("wasp_walk.anim");
	anim->precompute();
	anim_play = new Animation_Player(skel, anim, skin);

	glutInit(&argc, argv);
	TESTER = new Tester(argc,argv);

	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_POSITION, position2);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);
	GLfloat DiffuseLight[] = { 1, 0, 0 }; 
	GLfloat AmbientLight[] = { 0, 1, 0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight);
	glLightfv(GL_LIGHT1, GL_AMBIENT, AmbientLight);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST);
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	//GLUI
	TESTER->glui = GLUI_Master.create_glui("Control Panel",0,665,0);
	GLUI_Rollout *joint_panel = new GLUI_Rollout(TESTER->glui, "Joints", false);
	GLUI_Rollout **jointlist = new GLUI_Rollout*[skin->getnjoints()];
	GLUI_Spinner **spinner_dofs = new GLUI_Spinner*[skin->getnjoints() * 3];
	char namej[256];
	float **dofvalues = new float*[skin->getnjoints() * 3];
	for (int i = 0, ii = 0; i < skin->getnjoints(); i++, ii += 3)
	{
		skel->getnameofjoint(i, namej);
		dofvalues[ii] = skel->getdofpointers(i, 0);
		dofvalues[ii + 1] = skel->getdofpointers(i, 1);
		dofvalues[ii + 2] = skel->getdofpointers(i, 2);


		jointlist[i] = new GLUI_Rollout(joint_panel, namej, 0);
		spinner_dofs[i] = new GLUI_Spinner(jointlist[i], "X", dofvalues[ii]);
		spinner_dofs[i]->set_float_limits(-3.2, 3.2);
		spinner_dofs[i]->set_alignment(GLUI_ALIGN_RIGHT);
		spinner_dofs[i + 1] = new GLUI_Spinner(jointlist[i], "Y", dofvalues[ii+1]);
		spinner_dofs[i + 1]->set_float_limits(-3.2, 3.2);
		spinner_dofs[i+1]->set_alignment(GLUI_ALIGN_RIGHT);
		spinner_dofs[i + 2] = new GLUI_Spinner(jointlist[i], "Z", dofvalues[ii+2]);
		spinner_dofs[i + 2]->set_float_limits(-3.2, 3.2);
		spinner_dofs[i+2]->set_alignment(GLUI_ALIGN_RIGHT);
	}
	
	GLUI_Rollout *player_panel = new GLUI_Rollout(TESTER->glui, "Player", false);
	GLUI_Panel *time_panel = new GLUI_Panel(player_panel, "Time");
	Timer = new GLUI_StaticText(time_panel, "0");
	GLUI_Button *open_console_btn = new GLUI_Button(player_panel, "Pause", NULL, (GLUI_Update_CB)pause_callback);
	GLUI_Spinner *spinner_delta = new GLUI_Spinner(player_panel, "Delta", &anim_play->delta);
	spinner_delta->set_float_limits(0, 1);
	GLUI_Spinner *spinner_channels = new GLUI_Spinner(player_panel, "Channel", &anim_play->gluijoint);
	spinner_channels->set_int_limits(0, 72);
	GLUI_Listbox *list_channels = new GLUI_Listbox(player_panel, "Channel", &anim_play->gluijoint);
	list_channels->add_item(0, "Root X");
	list_channels->add_item(1, "Root Y");
	list_channels->add_item(2, "Root Z");
	for (int i = 3; i < (skin->getnjoints()*3 + 3); i+=3)
	{
		skel->getnameofjoint((i-3)/3, namej);
		std::string namex,namey,namez;
		skel->getnameofjoint((i - 3) / 3, namej); 
		namex = namej;
		namey = namej;
		namez = namej;
		namex.append(" X");
		namey.append(" Y");
		namez.append(" Z");
		const char *cstr1 = namex.c_str();
		const char *cstr2 = namey.c_str();
		const char *cstr3 = namez.c_str();
		list_channels->add_item(i, cstr1);
		list_channels->add_item(i+1, cstr2);
		list_channels->add_item(i+2, cstr3);
	}

	TESTER->glui->set_main_gfx_window(TESTER->WindowHandle);
	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(idle);


	glutMainLoop();
	
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

Tester::Tester(int argc,char **argv) {
	WinX=640;
	WinY=480;
	LeftDown=MiddleDown=RightDown=false;
	MouseX=MouseY=0;

	// Create the window
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( WinX, WinY );
	glutInitWindowPosition( 0, 0 );
	WindowHandle = glutCreateWindow( WINDOWTITLE );
	glutSetWindowTitle(WINDOWTITLE);
	glutSetWindow(WindowHandle);

	

	// Background color
	glClearColor( 0., 0., 0., 1. );

	// Callbacks
	glutDisplayFunc( display );
	//glutIdleFunc( idle );
	glutKeyboardFunc( keyboard );
	glutSpecialFunc(speckeyboard);
	glutMouseFunc( mousebutton );
	glutMotionFunc( mousemotion );
	glutPassiveMotionFunc( mousemotion );
	glutReshapeFunc( resize );
	Cam.SetAspect(float(WinX) / float(WinY));

	
	// Create the window
	/*

	WindowHandle2 = glutCreateWindow("CHANNEL SPANS");
	glutSetWindowTitle("CHANNEL SPANS");
	glutSetWindow(WindowHandle2);
	glClearColor(0., 0., 0., 1.);
	// Callbacks
	glutDisplayFunc(display2);
	glutReshapeFunc(resize);
	glutIdleFunc(idle);*/


	// Initialize components
	
}

////////////////////////////////////////////////////////////////////////////////

Tester::~Tester() {
	glFinish();
	glutDestroyWindow(WindowHandle);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Update() {
	// Update the components in the world
	Cam.Update();
	//Cube.Update();
	//skel->update();
	//skin->update(skel);
	anim_play->update();
	char timer[256];
	sprintf_s(timer, "%f", anim_play->gettime());
	Timer->set_text(timer);
	glui->sync_live();
	// Tell glut to re-display the scene
	glutSetWindow(WindowHandle);
	glutPostRedisplay();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Reset() {
	Cam.Reset();
	Cam.SetAspect(float(WinX)/float(WinY));

	//Cube.Reset();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Draw() {
	// Begin drawing scene
	glViewport(0, 0, WinX, WinY);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	// Draw components
	Cam.Draw();		// Sets up projection & viewing matrices
	//Cube.Draw();


	glPushMatrix(); // push current state of MODELVIEW matrix to stack
	glLoadIdentity(); // reset it again. (may not be required, but it my convention)


	//skel->draw();
	//skin->draw();
	anim_play->draw();
	// Finish drawing scene 
	glFinish();
	glutSwapBuffers();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Quit() {
	glFinish();
	glutDestroyWindow(WindowHandle);
	exit(0);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Resize(int x,int y) {
	WinX = x;
	WinY = y;
	Cam.SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Keyboard(int key, int x, int y) {

}

////////////////////////////////////////////////////////////////////////////////

void Tester::Speckeyboard(int key, int x, int y) {

}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseButton(int btn,int state,int x,int y) {
	if(btn==GLUT_LEFT_BUTTON) {
		LeftDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_MIDDLE_BUTTON) {
		MiddleDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_RIGHT_BUTTON) {
		RightDown = (state==GLUT_DOWN);
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseMotion(int nx,int ny) {
	int dx = nx - MouseX;
	int dy = -(ny - MouseY);

	MouseX = nx;
	MouseY = ny;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if(LeftDown) {
		const float rate=1.0f;
		Cam.SetAzimuth(Cam.GetAzimuth()+dx*rate);
		Cam.SetIncline(Cam.GetIncline()-dy*rate);
	}
	if(RightDown) {
		const float rate=0.01f;
		Cam.SetDistance(Cam.GetDistance()*(1.0f-dx*rate));
	}
}

////////////////////////////////////////////////////////////////////////////////
