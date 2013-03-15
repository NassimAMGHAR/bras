#include "Physic.h"
#include "BunnyMesh.h"



Physic::Physic(void)
{

}

Physic::~Physic(void)
{
	exit();
}


void Physic::init()
{

	m_dispatcher=0;
	btDefaultCollisionConstructionInfo cci;
	cci.m_defaultMaxPersistentManifoldPoolSize = 10;// 32768;
	m_collisionConfiguration = new btDefaultCollisionConfiguration(cci);
	
	
	m_dispatcher = new	btCollisionDispatcher(m_collisionConfiguration);

	btVector3 worldAabbMin(-1000,-1000,-1000);
	btVector3 worldAabbMax(1000,1000,1000);

	m_broadphase = new btAxisSweep3(worldAabbMin,worldAabbMax,maxProxies);

	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver();
	m_solver = solver;

	btDiscreteDynamicsWorld* world = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);
	
	m_dynamicsWorld = world;

	m_debugDrawer = new GLDebugDrawer();
	m_debugDrawer->setDebugMode(0);
	m_dynamicsWorld->setDebugDrawer( m_debugDrawer );

	world->getSimulationIslandManager()->setSplitIslands(false);
	world->getSolverInfo().m_numIterations = 16;
	world->getSolverInfo().m_solverMode = SOLVER_SIMD+SOLVER_USE_WARMSTARTING;//+SOLVER_RANDMIZE_ORDER;

	m_dynamicsWorld->getDispatchInfo().m_enableSPU = true;
	m_dynamicsWorld->setGravity(btVector3(0, 0,-2));
	
	m_previous = 0;
	

//	clientResetScene();
}

void Physic::delthrown(vector<btRigidBody*> thrown,vector<Object*> thrown_object){
	// remove thrown rigid bodies
	for (vector<btRigidBody*>::iterator i = thrown.begin(); i != thrown.end(); i++)
    {
		btRigidBody* body = *i;
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		m_dynamicsWorld->removeRigidBody(body);
		delete body;
	}	

	for (vector<Object*>::iterator i =thrown_object.begin(); i != thrown_object.end(); i++)
    {		
		btCollisionShape* shape = (*i)->getshape();		
		delete shape;
    }
}

void Physic::deleteRigidBody(btRigidBody* body){
	// remove thrown rigid bodies
	
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		m_dynamicsWorld->removeRigidBody(body);
		delete body;
}

void Physic::exit()
{
	//cleanup in the reverse order of creation/initialization

	//remove the rigidbodies from the dynamics world and delete them
	int i;
	for (i=m_dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
	{
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		m_dynamicsWorld->removeCollisionObject( obj );
		delete obj;
	}

	//delete collision shapes
	for (int j=0;j<m_collisionShapes.size();j++)
	{
		btCollisionShape* shape = m_collisionShapes[j];
		m_collisionShapes[j] = NULL;
		if(shape!=NULL)
			delete shape;
	}

	//delete dynamics world
	delete m_dynamicsWorld;

	//delete solver
	delete m_solver;

	//delete broadphase
	delete m_broadphase;

	//delete dispatcher
	delete m_dispatcher;

	delete m_collisionConfiguration;

}

btRigidBody* Physic::addRigidBody(float mass, btTransform * startTransform, btCollisionShape * shape)
{
	btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);

	btVector3 localInertia(0,0,0);
	if (isDynamic)
		shape->calculateLocalInertia(mass,localInertia);

	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects

#define USE_MOTIONSTATE 1
#ifdef USE_MOTIONSTATE
	btDefaultMotionState* myMotionState = new btDefaultMotionState(*startTransform);

	btRigidBody::btRigidBodyConstructionInfo cInfo(mass,myMotionState,shape,localInertia);

	btRigidBody* body = new btRigidBody(cInfo);
	body->setContactProcessingThreshold(BT_LARGE_FLOAT);
	body->setUserPointer(startTransform);

#else
	btRigidBody* body = new btRigidBody(mass,0,shape,localInertia);	
	body->setWorldTransform(startTransform);
#endif//
	m_collisionShapes.push_back(shape);
	m_dynamicsWorld->addRigidBody(body);

	return body;
}

void Physic::resetRigidBody(btRigidBody* body){
	m_dynamicsWorld->removeRigidBody(body); 
	m_dynamicsWorld->addRigidBody(body);	
}

void Physic::render()
{
	if (m_dynamicsWorld)
	{
		btIDebugDraw*	idraw =  m_dynamicsWorld->getDebugDrawer();

		//idraw->drawTriangle(btVector3(0,0,0),btVector3(0,0,10),btVector3(10,0,0),btVector3(1,0,0),1);
		m_dynamicsWorld->debugDrawWorld();	
		
	}
}

