#pragma once

#ifndef INTERACTOR
#define INTERACTOR

#include "SPHInteractor3d.h"
#include <cmath>
#include <gl\glew.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <glm\gtx\norm.hpp>
#include "GlmVec.h"
#include "Transform.h"

struct SPHParticle3d;
class ShaderProgram;
class Camera;

class Interactor : public SPHInteractor3d
{
	glm::vec3 min;
	glm::vec3 max;
	float distanceSquared;
	float distance;
	float dampening;

	SPHParticle3d* interactor_;

public:
	Interactor();
	~Interactor();

	void addInteractor(SPHParticle3d* SPHParticle);
	void applyDensity(SPHParticle3d& other, glm::vec3 rvec);
	void applyForce(SPHParticle3d& other, glm::vec3 rvec);
	void enforceInteractor(SPHParticle3d& other, glm::vec3 rvec);
	glm::vec3 directionTo(SPHParticle3d& other);
	void draw();

	//From PointDataVisualiser
	glm::vec3* buffer;
	int bufferElements;
	int bufferSize;
	ShaderProgram* shader;
	GLuint bufferID;
	bool useShader;
	GLuint vbo;
	GLuint vao;
	GLuint textureID;
	float pointSize;
	glm::vec3 color;

	Transform transform;

	void drawShaded(const Camera& camera);
	void clearBuffer();

	void setImage(const char* texturePath);
	void setColor(float r, float g, float b);

	void pushPoint(glm::vec3 point);
	void pushPoint(float x, float y, float z);
	void setPointSize(float size);
	void draw(const Camera& camera);
	void drawArray();





};

#endif
