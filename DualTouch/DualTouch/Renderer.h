#pragma once

//#include <GL/freeglut.h>
#include <GL/glut.h>
#include <vector>

#include "Object.h"

#include "BulletCollision/CollisionShapes/btShapeHull.h"

#define FLT_2_PI 6.283185307f

class Renderer
{
	struct ShapeCache
	{
		struct Edge { btVector3 n[2];int v[2]; };
		ShapeCache(btConvexShape* s) : m_shapehull(s) {}
		btShapeHull					m_shapehull;
		btAlignedObjectArray<Edge>	m_edges;
	};

public:
	Renderer(void);
	~Renderer(void);
	void renderScene();
	void renderShadows();
	void display();
	void init();

	void drawSky();

	void drawBox(const btVector3 &halfSize);
	void drawCone(const btScalar & radius , const btScalar & height);
	void drawSphere(const btScalar & radius);
	void drawCylinder(const btScalar & radius , const btScalar & height,int upAxis);
	void drawShadow(const btCollisionShape* shape,const btVector3 & extrusion);
	
	void setClearColor(float r,float g,float b){m_clearColor[0]=r;m_clearColor[1]=g;m_clearColor[2]=b;}
	Object* addObject(Object * object);
	void delObject(Object* o);
	void replaceObject(Object* oldObject, Object* newObject);
	Object * getObject(btCollisionShape * shape);

private:

	//GL_ShapeDrawer m_shapedrawer; 
	Renderer::ShapeCache* cache(btConvexShape* shape);
	btAlignedObjectArray<ShapeCache*>	m_shapecaches;

	unsigned int m_texturehandle;
	std::vector<Object *> m_objects;
	GLfloat m_lightPos[3];
	GLfloat m_wireColor[3];
	GLfloat m_clearColor[3];
	GLfloat m_matDiffuse[4];
	GLfloat m_matAmbient[4];
	bool m_oultines;
};
