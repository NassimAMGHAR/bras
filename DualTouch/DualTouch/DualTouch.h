#pragma once

#include "Renderer.h"
#include "Camera.h"
#include "Physic.h"
#include "HapticDevice.h"
#include <iostream>

const btScalar BALL_MASS = 0.2;
const btScalar Effector_Mass = 4;
const btScalar Effector_Size = 0.08f;
const btScalar PI = 3.1415926535897932384626433832795;

using namespace std;

const btVector3 gray(0.6f,0.6f,0.6f);
const btVector3 neutral(0.9f,0.9f,0.9f);
const btVector3 red(1.0f,0.0f,0.0f);


class DualTouch
{

public:
	DualTouch(void);
	~DualTouch(void);

	void reshape(int width, int height);
	void display1();
	void display2();
	void idle();


	void keyboard1(unsigned char key, int x, int y);
	void keyboardUp1(unsigned char key, int x, int y);
	void mouse1(int button, int state, int x, int y);
	void motion1(int x, int y);

	void keyboard2(unsigned char key, int x, int y);
	void keyboardUp2(unsigned char key, int x, int y);
	void mouse2(int button, int state, int x, int y);
	void motion2(int x, int y);

	void init1();
	void createScene();
	void createCursor(unsigned int deviceId);
	void addLauncher();
	void rotateCanon(btVector3* rotate);

	btVector3 getFinalPos(btTransform* target, btScalar init_vx);
	btScalar setVelocityTarget(btScalar time,btRigidBody* target,btScalar x);
	void throwObject();	
	void deleteThrowedObjects();
	//callbacks
	static void newConstraint(void * ptr,btRigidBody * body,unsigned int id);
	static void deleteConstraint(void * ptr,btRigidBody * body,unsigned int id);
	static void tickCallback(btDynamicsWorld *world, btScalar timeStep);

	void init2();

	void reset();

	Physic m_physic;
	Renderer m_renderer;
	Camera m_camera1;
	Camera m_camera2;
	HapticDevice m_hds;

	btRigidBody * cursors[NB_DEVICES_MAX];
	btVector3 m_cursorColors[NB_DEVICES_MAX];

	vector <btRigidBody *> m_throwed_rigid_list;
	vector <Object *> m_throwed_object_list;

	btRigidBody* m_curentThrowed;
	Object* m_curentObject;
	btScalar m_velocityY;
	btScalar m_velocityZ;
	btScalar m_time;
	bool m_moveTarget;
	btScalar m_theta;
	btScalar m_lunch_z;
	btScalar m_lunch_y;
	btScalar m_timeSpeed;
	btScalar m_impactY;
	btVector3 m_impactPos;
	btRigidBody* m_canon;
};
