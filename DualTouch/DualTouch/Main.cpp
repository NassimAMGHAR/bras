//First stone of what I will feed every day and all day for the next 2.5 years, yeah for it! 
// 31/05/2011 Dual Touch here we go.

#include <GL/glut.h>
#include <memory>
#include "WindowsManager.h"

void display1();
void display2();
void reshape1(int width, int height);
void reshape2(int width, int height);

void idle();
void idle2(int value);
void keyboard1(unsigned char key, int x, int y);
void keyboardUp1(unsigned char key, int x, int y);
void mouse1(int button, int state, int x, int y);
void motion1(int x, int y);
void entry1(int state);

void keyboard2(unsigned char key, int x, int y);
void keyboardUp2(unsigned char key, int x, int y);
void mouse2(int button, int state, int x, int y);
void motion2(int x, int y);
void entry2(int state);

WindowsManager wm;

void main(int argc, char** argv)
{
	//Give function pointers to WindowsManager
	//and let WindowsManager do the callback connection with glut.
	//Weird technic but that's why I like it.

	wm.m_display1 = display1;
	//wm.m_display2 = display2;
	wm.m_idle = idle;
	//wm.m_idle2 = idle2;
	wm.m_keyboard1 = keyboard1;
	wm.m_keyboardUp1 = keyboardUp1;

	//wm.m_keyboard2 = keyboard2;
	//wm.m_keyboardUp2 = keyboardUp2;

	wm.m_reshape1 = reshape1;
	//wm.m_reshape2 = reshape2;

	wm.m_mouse1 = mouse1;
	wm.m_motion1 = motion1;

	//wm.m_mouse2 = mouse2;
	//wm.m_motion2 = motion2;

	wm.m_entry1 = entry1;
	//wm.m_entry2 = entry2;

	wm.createWindows(argc,argv);
}

void idle()
{
	wm.idle();
}
void idle2(int value)
{
	wm.idle2(value);
}

void keyboard1(unsigned char key, int x, int y)
{
	wm.keyboard1(key,x,y);
}

void keyboard2(unsigned char key, int x, int y)
{
	wm.keyboard2(key,x,y);
}

void keyboardUp1(unsigned char key, int x, int y)
{
	wm.keyboardUp1(key,x,y);
}

void keyboardUp2(unsigned char key, int x, int y)
{
	wm.keyboardUp2(key,x,y);
}

void mouse1(int button, int state, int x, int y)
{
	wm.mouse1(button,state,x,y);
}

void mouse2(int button, int state, int x, int y)
{
	wm.mouse2(button,state,x,y);
}


void reshape1(int width, int height)
{
	wm.reshape1(width,height);
}

void reshape2(int width, int height)
{
	wm.reshape2(width,height);
}

void display1()
{
	wm.display1();
}

void display2()
{
	wm.display2();
}

void motion1(int x, int y)
{
	wm.motion1(x,y);
}


void motion2(int x, int y)
{
	wm.motion2(x,y);
}

void entry1(int state)
{
	wm.entry1(state);
}

void entry2(int state)
{
	wm.entry2(state);
}