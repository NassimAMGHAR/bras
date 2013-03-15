#pragma once

//#pragma comment (lib,"BulletMultiThreaded.lib")
#ifdef _DEBUG
#pragma comment (lib,"BulletDynamics_vs2010_debug.lib")
#pragma comment (lib,"BulletCollision_vs2010_debug.lib")
#pragma comment (lib,"LinearMath_vs2010_debug.lib")
#else
#pragma comment (lib,"BulletDynamics_vs2010.lib")
#pragma comment (lib,"BulletCollision_vs2010.lib")
#pragma comment (lib,"LinearMath_vs2010.lib")
#endif

#include "LinearMath/btAlignedObjectArray.h"

#include "Object.h"


#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionDispatch/btSphereSphereCollisionAlgorithm.h"
#include "BulletCollision/CollisionDispatch/btSphereTriangleCollisionAlgorithm.h"
#include "BulletCollision/CollisionDispatch/btSimulationIslandManager.h"
#include "LinearMath/btIDebugDraw.h"
#include <vector>
#include "../Demos/OpenGL/GLDebugDrawer.h"
#pragma comment (lib,"OpenGLSupport_vs2010.lib")
#include <time.h>

using namespace std;
const int maxProxies = 32766;
const int maxOverlap = 65535;

class Physic
{
public:
	Physic(void);
	~Physic(void);
	void init();
	void exit();
	void reset();
	void run();
	void tick();

	void render();

	void resetRigidBody(btRigidBody* body);
	void delthrown(vector<btRigidBody*> thrown,vector<Object*> thrown_object);
	void deleteRigidBody(btRigidBody* body);

	btRigidBody* addRigidBody(float mass, btTransform * startTransform,btCollisionShape* shape);
	btAlignedObjectArray<btCollisionShape*>	m_collisionShapes;
	


	btBroadphaseInterface*	m_broadphase;
	btCollisionDispatcher*	m_dispatcher;



	btConstraintSolver*	m_solver;

	btCollisionAlgorithmCreateFunc*	m_boxBoxCF;

	btDefaultCollisionConfiguration* m_collisionConfiguration;

	btDynamicsWorld * m_dynamicsWorld;

	clock_t m_previous;

	GLDebugDrawer * m_debugDrawer;

};
