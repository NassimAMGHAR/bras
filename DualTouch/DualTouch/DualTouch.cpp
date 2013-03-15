#include "DualTouch.h"

DualTouch::DualTouch(void)
{
	m_cursorColors[0]=red;
	//m_cursorColors[1]=blue;
}

DualTouch::~DualTouch(void)
{
	m_physic.exit();
}

void DualTouch::init1()
{
	m_velocityY = 7.0f;
	m_velocityZ = 5.0f;
	m_moveTarget = false;
	m_theta = 45;
	m_lunch_z = 5;
	m_lunch_y = 15;
	m_renderer.init();
	m_physic.init();
	
	m_camera1.moveTo(btVector3(0.5,-10,3));
	//m_camera2.moveTo(btVector3(-0.5,-10,3));

	m_hds.addDevice("PHANToM",m_camera1.m_view);
	//m_hds.addDevice("PHANToM 2",m_camera2.m_view);
	m_hds.init();

	m_hds.m_ptr=this;
	m_hds.m_newConstraint = newConstraint;
	m_hds.m_deleteConstraint = deleteConstraint;
	//m_hds.setDThrownList(m_throwed_rigid_list);

	createScene();

	createCursor(0);
	//createCursor(1);
	//m_physic.m_dynamicsWorld->setInternalTickCallback(tickCallback,this);
}

void DualTouch::init2()
{
	//m_hd2.init(std::string("PHANToM 2"));
	//m_hd2.setCameraView(&m_camera2.m_view);
	m_renderer.init();
}

void DualTouch::createScene()
{


	//ground
	const btScalar halfSize = 0.5f;
	btTransform * t = new btTransform(btQuaternion(),btVector3(0,0,-halfSize*2)); 
	btCollisionShape * shape = new btBoxShape (btVector3(100,halfSize*2,100));
	
	btRigidBody * body = m_physic.addRigidBody(0,t,shape);
	//body->setHitFraction(0);
	//body->setRestitution(0);
	//body->setFriction(0);
	//body->setRollingFriction(0);
	//body->setCollisionFlags( body->getCollisionFlags() | 
	//btCollisionObject::CF_KINEMATIC_OBJECT); 
	//body->setActivationState(DISABLE_DEACTIVATION); 
	m_renderer.addObject(new Object(shape,t,neutral));

	

}

void DualTouch::createCursor(unsigned int deviceId)
{
	btCollisionShape * shape = new btSphereShape(0.1f);
	btTransform *t = new btTransform(btQuaternion(),btVector3(0,0,0));

	btRigidBody *rigidBody = m_physic.addRigidBody(1,t,shape);  // < -------------------------------------------------
	cursors[deviceId]=rigidBody;
	rigidBody->setActivationState(DISABLE_DEACTIVATION);

	btGeneric6DofConstraint* dof6 = new btGeneric6DofConstraint(* rigidBody, *t,false);
	dof6->setLinearLowerLimit(btVector3(0,0,0));
	dof6->setLinearUpperLimit(btVector3(0,0,0));
	dof6->setAngularLowerLimit(btVector3(0,0,0));
	dof6->setAngularUpperLimit(btVector3(0,0,0));

	for(int axe=0;axe<6;axe++)
	{
		dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8f,axe);
		dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.2f,axe);
	}

	dof6->setUserConstraintPtr(NULL);

	m_hds.setConstraint(deviceId,dof6);

	m_physic.m_dynamicsWorld->addConstraint(dof6);  // << ------------------------------------------------------------
	m_renderer.addObject(new Object(shape,t,neutral));

	shape = new btConeShapeZ(0.25f,1);
	m_renderer.addObject(new Object(shape,&(m_hds.m_effectors[deviceId]),m_cursorColors[deviceId]));

}

void DualTouch::generateCube(){
	    float hx = 0.3f;
		float hy = 0.3f;
		float hz = 0.3f;
		const btScalar halfSize = 0.5f;
		int r = 4;
		int f = rand() % r;
		btTransform * t = new btTransform(btQuaternion(),btVector3(f-r/2,15,m_lunch_z)); 
		//btCollisionShape * shape = new btBoxShape (btVector3(hx,hy,hz));
		btCollisionShape * shape = new btSphereShape (hx);	
		btRigidBody* body = m_physic.addRigidBody(4,t,shape);
		body->setCollisionFlags( body->getCollisionFlags() | 
		btCollisionObject::CF_KINEMATIC_OBJECT); 
		body->setActivationState(DISABLE_DEACTIVATION); 
		
		// z height
		//body->setLinearVelocity(btVector3(0,-m_velocity*2,m_velocity/4));
		m_curentThrowed = body;
		m_hds.setThrown(m_curentThrowed);
		m_curentObject = m_renderer.addObject(new Object(shape,t,blue));
		//m_throwed_object_list.push_back(m_renderer.addObject(new Object(shape,t,blue)));
		//m_throwed_rigid_list.push_back(body);
		//m_hds.setDThrownList(m_throwed_rigid_list);
		//m_hds.setDThrownObject(m_throwed_object_list);
		//deleteThrowedObjects();
		m_moveTarget = true;
		m_time = 0;
}

