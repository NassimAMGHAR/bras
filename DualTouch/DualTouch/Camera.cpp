#include "Camera.h"

Camera::Camera(void)
{
	m_speed=0.1f;
	m_torque=0.01f;
	m_eye.setValue(-1,0,1);
	m_look.setValue(0,1,0);
	m_up.setValue(0,0,1);
	m_keyDown=false;
	m_oldx = 0;
	m_oldy = 0;

	m_view.setIdentity();
}

Camera::~Camera(void)
{
}

void Camera::keyboardDown(unsigned char key)
{
	m_key = key;
	m_keyDown = true;
}

void Camera::keyboardUp(unsigned char key)
{
	m_keyDown=false;
}

void Camera::mouseDown(int button,int x,int y)
{
	m_oldx = x;
	m_oldy = y;
}

void Camera::mouseUp(int button)
{

}

void Camera::mouseMotion(int x,int y)
{
	int vx = x - m_oldx;
	int vy = y - m_oldy;

	btQuaternion quaterX(m_up,m_torque*vx);
	btQuaternion quaterY(btVector3(m_speed*m_look.getY(),-m_speed*m_look.getX(),0),m_torque*vy);
	btQuaternion quater=quaterX+quaterY;

	btVector3 look;
	btMatrix3x3 mat(quater);

	look.setX(mat.tdotx(m_look));
	look.setY(mat.tdoty(m_look));
	look.setZ(mat.tdotz(m_look));

	m_look = look;

	mat.setRotation(quaterX);

	m_oldx = x;
	m_oldy = y;
}

void Camera::moveTo(btVector3 & origin)
{
	m_eye = origin;
}

void Camera::lookAt()
{
	glLoadIdentity(); 

	btVector3 translation(0,0,0);
	if(m_keyDown)
	{
		btVector3 foward(m_speed*m_look);//btVector3 foward(m_speed*m_look.getX(),m_speed*m_look.getY(),0);
		btVector3 right(m_speed*m_look.cross(m_up));//btVector3 right(m_speed*m_look.getY(),-m_speed*m_look.getX(),0);
		switch(m_key)
		{
			case('z'):translation+=foward;
				break;
			case('s'):translation-=foward;
				break;
			case('d'):translation+=right;
				break;
			case('q'):translation-=right;
				break;			
		}
	}

	m_eye=translation+m_eye;
	btVector3 center = m_eye + m_look;

	/*m_up.setX(_transform.getBasis().tdotx(m_up));
	m_up.setY(_transform.getBasis().tdoty(m_up));
	m_up.setZ(_transform.getBasis().tdotz(m_up));*/

    btVector3 f(m_look);
	f.normalize();
	btVector3 s(f.cross(m_up));
    s.normalize();
    btVector3 u(s.cross(f));
    u.normalize();

	btMatrix3x3 basis (s[0],s[1],s[2],u[0],u[1],u[2],-f[0],-f[1],-f[2]);
	btTransform view(basis,btVector3(0,0,0));
	btScalar m[16];
	view.getOpenGLMatrix(m);
	glMultMatrixf(m);
	glTranslatef(-m_eye.getX(),-m_eye.getY(),-m_eye.getZ());
	m_view.setBasis(basis);
	m_view.setOrigin(m_eye);
	/*gluLookAt(	m_eye.getX(),m_eye.getY(),m_eye.getZ(),
				center.getX(),center.getY(),center.getZ(),
				m_up.getX(),m_up.getY(),m_up.getZ());*/
				
}