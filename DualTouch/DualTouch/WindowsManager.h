#pragma once

#include <wtypes.h>
#include <time.h>
#include <GL/glut.h>
#include "DualTouch.h"

const short IntialSize = 700;
const short IntialPosition = 100;
const short IntialMargin = 14;
const short Margin = 10;

class WindowsManager
{
public:
	WindowsManager(void);
	~WindowsManager(void);

	void createWindows(int argc, char** argv);

	void setCurrentWindow(int window);
	void toggleFullScreen();
	void setFullScreen(bool enable);


	void toggleSubWindow();
	void setSubWindow(bool enable);
	void getWindow1Shape(int w,int h);

	void getDesktopResolution();

	void display1();
	void display2();
	void reshape1(int width, int height);
	void reshape2(int width, int height);

	void keyboardAll(unsigned char key);
	void keyboard1(unsigned char key, int x, int y);
	void keyboard2(unsigned char key, int x, int y);

	void keyboardUp1(unsigned char key, int x, int y);
	void keyboardUp2(unsigned char key, int x, int y);

	void mouse1(int button, int state, int x, int y);
	void mouse2(int button, int state, int x, int y);
	void motion1(int x, int y);
	void motion2(int x, int y);

	void entry1(int state);
	void entry2(int state);

	void(*m_display1)(void);
	void(*m_display2)(void);

	void(*m_keyboard1)(unsigned char,int,int);
	void(*m_keyboardUp1)( unsigned char, int, int );

	void(*m_keyboard2)(unsigned char,int,int);
	void(*m_keyboardUp2)( unsigned char, int, int );

	void(*m_mouse1) (int,int,int,int);
	void(*m_motion1) (int,int);

	void(*m_mouse2) (int,int,int,int);
	void(*m_motion2) (int,int);

	void(*m_reshape1)(int,int);
	void(*m_reshape2)(int,int);

	void(*m_entry1)(int);
	void(*m_entry2)(int);

	void(*m_idle)(void);
	void idle();

	void(*m_idle2)(int value);
	void idle2(int value);

private:

	int m_window1;
	int m_window2;
	bool m_subWindow;
	bool m_fullScreen;
	int m_width;
	int m_height;
	int m_lastX;
	int m_lastButton;
	DualTouch m_dt;
	RECT m_window1Rect;
	RECT m_window2Rect;
	time_t m_previous;
	short m_frameCounter;
};
