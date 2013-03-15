#pragma once

#include <LinearMath/btTransform.h> 
#include <GL/glut.h>


class Camera
{
public:
	Camera(void);
	~Camera(void);
	void lookAt();
	void keyboardDown(unsigned char key);
	void keyboardUp(unsigned char key);
	void mouseMotion(int x,int y);
	void mouseDown(int button,int x,int y);
	void mouseUp(int button);

	void moveTo(btVector3 & origin);

	bool m_keyDown;
	unsigned char m_key;
	btTransform m_view;

private:
	btScalar m_speed;
	btScalar m_torque;
	btVector3 m_eye;
	btVector3 m_look;
	btVector3 m_up;

	int  m_oldx,m_oldy;
};
