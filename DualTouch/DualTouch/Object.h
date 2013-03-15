#pragma once

#include "BulletCollision\CollisionShapes\btCollisionShape.h"
#include <iostream>
class Object
{
public:
	Object(btCollisionShape * shape,btTransform * transform,const btVector3 &color);
	~Object(void);
	btCollisionShape * m_shape;
	btTransform  * m_transform;
	btVector3 m_color;
	bool m_texture;
	void print();
	btVector3 getpos();
	btCollisionShape * getshape();
	void setColor(const btVector3 &color);	
};
