#include "Interactor.h"
//Customize
#include "SPHParticle3d.h"
#include "TextureManager.h"
#include "MarchingCubesShaded.h"
#include <iostream>
#include <glm\common.hpp>
#include <gl\glew.h>

#include "MarchingCubesShaded.h"
#include "MappedData.h"
#include "ShaderProgram.h"
#include "MarchingCubesFactory.h"
#include "Camera.h"
#include <glm\geometric.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>


Interactor::Interactor()
{
	setImage("data/images/point.png");
}

void Interactor::applyDensity(SPHParticle3d& other, glm::vec3 rvec)
{

}

void Interactor::applyForce(SPHParticle3d& other, glm::vec3 rvec)
{

}

void Interactor::enforceInteractor(SPHParticle3d& other, glm::vec3 rvec)
{

	if (!isWithin(other.position, min, max))
	{	// outside - move inside
		other.position = glm::clamp(other.position, min, max);
		//rvec.normalize();
		//rvec *= distance;
		//other.position -= rvec;
		//rvec.changeSign();
		if (rvec.x != 0)
		{
			other.velocity.x = -other.velocity.x*dampening;
			//other.position.x -= rvec.x;
		}
		if (rvec.y != 0)
		{
			other.velocity.y = -other.velocity.y*dampening;
			//other.position.y -= rvec.y;
		}
		if (rvec.z != 0)
		{
			other.velocity.z = -other.velocity.z*dampening;
			//other.position.z -= rvec.z;
		}
	}
	/*
	// inside - reflect speed
	float r = rvec.norm();
	float depth = distance - r;
	//glm::vec3 dir = rvec.normalized();
	float cosine = rvec.dotProduct( other.velocity );
	if( depth == 0 && cosine >= 0 )
	{
	if( rvec.x != 0 )
	{
	other.velocity.x = -other.velocity.x*dampening;
	other.position.x -= rvec.x;
	}
	if( rvec.y != 0 )
	{
	other.velocity.y = -other.velocity.y*dampening;
	other.position.y -= rvec.y;
	}
	if( rvec.z != 0 )
	{
	other.velocity.z = -other.velocity.z*dampening;
	other.position.z -= rvec.z;
	}
	/*
	other.position -= rvec*(depth);
	glm::vec3 newVelocity = other.velocity-rvec*2*cosine/(r*r);
	//cosine = abs(cosine);
	//newVelocity *= dir*(1.7);
	if( rvec.maxPart() != 0 )
	{
	newVelocity *= -1;
	}
	other.velocity = newVelocity;		/**/
	//}
}

glm::vec3 Interactor::directionTo(SPHParticle3d& other)
{
	glm::vec3 position = other.position;
	glm::vec3 toMin = position - min;
	glm::vec3 toMax = max - position;
	float minMin = minPart(toMin);
	float minMax = minPart(toMax);
	//if(position.x-0.036558483<=0.0000001 && position.y-0.074678488<=0.0000001 )
	//{
	//	int a = 0;
	//}
	if (minMin < minMax)
	{
		if (minMin < distance)
			return minDir(toMin)*(-distance);
		return -minDim(toMin);
	}
	else {
		if (minMax < distance)
			return minDir(toMax)*distance;
		return minDim(toMax);	// ERROR? missing minus?
	}
}

void Interactor::draw()
{
}

void Interactor::setImage(const char* texturePath)
{
	if (texturePath == nullptr)
	{
		textureID = 0;
		return;
	}
	textureID = TextureManager::instance.loadTexture(texturePath);
}


void Interactor::addInteractor(SPHParticle3d* SPHParticle) {

	interactor_ = SPHParticle;
}

void Interactor::putSphere(float x, float y, float z, float r)
{
	float value;

	glm::vec3 vPosition = marchingCubes->getPosition();
	glm::vec3 deltaSpan = marchingCubes->getScale();

	glm::vec3 start(x - r, y - r, z - r);
	start -= vPosition;
	start /= deltaSpan;

	glm::vec3 end(x + r, y + r, z + r);
	end -= vPosition;
	end /= deltaSpan;

	glm::vec3 center = (glm::vec3(x, y, z) - vPosition) / deltaSpan;
	r /= glm::length(deltaSpan);

	glm::ivec3 iStart(start.x, start.y, start.z);
	glm::ivec3 iEnd(end.x + 1, end.y + 1, end.z + 1);
	for (int i = iStart.x; i<iEnd.x; i++)
	{
		for (int j = iStart.y; j<iEnd.y; j++)
		{
			for (int k = iStart.z; k<iEnd.z; k++)
			{
				value = r - glm::length(center - glm::vec3(i, j, k)) + 1;
				if (value > 0)
				{
					//set(i, j, k, value);
				}
			}
		}
	}
}

/*

void Interactor::mySolidCylinder(GLUquadric*   quad, GLdouble base,
	GLdouble top,
	GLdouble height,
	GLint slices,
	GLint stacks)
{
	glColor3f(84.0 / 255, 0.0, 125.0 / 255.0);
	gluCylinder(quad, base, top, height, slices, stacks);
	//top   
	//DrawCircleArea(0.0, 0.0, height, top, slices);
	//base   
	//DrawCircleArea(0.0, 0.0, 0.0, base, slices);
}

GLvoid Interactor::DrawCircleArea(float cx, float cy, float cz, float r, int num_segments)
{
	GLfloat vertex[4];

	const GLfloat delta_angle = 2.0* 3.14159 / num_segments;
	glBegin(GL_TRIANGLE_FAN);

	vertex[0] = cx;
	vertex[1] = cy;
	vertex[2] = cz;
	vertex[3] = 1.0;
	glVertex4fv(vertex);

	//draw the vertex on the contour of the circle   
	for (int i = 0; i < num_segments; i++)
	{
		vertex[0] = std::cos(delta_angle*i) * r + cx;
		vertex[1] = std::sin(delta_angle*i) * r + cy;
		vertex[2] = cz;
		vertex[3] = 1.0;
		glVertex4fv(vertex);
	}

	vertex[0] = 1.0 * r + cx;
	vertex[1] = 0.0 * r + cy;
	vertex[2] = cz;
	vertex[3] = 1.0;
	glVertex4fv(vertex);
	glEnd();
}
*/