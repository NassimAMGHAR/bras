#include <HD/hd.h>
#include <HDU/hdu.h>
#include <HDU/hduVector.h>
#include <cstdio>
#include <btBulletDynamicsCommon.h>
#include <BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h>
#include <vector>
#include "Object.h"
#include <HDU/hduError.h>

#pragma comment (lib,"hd.lib")
#pragma comment (lib,"hdu.lib")

#pragma once

#define STIFFNESS 0.30
#define EPSILON 0.00001 /* zero, for purposes of calculating distances. */
#define SCALE_DEVICE_TO_WORLD 0.1f /* scaling distance */
#define SCALE_WORLD_TO_DEVICE 1.0f/SCALE_DEVICE_TO_WORLD /* scaling distance */
#define OFFSET_TO_CAMERA 90
#define NB_DEVICES_MAX 1 //2
#define MAXSCLAR 9999.9
#define MAXSMOOTH_LOOP 50
#define MXFORCE 10
#define MNFORCE -MXFORCE
#define MXDISTANCE 50
#define MNDISTANCE -MXDISTANCE
#define VARIATION_MAX 0.3

const btVector3 green(0.0f,1.0f,0.0f);
const btVector3 orange(0.77f,0.55f,0.44f);
const btVector3 blue(0.0f,0.33f,0.66f);

class HapticData
{
	public:
	HapticData(HHD id){
		m_id=id;m_ready=false;m_nbCollision=0;
		m_near = new btVector3(MAXSCLAR,MAXSCLAR,MAXSCLAR);
		m_smother =MAXSMOOTH_LOOP;m_currentThrown = NULL;m_done = false;
		m_continusFeadBack = false;
	};
	HapticData(){m_id=-1;m_ready=false;m_nbCollision=0;m_currentThrown = NULL;};
	HHD m_id;
	hduVector3Dd m_position;
	hduVector3Dd m_force;
	HDdouble m_transform[16];
	hduVector3Dd m_realPosition;	
	HDint m_buttons;
	bool m_ready;
	int m_nbCollision;
	btVector3* m_near;
	std::vector <btRigidBody *> m_thrown;
	btRigidBody * m_currentThrown;	
	btScalar setNear();
	hduVector3Dd m_target;	
	int m_smother;
	bool m_done;
	bool m_continusFeadBack;
	btVector3* m_impactPos;
	
};

class HapticSynchronizer
{
	public:

	HapticSynchronizer(){m_data=NULL;};
	~HapticSynchronizer(){if(m_data!=NULL){delete m_data;}};
	void setData(HapticData *data){m_data=data;}
	void setThrownList(std::vector <btRigidBody *> thrown);
	void setThrown(btRigidBody * thrown);
	void setImpactPos(btVector3* m_impactPos);	
	void setNear(btVector3* pos);
	HapticData * m_data;
	HapticData m_free;	
	btTransform m_effectors;
	btTransform* m_cameraViews;
	btGeneric6DofConstraint* m_constraints;
	btScalar m_timeK;
};

class HapticDevice
{
public:
	HapticDevice();
	~HapticDevice(void);
	void addDevice(char * name, btTransform &cameraView);
	void init();
	void run();
	void feedback(btDynamicsWorld &dynamic);
	void setConstraint(unsigned int devicesID, btGeneric6DofConstraint* constraint);
	btTypedConstraint*  createConstraint(btRigidBody &myBody,btRigidBody &itsBody);

	static HDCallbackCode HDCALLBACK aSchedule(void *pUserData);
	static HDCallbackCode HDCALLBACK sScheduleOut(void *pUserData);
	static HDCallbackCode HDCALLBACK sScheduleIn(void *pUserData);

	btRigidBody * getConstraintedBody(unsigned int devicesID);
	unsigned int getNbDevices(){return m_nbDevices;}

	btTransform m_effectors[NB_DEVICES_MAX];
	static btTransform transform(HapticSynchronizer* hdd,HapticData* data);
	static hduVector3Dd invertTransform(btVector3* trans,HapticSynchronizer* hs);
	static hduVector3Dd ComputeForce(hduVector3Dd* effector, hduVector3Dd* target, hduVector3Dd* velocity);
	static hduVector3Dd ForecToImpact(hduVector3Dd* effector,hduVector3Dd* impactpos);
	btVector3 getEffectorPosition();

	void(*m_newConstraint)(void * ptr,btRigidBody *,unsigned int );
	void(*m_deleteConstraint)(void * ptr,btRigidBody *,unsigned int );

	void * m_ptr;

	void setDThrownList(std::vector <btRigidBody *> thrown);
	void setThrown(btRigidBody * thrown);
	void setDThrownObject(std::vector <Object *> thrown_object);
	void setDThrownObject(Object * thrown);
	void setImpactPos(btVector3* pos);

	bool isReadyLaunch();
	void setWaitLunch();

	static void truncate(HDdouble* x,HDdouble* y,HDdouble* z);
	static bool inrange(HDdouble x,HDdouble y,HDdouble z);
	void setGround(btRigidBody* ground);

private:

	std::string m_name;
	unsigned int m_nbDevices;
	HapticSynchronizer m_hss[NB_DEVICES_MAX];
	HDSchedulerHandle m_updateDeviceCallback;
	btGeneric6DofConstraint* m_constraints[NB_DEVICES_MAX];
	btTypedConstraint* m_itsConstraints[NB_DEVICES_MAX];
	btTransform  * m_cameraViews[NB_DEVICES_MAX];
	HDint m_oldButtons[NB_DEVICES_MAX];	
	btVector3* m_impactPos;
	btScalar m_variator; 
	Object* m_ThrownObject;
	bool m_canLaunch;
	btRigidBody* m_ground;
};


