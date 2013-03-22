#include "WindowsManager.h"

WindowsManager::WindowsManager(void)
{
	m_window1 = -1;
	m_window2 = -1;
	//m_subWindow = true
	m_subWindow = false;
	m_fullScreen = false;
	m_width = IntialSize;
	m_height = IntialSize;
	getDesktopResolution();
	m_previous = 0;
}

WindowsManager::~WindowsManager(void)
{

}

void WindowsManager::createWindows(int argc, char** argv)
{
	glutInit(&argc, argv); 
	glutInitDisplayMode(GLUT_MULTISAMPLE | GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

	if(m_subWindow)
		glutInitWindowSize(IntialSize*2,IntialSize); 
	else
		glutInitWindowSize(IntialSize,IntialSize); 

	m_window1 = glutCreateWindow("Haptic launch"); 
	glutPositionWindow(IntialPosition,IntialPosition); 

	m_dt.init1();

	glutReshapeFunc(m_reshape1); 
	glutDisplayFunc(m_display1); 
	glutKeyboardFunc(m_keyboard1);
	glutKeyboardUpFunc(m_keyboardUp1);
	glutMouseFunc(m_mouse1);
	glutMotionFunc(m_motion1);
	glutEntryFunc(m_entry1);

	/*glutInitWindowSize(IntialSize,IntialSize); 
	m_window2 = glutCreateWindow("Dual Touch 2"); 
	glutPositionWindow(IntialPosition+IntialSize+IntialMargin,IntialPosition); 

	m_dt.init2();

	glutReshapeFunc(m_reshape2); 
	glutDisplayFunc(m_display2); 
	glutKeyboardFunc(m_keyboard2);
	glutKeyboardUpFunc(m_keyboardUp2);
	glutMouseFunc(m_mouse2);
	glutMotionFunc(m_motion2);
	glutEntryFunc(m_entry2);*/

	//setFullScreen(m_fullScreen);

	//glutTimerFunc(0,m_idle2,0);
	glutIdleFunc(m_idle);
	glutMainLoop(); 
	
}


void WindowsManager::toggleFullScreen()
{
	m_fullScreen = ! m_fullScreen;
	setFullScreen(m_fullScreen);
}

void WindowsManager::setFullScreen(bool enable)
{
	m_fullScreen = enable;

	if(m_fullScreen)
	{
		if(m_subWindow)
		{
			getDesktopResolution();	
			if(m_window2Rect.bottom==-1)//one screen available
			{
				glutFullScreen();
			}
			else
			{
				//Fullscreen in subwindow mode required the same resolution on both screen 
				//and the second screen has to been on the right of the first screen
				glutPositionWindow(m_window1Rect.left-IntialMargin/2,m_window1Rect.top-30); 
				glutReshapeWindow(m_window2Rect.right-m_window1Rect.left,m_window1Rect.bottom - m_window1Rect.top);
			}	
		}
		else
		{
			getDesktopResolution();	

			if(m_window2Rect.bottom==-1)//one screen available
			{
				setCurrentWindow(0);
				glutPositionWindow(m_window1Rect.left,m_window1Rect.top); 
				glutReshapeWindow((m_window1Rect.right- m_window1Rect.left)/2-IntialMargin,m_window1Rect.bottom - m_window1Rect.top-IntialMargin);

				setCurrentWindow(1);
				glutPositionWindow((m_window1Rect.right- m_window1Rect.left)/2,m_window1Rect.top); 
				glutReshapeWindow((m_window1Rect.right - m_window1Rect.left)/2-IntialMargin,m_window1Rect.bottom - m_window1Rect.top-IntialMargin);
			}
			else //two screens
			{
				setCurrentWindow(0);
				glutPositionWindow(m_window1Rect.left,m_window1Rect.top); 
				glutReshapeWindow(m_window1Rect.right - m_window1Rect.left-IntialMargin,m_window1Rect.bottom - m_window1Rect.top-IntialMargin);

				setCurrentWindow(1);
				glutPositionWindow(m_window2Rect.left,m_window2Rect.top); 
				glutReshapeWindow(m_window2Rect.right - m_window2Rect.left-IntialMargin,m_window2Rect.bottom - m_window2Rect.top-IntialMargin);
			}
		}
	}
	else
	{
		if(m_subWindow)
		{
			setCurrentWindow(0);
			glutPositionWindow(IntialPosition,IntialPosition); 
			glutReshapeWindow(IntialSize*2,IntialSize);

		}
		else
		{
			setCurrentWindow(0);
			glutPositionWindow(IntialPosition,IntialPosition); 
			glutReshapeWindow(IntialSize,IntialSize);
			
			setCurrentWindow(1);
			glutPositionWindow(IntialPosition+IntialSize+IntialMargin,IntialPosition); 
			glutReshapeWindow(IntialSize,IntialSize);
		}
	}
}

void WindowsManager::toggleSubWindow()
{
	m_subWindow = !m_subWindow;
	setSubWindow(m_subWindow);
}

void WindowsManager::setSubWindow(bool enable)
{
	m_subWindow = enable;

	if(m_subWindow)
	{
		glutDestroyWindow(m_window2);
		//m_dt.init2();
		reshape1(m_width, m_height);

		setFullScreen(m_fullScreen);
	}
	else
	{
		glutDestroyWindow(m_window2);
		m_window2 = glutCreateWindow("Dual Touch 2"); 
		
		//m_dt.init2();
		glutReshapeFunc(m_reshape2); 
		glutDisplayFunc(m_display2); 
		glutKeyboardFunc(m_keyboard2);
		glutKeyboardUpFunc(m_keyboardUp2);
		glutMouseFunc(m_mouse2);
		glutMotionFunc(m_motion2);
		glutEntryFunc(m_entry2);

		setFullScreen(m_fullScreen);
	}
}

void WindowsManager::setCurrentWindow(int window)
{
	if(window==0)
		glutSetWindow(m_window1);
	else
		glutSetWindow(m_window2);
}

// callback function called by EnumDisplayMonitors for each enabled monitor
BOOL CALLBACK EnumDispProc(HMONITOR hMon, HDC dcMon, RECT* pRcMon, LPARAM lParam)
{
	RECT * rects = reinterpret_cast<RECT*>(lParam);

	if( rects[0].bottom==-1)
		rects[0]= *pRcMon;
	else
		rects[1]= *pRcMon;

	return TRUE;
}

// Get the horizontal and vertical screen sizes in pixel
void WindowsManager::getDesktopResolution()
{
	/*RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	width = desktop.right;
	height = desktop.bottom;*/

	RECT rects[2];
	rects[0].bottom=-1;
	//rects[1].bottom=-1;
	EnumDisplayMonitors(NULL, NULL, EnumDispProc,reinterpret_cast<LPARAM>(rects));

	m_window1Rect = rects[0];
	//m_window2Rect = rects[1];
}




void WindowsManager::display1()
{
	if(m_subWindow)
		glViewport(0,0,m_width/2,m_height); 

	glClear(GL_COLOR_BUFFER_BIT);
	m_dt.display1(); 
	
	if(m_subWindow)
	{
		glViewport(m_width/2,0,m_width/2,m_height); 
		m_dt.display2();
	}

	glutSwapBuffers();

}

void WindowsManager::display2()
{
	if(m_subWindow)
		glViewport(m_width/2,0,m_width/2,m_height); 

	glClear(GL_COLOR_BUFFER_BIT);
	m_dt.display2(); 
	glutSwapBuffers();
}


void WindowsManager::idle()
{

	time_t now = time(NULL);
    time_t dt = (now-m_previous);
	if(dt>=1)
	{
		//printf("FPS:%i\n",m_frameCounter);
		m_previous = now;
		m_frameCounter=0;
	}
	else
	{
		m_frameCounter++;
	}

	m_dt.idle(); 
	glutPostWindowRedisplay(m_window1);
	//glutPostWindowRedisplay(m_window2);
}

void WindowsManager::idle2(int value)
{
	idle();
	glutTimerFunc(0,m_idle2,value);
}
void WindowsManager::keyboard1(unsigned char key, int x, int y)
{
	keyboardAll(key);
	if(m_subWindow)
	{
		if(x>m_width/2)
		{
			m_dt.keyboard2(key, x, y); 
			m_dt.keyboardUp1(key, x, y); 
		}
		else
		{
			m_dt.keyboard1(key, x, y); 
			m_dt.keyboardUp2(key, x, y); 
		}
	}
	else
		m_dt.keyboard1(key, x, y); 
}

void WindowsManager::keyboard2(unsigned char key, int x, int y)
{
	keyboardAll(key);
	m_dt.keyboard2(key, x, y); 
}

void WindowsManager::keyboardAll(unsigned char key)
{
	switch(key)
	{
	case('F'):
	case('f'):	toggleFullScreen();
				break;
	case('G'):
	case('g'):	toggleSubWindow();
				break;
	case(' '):	m_dt.reset();
				break;
	case(27):	//glutDestroyWindow(m_window2);//Escape			
				//glutDestroyWindow(m_window1);
				//exit(0);
				break;
	}
}

void WindowsManager::keyboardUp1(unsigned char key, int x, int y)
{
	if(m_subWindow)
	{
		if(x>m_width/2)
			m_dt.keyboardUp2(key, x, y); 
		else
			m_dt.keyboardUp1(key, x, y); 
	}
	else
		m_dt.keyboardUp1(key, x, y); 
}

void WindowsManager::keyboardUp2(unsigned char key, int x, int y)
{
	m_dt.keyboardUp2(key, x, y); 
}

void WindowsManager::mouse1(int button, int state, int x, int y)
{
	if(m_subWindow)
	{
		if(x>m_width/2)
			m_dt.mouse2(button, state, x, y); 
		else
		m_dt.mouse1(button, state, x, y); 
	}
	else
		m_dt.mouse1(button, state, x, y); 

	m_lastButton = button;
}

void WindowsManager::mouse2(int button, int state, int x, int y)
{
	m_dt.mouse2(button, state, x, y); 
}

void WindowsManager::motion1(int x,int y)
{
	if(m_subWindow)
	{
		if(x>m_width/2)
		{
			if(m_lastX<m_width/2)
				m_dt.mouse2(m_lastButton, GLUT_UP , x, y); 
			
			m_dt.motion2(x,y);
		}
		else
		{
			if(m_lastX>m_width/2)
				m_dt.mouse1(m_lastButton, GLUT_UP, x, y); 
			
			m_dt.motion1(x,y);
		}
	}
	else
		m_dt.motion1(x,y);
		
	m_lastX = x;
}

void WindowsManager::motion2(int x,int y)
{
	m_dt.motion2(x,y);
}


void WindowsManager::reshape1(int width, int height)
{
	m_width=width;
	m_height = height;
	if(m_subWindow)
		m_dt.reshape(m_width/2, height);
	else
		m_dt.reshape(width, height);
}

void WindowsManager::reshape2(int width, int height)
{
	m_dt.reshape(width, height);
}




void WindowsManager::entry1(int state)
{
	if(state ==  GLUT_LEFT)
		m_dt.keyboardUp1(0,0,0);
}

void WindowsManager::entry2(int state)
{
	if(state ==  GLUT_LEFT)
		m_dt.keyboardUp2(0,0,0);
}