void DualTouch::moveTarget(btScalar time,btRigidBody* target){
	btTransform* mytrans = new btTransform;
	target->getMotionState()->getWorldTransform(*mytrans);
	btVector3 gravity = m_physic.m_dynamicsWorld->getGravity();
	btScalar y = m_velocityY * cos(m_theta) * time;
	y = m_lunch_y - y;
	btScalar z = (gravity.z()/2 * pow(time,2)) +( m_velocityZ * sin(m_theta)*time) +  m_lunch_z;
	cout<< " position  " << mytrans->getOrigin().x() << "  " <<
							mytrans->getOrigin().y() << "  " <<
							mytrans->getOrigin().z() << "  " <<
							endl;
	mytrans->getOrigin().setY(y);
	mytrans->getOrigin().setZ(z);
	target->getMotionState()->setWorldTransform(*mytrans);	
	if(y<-2)
	{
		// create a new rigidbody for bullet
		btCollisionShape * shape = new btSphereShape (0.3f);	
		btRigidBody* newTarget = m_physic.addRigidBody(4,mytrans,shape);		
		
		m_hds.setThrown(newTarget);
		Object* newObject =m_renderer.addObject(new Object(shape,mytrans,blue));
		
		//delete the previous target
		m_physic.deleteRigidBody(m_curentThrowed);
		//m_renderer.delObject(m_curentObject); 

		//m_physic.resetRigidBody(target);
		// update
		m_curentThrowed = newTarget;
		m_curentObject = newObject;
		m_moveTarget = false;
		m_time = 0;	
	}
}

void DualTouch::deleteThrowedObjects(){
	
	 // clear rigd bodies	
	m_physic.delthrown(m_throwed_rigid_list, m_throwed_object_list);
	
	 
    // clear graphics
	for (vector<Object*>::iterator i = m_throwed_object_list.begin(); i != m_throwed_object_list.end(); i++)
    {		
		m_renderer.delObject(*i);
    }
	
	  m_throwed_rigid_list.clear();
	  m_throwed_object_list.clear();	

	  cout<<"objects " << m_throwed_object_list.size()<<endl;
	  cout<<"rigid " << m_throwed_rigid_list.size()<<endl;
}

void DualTouch::reset()
{
	m_physic.reset();
	for(int i=0;i<NB_DEVICES_MAX;i++)
	{
		cursors[i]->setActivationState(DISABLE_DEACTIVATION);
		
		cursors[i]->setWorldTransform(m_hds.m_effectors[i]);
	}
	
}

void DualTouch::reshape(int width, int height)
{
	glViewport(0,0,width,height); 
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();  
	gluPerspective(45,(width/(float)height),0.01,1000); 
	glMatrixMode(GL_MODELVIEW); 
}

void DualTouch::display1()
{
	m_renderer.setClearColor(red[0],red[1],red[2]);
	m_camera1.lookAt();
	m_renderer.display();
	glDisable(GL_LIGHTING);
	m_physic.render();
}

void DualTouch::display2()
{
	m_renderer.setClearColor(blue[0],blue[1],blue[2]);
	m_camera2.lookAt();
	m_renderer.display();	
	glDisable(GL_LIGHTING);
	m_physic.render();
}

void DualTouch::idle()
{	
	m_hds.run();	
	m_physic.run();
	m_physic.tick();
	m_hds.feedback(*m_physic.m_dynamicsWorld);
	
	if(m_moveTarget)
	{
		m_time+=0.03;
		moveTarget(m_time,m_curentThrowed); 		
	}
}

void DualTouch::tickCallback(btDynamicsWorld *world, btScalar timeStep)
{
	DualTouch * dt = (DualTouch *) world->getWorldUserInfo();
	dt->m_hds.run();
	dt->m_physic.tick();
	dt->m_hds.feedback(*dt->m_physic.m_dynamicsWorld);
}

void DualTouch::newConstraint(void *ptr ,btRigidBody * body,unsigned int id)
{
	DualTouch * my = (DualTouch *)ptr;
	if(body!=NULL)
	{
		Object * object =my->m_renderer.getObject(body->getCollisionShape());
		if(object->m_color==neutral)
		{
			object->m_color = my->m_cursorColors[id];
		}
		else
		{
			object->m_texture =true;
			object->m_color = neutral;
		}
	}
}

void DualTouch::deleteConstraint(void * ptr,btRigidBody * body,unsigned int id)
{
	DualTouch * my = (DualTouch *)ptr;
	if(body!=NULL)
	{
		Object * object =my->m_renderer.getObject(body->getCollisionShape());
		if(object->m_texture)
		{
			object->m_texture =false;
			if(id==0)
				object->m_color = my->m_cursorColors[1];
			else
				object->m_color = my->m_cursorColors[0];
		}
		else
		{
			object->m_color = neutral;
		}
	}
}

void DualTouch::keyboard1(unsigned char key, int x, int y)
{
	//m_camera1.m_key = key;
	switch(key){
		case('t'):generateCube();
				break;
		case('r'):deleteThrowedObjects();
				break;
		case('+'):
		case('p'): m_velocityY += 0.2f;
				   cout<<" velocity Y "<<m_velocityY<<endl;
				break;
		case('-'):
		case('o'): m_velocityY -= 0.2f;
				   cout<<" velocity Y "<<m_velocityY<<endl;
				break;
		default:
			m_camera1.keyboardDown(key);
	};

	
}

void DualTouch::keyboard2(unsigned char key, int x, int y)
{
	m_camera2.keyboardDown(key);
}

void DualTouch::keyboardUp1(unsigned char key, int x, int y)
{
	m_camera1.keyboardUp(key);
}

void DualTouch::keyboardUp2(unsigned char key, int x, int y)
{
	m_camera2.keyboardUp(key);
}

void DualTouch::mouse1(int button, int state, int x, int y)
{
	m_camera1.mouseDown(button,x,y);
	
}

void DualTouch::mouse2(int button, int state, int x, int y)
{
	m_camera2.mouseDown(button,x,y);
}

void DualTouch::motion1(int x, int y)
{
	m_camera1.mouseMotion(x,y);
}

void DualTouch::motion2(int x, int y)
{
	m_camera2.mouseMotion(x,y);
}