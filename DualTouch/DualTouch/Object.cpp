#include "Object.h"

Object::Object(btCollisionShape * shape,btTransform * transform,const btVector3 &color)
{
	m_shape = shape;
	m_transform = transform;
	m_color = color;
	m_texture = false;
}

Object::~Object(void)
{
	//delete m_shape; //physic class made it
	delete m_transform;
}

void Object::setColor(const btVector3 &color){
	m_color = color;;
}

void Object::print(){
	std::cout<<" x " << m_transform->getOrigin().getX()
		<<" y " << m_transform->getOrigin().getY()
		<<" z " << m_transform->getOrigin().getZ()
		<<std::endl;
}

btVector3 Object::getpos(){
	return m_transform->getOrigin();
}

btCollisionShape* Object::getshape(){
	return m_shape;
}