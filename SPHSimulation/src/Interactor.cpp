#include "Interactor.h"
//Customize
#include "SPHParticle3d.h"
#include "TextureManager.h"
#include "MarchingCubesShaded.h"
#include "ShaderProgram.h"
#include <iostream>
#include <glm\common.hpp>
#include <gl\glew.h>

#include "ShaderProgram.h"
#include "Camera.h"
#include <glm\geometric.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#define BUFFER_SIZE_INC 64

Interactor::Interactor():
	bufferSize(BUFFER_SIZE_INC), buffer(new glm::vec3[BUFFER_SIZE_INC]), bufferElements(0),
	textureID(0), bufferID(0), color({ 1.f,1.f,1.f }),
	shader(nullptr), useShader(useShader && GLEW_ARB_geometry_shader4 == GL_TRUE),
	pointSize(5.0f)
{
	setImage("data/images/ball.png");

	// Setup vao and vbo connections
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// NULL => do not copy the buffer
	glBufferData(GL_ARRAY_BUFFER, bufferSize * sizeof(float) * 3, NULL, GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLubyte *)NULL);

		shader = ShaderProgram::CreateShader(
			"data/shaders/interactor.vert",
			"data/shaders/interactor.geom",
			"data/shaders/interactor.frag",
			true);
	
}

Interactor::~Interactor()
{
	delete[] buffer;
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
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

void Interactor::setColor(float r, float g, float b)
{
	color = glm::vec3(r, g, b);
}

void Interactor::setPointSize(float size)
{
	pointSize = size;
}

void Interactor::clearBuffer()
{
	bufferElements = 0;
}

void Interactor::pushPoint(glm::vec3 point)
{
	pushPoint(point.x, point.y, point.z);
}

void Interactor::pushPoint(float x, float y, float z)
{
	bool resize = bufferElements + 1 > bufferSize;
	if (resize)
	{
		int newSize = bufferSize + BUFFER_SIZE_INC;
		glm::vec3 *newBuffer = new glm::vec3[newSize];
		memcpy_s(newBuffer, newSize * sizeof(glm::vec3), buffer, bufferElements * sizeof(glm::vec3));
		delete[] buffer;
		buffer = newBuffer;
		bufferSize = newSize;

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		// NULL => do not copy the buffer
		glBufferData(GL_ARRAY_BUFFER, bufferSize * sizeof(float) * 3, NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	buffer[bufferElements] = glm::vec3(x, y, z);
	bufferElements++;
}

void Interactor::drawArray()
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, bufferSize * sizeof(float) * 3, buffer);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, bufferElements);/**/
	glBindVertexArray(0);
}

void Interactor::drawShaded(const Camera& camera)
{
	glDisable(GL_CULL_FACE);
	glEnable(GL_CULL_FACE);
	auto projection = camera.getProjection();
	auto modelView = camera.getView() * transform.getTransformMatrix();
	auto MVP = projection * modelView;

	auto up = camera.getUp();

	shader->turnOn();
	shader->setUniformM4("ModelViewMatrix", glm::value_ptr(modelView));
	shader->setUniformM4("ProjectionMatrix", glm::value_ptr(projection));
	shader->setUniformM4("mvp", glm::value_ptr(MVP));
	shader->setUniformV3("up", up.x, up.y, up.z);
	shader->setUniformF("Size2", pointSize);
	shader->setUniformI("SpriteTex", 0);
	shader->setUniformV3("Color", color.x, color.y, color.z);

	drawArray();
	shader->turnOff();

	glEnable(GL_CULL_FACE);
}

void Interactor::draw(const Camera& camera)
{
	ShaderProgram::turnOff();
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);

	GLboolean blendEnabled = glIsEnabled(GL_BLEND);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, 0);
	glDepthMask(GL_FALSE);
	drawShaded(camera);

	glDepthMask(GL_TRUE);
	if (!blendEnabled) glDisable(GL_BLEND);

	glDisable(GL_TEXTURE_2D);
}