void Physic::run()
{
	clock_t now = clock();
    btScalar dt = (btScalar)((now-m_previous));
	if(m_previous == 0)
		dt = 0;
    m_previous = now;
	
	if (m_dynamicsWorld)
	{

#define FIXED_STEP 1
#ifdef FIXED_STEP
  		m_dynamicsWorld->stepSimulation(dt/CLOCKS_PER_SEC,0);//1.0f/60.0f
		//CProfileManager::dumpAll();
	
#else
		//during idle mode, just run 1 simulation step maximum
		int maxSimSubSteps = m_idle ? 1 : 1;
		if (m_idle)
			dt = 1.0/420.f;

		int numSimSteps = 0;
		numSimSteps = m_dynamicsWorld->stepSimulation(dt,maxSimSubSteps);
		
#endif //FIXED_STEP


	}
}

void Physic::tick()
{
	btScalar	m[16];
	btMatrix3x3	rot; 
	rot.setIdentity();

	const int	numObjects=m_dynamicsWorld->getNumCollisionObjects();
	for(int i=0;i<numObjects;i++)
	{
		btCollisionObject*	colObj=m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body=btRigidBody::upcast(colObj);
		if(body&&body->getMotionState())
		{
			btDefaultMotionState* myMotionState = (btDefaultMotionState*)body->getMotionState();
			myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(m);
			rot=myMotionState->m_graphicsWorldTrans.getBasis();
			myMotionState->getWorldTransform( *(btTransform *) body->getUserPointer());
		}
		else
		{
			colObj->getWorldTransform().getOpenGLMatrix(m);
			rot=colObj->getWorldTransform().getBasis();
		}
	}



	for (int k=0;k<m_dynamicsWorld->getNumConstraints();k++)
	{
		btTypedConstraint * constraint = m_dynamicsWorld->getConstraint(k);
		btRigidBody * constrainted = &constraint->getRigidBodyB();
		constraint->setUserConstraintPtr(NULL);		


		int numManifolds = m_dynamicsWorld->getDispatcher()->getNumManifolds();
		for (int i=0;i<numManifolds;i++)
		{
			btPersistentManifold* contactManifold =  m_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
			//btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
			btCollisionObject* obA = (btCollisionObject*)(contactManifold->getBody0());
			btCollisionObject* obB = (btCollisionObject*)(contactManifold->getBody1());

			if(constrainted==obA || constrainted==obB)
			{
				int numContacts = contactManifold->getNumContacts();
				for (int j=0;j<numContacts;j++)
				{
					if(constrainted==obA)
						constraint->setUserConstraintPtr(obB);
					else
						constraint->setUserConstraintPtr(obA);

					btManifoldPoint& pt = contactManifold->getContactPoint(j);
					if (pt.getDistance()<0.f)
					{
						const btVector3& ptB = pt.getPositionWorldOnB();
						const btVector3& normalOnB = pt.m_normalWorldOnB;
						const btVector3& ptA = pt.getPositionWorldOnA();
					}
				}
			}
		}
	}
}



void Physic::reset()
{
	// removePickingConstraint();

#ifdef SHOW_NUM_DEEP_PENETRATIONS
	gNumDeepPenetrationChecks = 0;
	gNumGjkChecks = 0;
#endif //SHOW_NUM_DEEP_PENETRATIONS

	m_previous = 0;
	int numObjects = 0;
	int i;

	if (m_dynamicsWorld)
	{
		int numConstraints = m_dynamicsWorld->getNumConstraints();
		for (i=0;i<numConstraints;i++)
		{
			m_dynamicsWorld->getConstraint(0)->setEnabled(true);
		}
		numObjects = m_dynamicsWorld->getNumCollisionObjects();
	
		///create a copy of the array, not a reference!
		btCollisionObjectArray copyArray = m_dynamicsWorld->getCollisionObjectArray();

		for (i=0;i<numObjects;i++)
		{
			btCollisionObject* colObj = copyArray[i];
			btRigidBody* body = btRigidBody::upcast(colObj);
			if (body)
			{
				if (body->getMotionState())
				{
					btDefaultMotionState* myMotionState = (btDefaultMotionState*)body->getMotionState();
					myMotionState->m_graphicsWorldTrans = myMotionState->m_startWorldTrans;
					body->setCenterOfMassTransform( myMotionState->m_graphicsWorldTrans );
					colObj->setInterpolationWorldTransform( myMotionState->m_startWorldTrans );
					colObj->forceActivationState(ACTIVE_TAG);
					colObj->activate();
					colObj->setDeactivationTime(0);
					//colObj->setActivationState(WANTS_DEACTIVATION);
				}
				//removed cached contact points (this is not necessary if all objects have been removed from the dynamics world)
				if (m_dynamicsWorld->getBroadphase()->getOverlappingPairCache())
					m_dynamicsWorld->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(colObj->getBroadphaseHandle(),m_dynamicsWorld->getDispatcher());

				btRigidBody* body = btRigidBody::upcast(colObj);
				if (body && !body->isStaticObject())
				{
					btRigidBody::upcast(colObj)->setLinearVelocity(btVector3(0,0,0));
					btRigidBody::upcast(colObj)->setAngularVelocity(btVector3(0,0,0));
				}
			}
		}

		///reset some internal cached data in the broadphase
		m_dynamicsWorld->getBroadphase()->resetPool(m_dynamicsWorld->getDispatcher());
		m_dynamicsWorld->getConstraintSolver()->reset();
	}
}


